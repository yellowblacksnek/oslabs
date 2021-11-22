#!/usr/bin/env bash

scripts_dir="scripts"

dir_name="measures"
mkdir -p $dir_name

ps_file="${dir_name}/threads_count.txt"
lsof_file="${dir_name}/files.txt"
maps_file="${dir_name}/mmap.txt"
strace_file="${dir_name}/strace.txt"
tcpdump_file="${dir_name}/tcpdump.txt"

cpu_file="${dir_name}/cpu.csv"
io_file="${dir_name}/io.csv"
net_file="${dir_name}/net.csv"
threads_file="${dir_name}/threads.csv"

./lab60e030 &
PID=$!
sleep 1
ps -T -p $PID | tail -n +2> $ps_file
lsof -p $PID > $lsof_file

cat /proc/$PID/maps > $maps_file

sudo strace -p $PID -f -e trace=network -t 2> $strace_file &
STRACE=$!

LINES=$(grep "TCP" $lsof_file | awk '{print $9}' | awk -F: '{print $2}')

COMMAND="tcpdump -i any -A"

for LINE in $LINES
do
	COMMAND="${COMMAND} port ${LINE} or"
done

eval "${COMMAND::-2} > $tcpdump_file &"
TCPDUMP=$!

function clean() {

	echo

	kill $PID
	kill $STRACE 
	kill $TCPDUMP
}

trap "clean" SIGINT SIGTERM SIGKILL

kill -0 $PID > /dev/null 2>&1
pid_exist=$?

while [ $pid_exist == 0 ]; do

  kill -0 $PID > /dev/null 2>&1
  pid_exist=$?

  sleep 1
done

clean
