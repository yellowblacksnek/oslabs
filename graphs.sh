#!/usr/bin/env bash

sudo gnuplot ./scripts/cpu.gpi
sudo gnuplot ./scripts/io.gpi
sudo gnuplot ./scripts/net.gpi
sudo gnuplot ./scripts/threads.gpi


