#!/bin/bash

# sysinfo
v="1"

if [ -e sched.exe ] 
then 
	echo ""
else
	g++ -o sched main/*.cpp main/algorithms/scheduler/*.cpp
fi

echo "Enter in file number example: input1 = 1"
echo -n ">"
read In

echo "Enter in 1 for VERBOSE, 0/anything else will display non VERBOSE"
echo -n ">"
read Ver

if ["$Ver" = "$v"]
then
	./sched.exe --verbose input$In.txt > output/output$In.txt
	cat output/output$In.txt
else
	./sched.exe input$In.txt > output/output$In.txt
	cat output/output$In.txt

fi

echo -n "Press any key to exit"
read End