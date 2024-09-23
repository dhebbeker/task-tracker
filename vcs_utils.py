"""
vcs_utils.py

This module provides utility functions to retrieve the version control system
(VCS) identifier using Git and to write it to a C source file.

The functions handle cases where Git is not installed or the current directory
is not a Git repository, and handle file write failures gracefully.
"""

import subprocess
import sys

def get_vcs_id():
    """
    Attempts to retrieve the VCS identifier using the 'git describe' command.
    
    Returns:
        str: A string containing the VCS identifier if successful, or an empty 
             string if Git is not available or the directory is not a Git repository.
    
    This function handles the following exceptions:
        - subprocess.CalledProcessError: Raised if the 'git' command fails, e.g.,
          if the current directory is not a Git repository.
        - FileNotFoundError: Raised if 'git' is not installed or not found in the
          system's PATH.
    
    All warnings are printed to stderr.
    """
    try:
        vcs_output = subprocess.run(
            ["git", "describe", "--always", "--dirty", "--all", "--long"],
            stdout=subprocess.PIPE, text=True, check=True
        )
        vcs_string = vcs_output.stdout.strip()
        print("vcs string: " + vcs_string)
        return vcs_string
    except (subprocess.CalledProcessError, FileNotFoundError) as e:
        # Print the warning to stderr
        print("Warning: Unable to retrieve VCS description. Error:", str(e), file=sys.stderr)
        return ""


def write_vcs_id_to_file(vcs_id, file_path):
    """
    Writes the given VCS identifier as a C-style string to a file.
    
    Args:
        vcs_id (str): The VCS identifier string to write to the file.
        file_path (str): The path to the C source file to write to.
    
    If the file exists, it will be overwritten. If writing to the file fails,
    a warning will be printed to stderr, but the script will not terminate
    with an error.
    """
    try:
        # Prepare the C-style string definition
        c_content = f'#define VCS_ID "{vcs_id}"\n'
        
        # Write the content to the file (overwriting if it exists)
        with open(file_path, 'w') as file:
            file.write(c_content)
    
    except IOError as e:
        # If writing to the file fails, print the warning to stderr
        print(f"Warning: Unable to write VCS ID to file '{file_path}'. Error: {str(e)}", file=sys.stderr)
