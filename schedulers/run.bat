@echo off
color a
cls

if not exist sched.exe g++ -o sched main/*.cpp main/algorithms/scheduler/*.cpp

echo Enter the number of the file: ex input1 = 1
set /p G=:

echo Enter in 1 for VERBOSE anything else will default to non
set /p Ver=:

if "%Ver%"=="1" (
   sched.exe --verbose input%G%.txt > output\output%G%.txt 
   type output\output%G%.txt
) else (
   sched.exe input%G%.txt > output\output%G%.txt 
   type output\output%G%.txt
)

echo ""
pause