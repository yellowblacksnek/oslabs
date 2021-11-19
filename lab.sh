#!/bin/bash

./lab &
PID=$!
sleep 5
ps -T -p $PID > threads.txt
grep -c ^ threads.txt
lsof -p $PID > files.txt

cat /proc/$PID/maps > map.txt

sudo strace -p $PID -f -e trace=network -t 2> trace.txt &
STRACE=$!

LINES=$(grep "TCP" files.txt | awk '{print $9}' | awk -F: '{print $2}')

echo "$LINES" | wc -w

COMMAND="tcpdump -i any -A"

for LINE in $LINES
do
	COMMAND="${COMMAND} port ${LINE} or"
done

eval "${COMMAND::-2} > packets.txt &"
TCPDUMP=$!

echo $PID
echo $STRACE
echo $TCPDUMP

sleep 30
kill $PID 
kill $STRACE 
kill $TCPDUMP