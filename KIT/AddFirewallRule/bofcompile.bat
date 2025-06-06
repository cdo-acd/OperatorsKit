@echo off
setlocal

REM Set this to your Visual Studio install path
set VS_PATH="C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build"

REM Build x86
call %VS_PATH%\vcvarsall.bat x86
cl.exe /nologo /c /Od /MT /W0 /GS- /Tc addfirewallrule.c /Fo:addfirewallrule_x86.obj
move /y addfirewallrule_x86.obj addfirewallrule.x86.o

REM Build x64
call %VS_PATH%\vcvarsall.bat x64
cl.exe /nologo /c /Od /MT /W0 /GS- /Tc addfirewallrule.c /Fo:addfirewallrule_x64.obj
move /y addfirewallrule_x64.obj addfirewallrule.x64.o