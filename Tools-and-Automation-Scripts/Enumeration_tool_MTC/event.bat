@echo off
setlocal enabledelayedexpansion

wpaexporter.exe -i power.etl -profile power.wpaProfile > null 2>&1
move "Generic_Events_Activity_by_Provider,_Task,_Opcode.csv" "power.csv"

echo HeteroSystemType >> Enumeration_Output.csv
for /f "tokens=11 delims=," %%a in ('findstr /i "HeterogenousPoliciesRundown" "power.csv"') do (
	echo %%a >> Enumeration_Output.csv
	set /a n = !n!+1
)

echo  DomainId,MemberCount,MemberEnumerated >> Enumeration_Output.csv
for /f "tokens=9,10,11 delims=," %%a in ('findstr /i "PTStateDomainFirmwareRundown" "power.csv"') do (
	echo %%a,%%b,%%c >> Enumeration_Output.csv
	set /a n = !n!+1
)

echo  ProcId,ArchClass,PerfClass,EffClass >> Enumeration_Output.csv
for /f "tokens=10,20,21,27 delims=," %%a in ('findstr /i "ProcessorPerformanceRundown" "power.csv"') do (
	echo   %%a,%%b,%%c,%%d >> Enumeration_Output.csv
	set /a n = !n!+1
)

echo  ProfileId,Setting,Type,Class,Value >> Enumeration_Output.csv
for /f "tokens=9,10,11,12,15 delims=," %%a in ('findstr /i "ProfileSettingRundown" "power.csv"') do (
	echo   %%a,%%b,%%c,%%d, %%e >> Enumeration_Output.csv
	set /a n = !n!+1
)
								
del power.csv
del null
del power.etl.*