def snap_connectivity(file_path, output_path=None, num_lines=3):
    # Open the file for reading
    with open(file_path, 'r') as file:
        lines = file.readlines()

    # Remove the first 'num_lines' lines
    remaining_lines = lines[num_lines:]

    # Remove the first number before tab for each line
    remaining_lines = [line.split('\t', 1)[1] for line in remaining_lines]

    # Write the remaining lines to the same file or a new file
    if output_path is None:
        output_path = file_path

    with open(output_path, 'w') as file:
        file.writelines(remaining_lines)
