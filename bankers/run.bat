@echo off
color a
cls

if not exist alloc.exe g++ -o alloc src/*.cpp 

echo Enter the number of the file: ex input1 = 1
set /p G=:

alloc.exe input%G%.txt > output\output%G%.txt 
type output\output%G%.txt

echo ""
pause