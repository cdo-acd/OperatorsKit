@ECHO OFF

cl.exe /nologo /c /Od /MT /W0 /GS- /Tc enumsecproducts.c
move /y enumsecproducts.obj enumsecproducts.o

cl.exe /nologo /c /Od /MT /W0 /GS- /Tc enumsecproducts_new.c
move /y enumsecproducts_new.obj enumsecproducts_new.o
