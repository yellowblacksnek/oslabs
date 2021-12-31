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
csv_filename="$dir_name/io.csv"

mkdir -p $dir_name

trap "sudo $SCRIPT_DIR/io.gpi" SIGINT SIGTERM SIGKILL

echo "Write IO data to: $csv_filename..."
touch $csv_filename

echo "Time;IO_read;IO_write" > $csv_filename

kill -0 $pid > /dev/null 2>&1
pid_exist=$?

while [ $pid_exist == 0 ]; do
  kill -0 $pid > /dev/null 2>&1
  pid_exist=$?

  if [ $pid_exist == 0 ]; then
    timestamp=$(date +"%b %d %H:%M:%S")

      io=$(iotop -o -qqq -b -n 1 -P -k)
      io=$(echo ${io} | grep "${pid}" )
      if [[ "$io" != "" ]]; then
        #echo $io
        io_read=$(echo $io | awk '{print $4}')
        io_write=$(echo $io | awk '{print $6}')

      else
        io_read=""
        io_write=""
      fi

      if [[ "$io_read" == "" ]]; then
        io_read="0.00"
      fi
      if [[ "$io_write" == "" ]]; then
        io_write="0.00"
      fi

    echo "$timestamp;$(bc<<<"scale=7; ${io_read}/1024");$(bc<<<"scale=7; ${io_write}/1024")" >> $csv_filename
    sleep 2
  fi
done

sudo $SCRIPT_DIR/io.gpi
