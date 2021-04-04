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

# TODO Help and usage not up to date
if [ $# -eq 0 ]; then
  exit 1;
fi
if [[ "$1" == "--help" ]]; then
  echo "Usage: $0 {--create|--plotline|--help} [<params...>]";
  echo "This scripts plots the training everage of the evaluation runs specified by IDs.";
  echo "IDs is *one* string containing a space-separated list if evaluation IDs to be plotted.";
  echo "An example usage could be \'$0 \"1 2 3 15\";\'.";
  exit 1;
fi


################################################################################
# Input validation
################################################################################

cplt_command="$1";
cplt_command_args=("${@:2}");

# Force a list of uint parameters after initial command
for arg in "${cplt_command_args[@]}"; do
  if [[ "$(./is_uint.sh "$arg")" == "0" ]]; then
    echo "[ERROR] $0: Invalid argument \"$arg\" (expected evaluation ID).";
  fi
done

################################################################################
# Commands
################################################################################

readonly project_root="..";
readonly eval_dir="$project_root/eval";
readonly temp_plotscript_name="plot.gpi";
readonly temp_plotscript_path="$eval_dir/plot.gpi";
readonly template_file_path="template_compare_plotscript.txt";

################################################################################
# Command --plotline to generate the line of gnuplot script required to add the
# graph of the requested ID to the graphic.
if [[ "$cplt_command" == "--plotline" ]]; then

  # Validate args
  if [ $# -ne 3 ]; then
    >&2 echo "[ERROR] $0: Expected 2 argument to --plotline, got \'$cplt_command_args\'.";
    exit 1;
  fi
  if [[ "$(./index.sh --test ${cplt_command_args[0]})" != "1" ]]; then
    >&2 echo "[ERROR] $0: ID \"${cplt_command_args[0]}\" does not exist.";
    exit 1;
  fi
  readonly data_id="$2";
  set +e;
  readonly data_path="$(./index.sh --data "$data_id" "$3")";
  ret="$?";
  set -e;
  if [ "$ret" -ne 0 ]; then
    >&2 echo "[ERROR] $0 --plotline: No data for \'${cplt_command_args[@]}\'";
    exit 1;
  fi

  # TODO In the future, this script could also be used to identify different
  #      stats formats to allow consistent printing when the format is changed.

  # Print the plotline by inserting the file name into a default scheme
  echo '     "'"$data_path"'" using 1:3, \';
  exit 0;

fi

################################################################################
# Command --create to write eval/plot.gpi
if [[ "$cplt_command" == "--create" ]]; then

  # Begin with template
  cp -vf "$template_file_path" "$temp_plotscript_path";

  # Add plotlines for each ID one by one
  one_or_more="0"; # Flag to abort if not a single line is valid
  for id in "${cplt_command_args[@]}"; do
    # Determine plotline
    set +e;
    #2>/dev/null data_path="$(./index.sh --data "$id" "tstats")";
    2>/dev/null plotline="$(./create_gnuplot_script.sh --plotline "$id" "tstats")";
    ret="$?";
    set -e;
    if [ "$ret" -ne 0 ]; then
      echo "[WARNING] $0 --create: Skipping ID=$id (no data found).";
      continue;
    fi

    # Add plotline
    echo "$plotline" >> "$temp_plotscript_path";
    echo "[SUCCESS] $0: Added plotline for ID \"$id\".";
    one_or_more="1";
  done
  # Add final empty line because every plotline ends with '\'
  echo -en '\n' >> "$temp_plotscript_path";

  # If not a singel line was added that is an error
  if [[ "$one_or_more" == "0" ]]; then
    echo "[ERROR] $0 --create: No data found for IDs \'${cplt_command_args[@]}\'.";
    exit 1;
  fi

  echo "[SUCCESS] $0 --create: Created temporary gnuplot script \"$temp_plotscript_name\".";
  exit 0;

fi

echo "[ERROR] $0: Unrecognized command \"$cplt_command\".";
exit 1;

# Tempalte format:
#plot "0_training.dat" using 26:27 w p pt 7 ps 1 lc rgb "grey40", \
#     "0_final.dat" using 1:2 w lp lt -1 lw 3, \
#     "0_training.dat" using 18:19 lt 1 lw 3 lc rgb "slategrey", \
#     "0_training.dat" using 10:11 lt 2 lw 5 lc rgb "skyblue", \
#     "0_training.dat" using 1:3 lt 2 lw 3 lc rgb "cyan"
