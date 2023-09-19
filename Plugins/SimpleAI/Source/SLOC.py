import os

def count_cpp_lines(directory):
    cpp_file_extensions = [".cpp", ".h", ".hpp", ".cc"]
    total_lines = 0
    total_files = 0

    for root, dirs, files in os.walk(directory):
        for file in files:
            if any(file.endswith(ext) for ext in cpp_file_extensions):
                print('file:', file)
                total_files += 1
                with open(os.path.join(root, file), 'r', encoding='utf-8', errors='ignore') as f:
                    total_lines += len(f.readlines())

    return total_lines, total_files

# Replace "your_directory_path_here" with the path to the directory you want to analyze
directory_path = input()
line_count, file_count = count_cpp_lines(directory_path)
print(f"The total number of lines in C++ files in the directory '{directory_path}' is: {line_count} with {file_count} files")
