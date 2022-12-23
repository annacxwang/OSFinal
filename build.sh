#!/bin/bash

gcc run.c -o run
gcc run2.c -o run2
gcc run3.c -o run3
gcc run4.c -o run4
gcc run5.c -o run5
gcc fast.c -o fast
./2.sh
./run part3 -w 364000 2000
./3.sh
./4.sh
./5.sh
