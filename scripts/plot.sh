#!/bin/bash

# plot.sh
# Script to plot stats from multiple training/eval runs together. Reads the
# max-drag stats from each run and plots them in the same graphic.

# Bash safe mode.
# From: http://redsymbol.net/articles/unofficial-bash-strict-mode/
set -euo pipefail
IFS=$'\n\t'

# Move to parent directory for behaviour independent of script location
parent_path=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )
cd "$parent_path"


################################################################################
# Usage
################################################################################

# TODO usage info and help are no longer up to date
if [ $# -eq 0 ]; then
  echo "Usage: $0 {--plot} [<params...>]"
  echo "This scripts plots the training everage of the evaluation runs specified by IDs.";
  echo "IDs is *one* string containing a space-separated list if evaluation IDs to be plotted.";
  echo "An example usage could be \'$0 \"1 2 3 15\";\'.";
fi

################################################################################
# Dispatch commands
################################################################################

plt_command="$1";

# Command --create_script that writes a gnuplot script file for the intended purpose
if [[ "$plt_command" == "--create_script" ]]; then
  set +e;
  ./create_gnuplot_script.sh "${@:2}";
  ret=$?;
  set -e;
  if [ "$ret" -ne 0 ]; then
    echo "[ERROR] $0: Creation of gnuplot script failed".;
  else
    echo "[SUCCESS] $0: created gnuplot script.";
  fi
  exit "$ret";
fi

# Command --show to plot the last created gnuplot script
if [[ "$plt_command" == "--show" ]]; then
  echo "Not implemented yet.";
  exit 1;
fi
