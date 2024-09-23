"""
generate_vcs_identifier.py

This script adds a CPP definition of a version identifier using version control
system information. It retrieves the VCS identifier from the 'vcs_utils' module
and appends it to the PlatformIO build environment as a preprocessor definition
if available.

This script should be loaded by PlatformIO during the project build and is not
designed to be run independently.
"""

from vcs_utils import get_vcs_id
Import("env")

# Get the VCS ID and conditionally append it to the environment definitions
vcs_id = get_vcs_id()
if vcs_id:  # Only append if a valid VCS ID is retrieved
    env.Append(CPPDEFINES=[
        ("VCS_ID", env.StringifyMacro(vcs_id)),
    ])
