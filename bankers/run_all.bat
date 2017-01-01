@echo off
color a
cls
for /L %%G IN (1,1,13) DO (
	alloc.exe input%%G.txt > output\output%%G.txt
	type output\output%%G.txt
)
echo done
pause