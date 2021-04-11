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
  echo "Usage: $0 {--create_comparison|--create_single} [<params...>]"
  echo "This scripts plots the training everage of the evaluation runs specified by IDs.";
  echo "IDs is *one* string containing a space-separated list if evaluation IDs to be plotted.";
  echo "An example usage could be \'$0 \"1 2 3 15\";\'.";
fi

################################################################################
# Dispatch commands
################################################################################

readonly project_path="..";

plt_command="$1";

# Command --create_script that writes a gnuplot script file comparing multiple
# training runs.
# TODO Change the create_... commands into creating + immediate plotting
if [[ "$plt_command" == "--create_compare" ]]; then
  set +e;
  ./create_gnuplot_script.sh --create_compare "${@:2}";
  ret=$?;
  set -e;
  if [ "$ret" -ne 0 ]; then
    echo "[ERROR] $0 --create_compare: Creation of gnuplot script failed".;
  else
    echo "[SUCCESS] $0 --create_compare: created gnuplot script.";
  fi
  exit "$ret";
fi

# Command --create_single that writes a gnuplot script file for a singel
# evaluation run.
# TODO Change the create_... commands into creating + immediate plotting
if [[ "$plt_command" == "--create_single" ]]; then
  set +e;
  bash ./create_gnuplot_script.sh --create_single;
  ret=$?;
  set -e;
  if [ "$ret" -ne 0 ]; then
    echo "[ERROR] $0 --create_single: Creation of gnuplot script failed".;
  else
    echo "[SUCCESS] $0 --create_single: Created gnuplot script.";
  fi
  exit "$ret";
fi

# Command --show to plot the last created gnuplot script
if [[ "$plt_command" == "--show" ]]; then
  if [ ! -f "../eval/plot.gpi" ]; then
    echo "[ERROR] $0 --show: No temporary plot prepared.";
    exit 1;
  fi
  (cd "$project_path"; gnuplot "eval/plot.gpi";);
  # TODO for testign show immediately
  _png "$project_path/eval/0_plot.png";
  exit 0;
fi

echo "[ERROR] $0: Unknown command \"$plt_command\".";
exit 1;
