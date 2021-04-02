#!/bin/false

# ./evaluate.sh sarsa "100000 0.01 0.2"
# The above starts evaluation of the "sarsa" algorithm with parameters 100k
# episodes, learning rate 0.01 and exploration 0.2. The parameter format
# depends on the algorithm (see help).

# Mid-level evaluation script. Ports the argument lists to whatever input
# format the binary expects. Use for all evaluation runs.
# Currently, this mainly means that the parameters are put into stdin rather
# than program arguments.

# Boilerplate safe mode
set -euo pipefail # http://redsymbol.net/articles/unofficial-bash-strict-mode/
IFS=$'\n\t'

# Move to parent directory for behaviour independant of script location
parent_path=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )
cd "$parent_path"

# Basic usage
if [ $# -lt 2 ]; then
  echo "Usage: ./$0 <algorithm> <parameters...>"
  echo "     : ./$0 help [<algorithm>]"
  exit 1; # failed usage
fi

echo "NUMBER OF ARGS:" $#;

# Detailed help
if [ $1 == "help" ] || [ $1 == "--help" ] || [ $1 == "-h" ]; then
  if [ $# == 2 ]; then
    echo "<algorithm> Any of {sarsa}";
    echo "<parameter_string> Algorithm specific parameter list (see help <algorithm>)";
  elif [ $# == 3 ]; then
    if [ $2 == "sarsa" ]; then
      echo "sarsa parameters: <N> <learning_rate> <exploration_rate>" where N is an integer and the others are floats
    fi
  else
    echo "Usage: ./$0 help [<algorithm>]"
    exit 1; # help command was wrong
  fi
  exit 0 # legit help
fi

################################################################################
# Execution
################################################################################

# Read script arguments
algorithm="$1";

# Read algorithm parameters
parameters="";
if [ "$algorithm" = "sarsa" ]; then
  if [ $# -ne 4 ]; then echo "[ERROR] Invalid number of arguments"; exit 1; fi
  episode_count="$2";
  learning_rate="$3";
  epsilon="$4";
  parameters="$episode_count $learning_rate $epsilon";
elif [[ "$algorithm" == "foo" ]]; then
  echo "[ERROR] Algorithm not implemented";
  exit 1;
else
  echo "[ERROR] Unknown algorithm $algorithm";
  exit 1;
fi

# Invoke binary
BINARY="zehntausend";
if [[ "$algorithm" == "sarsa" ]]; then
  # ommand=evaluate algorithm=sarsa stdin="n alpha epsilon"
  echo "$parameters" | "../$BINARY" "evaluate" "$algorithm";
  RET="$?";
  if [ "$RET" -ne 0 ]; then
    echo "Evaluation failed: Binary returned \"$RET\"";
    exit 1;
  fi
else
  echo "Unknown Algorithm: \"$algorithm\"";
  exit 1;
fi

echo "[SUCCESS] $0:Evaluation completed."
exit 0;
