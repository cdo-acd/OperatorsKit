@echo off
setlocal

REM Set this to your Visual Studio install path
set VS_PATH="C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build"

REM Build x86
call %VS_PATH%\vcvarsall.bat x86
cl.exe /nologo /c /Od /MT /W0 /GS- /Tc findlib.c /Fo:findlib_x86.obj
move /y findlib_x86.obj findlib.x86.o

REM Build x64
call %VS_PATH%\vcvarsall.bat x64
cl.exe /nologo /c /Od /MT /W0 /GS- /Tc findlib.c /Fo:findlib_x64.obj
move /y findlib_x64.obj findlib.x64.o