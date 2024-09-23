import subprocess
import sys

def get_vcs_id():
    """
    Attempts to retrieve the VCS identifier using the 'git describe' command.

    Uses git to retrieve the version control system (VCS) identifier.
    It is designed to handle cases where Git is not installed or the current
    directory is not a Git repository, without raising exceptions.
    
    Returns:
        str: A string containing the VCS identifier if successful, or an empty 
             string if Git is not available or the directory is not a Git repository.
    
    All warnings are printed to stderr.
    """
    try:
        vcs_output = subprocess.run(
            ["git", "describe", "--always", "--dirty", "--all", "--long"],
            stdout=subprocess.PIPE, text=True, check=True
        )
        vcs_string = vcs_output.stdout.strip()
        return vcs_string
    except (subprocess.CalledProcessError, FileNotFoundError) as e:
        print("Warning: Unable to retrieve VCS description. Error:", str(e), file=sys.stderr)
        return ""
