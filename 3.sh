#!/bin/bash

truncate -s 0 3.csv

for i in {1000..20000..1000}
do 
	echo $i":"
	./run3 part3 3.csv $i $((764000000/$i+1))
	sudo sh -c "/usr/bin/echo 3 > /proc/sys/vm/drop_caches"
done


for i in {50000..320000..30000}
do 
	echo $i":"
	./run3 part3 3.csv $i $((764000000/$i+1))
	sudo sh -c "/usr/bin/echo 3 > /proc/sys/vm/drop_caches"
done

for i in {350000..1000000..50000}
do 
	echo $i":"
	./run3 part3 3.csv $i $((764000000/$i+1))
	sudo sh -c "/usr/bin/echo 3 > /proc/sys/vm/drop_caches"
done

