#!/bin/bash
do
gcc run.c -o run
gcc run2.c -o run2
./run part3 -w 382000 2000
gcc run3.c -o run3
./3.sh
./4.sh

done
