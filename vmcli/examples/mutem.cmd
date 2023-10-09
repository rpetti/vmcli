@echo off
:: If Strip 1 is (un)muted, (un)mute strips 2 and 3 too.
CD /D %~dp0
SETLOCAL EnableDelayedExpansion

SET _cmd=vmcli Strip[1].Mute

:: Read the state of _cmd (i.e. Strip[1].Mute)
FOR /F "tokens=3 delims==." %%G IN ('%_cmd%') do (SET MUTED=%%G)
:: echo MUTED is %MUTED%

IF [%MUTED%] EQU [1] goto SETMUTE
IF [%MUTED%] EQU [0] goto SETMUTE
:: Got neither 0 nor 1 when reading state. This is an error, do nothing and just exit
EXIT
:SETMUTE
vmcli Strip[2].Mute=%MUTED% Strip[3].Mute=%MUTED% > NUL:

EXIT
