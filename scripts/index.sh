#!/bin/bash

# Script index.sh
#
# This script provides utils for using the index file for managing evaluation
# data. Meant to be used from scripts to automatically store and remove
# training runs from the project directory.

# Bash safe mode.
# From: http://redsymbol.net/articles/unofficial-bash-strict-mode/
set -euo pipefail
IFS=$'\n\t'

# Move to parent directory for behaviour independant of script location
parent_path=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )
cd "$parent_path"

# Usage prompt
if [ $# -eq 0 ]; then
  echo "Usage: ./$0 {--next|--newest|--save|--info|--help} [params...]"
  exit 1;

# Help promt
elif [[ "$1" == "help" ]]; then
  if [ $# -eq 1 ]; then
    echo "Perform operation on and with the eval index file eval/index.txt.";
    echo "\t--next: Print next available ID";
    echo "\t--newest: Print last used ID";
    echo "\t--save <which>: Save temporary file(s) and create index entry.";
    echo "\t--help [<command>]: Show help to one of the commands above.";
  elif [[ "$2" == "--save" ]]; then
    echo "--save {all|tstats|fstats|plot} [id]"
    echo "This command moves a temporary evaluation file into the permanent storage directory.";
    echo "One can choose to move any of the testing/evaluation statistics and final plot.";
    echo "Value \"all\" will perform the move for each of them.";
    echo "id determines the used index ID when creating new files. Do not use from outside of this script";
  elif [[ "$2" == "--info" ]]; then
    echo "--info [id]";
    echo "This command prints the \"info\" line of the traning/evaluation run corresponding to the given ID";
    echo "The default is id=0, printing the information of the temporary data";
    echo "If no entry exists for the given ID, the script exits non-zero.";
  else
    echo "No help for unrecognized parameter \"$2\"";
  fi

  exit 1; # Any help promt exists w/ failure
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
  echo "$((next_val + 1))";

# Command --newest to retrieve the last used index value
elif [[ "$idx_command" == "--newest" ]]; then
  # Output first column of last line of index_file
  if [ -s "$index_file" ]; then
    echo "$(tail "$index_file" -n1 | cut -s -d " " -f1)";
  else
    echo "0";
  fi

# Command --save to store temporary files in index
elif [[ "$idx_command" == "--save" ]]; then

  # Ensure correct arg count
  if [ $# -gt 3 ]; then
    echo "Expected 1 or 2 parameters for --save, got $(( $# -1 ))";
    exit 1;
  fi

  # Determine ID for saving by arg 3
  id_next_by_increment="$(bash index.sh --next)";
  if [ $# -ge 3 ]; then
    save_id="$3";
    if [ "$save_id" -gt "$id_next_by_increment" ] ||
       [ "$save_id" -lt "$(($id_next_by_increment - 1))" ]; then
      # Unreachable. Only called by "--save all".
      echo "[ERROR] $0: --save ID \"$save_id\" invalid.";
      echo "Do not use the ID parameter from outside of this script.";
      exit 1;
    fi
  else
    save_id="$id_next_by_increment";
  fi

  # Determine data to save by arg 2
  param_which="$2";

  # Delegate the "all" parameter
  if [[ "$param_which" == "all" ]]; then
    # Pass save_id such that the delegated parts don't all increment ID
    bash index.sh "$idx_command" "tstats" "$save_id" > /dev/null;
    bash index.sh "$idx_command" "fstats" "$save_id" > /dev/null;
    bash index.sh "$idx_command" "plot" "$save_id" > /dev/null;
  else
    # Determine source and target file names
    eval_dir="../eval";
    if [[ "$param_which" == "tstats" ]]; then
      file_from="$eval_dir/0_training.dat";
      file_to="$eval_dir/training_stats/${save_id}_training.dat";
    elif [[ "$param_which" == "fstats" ]]; then
      file_from="$eval_dir/0_final.dat";
      file_to="$eval_dir/final_stats/${save_id}_final.dat";
    elif [[ "$param_which" == "plot" ]]; then
      file_from="$eval_dir/0_plot.png";
      file_to="$eval_dir/plots/${save_id}_plot.png";
    else
      echo "Unrecognized parameter \"$param_which\" for command --save";
      exit 1;
    fi

    # Determine necessity of adding info to the index file
    set +e;
    ignore="$(./index.sh --info "$save_id")";
    index_entry_search="$?";
    set -e;

    # Add info to index file, if needed
    if [ "$index_entry_search" -ne 0 ]; then
      info_str="$(./index.sh --info 0)";
      echo "$save_id $info_str" >> "$eval_dir/index.txt";
      echo "[SUCCESS] $0: Information string for ID $save_id appendded to index.txt.";
    fi
    # Move file
    mv -v "$file_from" "$file_to";
    if [ $? -ne 0 ]; then echo "[ERROR] $0 --save: Move failed."; exit 1;
    fi
  fi # delegate y/n
  echo "[SUCCESS] $0: Evaluation data saved as ID=$save_id.";

# Command --info to retrieve the info part of the index line
elif [[ "$idx_command" == "--info" ]]; then

  # Determine index of interest
  if [ $# -lt 2 ]; then
    index_of_interest="0";
  else
    index_of_interest="$2";
  fi

  # Read corresponding index line
  # TODO Input validation could help. Allow id if matching regex ^[0-9]+$.
  if [ "$index_of_interest" -eq 0 ]; then
  #if [[ "$index_of_interest" == 0 ]]; then
    index_line="$(head -n1 "../eval/index0.txt")";
  else
    # Search for line beginning with correct ID followed by whitespace.
    # Only keep anything after the ID (the first space).
    # The option "set -e" means this exits immediately if grep does not find
    # a match, which is intended.
    index_line="$(grep "../eval/index.txt" -ie "^$index_of_interest\\s")";
  fi

  # Extract information string from the index line
  info_string="$(echo "$index_line" | cut -s -d " " -f2-)";

  # Put information string on stdout
  echo "$info_string";

fi # Commands if-else

exit 0;
