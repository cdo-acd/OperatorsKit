@echo off
setlocal

REM Set this to your Visual Studio install path
set VS_PATH="C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build"

REM Build x86
call %VS_PATH%\vcvarsall.bat x86
cl.exe /nologo /c /Od /MT /W0 /GS- /Tc silencesysmon.c /Fo:silencesysmon_x86.obj
move /y silencesysmon_x86.obj silencesysmon.x86.o
dumpbin /disasm silencesysmon.x86.o > silencesysmon.x86.disasm

REM Build x64
call %VS_PATH%\vcvarsall.bat x64
cl.exe /nologo /c /Od /MT /W0 /GS- /Tc silencesysmon.c /Fo:silencesysmon_x64.obj
move /y silencesysmon_x64.obj silencesysmon.x64.o
dumpbin /disasm silencesysmon.x64.o > silencesysmon.x64.disasm