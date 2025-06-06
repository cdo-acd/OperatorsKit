@echo off
setlocal

REM Set this to your Visual Studio install path
set VS_PATH="C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build"

REM Build x86
call %VS_PATH%\vcvarsall.bat x86
cl.exe /nologo /c /Od /MT /W0 /GS- /Tc loadlib.c /Fo:loadlib_x86.obj
move /y loadlib_x86.obj loadlib.x86.o
dumpbin /disasm loadlib.x86.o > loadlib.x86.disasm

REM Build x64
call %VS_PATH%\vcvarsall.bat x64
cl.exe /nologo /c /Od /MT /W0 /GS- /Tc loadlib.c /Fo:loadlib_x64.obj
move /y loadlib_x64.obj loadlib.x64.o
dumpbin /disasm loadlib.x64.o > loadlib.x64.disasm