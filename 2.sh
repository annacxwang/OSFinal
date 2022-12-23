#!/bin/bash

for i in {1000..40000..3000}
do 
	echo $i":"
	./run2 temp $i 
	sudo sh -c "/usr/bin/echo 3 > /proc/sys/vm/drop_caches"
done
