#!/bin/bash

# sysinfo

if [ -e linker.exe ] 
then 
	echo ""
else
	g++ -o linker linker/*.cpp linker/module/*.cpp
fi

echo "Enter in file name without extension"
echo "or hit enter to send input by keyboard"
echo -n ">"

read Input

if [ -z $Input ]
then
	./linker.exe
else
	cat $Input.txt | ./linker.exe 
fi

echo -n "Press any key to exit"
read End