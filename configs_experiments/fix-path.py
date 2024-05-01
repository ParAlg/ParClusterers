import os
import fileinput

def replace_string_in_files(directory, string_to_replace, new_string, current_directory):
    for root, _, files in os.walk(os.path.join(current_directory, "configs_experiments")):
        for filename in files:
            if filename.startswith("cluster_") and filename.endswith(".config"):
                print("fixing ", filename)
                file_path = os.path.join(root, filename)
                with fileinput.FileInput(file_path, inplace=True) as file:
                    for line in file:
                        print(line.replace(string_to_replace, new_string), end='')

# Get the absolute path of the current directory
current_directory = os.path.abspath(os.getcwd())

# Define the string to replace and its replacement
string_to_replace = "/home/sy/ParClusterers"
new_string = current_directory

# Replace the string in all matching files
replace_string_in_files(".", string_to_replace, new_string, current_directory)
