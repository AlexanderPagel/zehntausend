#!/bin/bash

# 

# Bash safe mode.
# From: http://redsymbol.net/articles/unofficial-bash-strict-mode/
set -euo pipefail
IFS=$'\n\t'

# Move to parent directory for behaviour independant of script location
parent_path=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )
cd "$parent_path"

# Usage and help
if [ $# -eq 0 ]; then
  echo "Usage: ./$0 {--next|--newest}"
  exit 1
fi

################################################################################
# Commands
################################################################################

idx_command="$1";
project_dir="..";
index_file="$project_dir/eval/index.txt";

# Command --next to retrieve the next available index value
if [[ "$idx_command" == "--next" ]]; then
  next_val="$(bash "index.sh" "--newest")";
  echo "$((next_val + 1))"
  exit 1;

# Command --newest to retrieve the last used index value
elif [[ "$idx_command" == "--newest" ]]; then
  # Output first column of last line of index_file
  echo "$(tail "$index_file" -n1 | cut -d " " -f1)"

fi

exit 0;
