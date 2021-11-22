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
csv_filename="$dir_name/cpu.csv"

mkdir -p $dir_name

echo "Write CPU data to: $csv_filename..."
touch $csv_filename

echo "Time;CPU" > $csv_filename

kill -0 $pid > /dev/null 2>&1
pid_exist=$?

while [ $pid_exist == 0 ]; do
  # check if process exists
  kill -0 $pid > /dev/null 2>&1
  pid_exist=$?

  if [ $pid_exist == 0 ]; then
    timestamp=$(date +"%b %d %H:%M:%S")
    cpu=$(top -b -n 2 -d 1 -p $1 | tail -1 | awk '{print $9}')

    echo "$timestamp;$cpu" >> $csv_filename
    sleep 1
  fi
done

sudo $SCRIPT_DIR/cpu.gpi