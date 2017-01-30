@echo off
set GDCC_PATH=.\..\gdcc
set GDCC_CC=%GDCC_PATH%\gdcc-cc.exe
set GDCC_LD=%GDCC_PATH%\gdcc-ld.exe
set GDCC_MAKELIB=%GDCC_PATH%\gdcc-makelib.exe

if not exist acs mkdir acs
%GDCC_MAKELIB% --bc-target ZDoom libGDCC -c -o .\acs\libGDCC.o
%GDCC_MAKELIB% --bc-target ZDoom libc    -c -o .\acs\libc.o
for /r %%x in (*.c) do ( %GDCC_CC% --bc-target=ZDoom -c %%x -o .\acs\%%~nx.o -i.\scripts\libzdgui\include)
%GDCC_LD% --bc-target=ZDoom -o .\acs\libzdgui .\acs\*.o
del /S  .\acs\*.o