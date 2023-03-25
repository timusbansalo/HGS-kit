@echo off
for /f "tokens=*" %%f in ('wmic cpu get NumberOfCores /value ^| find "="') do set %%f
for /f "tokens=*" %%f in ('wmic cpu get NumberOfLogicalProcessors /value ^| find "="') do set %%f
@For /f "tokens=2-4 delims=/ " %%a in ('date /t') do (set mydate=%%c%%a%%b)
@For /f "tokens=1-3 delims=/: " %%a in ('time /t') do (set mytime=%%a%%b)
@set TAG=%NumberOfCores%C%NumberOfLogicalProcessors%T_%mydate%_%mytime%
@set OUT=%~n1_%COMPUTERNAME%_%TAG%

start C:\"Program Files (x86)"\FreakOut\FreakOut_console.exe /log=freakOut_%OUT%.csv /period=250 /silent
pause
taskkill /F /IM FreakOut_console.exe