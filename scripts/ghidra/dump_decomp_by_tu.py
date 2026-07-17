#@category Decomp
#@runtime PyGhidra
#
# Dump the decompiler text for every non-external Ghidra function, grouped by
# translation unit/object using a decomp-toolkit style splits.txt.
#
# Optional symbols.txt support provides:
#   - linker symbol names in function metadata comments
#   - missing-function diagnostics
#   - function boundary diagnostics
#   - alias diagnostics
#
# This script intentionally DOES NOT create/rename functions or modify types.
# It exports the current Ghidra database as-is.
#
# GUI:
#   1. Launch Ghidra through support/pyghidraRun(.bat).
#   2. Open the analyzed program in CodeBrowser.
#   3. Run this script from Script Manager.
#   4. Select splits.txt, the output directory, and optionally symbols.txt.
#
# Headless, existing analyzed project:
#   support/pyghidraRun.bat -H <project_dir> <project_name> \
#       -process <program_name> -noanalysis \
#       -scriptPath <script_dir> \
#       -postScript dump_decomp_by_tu.py \
#       <splits.txt> <outdir> [symbols.txt]
#
# In headless mode the active CodeBrowser Tool Options do not exist. The script
# can only consume decompiler options stored in the program plus Ghidra defaults.

import os
import re
from bisect import bisect_right
from collections import defaultdict

from ghidra.app.decompiler import DecompInterface, PrettyPrinter
from ghidra.app.decompiler.component import DecompilerUtils


# ---------------------------------------------------------------------------
# Configuration
# ---------------------------------------------------------------------------

# Sections that may contain linked functions in splits.txt.
CODE_SECTIONS = {'.init', '.text', '.dbgtext'}

# Preserve the original unit filename and append this suffix:
#   Engine/Math_Z.cpp -> Engine/Math_Z.cpp.ghidra.c
OUTPUT_SUFFIX = '.ghidra.c'

# Add comments before each exact decompiler body with its address, Ghidra name,
# and matching symbols.txt names. Disable this for files containing only the
# decompiler bodies separated by blank lines.
ADD_FUNCTION_HEADERS = True

# Include successful decompiler warnings in a report.
REPORT_DECOMPILER_WARNINGS = True

# Emit a potentially large report of every Ghidra function that has no exact
# symbols.txt entry. Only applies when symbols.txt is supplied.
REPORT_GHIDRA_FUNCTIONS_WITHOUT_SYMBOLS = True

# Report gaps between adjacent ranges of the same code section. Gaps are often
# valid linker padding or omitted objects, so this is off by default.
REPORT_SPLIT_GAPS = False

# Use the timeout configured in Decompiler Options. Set an integer number of
# seconds here to force a fixed timeout instead.
FORCED_TIMEOUT_SECONDS = None

# DecompInterface caches symbol/function information. This script does not
# mutate the program, so flushing after every function is unnecessary and slow.
FLUSH_DECOMPILER_CACHE_EACH_FUNCTION = False


# ---------------------------------------------------------------------------
# Input formats
# ---------------------------------------------------------------------------

# Example unit header:
#   Engine/Math_Z.cpp:
UNIT_RE = re.compile(
    r'^(?P<name>[^\s:][^:]*\.(?:c|cc|cpp|cxx|s|S|asm|o))\s*:\s*$'
)

# Example section range:
#       .text start:0x8025DDC4 end:0x80260CF4
SECTION_RE = re.compile(
    r'^\s+(?P<section>\.\w[\w.$]*)\s+'
    r'start:\s*(?P<start>0x[0-9A-Fa-f]+)\s+'
    r'end:\s*(?P<end>0x[0-9A-Fa-f]+)'
)

# Example symbol:
#   Foo__3BarFv = .text:0x80001234; // type:function size:0x20 scope:global
#
# The linker/mangled name is intentionally treated as opaque text.
SYMBOL_RE = re.compile(
    r'^(?P<name>.*?)\s*=\s*'
    r'(?P<section>\.[A-Za-z0-9_.$]+):'
    r'(?P<address>0x[0-9A-Fa-f]+)\s*;\s*'
    r'//\s*'
    r'type:(?P<type>[A-Za-z0-9_]+)\s+'
    r'size:(?P<size>0x[0-9A-Fa-f]+)\s+'
    r'scope:(?P<scope>[A-Za-z0-9_]+)'
    r'(?:\s+.*)?$'
)


# ---------------------------------------------------------------------------
# General helpers
# ---------------------------------------------------------------------------


def as_path(value):
    return os.path.abspath(os.path.expanduser(str(value)))


def ensure_dir(path):
    if path and not os.path.isdir(path):
        os.makedirs(path, exist_ok=True)


def escape_comment(text):
    return str(text).replace('*/', '* /').replace('\r', ' ').replace('\n', ' ')


def clean_tsv(text):
    return str(text).replace('\t', ' ').replace('\r', ' ').replace('\n', ' ')


def write_text(path, text):
    ensure_dir(os.path.dirname(path))
    with open(path, 'w', encoding='utf-8', newline='\n') as stream:
        stream.write(text)


def write_lines(path, lines, heading=None):
    output = []
    if heading:
        output.append(heading.rstrip('\n'))
        output.append('')
    output.extend(str(line).rstrip('\n') for line in lines)
    if output:
        output.append('')
    write_text(path, '\n'.join(output))


def safe_unit_output_path(outdir, unit):
    """Mirror the split path without permitting an absolute/path-traversal path."""
    normalized = str(unit).replace('\\', '/')

    # Remove a possible drive prefix and leading separators.
    normalized = re.sub(r'^[A-Za-z]:', '', normalized).lstrip('/')

    parts = []
    for part in normalized.split('/'):
        if not part or part == '.':
            continue
        if part == '..':
            parts.append('__parent__')
        else:
            parts.append(part)

    if not parts:
        parts = ['_unnamed_unit']

    relative = os.path.join(*parts) + OUTPUT_SUFFIX
    return os.path.join(outdir, relative)


def resolve_inputs():
    args = [str(arg) for arg in getScriptArgs()]

    if len(args) >= 2:
        splits_path = as_path(args[0])
        outdir = as_path(args[1])
        symbols_path = None

        if len(args) >= 3 and args[2].strip().lower() not in ('', '-', 'none', 'null'):
            symbols_path = as_path(args[2])

        return splits_path, outdir, symbols_path

    if isRunningHeadless():
        printerr(
            'Usage: dump_decomp_by_tu.py <splits.txt> <outdir> [symbols.txt]'
        )
        return None

    splits_path = as_path(askFile('Select splits.txt', 'Select').getAbsolutePath())
    outdir = as_path(askDirectory('Select output directory', 'Select').getAbsolutePath())

    symbols_path = None
    if askYesNo(
        'Optional symbols.txt',
        'Use symbols.txt for linker names and validation reports?'
    ):
        symbols_path = as_path(
            askFile('Select symbols.txt', 'Select').getAbsolutePath()
        )

    return splits_path, outdir, symbols_path


# ---------------------------------------------------------------------------
# splits.txt
# ---------------------------------------------------------------------------


def parse_splits(path):
    ranges = []
    issues = []
    current_unit = None

    with open(path, 'r', encoding='utf-8-sig') as stream:
        for line_number, raw_line in enumerate(stream, 1):
            line = raw_line.rstrip('\r\n')
            stripped = line.strip()

            if not stripped or stripped.startswith('#'):
                continue

            unit_match = UNIT_RE.match(line)
            if unit_match:
                current_unit = unit_match.group('name').strip()
                continue

            section_match = SECTION_RE.match(line)
            if section_match:
                if current_unit is None:
                    # This intentionally ignores the top-level "Sections:" table.
                    continue

                section = section_match.group('section')
                if section not in CODE_SECTIONS:
                    continue

                start = int(section_match.group('start'), 16)
                end = int(section_match.group('end'), 16)

                if end <= start:
                    issues.append(
                        '%s:%d: invalid %s range for %s: 0x%08X-0x%08X'
                        % (path, line_number, section, current_unit, start, end)
                    )
                    continue

                ranges.append({
                    'unit': current_unit,
                    'section': section,
                    'start': start,
                    'end': end,
                    'line': line_number,
                })
                continue

            # Only complain about lines that look like malformed range entries.
            if current_unit is not None and ('start:' in line or 'end:' in line):
                issues.append(
                    '%s:%d: could not parse range line: %s'
                    % (path, line_number, line)
                )

    ranges.sort(key=lambda item: (item['start'], item['end'], item['unit']))
    issues.extend(validate_split_ranges(ranges))
    return ranges, issues


def validate_split_ranges(ranges):
    issues = []

    for index in range(1, len(ranges)):
        previous = ranges[index - 1]
        current = ranges[index]

        if current['start'] < previous['end']:
            issues.append(
                'OVERLAP: %s %s [0x%08X,0x%08X) overlaps '
                '%s %s [0x%08X,0x%08X)'
                % (
                    previous['unit'], previous['section'],
                    previous['start'], previous['end'],
                    current['unit'], current['section'],
                    current['start'], current['end'],
                )
            )
        elif (
            REPORT_SPLIT_GAPS
            and current['section'] == previous['section']
            and current['start'] > previous['end']
        ):
            issues.append(
                'GAP: %s 0x%08X-0x%08X between %s and %s'
                % (
                    current['section'], previous['end'], current['start'],
                    previous['unit'], current['unit'],
                )
            )

    return issues


class SplitIndex(object):
    def __init__(self, ranges):
        self.ranges = ranges
        self.starts = [item['start'] for item in ranges]

    def find(self, address):
        index = bisect_right(self.starts, address) - 1
        if index < 0:
            return None

        item = self.ranges[index]
        if item['start'] <= address < item['end']:
            return item
        return None


# ---------------------------------------------------------------------------
# symbols.txt
# ---------------------------------------------------------------------------


def parse_symbols(path):
    symbols = []
    issues = []

    with open(path, 'r', encoding='utf-8-sig') as stream:
        for line_number, raw_line in enumerate(stream, 1):
            line = raw_line.rstrip('\r\n')
            stripped = line.strip()

            if not stripped or stripped.startswith('#'):
                continue

            match = SYMBOL_RE.match(line)
            if match is None:
                # symbols.txt may contain other supported declarations. Only flag a
                # failed line that appears intended to describe a function.
                if 'type:function' in line:
                    issues.append(
                        '%s:%d: could not parse function symbol: %s'
                        % (path, line_number, line)
                    )
                continue

            if match.group('type') != 'function':
                continue

            address = int(match.group('address'), 16)
            size = int(match.group('size'), 16)

            symbols.append({
                'name': match.group('name').strip(),
                'section': match.group('section'),
                'address': address,
                'size': size,
                'end': address + size,
                'scope': match.group('scope'),
                'line': line_number,
            })

    symbols.sort(key=lambda item: (item['address'], item['name']))
    return symbols, issues


def index_symbols(symbols):
    by_address = defaultdict(list)
    for symbol in symbols:
        by_address[symbol['address']].append(symbol)
    return by_address


def make_address(offset):
    space = currentProgram.getAddressFactory().getDefaultAddressSpace()
    return space.getAddress(offset)


def function_body_info(function):
    body = function.getBody()
    if body is None or body.isEmpty():
        return None

    minimum = body.getMinAddress().getOffset()
    maximum_exclusive = body.getMaxAddress().getOffset() + 1
    size = int(body.getNumAddresses())

    return {
        'start': minimum,
        'end': maximum_exclusive,
        'size': size,
        'contiguous': size == (maximum_exclusive - minimum),
    }


def validate_symbols(symbols, split_index):
    function_manager = currentProgram.getFunctionManager()

    missing = []
    mismatches = []
    aliases = []

    by_address = index_symbols(symbols)

    for address in sorted(by_address):
        group = by_address[address]
        if len(group) > 1:
            aliases.append(
                '0x%08X: %s'
                % (address, ' | '.join(symbol['name'] for symbol in group))
            )

    for symbol in symbols:
        monitor.checkCancelled()

        address = make_address(symbol['address'])
        function = function_manager.getFunctionAt(address)
        split = split_index.find(symbol['address'])
        unit = split['unit'] if split else '<unsplit>'

        if function is None:
            containing = function_manager.getFunctionContaining(address)
            if containing is None:
                detail = 'no Ghidra function contains this address'
            else:
                detail = (
                    'inside Ghidra function %s @ 0x%08X'
                    % (
                        containing.getName(),
                        containing.getEntryPoint().getOffset(),
                    )
                )

            missing.append(
                '0x%08X size=0x%X end=0x%08X scope=%s unit=%s %s\n'
                '  %s'
                % (
                    symbol['address'], symbol['size'], symbol['end'],
                    symbol['scope'], unit, symbol['name'], detail,
                )
            )
            continue

        body = function_body_info(function)
        if body is None:
            mismatches.append(
                '0x%08X %s\n'
                '  symbols.txt: [0x%08X,0x%08X) size=0x%X\n'
                '  Ghidra: empty function body'
                % (
                    symbol['address'], symbol['name'],
                    symbol['address'], symbol['end'], symbol['size'],
                )
            )
            continue

        expected = (
            body['start'] == symbol['address']
            and body['end'] == symbol['end']
            and body['size'] == symbol['size']
            and body['contiguous']
        )

        if not expected:
            mismatches.append(
                '0x%08X %s\n'
                '  symbols.txt: [0x%08X,0x%08X) size=0x%X scope=%s\n'
                '  Ghidra:      [0x%08X,0x%08X) body_bytes=0x%X '
                'contiguous=%s name=%s'
                % (
                    symbol['address'], symbol['name'],
                    symbol['address'], symbol['end'], symbol['size'],
                    symbol['scope'],
                    body['start'], body['end'], body['size'],
                    body['contiguous'], function.getName(),
                )
            )

    return by_address, missing, mismatches, aliases


# ---------------------------------------------------------------------------
# Decompiler
# ---------------------------------------------------------------------------


def get_decompiler_options():
    tool = None
    try:
        if state is not None:
            tool = state.getTool()
    except Exception:
        tool = None

    options = DecompilerUtils.getDecompileOptions(tool, currentProgram)
    return tool, options


def create_decompiler(options):
    interface = DecompInterface()
    interface.setOptions(options)
    interface.toggleCCode(True)
    interface.toggleSyntaxTree(True)
    interface.setSimplificationStyle('decompile')

    if not interface.openProgram(currentProgram):
        message = interface.getLastMessage()
        interface.dispose()
        raise RuntimeError(
            'Failed to initialize the decompiler: %s' % str(message)
        )

    return interface


def render_gui_text(function, results, options):
    """Return the logical decompiler lines using the same PrettyPrinter line text
    API used to represent UI lines. Syntax colors, links, hover state, and visual
    soft wrapping are naturally not representable in a text file.
    """
    markup = results.getCCodeMarkup()
    if markup is None:
        return None

    # Current Ghidra exposes the active name transformer and a three-argument
    # PrettyPrinter constructor. The fallback keeps the script usable on older
    # PyGhidra-enabled builds that only expose the two-argument constructor.
    try:
        transformer = options.getNameTransformer()
    except Exception:
        transformer = None

    try:
        printer = PrettyPrinter(function, markup, transformer)
    except TypeError:
        printer = PrettyPrinter(function, markup)

    lines = []
    for line in printer.getLines():
        lines.append(str(PrettyPrinter.getText(line)))

    return '\n'.join(lines) + '\n'


def format_function_header(function, address, symbols):
    if not ADD_FUNCTION_HEADERS:
        return ''

    output = [
        '/*',
        ' * Address: 0x%08X' % address,
        ' * Ghidra: %s' % escape_comment(function.getName()),
    ]

    for symbol in symbols:
        output.append(
            ' * Linker: %s  [size=0x%X scope=%s section=%s]'
            % (
                escape_comment(symbol['name']),
                symbol['size'],
                escape_comment(symbol['scope']),
                escape_comment(symbol['section']),
            )
        )

    output.append(' */')
    return '\n'.join(output) + '\n'


def decompile_all(functions, interface, options, split_index, symbols_by_address):
    buckets = defaultdict(list)
    orphans = []
    failures = []
    warnings = []
    map_rows = []

    timeout = (
        int(FORCED_TIMEOUT_SECONDS)
        if FORCED_TIMEOUT_SECONDS is not None
        else int(options.getDefaultTimeout())
    )

    monitor.initialize(len(functions))
    monitor.setMessage('Decompiling functions')

    success_count = 0

    for index, function in enumerate(functions):
        monitor.checkCancelled()
        monitor.setProgress(index)

        address = function.getEntryPoint().getOffset()
        monitor.setMessage(
            'Decompiling %d/%d: %s @ 0x%08X'
            % (index + 1, len(functions), function.getName(), address)
        )

        matching_symbols = symbols_by_address.get(address, [])
        split = split_index.find(address)
        unit = split['unit'] if split else None

        status = 'ok'
        decompiler_message = ''

        try:
            results = interface.decompileFunction(function, timeout, monitor)

            if results is None:
                status = 'failed'
                decompiler_message = 'decompileFunction returned null'
                code = None
            elif not results.decompileCompleted():
                status = 'failed'
                decompiler_message = str(results.getErrorMessage() or 'unknown error')
                code = None
            else:
                code = render_gui_text(function, results, options)
                if code is None:
                    status = 'failed'
                    decompiler_message = 'decompilation produced no C markup'
                else:
                    success_count += 1
                    result_message = str(results.getErrorMessage() or '').strip()
                    if REPORT_DECOMPILER_WARNINGS and result_message:
                        warnings.append(
                            '0x%08X %s: %s'
                            % (address, function.getName(), result_message)
                        )

        except Exception as exception:
            status = 'exception'
            decompiler_message = '%s: %s' % (
                exception.__class__.__name__, str(exception)
            )
            code = None

        if code is None:
            failures.append(
                '0x%08X %s: %s'
                % (address, function.getName(), decompiler_message)
            )
            code = '/* DECOMPILATION FAILED: %s */\n' % escape_comment(
                decompiler_message
            )

        chunk = format_function_header(
            function, address, matching_symbols
        ) + code

        if not chunk.endswith('\n'):
            chunk += '\n'

        if unit is None:
            orphans.append((address, chunk))
        else:
            buckets[unit].append((address, chunk))

        body = function_body_info(function)
        body_end = body['end'] if body else address
        body_size = body['size'] if body else 0

        linker_names = ' | '.join(
            symbol['name'] for symbol in matching_symbols
        )

        map_rows.append(
            '\t'.join([
                '0x%08X' % address,
                '0x%08X' % body_end,
                '0x%X' % body_size,
                clean_tsv(function.getName()),
                clean_tsv(linker_names),
                clean_tsv(unit or '<unsplit>'),
                status,
            ])
        )

        if FLUSH_DECOMPILER_CACHE_EACH_FUNCTION:
            interface.flushCache()

    monitor.setProgress(len(functions))
    return buckets, orphans, failures, warnings, map_rows, success_count


# ---------------------------------------------------------------------------
# Output
# ---------------------------------------------------------------------------


def write_decompilation_files(outdir, buckets, orphans):
    unit_counts = []

    for unit in sorted(buckets):
        chunks = buckets[unit]
        chunks.sort(key=lambda item: item[0])

        path = safe_unit_output_path(outdir, unit)
        ensure_dir(os.path.dirname(path))

        with open(path, 'w', encoding='utf-8', newline='\n') as stream:
            stream.write(
                '/* Ghidra decompiler bodies grouped by linked TU/object: %s */\n'
                % escape_comment(unit)
            )
            stream.write(
                '/* Function bodies use Ghidra PrettyPrinter UI line text. */\n\n'
            )

            for _, chunk in chunks:
                stream.write(chunk)
                stream.write('\n')

        unit_counts.append((unit, len(chunks), path))

    if orphans:
        orphans.sort(key=lambda item: item[0])
        path = os.path.join(outdir, '_unsplit' + OUTPUT_SUFFIX)

        with open(path, 'w', encoding='utf-8', newline='\n') as stream:
            stream.write(
                '/* Functions whose entry point is outside every configured '
                'splits.txt code range. */\n\n'
            )
            for _, chunk in orphans:
                stream.write(chunk)
                stream.write('\n')

    return unit_counts


def write_reports(
    outdir,
    split_issues,
    symbol_issues,
    missing_symbols,
    boundary_mismatches,
    aliases,
    failures,
    warnings,
    map_rows,
    ghidra_without_symbols,
    summary_lines,
):
    report_dir = os.path.join(outdir, '_reports')
    ensure_dir(report_dir)

    write_lines(
        os.path.join(report_dir, 'summary.txt'),
        summary_lines,
    )

    map_heading = (
        'entry\tbody_end_exclusive\tbody_bytes\tghidra_name\t'
        'linker_symbols\tunit\tstatus'
    )
    write_text(
        os.path.join(report_dir, 'tu_function_map.tsv'),
        map_heading + '\n' + '\n'.join(map_rows) + ('\n' if map_rows else ''),
    )

    reports = [
        ('split_issues.txt', split_issues),
        ('symbol_parse_issues.txt', symbol_issues),
        ('symbols_missing_in_ghidra.txt', missing_symbols),
        ('function_boundary_mismatches.txt', boundary_mismatches),
        ('symbol_aliases.txt', aliases),
        ('decompile_failures.txt', failures),
        ('decompiler_warnings.txt', warnings),
        ('ghidra_functions_without_symbol.txt', ghidra_without_symbols),
    ]

    for filename, lines in reports:
        # Always overwrite reports so an older run cannot leave stale findings.
        write_lines(os.path.join(report_dir, filename), lines)


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------


def main():
    resolved = resolve_inputs()
    if resolved is None:
        return

    splits_path, outdir, symbols_path = resolved

    if not os.path.isfile(splits_path):
        printerr('splits.txt does not exist: %s' % splits_path)
        return

    if symbols_path is not None and not os.path.isfile(symbols_path):
        printerr('symbols.txt does not exist: %s' % symbols_path)
        return

    ensure_dir(outdir)

    ranges, split_issues = parse_splits(splits_path)
    if not ranges:
        printerr(
            'No code ranges were parsed from %s. Check UNIT_RE, SECTION_RE, '
            'and CODE_SECTIONS.' % splits_path
        )
        return

    split_index = SplitIndex(ranges)
    unit_names = sorted(set(item['unit'] for item in ranges))

    println(
        'Parsed %d code ranges across %d units'
        % (len(ranges), len(unit_names))
    )

    symbols = []
    symbol_issues = []
    symbols_by_address = defaultdict(list)
    missing_symbols = []
    boundary_mismatches = []
    aliases = []

    if symbols_path is not None:
        symbols, symbol_issues = parse_symbols(symbols_path)
        println('Parsed %d function symbols' % len(symbols))

        (
            symbols_by_address,
            missing_symbols,
            boundary_mismatches,
            aliases,
        ) = validate_symbols(symbols, split_index)

        println(
            'Symbol validation: %d missing, %d boundary mismatches, %d alias groups'
            % (len(missing_symbols), len(boundary_mismatches), len(aliases))
        )

    function_manager = currentProgram.getFunctionManager()
    functions = [
        function
        for function in function_manager.getFunctions(True)
        if not function.isExternal()
    ]
    functions.sort(key=lambda function: function.getEntryPoint().getOffset())

    ghidra_without_symbols = []
    if symbols_path is not None and REPORT_GHIDRA_FUNCTIONS_WITHOUT_SYMBOLS:
        for function in functions:
            address = function.getEntryPoint().getOffset()
            if address not in symbols_by_address:
                split = split_index.find(address)
                unit = split['unit'] if split else '<unsplit>'
                ghidra_without_symbols.append(
                    '0x%08X unit=%s %s'
                    % (address, unit, function.getName())
                )

    tool, options = get_decompiler_options()
    mode = 'GUI tool + program options' if tool is not None else 'headless/program options'

    println('Decompiler option source: %s' % mode)
    println('Decompiling %d non-external functions...' % len(functions))

    interface = create_decompiler(options)
    try:
        (
            buckets,
            orphans,
            failures,
            warnings,
            map_rows,
            success_count,
        ) = decompile_all(
            functions,
            interface,
            options,
            split_index,
            symbols_by_address,
        )
    finally:
        interface.dispose()

    unit_counts = write_decompilation_files(outdir, buckets, orphans)

    summary_lines = [
        'Program: %s' % currentProgram.getName(),
        'Output directory: %s' % outdir,
        'splits.txt: %s' % splits_path,
        'symbols.txt: %s' % (symbols_path or '<not supplied>'),
        'Decompiler option source: %s' % mode,
        'Decompiler timeout per function: %s seconds'
        % (
            FORCED_TIMEOUT_SECONDS
            if FORCED_TIMEOUT_SECONDS is not None
            else options.getDefaultTimeout()
        ),
        'Decompiler maximum line width: %s' % options.getMaxWidth(),
        '',
        'Code ranges: %d' % len(ranges),
        'Split units: %d' % len(unit_names),
        'Ghidra non-external functions: %d' % len(functions),
        'Successfully decompiled: %d' % success_count,
        'Decompile failures: %d' % len(failures),
        'Unsplit functions: %d' % len(orphans),
        'Written TU files: %d' % len(unit_counts),
        '',
        'Function symbols parsed: %d' % len(symbols),
        'Function symbols missing in Ghidra: %d' % len(missing_symbols),
        'Function boundary mismatches: %d' % len(boundary_mismatches),
        'Symbol alias address groups: %d' % len(aliases),
        'Ghidra functions without an exact symbol: %d'
        % len(ghidra_without_symbols),
        '',
        'Split parse/validation issues: %d' % len(split_issues),
        'Symbol parse issues: %d' % len(symbol_issues),
        'Decompiler warnings: %d' % len(warnings),
        '',
        'Per-unit counts:',
    ]

    for unit, count, path in unit_counts:
        summary_lines.append('  %5d  %s  ->  %s' % (count, unit, path))

    if orphans:
        summary_lines.append(
            '  %5d  <unsplit>  ->  %s'
            % (len(orphans), os.path.join(outdir, '_unsplit' + OUTPUT_SUFFIX))
        )

    write_reports(
        outdir=outdir,
        split_issues=split_issues,
        symbol_issues=symbol_issues,
        missing_symbols=missing_symbols,
        boundary_mismatches=boundary_mismatches,
        aliases=aliases,
        failures=failures,
        warnings=warnings,
        map_rows=map_rows,
        ghidra_without_symbols=ghidra_without_symbols,
        summary_lines=summary_lines,
    )

    println('')
    println('Done. Output: %s' % outdir)
    println('Successfully decompiled: %d/%d' % (success_count, len(functions)))
    println('TU files written: %d' % len(unit_counts))
    println('Unsplit functions: %d' % len(orphans))
    println('Reports: %s' % os.path.join(outdir, '_reports'))


main()
