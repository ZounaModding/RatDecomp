#!/usr/bin/env python3

###
# Generates build files for the project.
# This file also includes the project configuration,
# such as compiler flags and the object matching status.
#
# Usage:
#   python3 configure.py
#   ninja
#
# Append --help to see available options.
###
# fmt: off

import argparse
import os
import shutil
import sys
from pathlib import Path
from typing import Any, Dict, List

from tools.project import (
    Object,
    ProgressCategory,
    ProjectConfig,
    calculate_progress,
    generate_build,
    is_windows,
)

# Game versions
DEFAULT_VERSION = 0
VERSIONS = [
    "GLLE78",  # 0
]

def _build_combinations_help() -> str:
    """Summarise the available platform/config/profile combinations.

    Discovered from config/port rather than hardcoded so --help cannot drift
    when a config or profile is added.
    """

    root = Path(__file__).resolve().parent
    lines = ["build combinations:"]

    for platform, label, mode in (
        ("gc", "GameCube", "--build-mode matching (default) or source"),
        ("dc", "Dreamcast", "--build-mode source (only mode supported)"),
    ):
        base = root / "config" / "port" / platform
        if not base.is_dir():
            continue
        configs = sorted(p.stem[len("config-"):] for p in base.glob("config-*.toml"))
        profiles = sorted(p.stem for p in (base / "profiles").glob("*.toml"))
        lines.append(f"  --platform {platform:<4} {label}, {mode}")
        if configs:
            lines.append(f"      --build-config  {', '.join(configs)}")
        if profiles:
            lines.append(f"      --profile       {', '.join(profiles)}")

    lines += [
        "",
        "  --build-config selects config/port/<platform>/config-<name>.toml and",
        "  --profile selects config/port/<platform>/profiles/<name>.toml, so the",
        "  lists above are whatever those directories currently hold.",
        "",
        "  --romdisk / --no-romdisk is Dreamcast only. It defaults on for the debug",
        "  build config and off for every other one.",
        "",
        "  Matching-build extras (GameCube only): --map, --debug, --non-matching.",
        "",
        "examples:",
        "  python configure.py --platform gc",
        "  python configure.py --platform gc --build-mode source --build-config debug",
        "  python configure.py --platform dc",
        "  python configure.py --platform dc --build-config release --romdisk",
        "  python configure.py --platform dc --profile loading-hat --validate-only",
    ]
    return "\n".join(lines)


parser = argparse.ArgumentParser(
    epilog=_build_combinations_help(),
    formatter_class=argparse.RawDescriptionHelpFormatter,
)
parser.add_argument(
    "--platform",
    choices=["gc", "dc"],
    default="gc",
    help="target platform (default: gc)",
)
parser.add_argument(
    "--build-mode",
    choices=["matching", "source"],
    help="matching decomp build or source-only port build (default: matching for GC, source for DC)",
)
parser.add_argument(
    "--profile",
    help="source-build profile name or TOML path (source mode only)",
)
parser.add_argument(
    "--platform-config",
    metavar="TOML",
    type=Path,
    help="override config/port/<platform>/config.toml (source mode only)",
)
parser.add_argument(
    "--validate-only",
    action="store_true",
    help="validate a source-only platform/profile selection without generating Ninja",
)
parser.add_argument(
    "--build-config",
    choices=["debug", "release", "reldebinfo"],
    help="source-build configuration",
)
parser.add_argument(
    "--romdisk",
    action=argparse.BooleanOptionalAction,
    default=None,
    help="embed the Dreamcast romdisk (default: on for debug, off otherwise)",
)
parser.add_argument(
    "mode",
    choices=["configure", "progress"],
    default="configure",
    help="script mode (default: configure)",
    nargs="?",
)
parser.add_argument(
    "-v",
    "--version",
    choices=VERSIONS,
    type=str.upper,
    default=VERSIONS[DEFAULT_VERSION],
    help="version to build",
)
parser.add_argument(
    "--build-dir",
    metavar="DIR",
    type=Path,
    default=Path("build"),
    help="base build directory (default: build)",
)
parser.add_argument(
    "--binutils",
    metavar="BINARY",
    type=Path,
    help="path to binutils (optional)",
)
parser.add_argument(
    "--compilers",
    metavar="DIR",
    type=Path,
    help="path to compilers (optional)",
)
parser.add_argument(
    "--map",
    action="store_true",
    help="generate map file(s)",
)
parser.add_argument(
    "--debug",
    action="store_true",
    help="build with debug info (non-matching)",
)
if not is_windows():
    parser.add_argument(
        "--wrapper",
        metavar="BINARY",
        type=Path,
        help="path to wibo or wine (optional)",
    )
parser.add_argument(
    "--dtk",
    metavar="BINARY | DIR",
    type=Path,
    help="path to decomp-toolkit binary or source (optional)",
)
parser.add_argument(
    "--objdiff",
    metavar="BINARY | DIR",
    type=Path,
    help="path to objdiff-cli binary or source (optional)",
)
parser.add_argument(
    "--sjiswrap",
    metavar="EXE",
    type=Path,
    help="path to sjiswrap.exe (optional)",
)
parser.add_argument(
    "--verbose",
    action="store_true",
    help="print verbose output",
)
parser.add_argument(
    "--non-matching",
    dest="non_matching",
    action="store_true",
    help="builds equivalent (but non-matching) or modded objects",
)
parser.add_argument(
    "--no-progress",
    dest="progress",
    action="store_false",
    help="disable progress calculation",
)
args = parser.parse_args()

build_mode = args.build_mode or ("source" if args.platform == "dc" else "matching")
if args.platform == "dc" and build_mode != "source":
    parser.error("Dreamcast currently supports only --build-mode source")
if build_mode == "source" and args.mode != "configure":
    parser.error("the progress mode is available only for the matching GC build")
if build_mode == "source" and args.non_matching:
    parser.error("--non-matching is a matching-build option; source mode already compiles every selected source")
if build_mode == "matching" and args.platform != "gc":
    parser.error("matching builds are currently available only for GameCube")
if build_mode == "matching" and (args.profile or args.platform_config or args.validate_only):
    parser.error("--profile, --platform-config and --validate-only are source-build options")

source_build_config = args.build_config or "debug"

source_platform_config = args.platform_config

if build_mode == "source" and source_platform_config is None:
    source_platform_config = (
        Path("config")
        / "port"
        / args.platform
        / f"config-{source_build_config}.toml"
    )

config = ProjectConfig()
config.version = str(args.version)
version_num = VERSIONS.index(config.version)

# Apply arguments
config.build_dir = args.build_dir
config.dtk_path = args.dtk
config.objdiff_path = args.objdiff
config.binutils_path = args.binutils
config.compilers_path = args.compilers
config.generate_map = True # Always do map cause it's annoying to pass --map every time
config.non_matching = args.non_matching if build_mode == "matching" else False
config.sjiswrap_path = args.sjiswrap
config.progress = args.progress
if not is_windows():
    config.wrapper = args.wrapper
# Don't build asm unless we're --non-matching
if not config.non_matching:
    config.asm_dir = None

# Tool versions
config.binutils_tag = "2.42-1"
config.compilers_tag = "20251015"
config.dtk_tag = "v1.4.1"
config.objdiff_tag = "v2.7.1"
config.sjiswrap_tag = "v1.2.0"
config.wibo_tag = "1.0.0-beta.5"

# Project
config.config_path = Path("config") / config.version / "config.yml"
config.check_sha_path = Path("config") / config.version / "build.sha1"
config.asflags = [
    "-mgekko",
    "--strip-local-absolute",
    "-I include",
    f"-I build/{config.version}/include",
    f"--defsym BUILD_VERSION={version_num}",
    f"--defsym VERSION={config.version}",
]
config.ldflags = [
    "-fp hardware",
    "-nodefaults",
]
if args.debug:
    config.ldflags.append("-g")  # Or -gdwarf-2 for Wii linkers
if args.map:
    config.ldflags.append("-mapunused")
    # config.ldflags.append("-listclosure") # For Wii linkers

# Use for any additional files that should cause a re-configure when modified
config.reconfig_deps = []

# Optional numeric ID for decomp.me preset
# Can be overridden in libraries or objects
config.scratch_preset_id = 174

# Base flags, common to most GC/Wii games.
# Generally leave untouched, with overrides added below.
cflags_base = [
    "-nodefaults",
    "-proc gekko",
    "-align powerpc",
    "-enum int",
    "-fp hardware",
    "-Cpp_exceptions off",
    # "-W all",
    '-pragma "cats off"',
    '-pragma "warn_notinlined off"',
    "-maxerrors 1",
    "-nosyspath",
    "-fp_contract on",
    "-str reuse",
    "-multibyte",  # For Wii compilers, replace with `-enc SJIS`
    f"-i build/{config.version}/include",
    "-ir src/3rdParty/dolphin",
    "-ir include",
    "-i src/3rdParty/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Include",
    "-i src/3rdParty/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Include",
    "-i src/3rdParty/PowerPC_EABI_Support/MSL/MSL_C/PPC_EABI/Include",
    "-i src/3rdParty/PowerPC_EABI_Support/MSL/MSL_C++/MSL_Common/Include",
    "-i src/3rdParty/PowerPC_EABI_Support/Runtime/Inc",
    "-i src/3rdParty/PowerPC_EABI_Support/MetroTRK",
    "-i src/3rdParty/bink/includes",
    "-i include/dolphin",
    "-i include",
    f"-DBUILD_VERSION={version_num}",
    f"-DVERSION_{config.version}",
    "-D__GEKKO__",
    "-DGAMECUBE_Z",
    "-O4,p",
]
cflags_bink_base = [
    "-O3",
    "-mcpu=750",
    "-fno-exceptions",
    "-Wno-inline",
    "-nostdinc",
    "-I src/3rdParty/dolphin",
    "-I include",
    "-I include/dolphin",
    "-D__GEKKO__",
    "-I src/3rdParty/bink/includes",
    "-I src/3rdParty/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Include",
    "-G0",
]
cflags_noopt = cflags_base[:]

cflags_trk = [
    *cflags_base,
    "-use_lmw_stmw on",
    "-rostr",
    "-str reuse",
    "-common off",
    "-inline deferred,auto",
    "-char signed",
    "-sdata 0",
    "-sdata2 0",
]

# Dolphin library flags
cflags_dolphin = [
    "-nodefaults",
    "-proc gekko",
    "-align powerpc",
    "-enum int",
    "-fp hardware",
    "-Cpp_exceptions off",
    '-pragma "cats off"',
    '-pragma "warn_notinlined off"',
    "-maxerrors 1",
    "-nosyspath",
    "-char unsigned",
    "-O4,p",
    "-sym on",
    "-inline auto",
    "-i include",
    f"-i build/{config.version}/include",
    "-ir src/3rdParty/dolphin",
    "-i src/3rdParty/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Include",
    "-i src/3rdParty/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Include",
    "-i src/3rdParty/PowerPC_EABI_Support/MSL/MSL_C/PPC_EABI/Include",
    "-i src/3rdParty/PowerPC_EABI_Support/MSL/MSL_C++/MSL_Common/Include",
    "-i src/3rdParty/PowerPC_EABI_Support/Runtime/Inc",
    "-i src/3rdParty/PowerPC_EABI_Support/MetroTRK",
    "-i include/dolphin",
    f"-DVERSION={version_num}",
    "-D__GEKKO__",
    "-DSDK_REVISION=2",
]
cflags_noopt = cflags_dolphin[:]
cflags_noopt.remove("-O4,p")

cflags_rat_base = [
    *cflags_base,
    "-pool on",
    "-schedule on",
    "-fp fmadd",
    "-inline auto",
    "-use_lmw_stmw on",
    "-lang=c++",
    '-pragma "cpp_extensions on"',
    "-RTTI off",
    "-i src/Engine/includes",
    "-i src/LibGC/includes",
    "-i src/Rat/includes",
    "-O4,p",
    "-rostr",
]

# Source builds compile every TU from source, so this is unconditional there;
if config.non_matching or build_mode == "source":
    cflags_rat_base.extend(["-DNONMATCHING_Z"])

# Debug flags
if args.debug:
    # Or -sym dwarf-2 for Wii compilers
    cflags_base.extend(["-sym on", "-DDEBUG=1"])
else:
    cflags_base.append("-DNDEBUG=1")

# Metrowerks library flags
cflags_runtime = [
    *cflags_base,
    "-use_lmw_stmw on",
    "-str reuse,pool,readonly",
    "-common off",
    "-inline auto",
    "-O4,p",
]

# REL flags
cflags_rel = [
    *cflags_base,
    "-sdata 0",
    "-sdata2 0",
]

config.linker_version = "GC/2.7"


# Helper function for Dolphin libraries
def DolphinLib(lib_name: str, objects: List[Object]) -> Dict[str, Any]:
    return {
        "lib": lib_name,
        "mw_version": "GC/1.2.5n",
        "cflags": cflags_dolphin,
        "progress_category": "sdk",
        "objects": objects,
    }


# Helper function for REL script objects
def Rel(lib_name: str, objects: List[Object]) -> Dict[str, Any]:
    return {
        "lib": lib_name,
        "mw_version": "GC/1.3.2",
        "cflags": cflags_rel,
        "progress_category": "game",
        "objects": objects,
    }


Matching = True                   # Object matches and should be linked
NonMatching = False               # Object does not match and should not be linked
Equivalent = config.non_matching  # Object should be linked when configured with --non-matching


# Object is only matching for specific versions
def MatchingFor(*versions):
    return config.version in versions


config.warn_missing_config = True
config.warn_missing_source = False
config.libs = [
    DolphinLib(
        "base",
        [
            Object(Matching, "3rdParty/dolphin/base/PPCArch.c"),
        ],
    ),
    DolphinLib(
        "os",
        [
            Object(Matching, "3rdParty/dolphin/os/__start.c"),
            Object(Matching, "3rdParty/dolphin/os/OS.c"),
            Object(Matching, "3rdParty/dolphin/os/OSAlarm.c"),
            Object(Matching, "3rdParty/dolphin/os/OSAlloc.c"),
            Object(Matching, "3rdParty/dolphin/os/OSArena.c"),
            Object(Matching, "3rdParty/dolphin/os/OSAudioSystem.c"),
            Object(Matching, "3rdParty/dolphin/os/OSCache.c"),
            Object(Matching, "3rdParty/dolphin/os/OSContext.c"),
            Object(Matching, "3rdParty/dolphin/os/OSError.c"),
            Object(Matching, "3rdParty/dolphin/os/OSExec.c"),
            Object(Matching, "3rdParty/dolphin/os/OSFont.c"),
            Object(Matching, "3rdParty/dolphin/os/OSInterrupt.c"),
            Object(Matching, "3rdParty/dolphin/os/OSLink.c"),
            Object(Matching, "3rdParty/dolphin/os/OSMemory.c"),
            Object(Matching, "3rdParty/dolphin/os/OSMutex.c"),
            Object(Matching, "3rdParty/dolphin/os/OSReboot.c"),
            Object(Matching, "3rdParty/dolphin/os/OSReset.c"),
            Object(Matching, "3rdParty/dolphin/os/OSResetSW.c"),
            Object(Matching, "3rdParty/dolphin/os/OSRtc.c"),
            Object(Matching, "3rdParty/dolphin/os/OSSync.c"),
            Object(Matching, "3rdParty/dolphin/os/OSThread.c"),
            Object(Matching, "3rdParty/dolphin/os/OSTime.c"),
            Object(Matching, "3rdParty/dolphin/os/OSFatal.c"), # --non-matching
            Object(Matching, "3rdParty/dolphin/os/__ppc_eabi_init.cpp"),
        ],
    ),
    {
        "lib": "exi",
        "mw_version": "GC/1.2.5n",
        "cflags": cflags_noopt,
        "progress_category": "sdk",
        "objects": [
            Object(Matching, "3rdParty/dolphin/exi/EXIBios.c", extra_cflags=["-O3,p"]),
            Object(Matching, "3rdParty/dolphin/exi/EXIUart.c", extra_cflags=["-O4,p"]),
        ],
    },
    DolphinLib(
        "si",
        [
            Object(Matching, "3rdParty/dolphin/si/SIBios.c"),
            Object(Matching, "3rdParty/dolphin/si/SISamplingRate.c"),
        ],
    ),
    DolphinLib(
        "db",
        [
            Object(Matching, "3rdParty/dolphin/db/db.c"),
        ],
    ),
    DolphinLib(
        "mtx",
        [
            Object(Matching, "3rdParty/dolphin/mtx/mtx.c", extra_cflags=["-char signed", "-O4,p"]),
            Object(Matching, "3rdParty/dolphin/mtx/mtx44.c", extra_cflags=["-char signed"]),
        ],
    ),
    DolphinLib(
        "dvd",
        [
            Object(Matching, "3rdParty/dolphin/dvd/dvdlow.c", extra_cflags=["-char signed"]),
            Object(Matching, "3rdParty/dolphin/dvd/dvdfs.c", extra_cflags=["-char signed"]),
            Object(Matching, "3rdParty/dolphin/dvd/dvd.c", extra_cflags=["-char signed"]),
            Object(Matching, "3rdParty/dolphin/dvd/dvdqueue.c", extra_cflags=["-char signed"]),
            Object(Matching, "3rdParty/dolphin/dvd/dvderror.c", extra_cflags=["-char signed"]),
            Object(Matching, "3rdParty/dolphin/dvd/dvdidutils.c", extra_cflags=["-char signed"]),
            Object(Matching, "3rdParty/dolphin/dvd/dvdFatal.c", extra_cflags=["-char signed"]),
            Object(Matching, "3rdParty/dolphin/dvd/fstload.c", extra_cflags=["-char signed"]),
        ],
    ),
    DolphinLib(
        "vi",
        [
            Object(Matching, "3rdParty/dolphin/vi/vi.c"),
        ],
    ),
    DolphinLib(
        "pad",
        [
            Object(Matching, "3rdParty/dolphin/pad/Padclamp.c"),
            Object(Matching, "3rdParty/dolphin/pad/Pad.c"),
        ],
    ),
    DolphinLib(
        "ai",
        [
            Object(Matching, "3rdParty/dolphin/ai/ai.c"),
        ],
    ),
    DolphinLib(
        "locale",
        [
            Object(NonMatching, "3rdParty/dolphin/locale.c"),
        ],
    ),
    DolphinLib(
        "ax",
        [
            Object(Matching, "3rdParty/dolphin/ax/AX.c"),
            Object(Matching, "3rdParty/dolphin/ax/AXAlloc.c"),
            Object(Matching, "3rdParty/dolphin/ax/AXAux.c"),
            Object(Matching, "3rdParty/dolphin/ax/AXCL.c"),
            Object(NonMatching, "3rdParty/dolphin/ax/AXOut.c"),
            Object(NonMatching, "3rdParty/dolphin/ax/AXSPB.c"),
            Object(NonMatching, "3rdParty/dolphin/ax/AXVPB.c"),
            Object(Matching, "3rdParty/dolphin/ax/AXComp.c"),
            Object(Matching, "3rdParty/dolphin/ax/DSPCode.c"),
            Object(Matching, "3rdParty/dolphin/ax/AXProf.c"),
        ],
    ),
    DolphinLib(
        "dsp",
        [
            Object(Matching, "3rdParty/dolphin/dsp/dsp.c"),
            Object(Matching, "3rdParty/dolphin/dsp/dsp_debug.c"),
            Object(Matching, "3rdParty/dolphin/dsp/dsp_task.c"),
        ],
    ),
    DolphinLib(
        "mix",
        [
            Object(Matching, "3rdParty/dolphin/mix/mix.c"),
        ],
    ),
    DolphinLib(
        "card",
        [
            Object(Matching, "3rdParty/dolphin/card/CARDBios.c"),
            Object(Matching, "3rdParty/dolphin/card/CARDUnlock.c"),
            Object(Matching, "3rdParty/dolphin/card/CARDRdwr.c"),
            Object(Matching, "3rdParty/dolphin/card/CARDBlock.c"),
            Object(Matching, "3rdParty/dolphin/card/CARDDir.c"),
            Object(Matching, "3rdParty/dolphin/card/CARDCheck.c"),
            Object(Matching, "3rdParty/dolphin/card/CARDMount.c"),
            Object(Matching, "3rdParty/dolphin/card/CARDFormat.c"),
            Object(Matching, "3rdParty/dolphin/card/CARDOpen.c", extra_cflags=["-char signed"]),
            Object(Matching, "3rdParty/dolphin/card/CARDCreate.c"),
            Object(Matching, "3rdParty/dolphin/card/CARDRead.c"),
            Object(Matching, "3rdParty/dolphin/card/CARDWrite.c"),
            Object(Matching, "3rdParty/dolphin/card/CARDDelete.c"),
            Object(Matching, "3rdParty/dolphin/card/CARDStat.c"),
            Object(Matching, "3rdParty/dolphin/card/CARDNet.c"),
        ],
    ),
    DolphinLib(
        "gx",
        [
            Object(Matching, "3rdParty/dolphin/gx/GXInit.c", extra_cflags=["-opt nopeephole"]),
            Object(Matching, "3rdParty/dolphin/gx/GXFifo.c"),
            Object(Matching, "3rdParty/dolphin/gx/GXAttr.c"),
            Object(Matching, "3rdParty/dolphin/gx/GXMisc.c"),
            Object(Matching, "3rdParty/dolphin/gx/GXGeometry.c"),
            Object(Matching, "3rdParty/dolphin/gx/GXFrameBuf.c"),
            Object(Matching, "3rdParty/dolphin/gx/GXLight.c", extra_cflags=["-fp_contract off"]),
            Object(Matching, "3rdParty/dolphin/gx/GXTexture.c"),
            Object(Matching, "3rdParty/dolphin/gx/GXBump.c"),
            Object(Matching, "3rdParty/dolphin/gx/GXTev.c"),
            Object(NonMatching, "3rdParty/dolphin/gx/GXPixel.c"), # $VIOLET: non using the right sqrtf?
            Object(Matching, "3rdParty/dolphin/gx/GXDisplayList.c"),
            Object(Matching, "3rdParty/dolphin/gx/GXTransform.c", extra_cflags=["-fp_contract off"]),
            Object(Matching, "3rdParty/dolphin/gx/GXPerf.c"),
        ],
    ),
    DolphinLib(
        "gd",
        [
            Object(Matching, "3rdParty/dolphin/gd/GDBase.c"),
            Object(Matching, "3rdParty/dolphin/gd/GDGeometry.c"),
        ],
    ),
    DolphinLib(
        "ar",
        [
            Object(Matching, "3rdParty/dolphin/ar/ar.c"),
            Object(Matching, "3rdParty/dolphin/ar/arq.c"),
        ],
    ),
    DolphinLib(
        "texPalette",
        [
            Object(Matching, "3rdParty/dolphin/texPalette/texPalette.c"),
        ],
    ),
    {
        "lib": "Runtime.PPCEABI.H",
        "mw_version": config.linker_version,
        "cflags": cflags_runtime,
        "progress_category": "sdk",
        "host": False,
        "objects": [
            Object(NonMatching, "3rdParty/PowerPC_EABI_Support/Runtime/Src/__mem.c"),
            Object(Matching, "3rdParty/PowerPC_EABI_Support/Runtime/Src/__va_arg.c"),
            Object(NonMatching, "3rdParty/PowerPC_EABI_Support/Runtime/Src/global_destructor_chain.c"),
            Object(Matching, "3rdParty/PowerPC_EABI_Support/Runtime/Src/CPlusLibPPC.cp"),
            Object(NonMatching, "3rdParty/PowerPC_EABI_Support/Runtime/Src/NMWException.cp", extra_cflags=["-Cpp_exceptions on"]),
            Object(Matching, "3rdParty/PowerPC_EABI_Support/Runtime/Src/ptmf.c"),
            Object(Matching, "3rdParty/PowerPC_EABI_Support/Runtime/Src/runtime.c"),
            Object(NonMatching, "3rdParty/PowerPC_EABI_Support/Runtime/Src/__init_cpp_exceptions.cpp"),
            Object(NonMatching, "3rdParty/PowerPC_EABI_Support/Runtime/Src/Gecko_ExceptionPPC.cp"),
        ],
    },
    {
        "lib": "MSL_C",
        "mw_version": config.linker_version,
        "cflags": cflags_runtime,
        "progress_category": "sdk",
        "host": False,
        "objects": [
            Object(Matching, "3rdParty/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/abort_exit.c"),
            Object(NonMatching, "3rdParty/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/rand.c"),
            Object(NonMatching, "3rdParty/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/alloc.c"),
            Object(NonMatching, "3rdParty/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/errno.c"),
            Object(NonMatching, "3rdParty/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/ansi_files.c"),
            Object(NonMatching, "3rdParty/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Src/ansi_fp.c"),
            Object(NonMatching, "3rdParty/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/arith.c"),
            Object(NonMatching, "3rdParty/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/buffer_io.c"),
            Object(NonMatching, "3rdParty/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/char_io.c"),
            Object(Matching, "3rdParty/PowerPC_EABI_Support/MSL/MSL_C/PPC_EABI/Src/critical_regions.gamecube.c"),
            Object(Matching, "3rdParty/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/ctype.c"),
            Object(NonMatching, "3rdParty/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/direct_io.c"),
            Object(NonMatching, "3rdParty/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/file_io.c"),
            Object(NonMatching, "3rdParty/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/FILE_POS.c"),
            Object(NonMatching, "3rdParty/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/mbstring.c"),
            Object(NonMatching, "3rdParty/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/mem.c"),
            Object(NonMatching, "3rdParty/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/mem_funcs.c"),
            Object(NonMatching, "3rdParty/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/misc_io.c"),
            Object(NonMatching, "3rdParty/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/printf.c"),
            Object(NonMatching, "3rdParty/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/scanf.c"),
            Object(NonMatching, "3rdParty/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/float.c"),
            Object(Matching, "3rdParty/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/signal.c"),
            Object(NonMatching, "3rdParty/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/string.c"),
            Object(NonMatching, "3rdParty/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/strtold.c"),
            Object(NonMatching, "3rdParty/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/strtoul.c"),
            Object(Matching, "3rdParty/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/wchar_io.c"),
            Object(NonMatching, "3rdParty/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/qsort.c"),
            Object(Matching, "3rdParty/PowerPC_EABI_Support/MSL/MSL_C/PPC_EABI/Src/uart_console_io_gcn.c", extra_cflags=["-inline deferred,auto"]),
            Object(Matching, "3rdParty/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/e_acos.c"),
            Object(NonMatching, "3rdParty/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/e_log.c"),
            Object(Matching, "3rdParty/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/e_asin.c"),
            Object(Matching, "3rdParty/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/e_atan2.c"),
            Object(Matching, "3rdParty/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/e_exp.c"),
            Object(Matching, "3rdParty/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/e_fmod.c"),
            Object(Matching, "3rdParty/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/e_pow.c"),
            Object(Matching, "3rdParty/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/e_rem_pio2.c"),
            Object(Matching, "3rdParty/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/k_cos.c"),
            Object(Matching, "3rdParty/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/k_rem_pio2.c"),
            Object(Matching, "3rdParty/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/k_sin.c"),
            Object(Matching, "3rdParty/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/k_tan.c"),
            Object(Matching, "3rdParty/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_atan.c"),
            Object(Matching, "3rdParty/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_ceil.c"),
            Object(Matching, "3rdParty/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_copysign.c"),
            Object(Matching, "3rdParty/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_cos.c"),
            Object(Matching, "3rdParty/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_floor.c"),
            Object(Matching, "3rdParty/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_frexp.c"),
            Object(Matching, "3rdParty/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_ldexp.c"),
            Object(Matching, "3rdParty/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_modf.c"),
            Object(Matching, "3rdParty/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_sin.c"),
            Object(Matching, "3rdParty/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_tan.c"),
            Object(Matching, "3rdParty/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/w_acos.c"),
            Object(Matching, "3rdParty/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/w_asin.c"),
            Object(Matching, "3rdParty/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/w_atan2.c"),
            Object(Matching, "3rdParty/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/w_exp.c"),
            Object(Matching, "3rdParty/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/w_fmod.c"),
            Object(Matching, "3rdParty/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/w_pow.c"),
            Object(NonMatching, "3rdParty/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/w_log.c"),
            Object(Matching, "3rdParty/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/e_sqrt.c"),
            Object(NonMatching, "3rdParty/PowerPC_EABI_Support/MSL/MSL_C/PPC_EABI/Src/math_ppc.c"),
            Object(Matching, "3rdParty/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/w_sqrt.c"),
            Object(NonMatching, "3rdParty/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/extras.c"),
        ],
    },
    {
        "lib": "TRK_MINNOW_DOLPHIN",
        "mw_version": config.linker_version,
        "cflags": cflags_trk,
        "progress_category": "sdk",
        "host": False,
        "objects": [
            # debugger
            Object(Matching, "3rdParty/TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/mainloop.c"),
            Object(Matching, "3rdParty/TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/nubevent.c"),
            Object(Matching, "3rdParty/TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/nubinit.c"),
            Object(Matching, "3rdParty/TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/msg.c"),
            Object(Matching, "3rdParty/TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/msgbuf.c"),
            Object(Matching, "3rdParty/TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/serpoll.c", extra_cflags=["-sdata 8"]),
            Object(Matching, "3rdParty/TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Os/dolphin/usr_put.c"),
            Object(Matching, "3rdParty/TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/dispatch.c"),
            Object(Matching, "3rdParty/TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/msghndlr.c"),
            Object(Matching, "3rdParty/TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/support.c"),
            Object(Matching, "3rdParty/TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/mutex_TRK.c"),
            Object(Matching, "3rdParty/TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/notify.c"),
            Object(Matching, "3rdParty/TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Processor/ppc/Generic/flush_cache.c"),
            Object(Matching, "3rdParty/TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/mem_TRK.c"),
            Object(NonMatching, "3rdParty/TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Processor/ppc/Generic/targimpl.c"),
            Object(NonMatching, "3rdParty/TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Processor/ppc/Export/targsupp.s"),
            Object(Matching, "3rdParty/TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Processor/ppc/Generic/mpc_7xx_603e.c"),
            Object(Matching, "3rdParty/TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Processor/ppc/Generic/exception.s"),
            Object(Matching, "3rdParty/TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Os/dolphin/dolphin_trk.c"),
            Object(Matching, "3rdParty/TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/main_TRK.c"),
            Object(NonMatching, "3rdParty/TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Os/dolphin/dolphin_trk_glue.c"),
            Object(Matching, "3rdParty/TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Os/dolphin/targcont.c"),
            Object(Matching, "3rdParty/TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Os/dolphin/target_options.c"),
            Object(NonMatching, "3rdParty/TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Export/mslsupp.c"),
            Object(Matching, "3rdParty/TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Os/dolphin/UDP_Stubs.c"),

            # gamedev
            Object(Matching, "3rdParty/TRK_MINNOW_DOLPHIN/gamedev/cust_connection/cc/exi2/GCN/EXI2_DDH_GCN/main.c", extra_cflags=["-sdata 8"]),
            Object(Matching, "3rdParty/TRK_MINNOW_DOLPHIN/gamedev/cust_connection/utils/common/CircleBuffer.c"),
            Object(Matching, "3rdParty/TRK_MINNOW_DOLPHIN/gamedev/cust_connection/cc/exi2/GCN/EXI2_GDEV_GCN/main.c", extra_cflags=["-sdata 8"]),
            Object(Matching, "3rdParty/TRK_MINNOW_DOLPHIN/gamedev/cust_connection/utils/common/MWTrace.c"),
            Object(NonMatching, "3rdParty/TRK_MINNOW_DOLPHIN/gamedev/cust_connection/utils/gc/MWCriticalSection_gc.cpp", extra_cflags=["-lang=c"]),
        ],
    },
    {
        "lib": "amcstubs",
        "mw_version": config.linker_version,
        "cflags": cflags_dolphin,
        "progress_category": "sdk",
        "host": False,
        "objects": [
            Object(Matching, "3rdParty/amcstubs/AmcExi2Stubs.c"),
        ],
    },
    {
        "lib": "odemuexi2",
        "mw_version": "GC/1.2.5n",
        "cflags": cflags_runtime,
        "progress_category": "sdk",
        "host": False,
        "objects": [
            Object(Matching, "3rdParty/odemuexi2/DebuggerDriver.c", extra_cflags=["-inline deferred,auto"]),
        ],
    },
    {
        "lib": "odenotstub",
        "mw_version": config.linker_version,
        "cflags": cflags_dolphin,
        "progress_category": "sdk",
        "host": False,
        "objects": [
            Object(Matching, "3rdParty/odenotstub/odenotstub.c"),
        ],
    },
    {
        "lib": "Engine",
        "mw_version": config.linker_version,
        "cflags": cflags_rat_base,
        "progress_category": ["engine", "rat"],  # str | List[str]
        "objects": [
            Object(Matching, "Engine/InGameDatas_Z.cpp"),
            Object(NonMatching, "Engine/ObjectBankManager_Z.cpp",extra_cflags=["-O4,s","-inline off"]),
            Object(NonMatching, "Engine/GameManager_Z.cpp"),
            Object(NonMatching, "Engine/Math_Z.cpp"),
            Object(NonMatching, "Engine/ObjectsMoveCollShadow_Z.cpp"),
            Object(NonMatching, "Engine/InGameObjects_Z.cpp"),
            Object(NonMatching, "Engine/SuperSprayEle_Z.cpp"),
            Object(NonMatching, "Engine/GameSub_Z.cpp"),
            Object(Matching,    "Engine/StreamFile_Z.cpp"),
            Object(NonMatching, "Engine/LodMoveColl_Z.cpp"),
            Object(NonMatching, "Engine/LodMove_Z.cpp"),
            Object(NonMatching, "Engine/LodAgent_Z.cpp"),
            Object(NonMatching, "Engine/CameraZoneLoad_Z.cpp"),
            Object(Matching,    "Engine/Txt_Z.cpp"),
            Object(Matching,    "Engine/PointsLoad_Z.cpp"),
            Object(NonMatching, "Engine/Menu3DLoadAscii_Z.cpp"),
            Object(NonMatching, "Engine/LodShadow_Z.cpp"),
            Object(NonMatching, "Engine/Surface_Z.cpp"),
            Object(NonMatching, "Engine/ObjectMoveColl_Z.cpp"),
            Object(NonMatching, "Engine/SurfaceCollision_Z.cpp"),
            Object(NonMatching, "Engine/GameSubData_Z.cpp"),
            Object(NonMatching, "Engine/CollisionVolDraw_Z.cpp"),
            Object(NonMatching, "Engine/ParticlesAgent_Z.cpp"),
            Object(NonMatching, "Engine/CameraEngineZoneCapture_Z.cpp"),
            Object(Matching,    "Engine/VolatileMem_Z.cpp"),
            Object(NonMatching, "Engine/ObjectsGameClip_Z.cpp"),
            Object(NonMatching, "Engine/MeshLoad_Z.cpp"),
            Object(NonMatching, "Engine/CameraZoneDraw_Z.cpp"),
            Object(NonMatching, "Engine/Movie_Z.cpp"),
            Object(NonMatching, "Engine/SurfaceCollisionSphere_Z.cpp"),
            Object(NonMatching, "Engine/Viewport_Z.cpp"),
            Object(NonMatching, "Engine/ABC_Agent.cpp"),
            Object(NonMatching, "Engine/WorldLightning_Z.cpp"),
            Object(NonMatching, "Engine/HFog_Z.cpp"),
            Object(Matching,    "Engine/AnimationMaterial_Z.cpp"),
            Object(Matching,    "Engine/AnimationMorph_Z.cpp"),
            Object(Matching,    "Engine/Manipulator_Z.cpp"),
            Object(Matching,    "Engine/Parameters_Z.cpp"),
            Object(NonMatching, "Engine/MovieManipulator_Z.cpp"),
            Object(Matching,    "Engine/Language_Z.cpp"),
            Object(NonMatching, "Engine/SkelLoad_Z.cpp"),
            Object(NonMatching, "Engine/Emiter_Z.cpp"),
            Object(NonMatching, "Engine/ErrorLanguage_Z.cpp"),
            Object(NonMatching, "Engine/RotShapeLoad_Z.cpp"),
            Object(NonMatching, "Engine/ObjectsBounce_Z.cpp"),
            Object(NonMatching, "Engine/LodDraw_Z.cpp"),
            Object(NonMatching, "Engine/WorldAnim_Z.cpp"),
            Object(NonMatching, "Engine/LightDraw_Z.cpp"),
            Object(NonMatching, "Engine/Gaussian_Z.cpp"),
            Object(Matching,    "Engine/EffectManager_Z.cpp"),
            Object(NonMatching, "Engine/SurfaceCollisionCapsule_Z.cpp"),
            Object(NonMatching, "Engine/CameraUser_Z.cpp"),
            Object(NonMatching, "Engine/ParticlesEle_Z.cpp"),
            Object(Matching,    "Engine/ConsoleCommand_Z.cpp"),
            Object(NonMatching, "Engine/Menu3D_Z.cpp"),
            Object(NonMatching, "Engine/Pack_Z.cpp"),
            Object(NonMatching, "Engine/Occluder_Z.cpp"),
            Object(NonMatching, "Engine/SurfaceCollisionTools_Z.cpp"),
            Object(Matching,    "Engine/BaseObject_Z.cpp"),
            Object(NonMatching, "Engine/EmiterCollision_Z.cpp"),
            Object(NonMatching, "Engine/KeyframerFloat_Z.cpp"),
            Object(NonMatching, "Engine/GraphVertex_Z.cpp"),
            Object(Matching,    "Engine/Memory_Z.cpp"),
            Object(NonMatching, "Engine/GameManipulators_Z.cpp"),
            Object(NonMatching, "Engine/Warp_Z.cpp"),
            Object(NonMatching, "Engine/InGameObjectsManipulator_Z.cpp"),
            Object(NonMatching, "Engine/SplineLoad_Z.cpp"),
            Object(NonMatching, "Engine/DynamicAnim_Z.cpp"),
            Object(NonMatching, "Engine/InGameDatasDraw_Z.cpp"),
            Object(NonMatching, "Engine/MeshCollisionMovingSphere.cpp"),
            Object(NonMatching, "Engine/SkinDraw_Z.cpp"),
            Object(NonMatching, "Engine/OccluderDraw_Z.cpp"),
            Object(NonMatching, "Engine/SystemObject_Z.cpp"),
            Object(NonMatching, "Engine/ObjectsBreak_Z.cpp"),
            Object(NonMatching, "Engine/OmniDraw_Z.cpp"),
            Object(NonMatching, "Engine/Frustrum_Z.cpp"),
            Object(NonMatching, "Engine/WorldCollision_Z.cpp"),
            Object(NonMatching, "Engine/LodLighting_Z.cpp"),
            Object(NonMatching, "Engine/MovingAgent_Z.cpp"),
            Object(NonMatching, "Engine/MeshCollisionCapsule_Z.cpp"),
            Object(NonMatching, "Engine/GameStream_Z.cpp"),
            Object(Matching,    "Engine/Points_Z.cpp"),
            Object(NonMatching, "Engine/GameManagerRtc_Z.cpp"),
            Object(NonMatching, "Engine/SkinCollision_Z.cpp"),
            Object(NonMatching, "Engine/MatrixCache_Z.cpp"),
            Object(NonMatching, "Engine/SurfaceCache_Z.cpp"),
            Object(Matching,    "Engine/ABC_Category.cpp"),
            Object(Matching,    "Engine/AnimatedMsgAgent_Z.cpp"),
            Object(NonMatching, "Engine/StreamAgent_Z.cpp"),
            Object(NonMatching, "Engine/ObjectMoveCollShadow_Z.cpp"),
            Object(NonMatching, "Engine/SystemDatas_Z.cpp",extra_cflags=["-O4,s","-inline off"]),
            Object(NonMatching, "Engine/ParticlesLoad_Z.cpp"),
            Object(Matching,    "Engine/SkinAgent_Z.cpp"),
            Object(NonMatching, "Engine/MeshCollision_Z.cpp"),
            Object(NonMatching, "Engine/CameraAgent_Z.cpp"),
            Object(NonMatching, "Engine/InputEngine_Z.cpp"),
            Object(NonMatching, "Engine/KSys_Z.cpp"),
            Object(NonMatching, "Engine/SplineDraw_Z.cpp"),
            Object(Matching,    "Engine/BitmapLoad_Z.cpp"),
            Object(NonMatching, "Engine/Game_Z.cpp"),
            Object(NonMatching, "Engine/ObjectThrow_Z.cpp"),
            Object(NonMatching, "Engine/Rtc_Z.cpp"),
            Object(Matching,    "Engine/AnimationOmni_Z.cpp"),
            Object(NonMatching, "Engine/KeyframerrVec3f_Z.cpp"),
            Object(Matching,    "Engine/ManipulatorManager_Z.cpp"),
            Object(NonMatching, "Engine/ObjectsGame_Z.cpp"),
            Object(NonMatching, "Engine/GraphLoad_Z.cpp"),
            Object(NonMatching, "Engine/CameraEngine_Z.cpp"),
            Object(NonMatching, "Engine/ObjectsMoveColl_Z.cpp"),
            Object(Matching,    "Engine/ABC_AgentReserve.cpp"),
            Object(NonMatching, "Engine/HFogDraw_Z.cpp"),
            Object(Matching,    "Engine/Name_Z.cpp"),
            Object(NonMatching, "Engine/LodCollisionMovingSphere_Z.cpp"),
            Object(Matching,    "Engine/Sound_Z.cpp"),
            Object(NonMatching, "Engine/Sys_Z.cpp"),
            Object(Matching,    "Engine/UserDefine_Z.cpp"),
            Object(NonMatching, "Engine/MeshVolume_Z.cpp"),
            Object(NonMatching, "Engine/ObjectMoveCollNode_Z.cpp"),
            Object(NonMatching, "Engine/Renderer_Z.cpp",extra_cflags=["-O4,s"]),
            Object(NonMatching, "Engine/InGameObjectsDraw_Z.cpp"),
            Object(NonMatching, "Engine/ObjectGame_Z.cpp"),
            Object(NonMatching, "Engine/LightLoad_Z.cpp"),
            Object(Matching,    "Engine/CameraOccluder_Z.cpp"),
            Object(NonMatching, "Engine/SequenceMixAnim_Z.cpp"),
            Object(NonMatching, "Engine/SubLevelVolumeAgent_Z.cpp"),
            Object(NonMatching, "Engine/Tree_Z.cpp"),
            Object(NonMatching, "Engine/LodMoveCollCollectable_Z.cpp"),
            Object(Matching,    "Engine/AnimationCamera_Z.cpp"),
            Object(NonMatching, "Engine/ObjectsBreakColl_Z.cpp"),
            Object(NonMatching, "Engine/RotShapeCollision_Z.cpp"),
            Object(NonMatching, "Engine/PlayAnim_Z.cpp"),
            Object(NonMatching, "Engine/KeyframerrVec2f_Z.cpp"),
            Object(NonMatching, "Engine/ObjectThrowColl_Z.cpp"),
            Object(NonMatching, "Engine/SubWorldLoad_Z.cpp"),
            Object(NonMatching, "Engine/GraphDraw_Z.cpp"),
            Object(NonMatching, "Engine/SeadIndex_Z.cpp"),
            Object(NonMatching, "Engine/UnSharedCollisionTool_Z.cpp"),
            Object(Matching,    "Engine/ObjectMove_Z.cpp"),
            Object(NonMatching, "Engine/Menu3DDraw_Z.cpp"),
            Object(NonMatching, "Engine/MeshAABB_Z.cpp"),
            Object(NonMatching, "Engine/MaterialAnimLoad_Z.cpp"),
            Object(Matching,    "Engine/Console_Z.cpp"),
            Object(NonMatching, "Engine/SelfRunning_Z.cpp"),
            Object(Matching,    "Engine/BitArray_Z.cpp"),
            Object(NonMatching, "Engine/CameraZone_Z.cpp"),
            Object(Matching,    "Engine/String_Z.cpp"),
            Object(Matching,    "Engine/StreamManager_Z.cpp"),
            Object(NonMatching, "Engine/GameManagerCmd_Z.cpp"),
            Object(Matching,    "Engine/Animation_Z.cpp"),
            Object(NonMatching, "Engine/LodMoveCollCling_Z.cpp"),
            Object(NonMatching, "Engine/KeyframerRot_Z.cpp"),
            Object(NonMatching, "Engine/ObjectsBreakCollShadow_Z.cpp"),
            Object(NonMatching, "Engine/ClassManager_Z.cpp"),
            Object(NonMatching, "Engine/Collision_Z.cpp"),
            Object(NonMatching, "Engine/BoneNode_Z.cpp"),
            Object(NonMatching, "Engine/Node_Z.cpp"),
            Object(NonMatching, "Engine/ObjectMoveCollVol_Z.cpp"),
            Object(NonMatching, "Engine/Camera_Z.cpp"),
            Object(NonMatching, "Engine/FlareDraw_Z.cpp"),
            Object(Matching,    "Engine/ResourceObject_Z.cpp"),
            Object(NonMatching, "Engine/BigFileManager_Z.cpp"),
            Object(NonMatching, "Engine/SkinCollisionMovingSphere_Z.cpp"),
            Object(NonMatching, "Engine/AStar_Z.cpp"),
            Object(NonMatching, "Engine/Ghost_Z.cpp"),
            Object(NonMatching, "Engine/AnimFrameLoad_Z.cpp"),
            Object(Matching,    "Engine/AnimationLoad_Z.cpp"),
            Object(NonMatching, "Engine/RendererPrimitiveBuffers_Z.cpp"),
            Object(NonMatching, "Engine/WaterHeightmap_Z.cpp"),
            Object(NonMatching, "Engine/Light_Z.cpp"),
            Object(NonMatching, "Engine/AnimFrame_Z.cpp"),
            Object(NonMatching, "Engine/SeadHandle_Z.cpp"),
            Object(NonMatching, "Engine/Menu_Z.cpp"),
            Object(NonMatching, "Engine/MeshCollisionSphere.cpp"),
            Object(NonMatching, "Engine/SkelCollision_Z.cpp"),
            Object(NonMatching, "Engine/CollisionVolCollision_Z.cpp"),
            Object(NonMatching, "Engine/SubWorld_Z.cpp"),
            Object(NonMatching, "Engine/Lod_Z.cpp"),
            Object(NonMatching, "Engine/MeshDraw_Z.cpp"),
            Object(NonMatching, "Engine/InGameDatasLoadAscii_Z.cpp"),
            Object(NonMatching, "Engine/ObjectsMove_Z.cpp"),
            Object(NonMatching, "Engine/RotShape_Z.cpp"),
            Object(NonMatching, "Engine/DynamicAnim1_Z.cpp"),
            Object(NonMatching, "Engine/RtcAgent_Z.cpp"),
            Object(NonMatching, "Engine/OccluderCollision_Z.cpp"),
            Object(NonMatching, "Engine/File_Z.cpp", extra_cflags=["-O4,s","-inline off"]),
            Object(NonMatching, "Engine/SkinLoad_Z.cpp"),
            Object(NonMatching, "Engine/MaterialAnim_Z.cpp"),
            Object(NonMatching, "Engine/Flare_Z.cpp"),
            Object(NonMatching, "Engine/VehiculeMove_Z.cpp"),
            Object(NonMatching, "Engine/RotShapeDraw_Z.cpp"),
            Object(NonMatching, "Engine/Bitmap_Z.cpp", extra_cflags=["-O4,s"]),
            Object(NonMatching, "Engine/AnimatedAgent_Z.cpp"),
            Object(Matching,    "Engine/ABC_ScriptManager.cpp"),
            Object(NonMatching, "Engine/LodCollisions_Z.cpp"),
            Object(NonMatching, "Engine/SurfaceDraw_Z.cpp"),
            Object(NonMatching, "Engine/Error_Z.cpp"),
            Object(Matching,    "Engine/ClassNameResManager_Z.cpp"),
            Object(NonMatching, "Engine/SoundManager_Z.cpp"),
            Object(NonMatching, "Engine/AnimationManager_Z.cpp"),
            Object(NonMatching, "Engine/InGameObjectsLoadAscii_Z.cpp"),
            Object(NonMatching, "Engine/PopupMenu_Z.cpp"),
            Object(NonMatching, "Engine/SequenceAnim_Z.cpp"),
            Object(NonMatching, "Engine/WorldDraw_Z.cpp"),
            Object(NonMatching, "Engine/LodLoad_Z.cpp"),
            Object(NonMatching, "Engine/Menu2D_Z.cpp"),
            Object(NonMatching, "Engine/Handle_Z.cpp"),
            Object(NonMatching, "Engine/GameRtc_Z.cpp"),
            Object(NonMatching, "Engine/EmiterDraw_Z.cpp"),
            Object(Matching,    "Engine/AnimationManagerSkel_Z.cpp"),
            Object(Matching,    "Engine/CameraEngineZoneColl_Z.cpp"),
            Object(NonMatching, "Engine/Breakable_Z.cpp"),
            Object(Matching,    "Engine/Material_Z.cpp"),
            Object(NonMatching, "Engine/ObjectsGameSkel_Z.cpp"),
            Object(NonMatching, "Engine/DynamicAnim2_Z.cpp"),
            Object(NonMatching, "Engine/Agent_Z.cpp"),
            Object(Matching,    "Engine/RtcAnimationNode_Z.cpp"),
            Object(NonMatching, "Engine/WorldLoad_Z.cpp"),
            Object(NonMatching, "Engine/CollisionVolLoad_Z.cpp"),
            Object(NonMatching, "Engine/GameObj_Z.cpp"),
            Object(NonMatching, "Engine/Graph_Z.cpp"),
            Object(NonMatching, "Engine/Skin_Z.cpp"),
            Object(NonMatching, "Engine/InputAction_Z.cpp"),
            Object(NonMatching, "Engine/S_MALLOC.cpp"),
            Object(NonMatching, "Engine/SkinCollisionSphere_Z.cpp"),
            Object(NonMatching, "Engine/World_Z.cpp"),
            Object(NonMatching, "Engine/MaterialObj_Z.cpp"),
            Object(NonMatching, "Engine/MaterialManager_Z.cpp"),
            Object(Matching,    "Engine/TriangleCache_Z.cpp"),
            Object(Matching,    "Engine/Random_Z.cpp"),
            Object(NonMatching, "Engine/SurfaceLoad_Z.cpp"),
            Object(NonMatching, "Engine/LodMoveCollShadow_Z.cpp"),
            Object(NonMatching, "Engine/GraphLevel_Z.cpp"),
            Object(NonMatching, "Engine/SplineZoneLoad_Z.cpp"),
            Object(Matching,    "Engine/KeyframerLoad_Z.cpp"),
            Object(NonMatching, "Engine/Main_Z.cpp"),
            Object(Matching,    "Engine/Object_Z.cpp"),
            Object(NonMatching, "Engine/Menu2DDraw_Z.cpp"),
            Object(NonMatching, "Engine/WarpLoad_Z.cpp"),
            Object(NonMatching, "Engine/ConsoleInterp_Z.cpp"),
            Object(NonMatching, "Engine/RtcLoad_Z.cpp"),
            Object(NonMatching, "Engine/SuperSpray_Z.cpp"),
            Object(Matching,    "Engine/NetManager_Virtual_Z.cpp"),
            Object(NonMatching, "Engine/OccludedSeadHandle_Z.cpp"),
            Object(NonMatching, "Engine/CameraEngineZone_Z.cpp"),
            Object(NonMatching, "Engine/ObjectCollisionTools_Z.cpp"),
            Object(Matching,    "Engine/ABC_AgentMessage.cpp"),
            Object(NonMatching, "Engine/Fonts_Z.cpp"),
            Object(NonMatching, "Engine/PlayParticles_Z.cpp"),
            Object(Matching,    "Engine/NetManager_Z.cpp"),
            Object(Matching,    "Engine/AnimationMesh_Z.cpp"),
            Object(NonMatching, "Engine/CollisionVol_Z.cpp"),
            Object(NonMatching, "Engine/LodCollision_Z.cpp"),
            Object(NonMatching, "Engine/RtcPlayer_Z.cpp"),
            Object(Matching,    "Engine/ABC_Behavior.cpp"),
            Object(NonMatching, "Engine/UnPack_Z.cpp"),
            Object(NonMatching, "Engine/CameraStream_Z.cpp"),
            Object(NonMatching, "Engine/DebugTools_Z.cpp"),
            Object(NonMatching, "Engine/SurfaceCling_Z.cpp"),
            Object(Matching,    "Engine/KeyframerU32_Z.cpp"),
            Object(NonMatching, "Engine/HFogLoad_Z.cpp"),
            Object(NonMatching, "Engine/LodMoveCollNode_Z.cpp"),
            Object(Matching,    "Engine/KeyframerHdl_Z.cpp"),
            Object(NonMatching, "Engine/NodeLoad_Z.cpp"),
            Object(Matching,    "Engine/MaterialLoad_Z.cpp"),
            Object(NonMatching, "Engine/SplineZoneDraw_Z.cpp"),
            Object(NonMatching, "Engine/CollisionTool_Z.cpp"),
            Object(NonMatching, "Engine/KeyframerVec4f_Z.cpp"),
            Object(NonMatching, "Engine/UnSharedObjectCollisionTools_Z.cpp"),
            Object(NonMatching, "Engine/SurfaceCollisionMovingSphere_Z.cpp"),
            Object(Matching,    "Engine/AnimationManagerMes_Z.cpp"),
            Object(NonMatching, "Engine/GameAgent_Z.cpp"),
            Object(NonMatching, "Engine/AnimMessage_Z.cpp"),
            Object(NonMatching, "Engine/Omni_Z.cpp"),
            Object(NonMatching, "Engine/LodCollisionSphere_Z.cpp"),
            Object(NonMatching, "Engine/ABC_ScriptManagerReserve_Z.cpp"),
            Object(NonMatching, "Engine/ObjectsMoveCollNode_Z.cpp"),
            Object(Matching,    "Engine/LodDynAgent_Z.cpp"),
            Object(NonMatching, "Engine/LodMoveCollCarrying_Z.cpp"),
            Object(NonMatching, "Engine/Particles_Z.cpp"),
            Object(Matching,    "Engine/AnimationNode_Z.cpp"),
            Object(NonMatching, "Engine/Throwable_Z.cpp"),
            Object(NonMatching, "Engine/FontsDraw_Z.cpp"),
            Object(NonMatching, "Engine/Menu2DLoadAscii_Z.cpp"),
            Object(Matching,    "Engine/NetDataSet_Z.cpp"),
            Object(NonMatching, "Engine/Mesh_Z.cpp"),
            Object(Matching,    "Engine/CameraDraw_Z.cpp"),
            Object(NonMatching, "Engine/Spline_Z.cpp"),
            Object(NonMatching, "Engine/ConsoleMessage_Z.cpp"),
            Object(NonMatching, "Engine/Skel_Z.cpp"),
            Object(Matching,    "Engine/MeshMorph_Z.cpp"),
            Object(NonMatching, "Engine/SplineZone_Z.cpp"),
            Object(NonMatching, "Engine/ParticlesDraw_Z.cpp"),
            Object(Matching,    "Engine/VolumeAgent_Z.cpp"),
        ],
    },
    {
        "lib": "LibGC",
        "mw_version": config.linker_version,
        "cflags": cflags_rat_base,
        "progress_category": ["libgc", "rat"],  # str | List[str]
        "objects": [
            Object(Matching,    "LibGC/Streaming_Z.cpp"),
            Object(NonMatching, "LibGC/MathTools_Z.cpp"),
            Object(Matching,    "LibGC/LowMemory_Z.cpp"),
            Object(NonMatching, "LibGC/ARAM_Z.cpp"),
            Object(Matching,    "LibGC/Assert_Z.cpp"),
            Object(Matching,    "LibGC/String_Z.c"),
            Object(Matching,    "LibGC/Timer_Z.cpp"),
            Object(NonMatching, "LibGC/TextureManager_Z.cpp"),
            Object(Matching,    "LibGC/DisplayList_Z.cpp"),
            Object(NonMatching, "LibGC/MeshGCDraw_Z.cpp"),
            Object(Matching,    "LibGC/GCConsoleSystem_Z.cpp"),
            Object(NonMatching, "LibGC/SurfaceGC_Z.cpp"),
            Object(NonMatching, "LibGC/SystemObjectGC_Z.cpp"),
            Object(NonMatching, "LibGC/GCRenderer3D_Z.cpp"),
            Object(NonMatching, "LibGC/GCMovieManipulator_Z.cpp"),
            Object(NonMatching, "LibGC/SkinGCDraw_Z.cpp"),
            Object(NonMatching, "LibGC/SkelGC_Z.cpp"),
            Object(NonMatching, "LibGC/FlareGC_Z.cpp"),
            Object(NonMatching, "LibGC/GCSound_Z.cpp"),
            Object(NonMatching, "LibGC/InGameObjectsGC_Z.cpp"),
            Object(NonMatching, "LibGC/WarpGC_Z.cpp"),
            Object(NonMatching, "LibGC/RotShapeGC_Z.cpp"),
            Object(NonMatching, "LibGC/ParticlesGC_Z.cpp"),
            Object(NonMatching, "LibGC/RotShapeGCDraw_Z.cpp"),
            Object(NonMatching, "LibGC/GCMcManager_Z.cpp"),
            Object(NonMatching, "LibGC/FlareGCDraw_Z.cpp"),
            Object(NonMatching, "LibGC/GCRendererTransp_Z.cpp"),
            Object(NonMatching, "LibGC/GCFile_Z.cpp"),
            Object(Matching,    "LibGC/MaterialGC_Z.cpp"),
            Object(Matching,    "LibGC/LodGC_Z.cpp"),
            Object(NonMatching, "LibGC/MathGC_Z.cpp"),
            Object(NonMatching, "LibGC/GCDrawString.cpp"),
            Object(NonMatching, "LibGC/GCRendererLighting_Z.cpp"),
            Object(NonMatching, "LibGC/LodGCDraw_Z.cpp"),
            Object(NonMatching, "LibGC/ParticlesGCDraw_Z.cpp"),
            Object(NonMatching, "LibGC/GCFrustrum_Z.cpp"),
            Object(Matching,    "LibGC/GCStreamManager_Z.cpp"),
            Object(NonMatching, "LibGC/GCSoundManager_Z.cpp"),
            Object(Matching,    "LibGC/MeshGC_Z.cpp"),
            Object(NonMatching, "LibGC/SurfaceGCDraw_Z.cpp"),
            Object(NonMatching, "LibGC/GCRendererDraw_Z.cpp"),
            Object(NonMatching, "LibGC/GCSndStream_Z.cpp"),
            Object(Matching,    "LibGC/GCRenderer_Z.cpp"),
            Object(NonMatching, "LibGC/GCConsole_Z.cpp"),
            Object(NonMatching, "LibGC/WarpGCDraw_Z.cpp"),
            Object(NonMatching, "LibGC/GCSoundStreaming_Z.cpp"),
            Object(NonMatching, "LibGC/SkinGC_Z.cpp"),
            Object(NonMatching, "LibGC/GCInput_Z.cpp"),
            Object(Matching,    "LibGC/GCSoundBF.cpp"),
            Object(NonMatching, "LibGC/GCMovie_Z.cpp"),
            Object(NonMatching, "LibGC/GCMain_Z.cpp"),
            Object(Matching,    "LibGC/GCConsoleFolder_Z.cpp"),
            Object(NonMatching, "LibGC/GCRenderer2D_Z.cpp"),
            Object(NonMatching, "LibGC/SurfaceGCCache_Z.cpp"),
            Object(NonMatching, "LibGC/wiitextures.c"),
            Object(Matching,    "LibGC/LowLevelTools_Z.cpp"),
        ],
    },
    {
        "lib": "Game",
        "mw_version": config.linker_version,
        "cflags": cflags_rat_base,
        "progress_category": ["game", "rat"],  # str | List[str]
        "objects": [
            Object(NonMatching, "Rat/Mission_Heist.cpp"),
            Object(NonMatching, "Rat/Mission_LinguiniMaking.cpp"),
            Object(NonMatching, "Rat/MenuManager_G.cpp"),
            Object(NonMatching, "Rat/CameraGame_G.cpp"),
            Object(NonMatching, "Rat/IsToBePlayed.cpp"),
            Object(NonMatching, "Rat/CreatureDeaths.cpp"),
            Object(NonMatching, "Rat/CreaturesBallMoveStates_G.cpp"),
            Object(NonMatching, "Rat/DummyGen.cpp"),
            Object(NonMatching, "Rat/CreatureFightStates_G.cpp"),
            Object(NonMatching, "Rat/BriefInfo.cpp"),
            Object(NonMatching, "Rat/Mission_Dream.cpp"),
            Object(NonMatching, "Rat/Mission_CookingPlateInterface.cpp"),
            Object(NonMatching, "Rat/PlayerMove_G.cpp"),
            Object(NonMatching, "Rat/Menu3DScenery.cpp"),
            Object(NonMatching, "Rat/VolumeDelayedStart_G.cpp"),
            Object(NonMatching, "Rat/Mission_Wii_Fish.cpp"),
            Object(NonMatching, "Rat/LogicAgent_G.cpp"),
            Object(NonMatching, "Rat/Mission_CollectTrigger.cpp"),
            Object(NonMatching, "Rat/GameMgr_G.cpp"),
            Object(NonMatching, "Rat/CreatureImpactStates_G.cpp"),
            Object(NonMatching, "Rat/IT_CarryingMove.cpp"),
            Object(NonMatching, "Rat/CharsData_G.cpp"),
            Object(NonMatching, "Rat/LoadingDraw_G.cpp"),
            Object(NonMatching, "Rat/IT_DYNOBJMove_G.cpp"),
            Object(NonMatching, "Rat/CreaturesWaitStates_G.cpp"),
            Object(NonMatching, "Rat/Mission_Wii_Wash.cpp"),
            Object(NonMatching, "Rat/FriendsMove_G.cpp"),
            Object(NonMatching, "Rat/InGameSounds_G.cpp"),
            Object(NonMatching, "Rat/CreaturesSwim_G.cpp"),
            Object(NonMatching, "Rat/Heros_G.cpp"),
            Object(NonMatching, "Rat/CreaturesPoleStates_G.cpp"),
            Object(NonMatching, "Rat/Shop.cpp"),
            Object(NonMatching, "Rat/ActionHelper_G.cpp"),
            Object(NonMatching, "Rat/CreaturesBoneManip.cpp"),
            Object(NonMatching, "Rat/InGameCreatures_G.cpp"),
            Object(NonMatching, "Rat/ObjectAgent_G.cpp"),
            Object(NonMatching, "Rat/MenuKeys.cpp"),
            Object(NonMatching, "Rat/StreamGameAgent_G.cpp"),
            Object(NonMatching, "Rat/Dialogs_Extras.cpp"),
            Object(NonMatching, "Rat/Mission_Goto.cpp"),
            Object(NonMatching, "Rat/IT_BallWalk_G.cpp"),
            Object(NonMatching, "Rat/BaseInGame3D.cpp"),
            Object(NonMatching, "Rat/ItemMgr_G.cpp"),
            Object(NonMatching, "Rat/PhysicObj_G.cpp"),
            Object(NonMatching, "Rat/InGamePlayerPages_G.cpp"),
            Object(NonMatching, "Rat/CreaturesStates_G.cpp"),
            Object(NonMatching, "Rat/Mission_Wii_CrepeInterface.cpp"),
            Object(NonMatching, "Rat/Frame_G.cpp"),
            Object(NonMatching, "Rat/CreatureStatic.cpp"),
            Object(NonMatching, "Rat/LiquidFlow_G.cpp"),
            Object(NonMatching, "Rat/Mission_Multi_Collect.cpp"),
            Object(NonMatching, "Rat/MissionTemplate_G.cpp"),
            Object(NonMatching, "Rat/CreatureGlissade.cpp"),
            Object(NonMatching, "Rat/Game_SoundMgr_G.cpp"),
            Object(NonMatching, "Rat/CreatureSkid.cpp"),
            Object(NonMatching, "Rat/Mission_CuttingPotatoes.cpp"),
            Object(NonMatching, "Rat/CreatureControlPanel.cpp"),
            Object(NonMatching, "Rat/CreaturesPushButtonStates_G.cpp"),
            Object(NonMatching, "Rat/CreatureRat_G.cpp"),
            Object(NonMatching, "Rat/EvenementVolAgent_G.cpp"),
            Object(NonMatching, "Rat/Mission_ControlPanel.cpp"),
            Object(NonMatching, "Rat/MenuLogicAgent_G.cpp"),
            Object(NonMatching, "Rat/GusteauMove.cpp"),
            Object(NonMatching, "Rat/VibrationManager_G.cpp"),
            Object(NonMatching, "Rat/CameraAgent_G.cpp"),
            Object(NonMatching, "Rat/CreatureSemaphore.cpp"),
            Object(NonMatching, "Rat/IT_Switch.cpp"),
            Object(NonMatching, "Rat/Mission_CookingSoupInterfaceYesNo.cpp"),
            Object(NonMatching, "Rat/Mission_Colony.cpp"),
            Object(NonMatching, "Rat/Mission_WireConnect.cpp"),
            Object(NonMatching, "Rat/CreatureBumper.cpp"),
            Object(NonMatching, "Rat/Mission_LinguiniMakingInterface.cpp"),
            Object(NonMatching, "Rat/ScriptExternalCommands_G.cpp"),
            Object(NonMatching, "Rat/TextGameDraw_G.cpp"),
            Object(NonMatching, "Rat/FlyUpVolumeAgent_G.cpp"),
            Object(NonMatching, "Rat/IT_FishMove.cpp"),
            Object(NonMatching, "Rat/LevelManipulator_G.cpp"),
            Object(NonMatching, "Rat/CameraEdited.cpp"),
            Object(NonMatching, "Rat/Smell_G.cpp"),
            Object(NonMatching, "Rat/PhysicColl_G.cpp"),
            Object(NonMatching, "Rat/EnemyGenerator_G.cpp"),
            Object(NonMatching, "Rat/Mission_CookingLinguiniInterface.cpp"),
            Object(NonMatching, "Rat/Config_G.cpp"),
            Object(NonMatching, "Rat/BaseInGameDatas_G.cpp"),
            Object(NonMatching, "Rat/CreaturePST_TOOLS_G.cpp"),
            Object(NonMatching, "Rat/InGamePlayer_G.cpp"),
            Object(Matching, "Rat/ResName.cpp"),
            Object(NonMatching, "Rat/Dialogs_LoadSave.cpp"),
            Object(NonMatching, "Rat/LightVolumeAgent_G.cpp"),
            Object(NonMatching, "Rat/Mission_MiniGame.cpp"),
            Object(NonMatching, "Rat/CreaturesManager_G.cpp"),
            Object(NonMatching, "Rat/RtcScriptAgent_G.cpp"),
            Object(NonMatching, "Rat/Mission_Wii_WashInterface.cpp"),
            Object(NonMatching, "Rat/CreatureFishing.cpp"),
            Object(NonMatching, "Rat/IT_Fish_G.cpp"),
            Object(NonMatching, "Rat/MultiMapLevel.cpp"),
            Object(NonMatching, "Rat/GuardAgent_G.cpp"),
            Object(NonMatching, "Rat/DialogGroup_G.cpp"),
            Object(NonMatching, "Rat/WanderingPath_G.cpp"),
            Object(NonMatching, "Rat/GangAgent_G.cpp"),
            Object(NonMatching, "Rat/FootPrints_G.cpp"),
            Object(NonMatching, "Rat/AutoCompletion_G.cpp"),
            Object(NonMatching, "Rat/ThrownMove_G.cpp"),
            Object(NonMatching, "Rat/Mission_MutliTemplate_G.cpp"),
            Object(NonMatching, "Rat/Mission_WireConnectInterface.cpp"),
            Object(NonMatching, "Rat/Mission_CuttingPotatoesInterface.cpp"),
            Object(NonMatching, "Rat/ChangeLevelVolumeAgent_G.cpp"),
            Object(NonMatching, "Rat/Friends_G.cpp"),
            Object(NonMatching, "Rat/Dialogs_System.cpp"),
            Object(NonMatching, "Rat/ScriptExternals_G.cpp"),
            Object(NonMatching, "Rat/LevelAdvancement_G.cpp"),
            Object(NonMatching, "Rat/Ropes_G.cpp"),
            Object(NonMatching, "Rat/Mission_Mutli_Rope.cpp"),
            Object(NonMatching, "Rat/Mission_CookingSoupYesNo.cpp"),
            Object(NonMatching, "Rat/CreaturesDashStates_G.cpp"),
            Object(NonMatching, "Rat/Mission_Collect.cpp"),
            Object(NonMatching, "Rat/BaseAgent_G.cpp"),
            Object(NonMatching, "Rat/UnLock_G.cpp"),
            Object(NonMatching, "Rat/CollectAgent_G.cpp"),
            Object(NonMatching, "Rat/Mission_Chase.cpp"),
            Object(NonMatching, "Rat/IT_SOUND_G.CPP"),
            Object(NonMatching, "Rat/ScriptManagerRtcScript_G.cpp"),
            Object(NonMatching, "Rat/Mission_CookinCakeInterface.cpp"),
            Object(NonMatching, "Rat/PhysicWorld_G.cpp"),
            Object(NonMatching, "Rat/IT_DYNOBJ_G.cpp"),
            Object(NonMatching, "Rat/MissionVolumeAgent_G.cpp"),
            Object(NonMatching, "Rat/ScriptManager_G.cpp"),
            Object(NonMatching, "Rat/PermanentDraw.cpp"),
            Object(NonMatching, "Rat/CreatureVehiculeStates_G.cpp"),
            Object(NonMatching, "Rat/CreaturesWallClimbStates.cpp"),
            Object(NonMatching, "Rat/Dialogs_Pause.cpp"),
            Object(NonMatching, "Rat/Mission_Multi_Collect_Interface.cpp"),
            Object(NonMatching, "Rat/EndMissionVolumeAgent_G.cpp"),
            Object(NonMatching, "Rat/VibrationKeyframers_G.cpp"),
            Object(NonMatching, "Rat/HumanHideVolumeAgent_G.cpp"),
            Object(NonMatching, "Rat/Dialogs_MisssionEnd.cpp"),
            Object(NonMatching, "Rat/UserCommand_G.cpp"),
            Object(NonMatching, "Rat/CreaturesJumpStates_G.cpp"),
            Object(NonMatching, "Rat/MenuTools.cpp"),
            Object(NonMatching, "Rat/SaveVolumeAgent_G.cpp"),
            Object(NonMatching, "Rat/GroupTriggerZone_G.cpp"),
            Object(NonMatching, "Rat/BaseInGameDatas_Text_G.cpp"),
            Object(NonMatching, "Rat/Mission_Multi_Race.cpp"),
            Object(NonMatching, "Rat/Mission_Wii_Crepe.cpp"),
            Object(NonMatching, "Rat/InGameFX_G.cpp"),
            Object(NonMatching, "Rat/CheckpointVolAgent_G.cpp"),
            Object(NonMatching, "Rat/MissionManager_G.cpp"),
            Object(NonMatching, "Rat/CreaturesEnemiesStates_G.cpp"),
            Object(NonMatching, "Rat/Mission_ShootTarget.cpp"),
            Object(NonMatching, "Rat/RtcScripted_G.cpp"),
            Object(NonMatching, "Rat/Mission_Multi_Ball_Interface.cpp"),
            Object(NonMatching, "Rat/NodeVolumeAgent_G.cpp"),
            Object(NonMatching, "Rat/Mission_CookingSoup.cpp"),
            Object(NonMatching, "Rat/Mission_Mutli_Race_Interface.cpp"),
            Object(NonMatching, "Rat/MemoryCardMgr_G.cpp"),
            Object(NonMatching, "Rat/PersoLight_G.cpp"),
            Object(NonMatching, "Rat/ConditionVolumeAgent_G.cpp"),
            Object(NonMatching, "Rat/IT_BallWalkMove_G.cpp"),
            Object(NonMatching, "Rat/CameraChase.cpp"),
            Object(NonMatching, "Rat/IT_LinkObj.cpp"),
            Object(NonMatching, "Rat/BaseInGameItems_G.cpp"),
            Object(NonMatching, "Rat/CreaturePointJump.cpp"),
            Object(NonMatching, "Rat/Dialogs_Common.cpp"),
            Object(NonMatching, "Rat/CameraTools.cpp"),
            Object(NonMatching, "Rat/FXVolumeAgent_G.cpp"),
            Object(NonMatching, "Rat/Mission_Switch.cpp"),
            Object(NonMatching, "Rat/Game_ParticleMgr_G.cpp"),
            Object(NonMatching, "Rat/ProGounrMgr_G.cpp"),
            Object(NonMatching, "Rat/CreatureSticky.cpp"),
            Object(NonMatching, "Rat/IT_Condition.cpp"),
            Object(NonMatching, "Rat/IT_Break.cpp"),
            Object(NonMatching, "Rat/CreaturesMove_G.cpp"),
            Object(NonMatching, "Rat/Dialog_MC_Base.cpp"),
            Object(NonMatching, "Rat/Mission_Semaphore.cpp"),
            Object(NonMatching, "Rat/CreatureShootTarget.cpp"),
            Object(NonMatching, "Rat/GuardMove_G.cpp"),
            Object(NonMatching, "Rat/Player_G.cpp"),
            Object(NonMatching, "Rat/ScriptManagerEngine_G.cpp"),
            Object(NonMatching, "Rat/IT_BallMove_G.cpp"),
            Object(NonMatching, "Rat/ZoneVolumeAgent_G.cpp"),
            Object(NonMatching, "Rat/MenuStyle.cpp"),
            Object(NonMatching, "Rat/LangDefine.cpp"),
            Object(NonMatching, "Rat/GroupAgent_G.cpp"),
            Object(NonMatching, "Rat/GusteauAgent.cpp"),
            Object(NonMatching, "Rat/CreaturesGrabStates_G.cpp"),
            Object(NonMatching, "Rat/Mission_Multi_Rope_Interface.cpp"),
            Object(NonMatching, "Rat/LevelData_G.cpp"),
            Object(NonMatching, "Rat/Mission_CookingSoupInterface.cpp"),
            Object(NonMatching, "Rat/Mission_CookingLinguini.cpp"),
            Object(NonMatching, "Rat/DetectVolumeAgent_G.cpp"),
            Object(NonMatching, "Rat/Mission_Glissade.cpp"),
            Object(NonMatching, "Rat/DialogVoices.cpp"),
            Object(NonMatching, "Rat/GameLogicAgent_G.cpp"),
            Object(NonMatching, "Rat/MenuParser.cpp"),
            Object(NonMatching, "Rat/RtcScriptFunctions_G.cpp"),
            Object(NonMatching, "Rat/PaddleChecker_G.cpp"),
            Object(NonMatching, "Rat/CreaturesMachineCommands_G.cpp"),
            Object(NonMatching, "Rat/IT_Bumper.cpp"),
            Object(NonMatching, "Rat/ImpactVolumeAgent_G.cpp"),
            Object(NonMatching, "Rat/MusicManager_G.cpp"),
            Object(NonMatching, "Rat/Mission_Wii_FishInterface.cpp"),
            Object(NonMatching, "Rat/Mission_Tutorial.cpp"),
            Object(NonMatching, "Rat/MainGame_G.cpp"),
            Object(NonMatching, "Rat/InGamePlayerGarfield_G.cpp"),
            Object(NonMatching, "Rat/Dialogs_MainMenu.cpp"),
            Object(NonMatching, "Rat/CreatureCarry.cpp"),
            Object(NonMatching, "Rat/LevelAgent_G.cpp"),
            Object(NonMatching, "Rat/GlobalInGameDatas_G.cpp"),
            Object(NonMatching, "Rat/Purchase.cpp"),
            Object(NonMatching, "Rat/Creatures_G.cpp"),
            Object(NonMatching, "Rat/Dialog_G.cpp"),
            Object(NonMatching, "Rat/CreatureRatBeamRun.cpp"),
            Object(NonMatching, "Rat/CreatureRatRope.cpp"),
            Object(NonMatching, "Rat/Mission_CookingPlate.cpp"),
            Object(NonMatching, "Rat/V_RaftMove.cpp"),
            Object(NonMatching, "Rat/LevelMenu_G.cpp"),
            Object(NonMatching, "Rat/IT_Ball_G.cpp"),
            Object(NonMatching, "Rat/Mission_Multi_Ball.cpp"),
            Object(NonMatching, "Rat/Mission_CookingCake.cpp"),
            Object(NonMatching, "Rat/Mission_TimeCount.cpp"),
            Object(NonMatching, "Rat/CameraFog_G.cpp"),
            Object(NonMatching, "Rat/PhysicAgt_G.cpp"),
            Object(NonMatching, "Rat/Mission_PushButtonInterface.cpp"),
            Object(NonMatching, "Rat/Mission_PushButton.cpp"),
            Object(NonMatching, "Rat/Enemies_G.cpp"),
            Object(NonMatching, "Rat/Button_G.cpp"),
            Object(NonMatching, "Rat/IT_Stone_G.cpp"),
            Object(NonMatching, "Rat/GroupMove_G.cpp"),
            Object(NonMatching, "Rat/IT_BreakMove.cpp"),
            Object(NonMatching, "Rat/Mission_PushLift.cpp"),
            Object(NonMatching, "Rat/CreaturesCommands_G.cpp"),
            Object(NonMatching, "Rat/InGamePlayer_Draw_G.cpp"),
            Object(NonMatching, "Rat/V_Raft.cpp"),
            Object(NonMatching, "Rat/IT_Carrying.cpp"),
            Object(NonMatching, "Rat/InputDef_G.cpp"),
            Object(NonMatching, "Rat/Mission_Global.cpp"),
            Object(NonMatching, "Rat/CameraRTC.cpp"),
            Object(NonMatching, "Rat/Mission_Raft.cpp"),
            Object(NonMatching, "Rat/IT_LinkObjMove.cpp"),
            Object(NonMatching, "Rat/IT_CollectMove_G.cpp"),
            Object(NonMatching, "Rat/PointJump_G.cpp"),
            Object(NonMatching, "Rat/LogicLevel.cpp"),
            Object(NonMatching, "Rat/MissionInfo_G.cpp"),
            Object(NonMatching, "Rat/Mission_PreLinguiniMaking.cpp"),
            Object(NonMatching, "Rat/CreatureOtherStates_G.cpp"),
            Object(NonMatching, "Rat/CreatureFlying.cpp"),
            Object(NonMatching, "Rat/VolumeDelayedStartMove_G.cpp"),
            Object(NonMatching, "Rat/Mission_Container.cpp"),
            Object(NonMatching, "Rat/Mission_GlissadeInterface.cpp"),
        ],
    },
    {
        "lib": "bink",
        "mw_version": "ProDG/3.5",
        "cflags": cflags_bink_base,
        "progress_category": "bink",  # str | List[str]
        "objects": [
            Object(NonMatching, "3rdParty/bink/src/sdk/decode/ngc/binkngc.c"),
            Object(NonMatching, "3rdParty/bink/src/sdk/decode/ngc/ngcsnd.c"),
            Object(NonMatching, "3rdParty/bink/src/sdk/decode/binkread.c"),
            Object(NonMatching, "3rdParty/bink/src/sdk/decode/ngc/ngcfile.c"),
            Object(NonMatching, "3rdParty/shared/memory/ngc/radmem.c"),
            Object(NonMatching, "3rdParty/bink/src/sdk/decode/yuv.c"),
            Object(NonMatching, "3rdParty/bink/src/sdk/decode/binkacd.c"),
            Object(NonMatching, "3rdParty/shared/time/ngc/radcb.c"),
            Object(NonMatching, "3rdParty/bink/src/sdk/decode/expand.c"),
            Object(NonMatching, "3rdParty/bink/src/sdk/popmal.c"),
            Object(NonMatching, "3rdParty/bink/src/sdk/decode/ppc/ppcyuy2.c"),
            Object(NonMatching, "3rdParty/bink/src/sdk/fft.c"),
            Object(NonMatching, "3rdParty/bink/src/sdk/dct.c"),
            Object(NonMatching, "3rdParty/bink/src/sdk/bitplane.c"),
        ],
    }
]

def _discover_objects(lib_dir: str) -> List[Object]:
    """Every buildable source under src/<lib_dir>, in sorted order.

    Port layers have no original binary to match, so listing them by hand only
    creates a second place to forget. Matching status is meaningless here and is
    always NonMatching; the profiles in config/port/<platform>/profiles still
    decide which of these actually get built.
    """

    root = Path("src") / lib_dir
    suffixes = {".c", ".cc", ".cp", ".cpp", ".cxx", ".s", ".S"}
    sources = sorted(
        path.relative_to("src").as_posix()
        for path in root.rglob("*")
        if path.is_file() and path.suffix in suffixes
    )
    if not sources:
        raise SystemExit(f"No buildable sources found under {root}")
    return [Object(NonMatching, source) for source in sources]


# Platform-specific source inventory.
#
# The GameCube build keeps the original full library list unchanged.
# Dreamcast keeps only platform-independent engine/game sources and replaces
# LibGC, Dolphin SDK, MetroTRK, PPC runtime, and NGC Bink with LibDC/KOS.
if args.platform == "dc":
    libs_by_name = {
        library["lib"]: library
        for library in config.libs
    }

    config.libs = [
        libs_by_name["Engine"],
        {
            "lib": "LibDC",

            # These fields are required by the shared project description,
            # but the Dreamcast source backend uses config/port/dc/config.toml
            # for its actual GCC/KOS flags.
            "mw_version": config.linker_version,
            "cflags": [],
            "progress_category": "game",

            "objects": _discover_objects("LibDC"),
        },
        libs_by_name["Game"],
    ]


# Optional callback to adjust link order. This can be used to add, remove, or reorder objects.
# This is called once per module, with the module ID and the current link order.
#
# For example, this adds "dummy.c" to the end of the DOL link order if configured with --non-matching.
# "dummy.c" *must* be configured as a Matching (or Equivalent) object in order to be linked.
def link_order_callback(module_id: int, objects: List[str]) -> List[str]:
    # Don't modify the link order for matching builds
    if not config.non_matching:
        return objects
    if module_id == 0:  # DOL
        return objects + ["3rdParty/dolphin/os/OSFatal.c"]
    return objects

# Uncomment to enable the link order callback.
config.link_order_callback = link_order_callback


# Optional extra categories for progress tracking
# Adjust as desired for your project
config.progress_categories = [
    ProgressCategory("rat", "Ratatouille Code"),
    ProgressCategory("game", "Game Code"),
    ProgressCategory("engine", "Engine Code"),
    ProgressCategory("libgc", "LibGC Code"),
    ProgressCategory("sdk", "SDK Code"),
    ProgressCategory("bink", "Bink Code"),
]
config.progress_each_module = args.verbose


def _mwcc_include_dirs() -> list[str]:
    """Include directories MWCC receives as `-i`/`-ir`, which clang cannot parse.

    Derived from the cflags above so the clangd config cannot drift from the build.
    """
    dirs: list[str] = []
    for flag in (*cflags_base, *cflags_rat_base):
        for prefix in ("-ir ", "-i "):
            if flag.startswith(prefix):
                path = flag[len(prefix) :].strip()
                if path not in dirs:
                    dirs.append(path)
                break
    return dirs


def _dreamcast_include_roots() -> Dict[str, str] | None:
    """System include roots for the generated .clangd, derived from the environment.

    The Dreamcast build already locates KallistiOS through KOS_BASE, so reusing
    it keeps the generated file free of machine-specific paths. Returns None when
    the toolchain cannot be located, so a missing SDK produces a .clangd without
    include paths rather than one pointing at somebody else's install.
    """

    from tools.source_build import _native_environment_path

    def resolve(name: str) -> Path | None:
        raw = os.environ.get(name)
        if not raw:
            return None
        try:
            path = _native_environment_path(raw, name)
        except RuntimeError:
            return None
        return path if path.is_dir() else None

    kos = resolve("KOS_BASE")
    if kos is None:
        return None

    # KOS_CC_BASE when the shell exports it, otherwise the sibling of KOS_BASE
    # that DreamSDK and a manual KallistiOS build both lay out the same way.
    sh_elf = resolve("KOS_CC_BASE")
    if sh_elf is None and (kos.parent / "sh-elf").is_dir():
        sh_elf = kos.parent / "sh-elf"
    if sh_elf is None:
        return None

    # Discovered rather than pinned so a toolchain upgrade does not need an edit.
    versions = sorted(
        (path for path in (sh_elf / "sh-elf" / "include" / "c++").glob("*") if path.is_dir()),
        key=lambda path: path.name,
        reverse=True,
    )
    if not versions:
        return None

    return {
        "kos": kos.as_posix(),
        "kos_ports": (kos.parent / "kos-ports").as_posix(),
        "sh_elf": (sh_elf / "sh-elf" / "include").as_posix(),
        "cxx": versions[0].as_posix(),
    }


def _clangd_config_text(*, dreamcast: bool) -> str:
    lines = [
        "# Generated by configure.py. Re-run configure.py to switch the active clangd build.",
        "Diagnostics:",
        "  Suppress:",
        "    - ms_attributes_not_enabled",
        "    - asm_unknown_register_name",
        "    - asm_invalid_input_constraint",
        "    - asm_invalid_output_constraint",
        "",
        "---",
        "",
        "CompileFlags:",
        "  CompilationDatabase: .",
    ]

    if not dreamcast:
        # MWCC spells include paths `-i dir` / `-ir dir`, which clang drops
        # wholesale, leaving every project header unresolvable. Restate them as
        # -I and strip the Metrowerks-only switches clang would choke on.
        lines.extend(
            [
                "",
                "---",
                "",
                "If:",
                "  PathMatch:",
                "    - src/.*",
                "",
                "CompileFlags:",
                "  Compiler: clang++",
                "",
                "  Remove:",
                "    - --target=*",
                "    - -proc*",
                "    - -align*",
                "    - -enum*",
                "    - -fp*",
                "    - -Cpp_exceptions*",
                "    - -pragma*",
                "    - -maxerrors*",
                "    - -nodefaults",
                "    - -nosyspath",
                "    - -str*",
                "    - -multibyte",
                # -O* rather than -O4* to also catch MWCC's -O3,p and ProDG's
                # -G0; optimisation level is irrelevant to a syntax-only parse.
                "    - -O*",
                "    - -G0",
                "    - -rostr",
                "    - -pool*",
                "    - -schedule*",
                "    - -inline*",
                "    - -use_lmw_stmw*",
                "    - -lang=*",
                "    - -RTTI*",
                "    - -sym*",
                "    - -common*",
                "    - -char*",
                "    - -sdata*",
                "    - -i",
                "    - -ir",
                "    - -i *",
                "    - -ir *",
                "",
                "  Add:",
                # 32-bit big-endian PPC so MSL's headers agree with clang's
                # builtins; on the 64-bit host size_t collides as
                # 'unsigned long' vs 'unsigned long long'.
                "    - --target=powerpc-unknown-eabi",
                # MWCC 2.7 predates C++11: writable string literals and
                # 'register' are legal, and clang 18 would default to C++17.
                "    - -std=gnu++03",
                "    - -nostdlibinc",
                "    - -ferror-limit=0",
                # dolphin/os.h marks OSReport and friends __declspec(weak).
                "    - -fdeclspec",
                # The dolphin SDK uses 'register', removed in C++17.
                "    - -Wno-register",
            ]
        )
        lines.extend(f"    - -I{path}" for path in _mwcc_include_dirs())

    if dreamcast:
        lines.extend(
            [
                "",
                "---",
                "",
                "If:",
                "  PathMatch:",
                "    - src/LibDC/.*",
                "    - src/Engine/.*",
                "    - src/Rat/.*",
                "",
                "CompileFlags:",
                "  Compiler: clang++",
                "",
                "  Remove:",
                "    - --target=*",
                "",
                "  Add:",
                "    - --target=i386-unknown-none-elf",
                "    - -nostdlibinc",
                "    - -nostdinc++",
                "    - -ferror-limit=0",
                "",
                "    - -D_arch_dreamcast=1",
                "    - -D_arch_sub_pristine=1",
                "    - -D__DREAMCAST__=1",
                "    - -D__sh__=1",
                "    - -D__SH4__=1",
                "    - -D__ELF__=1",
                "",
            ]
        )

        roots = _dreamcast_include_roots()
        if roots is None:
            lines.extend(
                [
                    "    # System include paths are derived from KOS_BASE, which was not",
                    "    # resolvable when configure.py last ran. Re-run it from the",
                    "    # DreamSDK/KallistiOS shell to pick up the KOS and newlib headers.",
                ]
            )
        else:
            lines.extend(
                [
                    "    - -isystem",
                    f"    - {roots['kos']}/include",
                    "    - -isystem",
                    f"    - {roots['kos']}/kernel/arch/dreamcast/include",
                    "    - -isystem",
                    f"    - {roots['kos']}/addons/include",
                    "    - -isystem",
                    f"    - {roots['kos_ports']}/include",
                    "",
                    "    # Generic libstdc++ headers.",
                    "    - -isystem",
                    f"    - {roots['cxx']}",
                    "",
                    "    # Target-specific libstdc++ headers.",
                    "    - -isystem",
                    f"    - {roots['cxx']}/sh-elf",
                    "    - -isystem",
                    f"    - {roots['cxx']}/backward",
                    "",
                    "    # Newlib C headers must follow the libstdc++ wrappers.",
                    "    - -isystem",
                    f"    - {roots['sh_elf']}",
                ]
            )

    return "\n".join(lines) + "\n"


def activate_clangd_build(
    repo_root: Path,
    source_database: Path,
    configuration_name: str,
    *,
    dreamcast: bool,
) -> None:
    source_database = source_database.resolve()
    active_database = (repo_root / "compile_commands.json").resolve()

    if not source_database.is_file():
        print(
            f"warning: clangd database was not generated: {source_database}",
            file=sys.stderr,
        )
        return

    if source_database != active_database:
        source_bytes = source_database.read_bytes()
        if not active_database.is_file() or active_database.read_bytes() != source_bytes:
            active_database.write_bytes(source_bytes)

    # .clangd cannot test which macros are present in compile_commands.json.
    # Generate the Dreamcast-only fragment only while a DC build is active.
    clangd_path = repo_root / ".clangd"
    clangd_text = _clangd_config_text(dreamcast=dreamcast)
    if not clangd_path.is_file() or clangd_path.read_text(encoding="utf-8") != clangd_text:
        clangd_path.write_text(
            clangd_text,
            encoding="utf-8",
            newline="\n",
        )

    stamp_path = repo_root / ".cache" / "clangd-active-build.txt"
    previous_configuration = ""
    if stamp_path.is_file():
        previous_configuration = stamp_path.read_text(encoding="utf-8").strip()

    if previous_configuration != configuration_name:
        shutil.rmtree(repo_root / ".cache" / "clangd" / "index", ignore_errors=True)

    stamp_path.parent.mkdir(parents=True, exist_ok=True)
    stamp_path.write_text(configuration_name + "\n", encoding="utf-8", newline="\n")

    print(f"Active clangd build: {configuration_name}")
    print(f"Compilation database: {active_database}")


def activate_ninja_build(repo_root: Path, generated_ninja: Path) -> None:
    """Make plain `ninja` delegate to the selected source-build manifest.

    Do not include/subninja the generated manifest here. Some Ninja versions
    use the root invocation's bookkeeping database in that arrangement, while
    direct `ninja -f <manifest>` builds use the configuration-local database.
    A tiny launcher keeps each source configuration's .ninja_log and
    .ninja_deps isolated and still lets the user run plain `ninja`.
    """
    generated_ninja = generated_ninja.resolve()
    active_manifest = (repo_root / "build.ninja").resolve()

    if generated_ninja == active_manifest:
        return

    try:
        relative_manifest = generated_ninja.relative_to(repo_root.resolve()).as_posix()
    except ValueError:
        raise RuntimeError(
            f"generated Ninja manifest is outside the repository: {generated_ninja}"
        )

    wrapper_text = (
        "# Generated by configure.py. Re-run configure.py to switch the active build.\n"
        "ninja_required_version = 1.3\n"
        "builddir = build/.active-ninja\n"
        f"active_manifest = {relative_manifest}\n"
        "\n"
        "rule active_ninja\n"
        "  command = ninja -f $active_manifest $active_target\n"
        "  description = ACTIVE NINJA $active_target\n"
        "  pool = console\n"
        "\n"
        "# A phony edge with no inputs is always dirty. This intentionally invokes\n"
        "# the selected nested Ninja each time; the nested build performs the real\n"
        "# incremental dependency and command-line checks.\n"
        "build __active_ninja_always: phony\n"
        "\n"
        "build all: active_ninja __active_ninja_always | $active_manifest\n"
        "  active_target = all\n"
        "build compile: active_ninja __active_ninja_always | $active_manifest\n"
        "  active_target = compile\n"
        "\n"
        "default all\n"
    )

    if (
        not active_manifest.is_file()
        or active_manifest.read_text(encoding="utf-8") != wrapper_text
    ):
        active_manifest.write_text(
            wrapper_text,
            encoding="utf-8",
            newline="\n",
        )

    print(f"Active Ninja build: {relative_manifest}")
    print("Build with: ninja")


repo_root = Path(__file__).resolve().parent

if build_mode == "source":
    from tools.source_build import SourceBuildOptions, generate_source_build

    generated_ninja = generate_source_build(
        config,
        SourceBuildOptions(
            platform=args.platform,
            profile=args.profile,
            platform_config=source_platform_config,
            base_build_dir=args.build_dir,
            build_config=source_build_config,
            romdisk=args.romdisk,
            validate_only=args.validate_only,
            verbose=args.verbose,
        ),
        repo_root=repo_root,
    )

    if not args.validate_only:
        generated_ninja = (
            generated_ninja
            if generated_ninja.is_absolute()
            else repo_root / generated_ninja
        )
        activate_clangd_build(
            repo_root,
            generated_ninja.parent / "compile_commands.json",
            (
                f"{args.platform}-source:"
                f"{args.profile or 'default'}:"
                f"{source_build_config}"
            ),
            dreamcast=args.platform == "dc",
        )
        activate_ninja_build(repo_root, generated_ninja)

        # generate_source_build may intentionally avoid rewriting build.ninja
        # when its contents are unchanged. If configure.py itself is newer,
        # Ninja would otherwise keep rerunning the regeneration edge because
        # the manifest remains older than one of its inputs.
        generated_ninja.touch()
elif args.mode == "configure":
    # Default behavior: normal GameCube matching/decompilation build.
    generate_build(config)
    activate_clangd_build(
        repo_root,
        repo_root / "compile_commands.json",
        "gc-non-matching" if config.non_matching else "gc-matching",
        dreamcast=False,
    )
elif args.mode == "progress":
    calculate_progress(config)
else:
    sys.exit("Unknown mode: " + args.mode)
