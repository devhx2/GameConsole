@echo off
cd src
cl /c /utf-8 *.c
cl /Fe"../out/" /utf-8 *.c
del *.obj
