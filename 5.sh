#!/bin/bash

truncate -s 0 5.csv


for i in {10000..728000000..10000}
do 
	sudo sh -c "/usr/bin/echo 3 > /proc/sys/vm/drop_caches"
	echo $i":"
	./run5 part3 5.csv $i
	
done
