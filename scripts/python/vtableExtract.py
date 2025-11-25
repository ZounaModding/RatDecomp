from cwfilt import demangle

import re
import sys

def extract_vtable_function_names(text):
    vtable_match = re.search(r'__vt__\d+(\w+)', text)
    if not vtable_match:
        return []

    vtable_name = vtable_match.group(1)

    pattern = re.compile(r'\.4byte\s+(\w*' + re.escape(vtable_name) + r'\w*)')
    pattern_match = [vtable_name] + pattern.findall(text)
    return pattern_match

def printClasses(funcs):
    class_name = funcs[0]

    typedefs = {
        'char': 'Char',
        'unsigned long': 'U32',
        'long': 'S32',
        'unsigned short': 'U16',
        'short': 'S16',
        'unsigned char': 'U8',
        'signed char': 'S8',
        'float': 'Float',
        'double': 'F64',
        'unsigned long long': 'U64',
        'long long': 'S64',
    }

    typedef_keys = sorted(typedefs.keys(), key=len, reverse=True)

    def replace_typedefs(type_str):
        for key in typedef_keys:
            type_str = re.sub(r'\b' + re.escape(key) + r'\b', typedefs[key], type_str)
        return type_str

    declarations = []
    temp_offset = 8
    
    for i in range(1, len(funcs)):
        if '__dt' in funcs[i]:
            decl = f'\tvirtual ~{class_name}();'
        else:
            method = demangle(f" {funcs[i]}")
            if method.startswith(f'{class_name}::'):
                method = method[len(f'{class_name}::'):]

            name, params = method.split('(', 1)
            params = params.rstrip(')')

            param_list = [p.strip() for p in params.split(',') if p.strip()]

            if len(param_list) == 1 and param_list[0] == 'void':
                param_str = ''
            else:
                named_params = []
                for j, p in enumerate(param_list):
                    replaced_type = replace_typedefs(p)
                    named_params.append(f'{replaced_type} a{j+1}')
                param_str = ', '.join(named_params)

            decl = f'\tvirtual void {name}({param_str});'
        
        declarations.append((decl, temp_offset))
        temp_offset += 4

    max_length = max(len(decl) for decl, _ in declarations)
    
    body = ''
    for decl, offset in declarations:
        padding = max_length - len(decl)
        body += f'{decl}{" " * padding} /* 0x{offset:02X} */\n'

    print(f'class {class_name}\n{{\n{body}}}')

def main():
    if len(sys.argv) < 2:
        print("Usage: python extract_vtable_funcs.py <filename>")
        return

    filename = sys.argv[1]
    try:
        with open(filename, 'r') as f:
            text = f.read()
    except FileNotFoundError:
        print(f"File not found: {filename}")
        return

    result = extract_vtable_function_names(text)
    printClasses(result)

if __name__ == "__main__":
    main()
