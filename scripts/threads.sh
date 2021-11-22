#!/usr/bin/env bash

pid=$1

if [ -z $1 ]; then
  echo "ERROR: Process ID not specified."
  echo
  echo "Usage: $(basename "$0") <PID>"
  exit 1
fi

kill -0 $pid > /dev/null 2>&1
pid_exist=$?

if [ $pid_exist != 0 ]; then
  echo "ERROR: Process ID $pid not found."
  exit 1
fi

SCRIPT_DIR="$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
dir_name="$SCRIPT_DIR/../measures"
csv_filename="$dir_name/threads.csv"

mkdir -p $dir_name

trap "sudo $SCRIPT_DIR/threads.gpi" SIGINT SIGTERM SIGKILL

echo "Write threads data to: $csv_filename..."
touch $csv_filename

echo "Time;R;S;T;Z;D" > $csv_filename

kill -0 $pid > /dev/null 2>&1
pid_exist=$?

while [ $pid_exist == 0 ]; do
  kill -0 $pid > /dev/null 2>&1
  pid_exist=$?

  if [ $pid_exist == 0 ]; then
      timestamp=$(date +"%b %d %H:%M:%S")
      top=$(top -H -p $pid -b -n 1 | tail -n +8 | awk '{print $1, $8"\n"}')

      R=$(echo "$top" | grep "R" | wc -l)
      S=$(echo "$top" | grep "S" | wc -l)
      T=$(echo "$top" | grep "T" | wc -l)
      Z=$(echo "$top" | grep "Z" | wc -l)
      D=$(echo "$top" | grep "D" | wc -l)

	  echo "$timestamp;$R;$S;$T;$Z;$D" >> $csv_filename
    sleep 1
  fi
done

sudo $SCRIPT_DIR/threads.gpi

