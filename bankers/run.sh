#!/bin/bash

# sysinfo

if [ -e sched.exe ] 
then 
	echo ""
else
	g++ -o alloc src/*.cpp
fi

echo "Enter in file number example: input1 = 1"
echo -n ">"
read In

./alloc.exe input$In.txt > output/output$In.txt
cat output/output$In.txt

echo -n "Press any key to exit"
read End