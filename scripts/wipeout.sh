#!/bin/bash

# Script wipeout.sh to remove all generated training and other data and
# re-construct the required directory structures.

# Bash safe mode.
# From: http://redsymbol.net/articles/unofficial-bash-strict-mode/
set -euo pipefail
IFS=$'\n\t'

# Move to parent directory for behaviour independant of script location
parent_path=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )
cd "$parent_path"


if [ $# -eq 0 ]; then
  echo "Usage: $0 {eval|build|everything}";
  exit 1;
fi

if [[ "$1" == "--help" ]]; then
  echo "Help not implemented yet.";
  exit 1;
fi

################################################################################
# Commands
################################################################################

cmd="$1";
project_root="..";
if [[ "$cmd" == "eval" ]]; then
  rm -rf "$project_root/eval";
  # TODO Re-establish directory structure
  echo "[SUCCESS] $0: Wiped out evaluation files.";
elif [[ "$cmd" == "build" ]]; then
else
  echo "Not error message yet.";
fi

exit 0;

# TODO delegate the re-construction of the directory structure to
#      a different script? Or just different command?
