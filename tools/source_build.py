"""Source-only platform build generation shared by configure.py.

This module consumes the same resolved ``ProjectConfig.libs`` object graph used
by the matching decompilation build.  It never reads extracted/original object
files and never interprets ``Object.completed``; every selected unit is built
from repository source.
"""

from __future__ import annotations

import fnmatch
import io
import json
import os
import shlex
import sys
import subprocess
from dataclasses import dataclass, field
from pathlib import Path
from typing import Any, Iterable, Sequence, TYPE_CHECKING

try:
    import tomllib
except ModuleNotFoundError:  # pragma: no cover - Python 3.11+ is required.
    tomllib = None  # type: ignore[assignment]

from . import ninja_syntax
from .project import Object, ProjectConfig, file_is_asm, file_is_c, file_is_cpp, is_windows

SOURCE_SUFFIXES = {".c", ".cc", ".cp", ".cpp", ".cxx", ".s", ".S"}
DC_ROMDISK_DIR = Path("assets/dc/romdisk")
ROMDISK_EXCLUDED_NAMES = {".gitignore", ".DS_Store", "Thumbs.db"}


@dataclass(frozen=True)
class SourceUnit:
    obj: Object
    path: Path


@dataclass
class SourceProfile:
    name: str
    description: str = ""
    include: list[str] = field(default_factory=list)
    exclude: list[str] = field(default_factory=list)
    extra_sources: list[Path] = field(default_factory=list)
    allow_missing: list[str] = field(default_factory=list)


@dataclass(frozen=True)
class FlagOverride:
    pattern: str
    append: tuple[str, ...] = ()
    remove: tuple[str, ...] = ()


@dataclass
class SourcePlatform:
    name: str
    backend: str
    default_profile: str
    output_subdir: Path
    output_name: str
    emit_map: bool = True
    emit_binary: bool = False
    linker_template: Path | None = None
    lcf_origin: int = 0x80003100
    lcf_stack_size: int = 0x10000
    lcf_arena_hi: int = 0x81700000
    kos_base: Path | None = None
    kos_base_env: str | None = None
    kos_library: Path | None = None
    build_kos: bool = False
    tools: dict[str, str] = field(default_factory=dict)
    common_flags: list[str] = field(default_factory=list)
    c_flags: list[str] = field(default_factory=list)
    cxx_flags: list[str] = field(default_factory=list)
    asm_flags: list[str] = field(default_factory=list)
    link_flags: list[str] = field(default_factory=list)
    include_flags: list[str] = field(default_factory=list)
    libraries: list[str] = field(default_factory=list)
    gc_append_flags: list[str] = field(default_factory=list)
    gc_remove_flags: list[str] = field(default_factory=list)
    overrides: list[FlagOverride] = field(default_factory=list)


@dataclass
class SourceBuildOptions:
    platform: str
    profile: str | None
    platform_config: Path | None
    base_build_dir: Path
    build_config: str = "debug"
    romdisk: bool | None = None
    validate_only: bool = False
    verbose: bool = False


def _require_tomllib() -> None:
    if tomllib is None:
        raise RuntimeError("source-only builds require Python 3.11 or newer")


def _load_toml(path: Path) -> dict[str, Any]:
    _require_tomllib()
    if not path.is_file():
        raise FileNotFoundError(path)
    with path.open("rb") as stream:
        return tomllib.load(stream)


def _as_path(value: Any) -> Path | None:
    if value is None or value == "":
        return None
    return Path(str(value))


def _as_int(value: Any, default: int) -> int:
    if value is None:
        return default
    if isinstance(value, int):
        return value
    return int(str(value), 0)


def load_platform(repo_root: Path, platform_name: str, explicit_path: Path | None) -> tuple[SourcePlatform, Path]:
    path = explicit_path or Path("config") / "port" / platform_name / "config.toml"
    if not path.is_absolute():
        path = repo_root / path
    raw = _load_toml(path)
    section = raw.get("platform", raw)
    flags = raw.get("flags", {})
    tools = {str(k): str(v) for k, v in raw.get("tools", {}).items()}
    lcf = raw.get("lcf", {})

    overrides: list[FlagOverride] = []
    for item in raw.get("overrides", []):
        overrides.append(
            FlagOverride(
                pattern=str(item["pattern"]),
                append=tuple(str(v) for v in item.get("append", [])),
                remove=tuple(str(v) for v in item.get("remove", [])),
            )
        )

    result = SourcePlatform(
        name=str(section.get("name", platform_name)),
        backend=str(section.get("backend", platform_name)),
        default_profile=str(section.get("default_profile", "all-source")),
        output_subdir=Path(str(section.get("output_subdir", f"port-{platform_name}"))),
        output_name=str(section.get("output_name", f"rat_{platform_name}")),
        emit_map=bool(section.get("emit_map", True)),
        emit_binary=bool(section.get("emit_binary", False)),
        linker_template=_as_path(section.get("linker_template")),
        lcf_origin=_as_int(lcf.get("origin"), 0x80003100),
        lcf_stack_size=_as_int(lcf.get("stack_size"), 0x10000),
        lcf_arena_hi=_as_int(lcf.get("arena_hi"), 0x81700000),
        kos_base=_as_path(section.get("kos_base")),
        kos_base_env=(str(section["kos_base_env"]) if section.get("kos_base_env") else None),
        kos_library=_as_path(section.get("kos_library")),
        build_kos=bool(section.get("build_kos", False)),
        tools=tools,
        common_flags=[str(v) for v in flags.get("common", [])],
        c_flags=[str(v) for v in flags.get("c", [])],
        cxx_flags=[str(v) for v in flags.get("cxx", [])],
        asm_flags=[str(v) for v in flags.get("asm", [])],
        link_flags=[str(v) for v in flags.get("link", [])],
        include_flags=[str(v) for v in flags.get("includes", [])],
        libraries=[str(v) for v in flags.get("libraries", [])],
        gc_append_flags=[str(v) for v in flags.get("gc_append", [])],
        gc_remove_flags=[str(v) for v in flags.get("gc_remove", [])],
        overrides=overrides,
    )
    return result, path


def load_profile(repo_root: Path, platform_name: str, profile_name_or_path: str) -> tuple[SourceProfile, Path]:
    candidate = Path(profile_name_or_path)
    if candidate.suffix.lower() != ".toml":
        candidate = Path("config") / "port" / platform_name / "profiles" / f"{profile_name_or_path}.toml"
    if not candidate.is_absolute():
        candidate = repo_root / candidate
    raw = _load_toml(candidate)
    section = raw.get("profile", raw)
    profile = SourceProfile(
        name=str(section.get("name", candidate.stem)),
        description=str(section.get("description", "")),
        include=[str(v) for v in section.get("include", [])],
        exclude=[str(v) for v in section.get("exclude", [])],
        extra_sources=[Path(str(v)) for v in section.get("extra_sources", [])],
        allow_missing=[str(v) for v in section.get("allow_missing", [])],
    )
    if not profile.include and not profile.extra_sources:
        raise ValueError(f"Profile {candidate} selects no sources")
    return profile, candidate


def _matches(path: Path, patterns: Sequence[str]) -> bool:
    text = path.as_posix()
    return any(fnmatch.fnmatchcase(text, pattern) for pattern in patterns)


def _normalize_repo_path(repo_root: Path, path: Path) -> Path:
    if path.is_absolute():
        try:
            return path.resolve().relative_to(repo_root.resolve())
        except ValueError as exc:
            raise ValueError(f"Path is outside the repository: {path}") from exc
    return Path(path.as_posix().lstrip("./"))


def select_units(
    repo_root: Path,
    config: ProjectConfig,
    profile: SourceProfile,
) -> tuple[list[SourceUnit], list[Path]]:
    # config.objects() preserves the declaration order from configure.py.
    resolved = list(config.objects().values())
    by_source: dict[Path, Object] = {}
    for obj in resolved:
        if obj.src_path is None:
            continue
        source = _normalize_repo_path(repo_root, obj.src_path)
        if source in by_source:
            raise RuntimeError(f"Multiple Object entries resolve to source {source}")
        by_source[source] = obj

    selected_paths: list[Path] = []
    selected_set: set[Path] = set()
    for obj in resolved:
        if obj.src_path is None:
            continue
        source = _normalize_repo_path(repo_root, obj.src_path)
        if source.suffix not in SOURCE_SUFFIXES and source.suffix.lower() not in SOURCE_SUFFIXES:
            continue
        if not _matches(source, profile.include):
            continue
        if _matches(source, profile.exclude):
            continue
        selected_paths.append(source)
        selected_set.add(source)

    # Explicit extras override broad exclusions, but must still be declared in
    # configure.py. This keeps configure.py as the single canonical inventory.
    for extra in profile.extra_sources:
        source = _normalize_repo_path(repo_root, extra)
        if source not in by_source:
            raise RuntimeError(
                f"Profile extra source {source} is not declared as Object(...) in configure.py"
            )
        if source not in selected_set:
            selected_paths.append(source)
            selected_set.add(source)

    units: list[SourceUnit] = []
    missing: list[Path] = []
    for source in selected_paths:
        absolute = repo_root / source
        if not absolute.is_file():
            if _matches(source, profile.allow_missing):
                missing.append(source)
                continue
            raise RuntimeError(f"Selected source file is missing: {source}")
        if "orig" in {part.lower() for part in source.parts} or "asm" in {
            part.lower() for part in source.parts
        }:
            raise RuntimeError(f"Source-only build selected forbidden extracted path: {source}")
        units.append(SourceUnit(by_source[source], source))

    if not units:
        raise RuntimeError(f"Profile {profile.name!r} selected no existing source files")
    return units, missing


def _quote(value: str | Path) -> str:
    text = str(value)
    if not text:
        return text
    if is_windows():
        if any(ch.isspace() for ch in text) or '"' in text:
            return '"' + text.replace('"', '\\"') + '"'
        return text
    return shlex.quote(text)


def _native_exe(name: str) -> str:
    return f"{name}.exe" if is_windows() else name


def _compiler_wrapper(config: ProjectConfig) -> tuple[Path | None, bool]:
    wrapper = config.compiler_wrapper()
    return wrapper, wrapper is not None and config.use_wibo()


def _write_download_rule(n: ninja_syntax.Writer, config: ProjectConfig) -> Path:
    download_tool = config.tools_dir / "download_tool.py"
    n.rule(
        "download_tool",
        command=f"$python {download_tool} $tool $out --tag $tag",
        description="TOOL $out",
    )
    return download_tool


def _resolve_downloaded_tool(
    n: ninja_syntax.Writer,
    config: ProjectConfig,
    download_tool: Path,
    *,
    explicit: Path | None,
    tag: str | None,
    tool_name: str,
    filename: str,
) -> tuple[Path, Path | None]:
    if explicit is not None:
        if explicit.is_file():
            return explicit, None
        # Source checkouts (DTK) are intentionally not built here; matching
        # builds retain tools.project's Cargo handling. Source builds accept an
        # executable path or the configured downloadable release.
        raise RuntimeError(f"Source build expects an executable path for {tool_name}: {explicit}")
    if tag is None:
        raise RuntimeError(f"No path or download tag configured for {tool_name}")
    output = config.build_dir / "tools" / filename
    n.build(
        output,
        "download_tool",
        implicit=download_tool,
        variables={"tool": tool_name, "tag": tag},
    )
    return output, output


def _apply_flag_edits(flags: Iterable[str], remove: Iterable[str], append: Iterable[str]) -> list[str]:
    removed = set(remove)
    result = [flag for flag in flags if flag not in removed]
    result.extend(append)
    return result


def _object_path(build_dir: Path, source: Path) -> Path:
    return build_dir / "obj" / source.with_suffix(".o")


def _make_manifest(path: Path, platform: SourcePlatform, profile: SourceProfile, units: Sequence[SourceUnit]) -> None:
    lines = [
        "# Generated by configure.py",
        f"# Platform: {platform.name}",
        f"# Profile: {profile.name}",
        "# Every input is compiled from repository source.",
        "",
        *(unit.path.as_posix() for unit in units),
    ]
    path.write_text("\n".join(lines) + "\n", encoding="utf-8", newline="\n")


def _make_lcf(repo_root: Path, platform: SourcePlatform, output: Path) -> None:
    if platform.linker_template is None:
        raise RuntimeError("GC source build requires platform.linker_template")
    template = platform.linker_template
    if not template.is_absolute():
        template = repo_root / template
    text = template.read_text(encoding="utf-8")
    replacements = {
        "@ORIGIN@": f"0x{platform.lcf_origin:08X}",
        "@STACK_SIZE@": f"0x{platform.lcf_stack_size:X}",
        "@ARENA_HI@": f"0x{platform.lcf_arena_hi:08X}",
    }
    for old, new in replacements.items():
        text = text.replace(old, new)
    output.write_text(text, encoding="utf-8", newline="\n")


def _write_if_changed(path: Path, content: str) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    if path.is_file() and path.read_text(encoding="utf-8") == content:
        return
    path.write_text(content, encoding="utf-8", newline="\n")


def _romdisk_inputs(repo_root: Path, romdisk_dir: Path) -> tuple[list[Path], list[Path]]:
    """Return tracked romdisk files and directories relative to the repository.

    Individual files make normal content edits rebuild the romdisk. Directories
    are dependencies of the configure edge so adding or removing a file causes
    Ninja to regenerate the manifest and refresh the file list.
    """

    absolute = repo_root / romdisk_dir
    if not absolute.is_dir():
        raise RuntimeError(f"Dreamcast romdisk directory does not exist: {romdisk_dir}")

    files = sorted(
        path.relative_to(repo_root)
        for path in absolute.rglob("*")
        if path.is_file() and path.name not in ROMDISK_EXCLUDED_NAMES
    )
    directories = sorted(
        [romdisk_dir]
        + [path.relative_to(repo_root) for path in absolute.rglob("*") if path.is_dir()]
    )
    return files, directories


def _make_relative_path(from_dir: Path, target: Path) -> str:
    """Return a forward-slash path suitable for an MSYS or POSIX Makefile."""

    try:
        return Path(os.path.relpath(target.resolve(), from_dir.resolve())).as_posix()
    except ValueError:
        # Different Windows drives cannot be represented with a relative path.
        # GNU Make under DreamSDK accepts drive-letter paths written with '/'.
        return target.resolve().as_posix()


def _make_romdisk_makefile(
    repo_root: Path,
    build_dir: Path,
    romdisk_dir: Path,
    output: Path,
) -> None:
    asset_path = _make_relative_path(build_dir, repo_root / romdisk_dir)
    _write_if_changed(
        output,
        "# Generated by configure.py for the Dreamcast debug build.\n"
        f"KOS_ROMDISK_DIR := {asset_path}\n"
        "include $(KOS_BASE)/Makefile.rules\n",
    )


def _override_flags(platform: SourcePlatform, source: Path, flags: list[str]) -> list[str]:
    result = list(flags)
    for override in platform.overrides:
        if fnmatch.fnmatchcase(source.as_posix(), override.pattern):
            result = _apply_flag_edits(result, override.remove, override.append)
    return result


def _generate_gc(
    repo_root: Path,
    project: ProjectConfig,
    platform: SourcePlatform,
    profile: SourceProfile,
    platform_path: Path,
    profile_path: Path,
    units: Sequence[SourceUnit],
    build_dir: Path,
) -> None:
    build_dir.mkdir(parents=True, exist_ok=True)
    lcf = build_dir / "port.lcf"
    _make_lcf(repo_root, platform, lcf)

    output = io.StringIO()
    n = ninja_syntax.Writer(output, width=110)
    n.variable("ninja_required_version", "1.3")
    n.variable("python", f'"{sys.executable}"')
    n.variable("configure_args", sys.argv[1:])
    n.newline()
    n.comment("Source-only GameCube build generated from configure.py's Object inventory.")

    download_tool = _write_download_rule(n, project)
    compilers = project.compilers()
    compilers_dep: Path | None = None
    if project.compilers_path is None:
        if project.compilers_tag is None:
            raise RuntimeError("ProjectConfig.compilers_tag missing")
        compilers_dep = compilers
        n.build(
            compilers,
            "download_tool",
            implicit=download_tool,
            variables={"tool": "compilers", "tag": project.compilers_tag},
        )

    dtk, dtk_dep = _resolve_downloaded_tool(
        n,
        project,
        download_tool,
        explicit=project.dtk_path,
        tag=project.dtk_tag,
        tool_name="dtk",
        filename=_native_exe("dtk"),
    )

    needs_sjis = any(bool(unit.obj.options.get("shift_jis")) for unit in units)
    sjiswrap: Path | None = None
    sjis_dep: Path | None = None
    if needs_sjis:
        sjiswrap, sjis_dep = _resolve_downloaded_tool(
            n,
            project,
            download_tool,
            explicit=project.sjiswrap_path,
            tag=project.sjiswrap_tag,
            tool_name="sjiswrap",
            filename="sjiswrap.exe",
        )

    needs_asm = any(file_is_asm(unit.path) for unit in units)
    binutils: Path | None = None
    binutils_dep: Path | None = None
    if needs_asm:
        if project.binutils_path is not None:
            binutils = project.binutils_path
        elif project.binutils_tag:
            binutils = project.build_dir / "binutils"
            binutils_dep = binutils
            n.build(
                binutils,
                "download_tool",
                implicit=download_tool,
                variables={"tool": "binutils", "tag": project.binutils_tag},
            )
        else:
            raise RuntimeError("ProjectConfig.binutils_tag missing")

    wrapper, download_wrapper = _compiler_wrapper(project)
    wrapper_dep: Path | None = None
    if download_wrapper and wrapper is not None:
        wrapper_dep = wrapper
        n.build(
            wrapper,
            "download_tool",
            implicit=download_tool,
            variables={"tool": "wibo", "tag": project.wibo_tag},
        )
    wrapper_prefix = f"{_quote(wrapper)} " if wrapper is not None else ""

    compiler_root = compilers / "$mw_version"
    mwcc = compiler_root / "mwcceppc.exe"
    mwcc_cmd = f"{wrapper_prefix}{mwcc} $cflags -MMD -c $in -o $basedir"
    mwcc_sjis_cmd = f"{wrapper_prefix}{sjiswrap} {mwcc} $cflags -MMD -c $in -o $basedir"
    mwcc_implicit: list[Path] = [p for p in (compilers_dep or mwcc, wrapper_dep) if p is not None]
    mwcc_sjis_implicit: list[Path] = [*mwcc_implicit]
    if sjis_dep is not None:
        mwcc_sjis_implicit.append(sjis_dep)
    elif sjiswrap is not None:
        mwcc_sjis_implicit.append(sjiswrap)

    if not is_windows():
        transform_dep = project.tools_dir / "transform_dep.py"
        mwcc_cmd += f" && $python {transform_dep} $basefile.d $basefile.d"
        mwcc_sjis_cmd += f" && $python {transform_dep} $basefile.d $basefile.d"
        mwcc_implicit.append(transform_dep)
        mwcc_sjis_implicit.append(transform_dep)

    n.rule("mwcc", command=mwcc_cmd, description="MWCC $out", depfile="$basefile.d", deps="gcc")
    if needs_sjis:
        n.rule("mwcc_sjis", command=mwcc_sjis_cmd, description="MWCC $out", depfile="$basefile.d", deps="gcc")

    prodg = compiler_root / "ngccc.exe"
    if is_windows():
        prodg_root = os.path.abspath(compilers).replace("/", "\\") + "\\$mw_version"
        prodg_cmd = f'cmd /v /c "set SN_NGC_PATH={prodg_root}&& {prodg} $cflags -c -o $out $in"'
    else:
        prodg_root = os.path.abspath(compilers) + "/$mw_version"
        prodg_cmd = f"env SN_NGC_PATH={_quote(prodg_root)} {wrapper_prefix}{prodg} $cflags -c -o $out $in"
    n.rule("prodg", command=prodg_cmd, description="ProDG $out")

    if needs_asm:
        assert binutils is not None
        assembler = binutils / _native_exe("powerpc-eabi-as")
        # Do not chain the assembler and DTK with ``&&``. Ninja executes native
        # Windows commands directly, so shell operators would be passed to
        # powerpc-eabi-as as literal input filenames. Assemble to an intermediate
        # object, then let DTK produce the final fixed object in a separate edge.
        n.rule(
            "as",
            command=f"{assembler} $asflags -o $out $in -MD $out.d",
            description="AS $out",
            depfile="$out.d",
            deps="gcc",
        )
        n.rule(
            "elf_fixup",
            command=f"{dtk} elf fixup $in $out",
            description="FIXUP $out",
        )

    linker = compilers / str(project.linker_version) / "mwldeppc.exe"
    link_cmd = f"{wrapper_prefix}{linker} $ldflags -o $out @$out.rsp"
    n.rule("link", command=link_cmd, description="LINK $out", rspfile="$out.rsp", rspfile_content="$in_newline")
    n.rule("elf2dol", command=f"{dtk} elf2dol $in $out", description="DOL $out")

    object_paths: list[Path] = []
    compile_commands: list[dict[str, Any]] = []
    for unit in units:
        obj_path = _object_path(build_dir, unit.path)
        obj_path.parent.mkdir(parents=True, exist_ok=True)
        object_paths.append(obj_path)
        source = unit.path
        resolved = unit.obj
        if file_is_asm(source):
            flags = list(resolved.options.get("asflags") or []) + list(resolved.options.get("extra_asflags") or [])
            raw_obj_path = obj_path.with_suffix(".raw.o")
            n.build(
                raw_obj_path,
                "as",
                source,
                implicit=[p for p in (binutils_dep,) if p is not None],
                variables={"asflags": " ".join(flags)},
            )
            n.build(
                obj_path,
                "elf_fixup",
                raw_obj_path,
                implicit=dtk_dep or dtk,
            )
            continue

        base_flags = list(resolved.options.get("cflags") or []) + list(resolved.options.get("extra_cflags") or [])
        if not any(flag.startswith("-lang") for flag in base_flags):
            base_flags.insert(0, "-lang=c++" if file_is_cpp(source) else "-lang=c")
        flags = _apply_flag_edits(base_flags, platform.gc_remove_flags, platform.gc_append_flags)
        flags = _override_flags(platform, source, flags)
        mw_version = str(resolved.options.get("mw_version") or project.linker_version)
        is_prodg = "prodg" in mw_version.lower()
        rule = "prodg" if is_prodg else ("mwcc_sjis" if resolved.options.get("shift_jis") else "mwcc")
        implicit: list[Path] = []
        if compilers_dep is not None:
            implicit.append(compilers_dep)
        elif is_prodg:
            implicit.append(compilers / mw_version / "ngccc.exe")
        else:
            implicit.append(compilers / mw_version / "mwcceppc.exe")
        if wrapper_dep is not None:
            implicit.append(wrapper_dep)
        if rule == "mwcc_sjis" and sjiswrap is not None:
            implicit.append(sjis_dep or sjiswrap)
        n.build(
            obj_path,
            rule,
            source,
            implicit=implicit,
            variables={
                "mw_version": Path(mw_version),
                "cflags": " ".join(flags),
                "basedir": obj_path.parent,
                "basefile": obj_path.with_suffix(""),
            },
        )

        compiler = compilers / mw_version / ("ngccc.exe" if is_prodg else "mwcceppc.exe")
        command_parts = [str(compiler), *flags, "-c", str(source), "-o", str(obj_path)]
        compile_commands.append(
            {
                "directory": str(repo_root),
                "file": str(repo_root / source),
                "output": str(repo_root / obj_path),
                "command": " ".join(_quote(part) for part in command_parts),
            }
        )

    elf = build_dir / f"{platform.output_name}.elf"
    dol = build_dir / f"{platform.output_name}.dol"
    map_path = Path(str(elf) + ".MAP") if platform.emit_map else None
    ldflags = list(project.ldflags or []) + list(platform.link_flags)
    ldflags.extend(["-lcf", str(lcf)])
    if map_path is not None:
        ldflags.extend(["-map", str(map_path)])
    link_implicit: list[Path] = [lcf, compilers_dep or linker]
    if wrapper_dep is not None:
        link_implicit.append(wrapper_dep)
    n.build(
        elf,
        "link",
        object_paths,
        implicit=link_implicit,
        implicit_outputs=map_path,
        variables={"ldflags": " ".join(ldflags)},
    )
    n.build(dol, "elf2dol", elf, implicit=dtk_dep or dtk)
    n.build("compile", "phony", object_paths)
    n.build("all", "phony", dol)

    _write_reconfigure_rule(n, build_dir, platform_path, profile_path)
    n.default("all")
    _write_if_changed(build_dir / "build.ninja", output.getvalue())
    (build_dir / "compile_commands.json").write_text(json.dumps(compile_commands, indent=2) + "\n", encoding="utf-8")


def _native_environment_path(value: str, variable_name: str) -> Path:
    """Resolve an environment path for native filesystem access.

    DreamSDK exposes paths such as ``/opt/toolchains/dc/kos`` from its MSYS2
    shell.  Native Windows Python interprets that as ``X:/opt/...`` relative to
    the current drive, so use MSYS2's ``cygpath`` to obtain the real host path.
    The original environment value is kept unchanged for KOS commands.
    """

    if is_windows() and value.startswith("/"):
        try:
            result = subprocess.run(
                ["cygpath", "-m", value],
                check=True,
                capture_output=True,
                text=True,
            )
        except FileNotFoundError as exc:
            raise RuntimeError(
                f"{variable_name} uses an MSYS path ({value!r}), but cygpath "
                "was not found. Run configure.py from the DreamSDK shell."
            ) from exc
        except subprocess.CalledProcessError as exc:
            detail = (exc.stderr or exc.stdout or "").strip()
            suffix = f": {detail}" if detail else ""
            raise RuntimeError(
                f"Failed to convert {variable_name}={value!r} with cygpath{suffix}"
            ) from exc

        converted = result.stdout.strip()
        if not converted:
            raise RuntimeError(
                f"cygpath returned an empty path for {variable_name}={value!r}"
            )
        return Path(converted)

    return Path(value)


def _resolve_kos_base(repo_root: Path, platform: SourcePlatform) -> tuple[Path, str]:
    """Return the native KOS path and the value passed to KOS_BASE.

    ``kos_base_env`` is intended for DreamSDK/WSL installations and keeps the
    shared TOML free of machine-specific drive letters. ``kos_base`` remains
    available for a repository-vendored KallistiOS checkout.
    """

    if platform.kos_base is not None and platform.kos_base_env is not None:
        raise RuntimeError(
            "Dreamcast platform config must specify only one of kos_base or kos_base_env"
        )

    if platform.kos_base_env is not None:
        env_name = platform.kos_base_env
        raw_value = os.environ.get(env_name)
        if not raw_value:
            raise RuntimeError(
                f"Environment variable {env_name} is not set. "
                "Run configure.py from the DreamSDK/KallistiOS shell."
            )
        native_path = _native_environment_path(raw_value, env_name)
        if not native_path.is_absolute():
            native_path = repo_root / native_path
        # KOS itself should receive the shell-native value (for example
        # /opt/toolchains/dc/kos), not the cygpath-converted Windows path.
        return native_path, raw_value

    configured_path = platform.kos_base or Path("src/3rdParty/KallistiOS")
    if not configured_path.is_absolute():
        configured_path = repo_root / configured_path
    native_path = configured_path.resolve()
    return native_path, native_path.as_posix()


def _kos_command(kos_base_value: str, command: str, args: str, runner: Path) -> str:
    parts = [
        "$python",
        _quote(runner),
        "--env",
        _quote(f"KOS_BASE={kos_base_value}"),
        "--",
        _quote(command),
    ]
    if args:
        parts.append(args)
    return " ".join(parts)


def _generate_dc(
    repo_root: Path,
    project: ProjectConfig,
    platform: SourcePlatform,
    profile: SourceProfile,
    platform_path: Path,
    profile_path: Path,
    units: Sequence[SourceUnit],
    build_dir: Path,
    romdisk_dir: Path | None,
) -> None:
    build_dir.mkdir(parents=True, exist_ok=True)
    kos_base, kos_base_value = _resolve_kos_base(repo_root, platform)
    if not kos_base.is_dir():
        source = f"${platform.kos_base_env}" if platform.kos_base_env else str(platform.kos_base)
        raise RuntimeError(f"KallistiOS directory does not exist ({source}): {kos_base}")

    tools = {
        "cc": "kos-cc",
        "cxx": "kos-c++",
        "as": "kos-as",
        "ld": "kos-c++",
        "objcopy": "kos-objcopy",
        "make": "make",
        **platform.tools,
    }
    env_runner = project.tools_dir / "run_with_env.py"

    output = io.StringIO()
    n = ninja_syntax.Writer(output, width=110)
    n.variable("ninja_required_version", "1.3")
    n.variable("builddir", build_dir)
    n.variable("python", f'"{sys.executable}"')
    n.variable("configure_args", sys.argv[1:])
    n.newline()
    n.comment("Dreamcast source build using the configured KallistiOS installation.")

    kos_dependency: Path | None = None
    if platform.build_kos:
        kos_library = platform.kos_library or Path("lib/dreamcast/libkallisti.a")
        if not kos_library.is_absolute():
            kos_library = kos_base / kos_library
        kos_dependency = kos_library
        make_cmd = _kos_command(kos_base_value, tools["make"], f"-C {_quote(kos_base)}", env_runner)
        n.rule(
            "build_kos",
            command=make_cmd,
            description="KOS $out",
            restat=True,
        )
        # Make performs its own dependency tracking. The always-dirty phony edge
        # asks Make to check the vendored tree each Ninja invocation; restat keeps
        # downstream objects/linking clean when libkallisti.a did not change.
        n.build("kos_always", "phony")
        n.build(
            kos_library,
            "build_kos",
            implicit=[env_runner, kos_base / "Makefile", kos_base / "Makefile.rules", "kos_always"],
        )

    common = [*platform.common_flags, *platform.include_flags]
    cflags = [*common, *platform.c_flags]
    cxxflags = [*common, *platform.cxx_flags]
    n.rule(
        "dc_cc",
        command=_kos_command(kos_base_value, tools["cc"], "$cflags -MMD -MF $out.d -c $in -o $out", env_runner),
        description="DC CC $out",
        depfile="$out.d",
        deps="gcc",
    )
    n.rule(
        "dc_cxx",
        command=_kos_command(kos_base_value, tools["cxx"], "$cflags -MMD -MF $out.d -c $in -o $out", env_runner),
        description="DC CXX $out",
        depfile="$out.d",
        deps="gcc",
    )
    n.rule(
        "dc_as",
        command=_kos_command(kos_base_value, tools["as"], "$asflags $in -o $out", env_runner),
        description="DC AS $out",
    )
    n.rule(
        "dc_link",
        command=_kos_command(kos_base_value, tools["ld"], "$ldflags -o $out @$out.rsp $libs", env_runner),
        description="DC LINK $out",
        rspfile="$out.rsp",
        rspfile_content="$in_newline",
    )
    n.rule(
        "dc_objcopy",
        command=_kos_command(kos_base_value, tools["objcopy"], "-O binary $in $out", env_runner),
        description="DC BIN $out",
    )

    romdisk_directories: list[Path] = []
    romdisk_object: Path | None = None
    if romdisk_dir is not None:
        romdisk_files, romdisk_directories = _romdisk_inputs(repo_root, romdisk_dir)
        romdisk_makefile = build_dir / "romdisk.mk"
        romdisk_image = build_dir / "romdisk.img"
        romdisk_object = build_dir / "romdisk.o"
        _make_romdisk_makefile(repo_root, build_dir, romdisk_dir, romdisk_makefile)

        # Use KOS's canonical romdisk rules. The -B is intentional: Ninja owns
        # the actual asset dependency graph, while Makefile.rules does not list
        # files beneath KOS_ROMDISK_DIR as prerequisites of romdisk.img.
        make_args = (
            f"-B -C {_quote(build_dir.as_posix())} "
            f"-f {_quote(romdisk_makefile.name)} {_quote(romdisk_object.name)}"
        )
        n.rule(
            "dc_romdisk",
            command=_kos_command(kos_base_value, tools["make"], make_args, env_runner),
            description="DC ROMDISK $out",
            restat=True,
        )
        romdisk_implicit: list[Path] = [
            env_runner,
            romdisk_makefile,
            kos_base / "Makefile.rules",
        ]
        if kos_dependency is not None:
            romdisk_implicit.append(kos_dependency)
        n.build(
            romdisk_object.as_posix(),
            "dc_romdisk",
            [path.as_posix() for path in romdisk_files],
            implicit=romdisk_implicit,
            implicit_outputs=romdisk_image.as_posix(),
        )

    # Emit posix-style paths: node names must be consistent between compile
    # outputs and link inputs, and backslashes inside the link rsp would be
    # eaten by sh-elf ld as escape characters.
    object_paths: list[str] = []
    compile_commands: list[dict[str, Any]] = []
    order_only = kos_dependency
    for unit in units:
        obj_path = _object_path(build_dir, unit.path)
        obj_path.parent.mkdir(parents=True, exist_ok=True)
        object_paths.append(obj_path.as_posix())
        source = unit.path
        if file_is_asm(source):
            flags = _override_flags(platform, source, list(platform.asm_flags))
            n.build(
                obj_path.as_posix(),
                "dc_as",
                source.as_posix(),
                implicit=env_runner,
                variables={"asflags": " ".join(flags)},
                order_only=order_only,
            )
            continue
        flags = list(cxxflags if file_is_cpp(source) else cflags)
        flags = _override_flags(platform, source, flags)
        rule = "dc_cxx" if file_is_cpp(source) else "dc_cc"
        n.build(
            obj_path.as_posix(),
            rule,
            source.as_posix(),
            implicit=env_runner,
            variables={"cflags": " ".join(flags)},
            order_only=order_only,
        )
        compiler = tools["cxx"] if file_is_cpp(source) else tools["cc"]
        compile_commands.append(
            {
                "directory": str(repo_root),
                "file": str(repo_root / source),
                "output": str(repo_root / obj_path),
                "command": " ".join(
                    [_quote(compiler), *(_quote(flag) for flag in flags), "-c", _quote(source), "-o", _quote(obj_path)]
                ),
            }
        )

    if romdisk_object is not None:
        object_paths.append(romdisk_object.as_posix())

    elf = build_dir / f"{platform.output_name}.elf"
    binary = build_dir / f"{platform.output_name}.bin"
    map_path = build_dir / f"{platform.output_name}.map"
    ldflags = list(platform.link_flags)
    if platform.emit_map:
        ldflags.append(f"-Wl,-Map,{map_path.as_posix()}")
    n.build(
        elf.as_posix(),
        "dc_link",
        object_paths,
        implicit_outputs=map_path.as_posix() if platform.emit_map else None,
        implicit=[p for p in (env_runner, kos_dependency) if p is not None],
        variables={"ldflags": " ".join(ldflags), "libs": " ".join(platform.libraries)},
    )
    final = elf
    if platform.emit_binary:
        n.build(binary.as_posix(), "dc_objcopy", elf.as_posix(), implicit=env_runner, order_only=order_only)
        final = binary
    n.build("compile", "phony", object_paths)
    n.build("all", "phony", final.as_posix())
    _write_reconfigure_rule(
        n,
        build_dir,
        platform_path,
        profile_path,
        extra_dependencies=romdisk_directories,
    )
    n.default("all")
    _write_if_changed(build_dir / "build.ninja", output.getvalue())
    (build_dir / "compile_commands.json").write_text(json.dumps(compile_commands, indent=2) + "\n", encoding="utf-8")


def _write_reconfigure_rule(
    n: ninja_syntax.Writer,
    build_dir: Path,
    platform_path: Path,
    profile_path: Path,
    *,
    extra_dependencies: Sequence[Path] = (),
) -> None:
    configure_script = Path(os.path.relpath(os.path.abspath(sys.argv[0])))
    source_build_script = Path(os.path.relpath(__file__))
    n.rule(
        "configure",
        command="$python " + str(configure_script) + " $configure_args",
        description=f"RUN {configure_script}",
        generator=True,
    )
    n.build(
        build_dir / "build.ninja",
        "configure",
        implicit=[
            configure_script,
            source_build_script,
            platform_path,
            profile_path,
            *extra_dependencies,
        ],
    )


def generate_source_build(
    project: ProjectConfig,
    options: SourceBuildOptions,
    *,
    repo_root: Path | None = None,
) -> Path:
    repo_root = (repo_root or Path.cwd()).resolve()
    platform, platform_path = load_platform(repo_root, options.platform, options.platform_config)
    profile_name = options.profile or platform.default_profile
    profile, profile_path = load_profile(repo_root, options.platform, profile_name)
    units, missing = select_units(repo_root, project, profile)
    build_dir = options.base_build_dir / platform.output_subdir
    backend = platform.backend.lower()

    romdisk_dir: Path | None = None
    if backend in {"dc", "dreamcast", "kos"}:
        # Independent of the build configuration, defaulting on only for debug.
        enable_romdisk = options.romdisk
        if enable_romdisk is None:
            enable_romdisk = options.build_config == "debug"
        if enable_romdisk:
            romdisk_dir = DC_ROMDISK_DIR
            # Validate this before --validate-only returns so a configuration
            # cannot appear valid while lacking its required romdisk assets.
            _romdisk_inputs(repo_root, romdisk_dir)

    if options.verbose or options.validate_only:
        print(f"Platform: {platform.name} ({platform.backend})")
        print(f"Profile: {profile.name}")
        if profile.description:
            print(profile.description)
        print(f"Selected sources: {len(units)}")
        if missing:
            print(f"Skipped declared-but-missing sources: {len(missing)}")
            for path in missing:
                print(f"  missing: {path}")

    if options.validate_only:
        print("Source-only validation passed.")
        return build_dir / "build.ninja"

    build_dir.mkdir(parents=True, exist_ok=True)
    _make_manifest(build_dir / "source_manifest.txt", platform, profile, units)
    missing_path = build_dir / "missing_sources.txt"
    if missing:
        missing_path.write_text("".join(f"{path.as_posix()}\n" for path in missing), encoding="utf-8", newline="\n")
    elif missing_path.exists():
        missing_path.unlink()

    if backend in {"gc", "gamecube"}:
        _generate_gc(repo_root, project, platform, profile, platform_path, profile_path, units, build_dir)
    elif backend in {"dc", "dreamcast", "kos"}:
        _generate_dc(
            repo_root,
            project,
            platform,
            profile,
            platform_path,
            profile_path,
            units,
            build_dir,
            romdisk_dir,
        )
    else:
        raise RuntimeError(f"Unsupported source-build backend: {platform.backend}")

    ninja_path = build_dir / "build.ninja"
    print(f"Generated {ninja_path}")
    print(f"Source manifest: {build_dir / 'source_manifest.txt'}")
    #print(f"Build with: ninja -f {ninja_path}")
    if platform.backend.lower() in {"dc", "dreamcast", "kos"}:
        kos_base, _ = _resolve_kos_base(repo_root, platform)
        if not kos_base.is_dir():
            print(f"Note: KallistiOS was not found at {kos_base}")
    return ninja_path

