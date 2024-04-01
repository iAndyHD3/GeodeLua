def filter_lines(input_file, output_file):
    # Read the contents of the input file
    with open(input_file, 'r') as file:
        lines = file.readlines()

    # Filter lines that start with "protected" or "private"
    filtered_lines = [line for line in lines if line.strip().startswith(("protected", "private"))]

    # Write the filtered lines to the output file
    with open(output_file, 'w') as file:
        file.writelines(filtered_lines)

if __name__ == "__main__":
    input_file = 'symbols_demangled.txt'
    output_file = 'protected_private.txt'
    filter_lines(input_file, output_file)
