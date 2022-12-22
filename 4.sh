#!/bin/bash

truncate -s 0 4.csv


for i in {1000..20000..1000}
do 
	sudo sh -c "/usr/bin/echo 3 > /proc/sys/vm/drop_caches"
	echo $i":"
	./run4 part3 4.csv $i $((764000000/$i+1))
	
done


for i in {50000..320000..30000}
do 
	sudo sh -c "/usr/bin/echo 3 > /proc/sys/vm/drop_caches"
	echo $i":"
	./run4 part3 4.csv $i $((764000000/$i+1))
done

for i in {350000..1000000..50000}
do 
	sudo sh -c "/usr/bin/echo 3 > /proc/sys/vm/drop_caches"
	echo $i":"
	./run4 part3 4.csv $i $((764000000/$i+1))
	
done



