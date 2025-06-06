@echo off
setlocal

REM Set this to your Visual Studio install path
set VS_PATH="C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build"

REM Build x86
call %VS_PATH%\vcvarsall.bat x86
cl.exe /nologo /c /Od /MT /W0 /GS- /Tc capturenetntlm.c /Fo:capturenetntlm_x86.obj
move /y capturenetntlm_x86.obj capturenetntlm.x86.o

REM Build x64
call %VS_PATH%\vcvarsall.bat x64
cl.exe /nologo /c /Od /MT /W0 /GS- /Tc capturenetntlm.c /Fo:capturenetntlm_x64.obj
move /y capturenetntlm_x64.obj capturenetntlm.x64.o