@echo off
color a
cls

if not exist linker.exe g++ -o linker linker/*.cpp linker/module/*.cpp

echo Enter in file name without extension
echo or hit enter to send input by keyboard
set /p Input=:

if "%Input%"=="" (
   echo no file enter manual info
   linker.exe
) else (
   type "%Input%".txt | linker.exe
)

echo ""
pause

