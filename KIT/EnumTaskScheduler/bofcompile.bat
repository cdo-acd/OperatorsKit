@echo off
setlocal

REM Set this to your Visual Studio install path
set VS_PATH="C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build"

REM Build x86
call %VS_PATH%\vcvarsall.bat x86
cl.exe /nologo /c /Od /MT /W0 /GS- /Tc enumtaskscheduler.c /Fo:enumtaskscheduler_x86.obj
move /y enumtaskscheduler_x86.obj enumtaskscheduler.x86.o

REM Build x64
call %VS_PATH%\vcvarsall.bat x64
cl.exe /nologo /c /Od /MT /W0 /GS- /Tc enumtaskscheduler.c /Fo:enumtaskscheduler_x64.obj
move /y enumtaskscheduler_x64.obj enumtaskscheduler.x64.o