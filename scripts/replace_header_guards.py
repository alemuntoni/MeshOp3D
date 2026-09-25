import os
import glob

def replace_header_guard(file_string, file_path, module_name):
    # remove extension from file_path
    file_path = os.path.splitext(file_path)[0]

    # split path into components
    parts = file_path.replace('\\', '/').split('/')
    
    # exclude 'include', and if the next folder is 'mop' or the module name, exclude it too
    new_parts = []
    i = 0
    while i < len(parts):
        p = parts[i]
        if not p:
            i += 1
            continue
        if p == 'include':
            if i + 1 < len(parts) and (parts[i+1] == 'mop' or parts[i+1] == module_name):
                i += 1
        else:
            new_parts.append(p)
        i += 1
    parts = new_parts

    # replace '/' with '_' in the file path
    path_guard = '_'.join(parts).upper()

    # add 'MOP_', module name and '_H' suffix
    if path_guard:
        header_guard = f"MOP_{module_name.upper()}_{path_guard}_H"
    else:
        header_guard = f"MOP_{module_name.upper()}_H"

    # look for the first line that starts with '#ifndef', and replace that line
    # and the following one with the new header guard
    lines = file_string.split('\n')
    for i, line in enumerate(lines):
        if line.startswith('#ifndef'):
            lines[i] = '#ifndef ' + header_guard
            lines[i + 1] = '#define ' + header_guard
            break

    # now replace the latest line that starts with '#endif' and replace it with
    # a new line in the form of '#endif // ' + header_guard
    for i, line in enumerate(reversed(lines)):
        if line.startswith('#endif'):
            lines[-i - 1] = '#endif // ' + header_guard
            break
    
    return '\n'.join(lines)

def replace_header_guards_in_dir(folder_path, module_name):
    for file_path in glob.glob(os.path.join(folder_path, '*')):
        if os.path.isdir(file_path):
            replace_header_guards_in_dir(file_path, module_name)
        elif file_path.endswith(('.h', '.hpp')):
            # Read in the file
            with open(file_path, 'r') as file:
                file_string = file.read()

            file_string = replace_header_guard(file_string, file_path, module_name)

            with open(file_path, 'w') as file:
                file.write(file_string)

if __name__ == "__main__":
    mop_modules = ['actions', 'application', 'core']

    script_dir = os.path.dirname(os.path.abspath(__file__))
    root_dir = os.path.abspath(os.path.join(script_dir, '..'))
    os.chdir(root_dir)

    for module in mop_modules:
        if os.path.isdir(module):
            os.chdir(module)

            replace_header_guards_in_dir('', module)

            os.chdir('../')
