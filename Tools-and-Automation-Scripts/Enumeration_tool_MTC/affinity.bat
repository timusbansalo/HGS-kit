@echo off
setlocal EnableDelayedExpansion

:Main
setlocal
set /a lp=%1-1
set /a aff=0x1
FOR /L %%A IN (0,1,%lp%) DO (
	call :DectoHex !aff! hexOut
	start "" /b /wait /affinity !hexOut! .\Emulator_Validation1.exe %%A
	set /a aff=!aff!*2
)
goto :eof

:DectoHex
set /a A=%1
set map=0123456789ABCDEF
set H=

:Hexloop
set /a B=!A! %%16 & set /a A /=16 
set H=!map:~%B%,1!!H!
if !A! gtr 0 goto :Hexloop
set %2=!H!

:eof