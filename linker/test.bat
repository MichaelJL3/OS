@echo off
color a
cls
for /L %%G IN (1,1,9) DO type "%%G".txt | linker.exe > output/out"%%G".txt