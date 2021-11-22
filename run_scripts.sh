#!/usr/bin/env bash

scripts_dir="scripts"

dir_name="measures"
mkdir -p $dir_name

pid=$1

sudo ./scripts/cpu.sh $pid &
sudo ./scripts/io.sh $pid &
sudo ./scripts/threads.sh $pid &
sudo stap "./scripts/net.stp" -x $pid -o $dir_name/net.csv && sudo gnuplot "./scripts/net.gpi" &

