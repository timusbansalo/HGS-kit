@echo off
for /f "tokens=*" %%f in ('wmic cpu get NumberOfCores /value ^| find "="') do set %%f
for /f "tokens=*" %%f in ('wmic cpu get NumberOfLogicalProcessors /value ^| find "="') do set %%f
@For /f "tokens=2-4 delims=/ " %%a in ('date /t') do (set mydate=%%c-%%a-%%b)
@For /f "tokens=1-3 delims=/: " %%a in ('time /t') do (set mytime=%%a%%b)
@set TAG=%NumberOfCores%C_%NumberOfLogicalProcessors%T_%mydate%_%mytime%
@set OUT=%COMPUTERNAME%_%TAG%

REM emon -M > emon-M_%OUT%.txt
REM emon -v > emon-v_%OUT%.txt
emon -i %1 -f emon_%OUT%.txt