"""
generate_vcs_identifier.py

This script generates a source file containing a version control system (VCS)
identifier string. It retrieves the VCS identifier from the 'vcs_utils' module
and writes it to a specified source file.

The script will overwrite the file if it already exists, and any write failures 
are handled gracefully.

This script should be loaded by PlatformIO during the project build and is not
designed to be run independently.
"""

from vcs_utils import get_vcs_id, write_vcs_id_to_file
import os.path
Import("env")

# Get the source directory path from PlatformIO configuration
source_dir = env.subst("$PROJECT_DIR")

# Path to the output file within
output_file = os.path.join(source_dir, 'lib', 'utilities', 'vcs_identifier_gen.hpp')

vcs_id = get_vcs_id()

if vcs_id:
    write_vcs_id_to_file(vcs_id, output_file)
