@echo off
color a
cls

if not exist sched.exe g++ -o sched main/*.cpp main/algorithms/scheduler/*.cpp

echo Enter in 1 for VERBOSE anything else will default to non
set /p Ver=:

if "%Ver%"=="1" (
   for /L %%G IN (1,1,7) DO (
	sched.exe --verbose input%%G.txt > output\output%%G.txt 
        type output\output%%G.txt
	pause
   ) 
) else (
   for /L %%G IN (1,1,7) DO (
	sched.exe input%%G.txt > output\output%%G.txt 
        type output\output%%G.txt
	pause
   ) 
)

echo "done"
pause