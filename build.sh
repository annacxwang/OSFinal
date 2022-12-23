#!/bin/bash
do
gcc run.c -o run
gcc run2.c -o run2
./2.sh
./run part3 -w 364000 2000
gcc run3.c -o run3
./3.sh
gcc run4.c -o run4
./4.sh
gcc run5.c -o run5
./5.sh
done
