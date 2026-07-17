#@category Decomp
#@runtime PyGhidra
#
# Export only the current program's Ghidra datatype database as ANSI C.
# No functions are decompiled and the program database is not modified.
#
# Headless, existing analyzed project:
#   support/pyghidraRun.bat -H <project_dir> <project_name> ^
#       -process <program_name> -noanalysis ^
#       -scriptPath <script_dir> ^
#       -postScript dump_types.py <output_directory>
#
# GUI:
#   Run from Script Manager and choose an output directory.
#
# Outputs:
#   <output_directory>/types.h
#   <output_directory>/_type_reports/types_manifest.tsv
#   <output_directory>/_type_reports/same_name_groups.txt
#   <output_directory>/_type_reports/skipped_types.txt
#
# The C header uses Ghidra's DataTypeWriter, the same datatype writer used by
# Ghidra's built-in C/C++ exporter. Ghidra categories are not C namespaces, so
# multiple non-equivalent types with the same bare name cannot all coexist in
# one normal C header. Such names are listed in same_name_groups.txt; the first
# type in deterministic path order is the one DataTypeWriter will emit.

import os
import re
from collections import defaultdict

from ghidra.program.model.data import (
    DataTypeWriter,
    FactoryDataType,
    FunctionDefinition,
)
from java.io import BufferedWriter, File, FileOutputStream, OutputStreamWriter
from java.lang import System
from java.nio.charset import StandardCharsets
from java.util import ArrayList


# ---------------------------------------------------------------------------
# Configuration
# ---------------------------------------------------------------------------

OUTPUT_NAME = 'types.h'
REPORT_DIR_NAME = '_type_reports'
USE_CPP_STYLE_COMMENTS = True
ADD_INCLUDE_GUARD = True
ADD_DECOMPILER_FAKE_TYPES = True

# Ghidra's DataTypeWriter ignores standalone FunctionDefinition datatypes.
# They remain listed in types_manifest.tsv, and are still emitted when used by
# a typedef/pointer/member declaration that DataTypeWriter can represent.


# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------


def as_path(value):
    return os.path.abspath(os.path.expanduser(str(value)))


def ensure_dir(path):
    if path and not os.path.isdir(path):
        os.makedirs(path, exist_ok=True)


def write_text(path, text):
    ensure_dir(os.path.dirname(path))
    with open(path, 'w', encoding='utf-8', newline='\n') as stream:
        stream.write(text)


def clean_tsv(value):
    return str(value).replace('\t', ' ').replace('\r', ' ').replace('\n', ' ')


def get_class_name(value):
    try:
        return str(value.getClass().getName())
    except Exception:
        return type(value).__name__


def get_simple_class_name(value):
    try:
        return str(value.getClass().getSimpleName())
    except Exception:
        return type(value).__name__


def get_path_name(dt):
    try:
        return str(dt.getPathName())
    except Exception:
        try:
            return '%s/%s' % (dt.getCategoryPath(), dt.getName())
        except Exception:
            return str(dt)


def get_category_path(dt):
    try:
        return str(dt.getCategoryPath())
    except Exception:
        return ''


def get_name(dt):
    try:
        return str(dt.getName())
    except Exception:
        return str(dt)


def get_display_name(dt):
    try:
        return str(dt.getDisplayName())
    except Exception:
        return get_name(dt)


def get_length(dt):
    try:
        return int(dt.getLength())
    except Exception:
        return -1


def make_include_guard(filename):
    guard = re.sub(r'[^A-Za-z0-9_]', '_', filename).upper()
    if not guard or guard[0].isdigit():
        guard = '_' + guard
    return 'GHIDRA_%s_INCLUDED' % guard


def resolve_output_directory():
    args = [str(arg) for arg in getScriptArgs()]

    if args:
        return as_path(args[0])

    if isRunningHeadless():
        printerr('Usage: dump_types.py <output_directory>')
        return None

    return as_path(
        askDirectory('Select datatype output directory', 'Select').getAbsolutePath()
    )


# ---------------------------------------------------------------------------
# Ghidra-compatible fake decompiler types
# ---------------------------------------------------------------------------


def write_define(writer, name, c_type, eol):
    writer.write('#define %s %s%s' % (name, c_type, eol))


def write_fake_decompiler_types(writer, data_organization, eol):
    """Mirror the extra fabricated-type declarations used by CppExporter."""
    writer.write('%s// Decompiler-fabricated fallback types%s' % (eol, eol))

    for size in range(9, 17):
        c_type = str(
            data_organization.getIntegerCTypeApproximation(size, False)
        )
        write_define(writer, 'unkbyte%d' % size, c_type, eol)

    writer.write(eol)

    for size in range(9, 17):
        c_type = str(
            data_organization.getIntegerCTypeApproximation(size, False)
        )
        write_define(writer, 'unkuint%d' % size, c_type, eol)

    writer.write(eol)

    for size in range(9, 17):
        c_type = str(
            data_organization.getIntegerCTypeApproximation(size, True)
        )
        write_define(writer, 'unkint%d' % size, c_type, eol)

    writer.write(eol)

    for size in (1, 2, 3):
        write_define(writer, 'unkfloat%d' % size, 'float', eol)
    for size in (5, 6, 7):
        write_define(writer, 'unkfloat%d' % size, 'double', eol)
    for size in (9, 11, 12, 13, 14, 15, 16):
        write_define(writer, 'unkfloat%d' % size, 'long double', eol)

    writer.write(eol)
    write_define(writer, 'BADSPACEBASE', 'void', eol)
    write_define(writer, 'code', 'void', eol)
    writer.write(eol)


# ---------------------------------------------------------------------------
# Type collection and reports
# ---------------------------------------------------------------------------


def collect_types(dtm):
    included = []
    skipped = []

    iterator = dtm.getAllDataTypes()
    for dt in iterator:
        monitor.checkCancelled()

        path_name = get_path_name(dt)
        class_name = get_class_name(dt)
        simple_class_name = get_simple_class_name(dt)

        # BadDataType is an internal placeholder for a broken/unresolvable
        # datatype. It cannot produce a meaningful C declaration and has caused
        # reflective-access warnings in some development Ghidra builds.
        if simple_class_name == 'BadDataType' or class_name.endswith('.BadDataType'):
            skipped.append((path_name, class_name, 'BadDataType placeholder'))
            continue

        # DataTypeWriter rejects FactoryDataType instances. Dynamic/builtin
        # types reached through real user types are handled by DataTypeWriter.
        if isinstance(dt, FactoryDataType):
            skipped.append((path_name, class_name, 'FactoryDataType is not writable'))
            continue

        included.append(dt)

    included.sort(
        key=lambda dt: (
            get_path_name(dt).lower(),
            get_path_name(dt),
            get_class_name(dt),
        )
    )

    return included, skipped


def build_same_name_report(types):
    groups = defaultdict(list)

    for dt in types:
        groups[get_name(dt)].append(dt)

    lines = []
    duplicate_group_count = 0

    for name in sorted(groups, key=lambda value: (value.lower(), value)):
        values = groups[name]
        paths = sorted({get_path_name(dt) for dt in values})

        if len(paths) < 2:
            continue

        duplicate_group_count += 1
        lines.append('%s (%d paths)' % (name, len(paths)))
        for path in paths:
            lines.append('  %s' % path)
        lines.append('')

    if not lines:
        lines.append('No datatype names occur in multiple category paths.')
        lines.append('')

    return duplicate_group_count, '\n'.join(lines)


def write_manifest(path, types, skipped):
    rows = [
        'status\tpath\tcategory\tname\tdisplay_name\tlength\tjava_class'
    ]

    for dt in types:
        if isinstance(dt, FunctionDefinition):
            status = 'included; standalone FunctionDefinition is not directly emitted'
        else:
            status = 'included'

        rows.append(
            '%s\t%s\t%s\t%s\t%s\t%d\t%s'
            % (
                clean_tsv(status),
                clean_tsv(get_path_name(dt)),
                clean_tsv(get_category_path(dt)),
                clean_tsv(get_name(dt)),
                clean_tsv(get_display_name(dt)),
                get_length(dt),
                clean_tsv(get_class_name(dt)),
            )
        )

    for path_name, class_name, reason in skipped:
        rows.append(
            'skipped\t%s\t\t\t\t-1\t%s'
            % (
                clean_tsv('%s [%s]' % (path_name, reason)),
                clean_tsv(class_name),
            )
        )

    rows.append('')
    write_text(path, '\n'.join(rows))


# ---------------------------------------------------------------------------
# Header export
# ---------------------------------------------------------------------------


def export_types_header(output_path, types):
    eol = str(System.lineSeparator())
    temporary_path = output_path + '.tmp'

    ensure_dir(os.path.dirname(output_path))

    java_file = File(temporary_path)
    stream = FileOutputStream(java_file)
    encoding_writer = OutputStreamWriter(stream, StandardCharsets.UTF_8)
    writer = BufferedWriter(encoding_writer)

    try:
        guard = make_include_guard(os.path.basename(output_path))

        writer.write(
            '// Ghidra datatype export for %s%s'
            % (str(currentProgram.getName()), eol)
        )
        writer.write(
            '// Generated by dump_types.py; no program data was modified.%s%s'
            % (eol, eol)
        )

        if ADD_INCLUDE_GUARD:
            writer.write('#ifndef %s%s#define %s%s%s' % (
                guard, eol, guard, eol, eol
            ))

        # Constructing DataTypeWriter emits Ghidra's builtin declarations first,
        # just like the built-in C/C++ exporter.
        dt_writer = DataTypeWriter(
            currentProgram.getDataTypeManager(),
            writer,
            USE_CPP_STYLE_COMMENTS,
        )

        if ADD_DECOMPILER_FAKE_TYPES:
            write_fake_decompiler_types(
                writer,
                currentProgram.getDataTypeManager().getDataOrganization(),
                eol,
            )

        java_types = ArrayList()
        for dt in types:
            java_types.add(dt)

        # False means an invalid top-level datatype is logged/skipped rather
        # than aborting the entire export. We already filter the known invalid
        # FactoryDataType and BadDataType cases above.
        dt_writer.write(java_types, monitor, False)

        if ADD_INCLUDE_GUARD:
            writer.write('%s#endif // %s%s' % (eol, guard, eol))

        writer.flush()
    finally:
        try:
            writer.close()
        except Exception:
            pass

    os.replace(temporary_path, output_path)


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------


def main():
    output_directory = resolve_output_directory()
    if output_directory is None:
        return

    ensure_dir(output_directory)
    report_directory = os.path.join(output_directory, REPORT_DIR_NAME)
    ensure_dir(report_directory)

    dtm = currentProgram.getDataTypeManager()

    println('Collecting datatypes from %s...' % currentProgram.getName())
    types, skipped = collect_types(dtm)
    println('Collected %d writable datatype entries; skipped %d.' % (
        len(types), len(skipped)
    ))

    duplicate_group_count, duplicate_report = build_same_name_report(types)

    manifest_path = os.path.join(report_directory, 'types_manifest.tsv')
    duplicate_path = os.path.join(report_directory, 'same_name_groups.txt')
    skipped_path = os.path.join(report_directory, 'skipped_types.txt')

    write_manifest(manifest_path, types, skipped)
    write_text(duplicate_path, duplicate_report)

    skipped_lines = []
    for path_name, class_name, reason in skipped:
        skipped_lines.append('%s\n  class: %s\n  reason: %s\n' % (
            path_name,
            class_name,
            reason,
        ))
    if not skipped_lines:
        skipped_lines.append('No datatypes were skipped.\n')
    write_text(skipped_path, '\n'.join(skipped_lines))

    output_path = os.path.join(output_directory, OUTPUT_NAME)
    println('Writing %s...' % output_path)

    try:
        export_types_header(output_path, types)
    except Exception as error:
        temporary_path = output_path + '.tmp'
        try:
            if os.path.exists(temporary_path):
                os.remove(temporary_path)
        except Exception:
            pass

        printerr('Datatype export failed: %s' % error)
        raise

    println('Done. Datatypes written: %s' % output_path)
    println('Same-name groups: %d' % duplicate_group_count)
    println('Skipped datatypes: %d' % len(skipped))
    println('Reports: %s' % report_directory)


main()
