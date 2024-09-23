"""Adds a CPP definition of a version identifier using version control system.

This script shall be loaded by PlatformIO when building the project.
It is not designed to be run independently.
"""

import subprocess
Import("env")

def get_vcs_id():    
    vcs_output = subprocess.run(["git", "describe", "--always", "--dirty", "--all"], stdout=subprocess.PIPE, text=True)
    vcs_string = vcs_output.stdout.strip()
    print ("vcs string: " + vcs_string)
    return (vcs_string)

env.Append(CPPDEFINES=[
  ("VCS_ID", env.StringifyMacro(get_vcs_id())),
])