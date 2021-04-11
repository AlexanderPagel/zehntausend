#!/bin/bash

# Bash safe mode.
# From: http://redsymbol.net/articles/unofficial-bash-strict-mode/
set -euo pipefail
IFS=$'\n\t'

# Move to parent directory for behaviour independant of script location
parent_path=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )
cd "$parent_path"


# Usage and help
if [ $# -ne 1 ]; then
  echo "Usage: $0 <input_str>";
  echo "Matches the first argument with a regex for unsigned integer values".;
  echo "Outputs "1" if the input string is an unsigned integer, "0" otheerwise.";
  echo "(Returns with exis status 0 if a decision is made, nonzero otherwise.)";
  exit 1;
fi


################################################################################
# Output 1 if input is an integral number
################################################################################

# Ensure just 1 input line
input="$1";
line_count=$(echo "$input" | wc -l);
if [ "$line_count" -ne "1" ]; then
  # Use stderr since stdout is used to return value
  >&2 echo "[ERROR] $0: Input not a single line.";
  exit 1;
fi

# If the only line matches digits only, then we exit with zero
ignore=$(echo "$1" | grep -q -E '^[0-9]+$');
if [ $? -ne 0 ]; then
  echo "0"; # Echo false to indicate input is not a uint
  exit 1;
else
  echo "1"; # Echo true to indicate input is a uint
  exit 0;
fi
