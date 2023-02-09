@echo off
REM Ensure admin priveleges
net localgroup administrators | find "%USERNAME%"
if not %ERRORLEVEL%==0 (
    echo OS settings install script must be called with administrative privileges
    goto EOF
)

set OVERLAY_BEST_EFF=961cc777-2547-4f9d-8174-7d86181b8a7a
set OVERLAY_BALANCED=00000000-0000-0000-0000-000000000000
set OVERLAY_BEST_PERF=ded574b5-45a0-4f42-8737-46345c09c238

set Medium_Qos=PROFILE_ENTRYLEVELPERF
set Low_Qos=PROFILE_BACKGROUND
set Eco_Qos=PROFILE_ECO
set MM_Qos=PROFILE_MULTIMEDIA
set Utility_Qos=PROFILE_UTILITY
set Sub_Group=SUB_PROCESSOR
set EPP_Small=PERFEPP
set EPP_Big=PERFEPP1
set Fmax_Small=PROCFREQMAX
set Fmax_Big=PROCFREQMAX1
set CP_Concurrency=CPCONCURRENCY
set CP_Headroom=CPHEADROOM
set CP_Increase_Time=CPINCREASETIME
set CP_Decrease_Time=CPDECREASETIME
set CP_Min_Cores_Small=CPMINCORES
set CP_Min_Cores_Big=CPMINCORES1
set Soft_Park_Latency=SOFTPARKLATENCY
set HETERO_GUID=7f2f5cfa-f10c-4823-b5e1-e93ae85f46b5
set Short_Scheduling_Policy=SHORTSCHEDPOLICY
set Scheduling_policy=SCHEDPOLICY
set SMT_Unpark_Policy=SmtUnparkPolicy
set Module_Unpark_Policy=MODULEUNPARKPOLICY

set AC=setacvalueindex
set DC=setdcvalueindex
set AC_PROFILE_INDEX=setacprofileindex
set DC_PROFILE_INDEX=setdcprofileindex


Rem Best Efficiency Power Mode: SMT Unpark Policy
powercfg.exe /%DC% %OVERLAY_BEST_EFF% %Sub_Group% %SMT_Unpark_Policy% 3
Rem Best Efficiency Power Mode: Module Unpark Policy
powercfg.exe /%AC% %OVERLAY_BEST_EFF% %Sub_Group% %Module_Unpark_Policy% 2
powercfg.exe /%DC% %OVERLAY_BEST_EFF% %Sub_Group% %Module_Unpark_Policy% 2
Rem Best Efficiency Power Mode: CpConcurrency
powercfg.exe /%AC% %OVERLAY_BEST_EFF% %Sub_Group% %CP_Concurrency% 80
powercfg.exe /%DC% %OVERLAY_BEST_EFF% %Sub_Group% %CP_Concurrency% 80
Rem Best Efficiency Power Mode: CpHeadroom
powercfg.exe /%AC% %OVERLAY_BEST_EFF% %Sub_Group% %CP_Headroom% 70
powercfg.exe /%DC% %OVERLAY_BEST_EFF% %Sub_Group% %CP_Headroom% 70
Rem Best Efficiency Power Mode: CpIncreaseTime
powercfg.exe /%AC% %OVERLAY_BEST_EFF% %Sub_Group% %CP_Increase_Time% 7
powercfg.exe /%DC% %OVERLAY_BEST_EFF% %Sub_Group% %CP_Increase_Time% 7
Rem Best Efficiency Power Mode: CpDecreaseTime
powercfg.exe /%AC% %OVERLAY_BEST_EFF% %Sub_Group% %CP_Decrease_Time% 1
powercfg.exe /%DC% %OVERLAY_BEST_EFF% %Sub_Group% %CP_Decrease_Time% 1
Rem Best Efficiency Power Mode: Hetero policy
powercfg.exe /%AC% %OVERLAY_BEST_EFF% %Sub_Group% %HETERO_GUID% 4
powercfg.exe /%DC% %OVERLAY_BEST_EFF% %Sub_Group% %HETERO_GUID% 4
Rem Best Efficiency Power Mode: CPMincores
powercfg.exe /%AC% %OVERLAY_BEST_EFF% %Sub_Group% %CP_Min_Cores_Small% 4
powercfg.exe /%DC% %OVERLAY_BEST_EFF% %Sub_Group% %CP_Min_Cores_Small% 4
powercfg.exe /%AC% %OVERLAY_BEST_EFF% %Sub_Group% %CP_Min_Cores_Big% 4
powercfg.exe /%DC% %OVERLAY_BEST_EFF% %Sub_Group% %CP_Min_Cores_Big% 4
Rem Best Efficiency Power Mode: Softparklatency
powercfg.exe /%AC% %OVERLAY_BEST_EFF% %Sub_Group% %Soft_Park_Latency% 0
powercfg.exe /%DC% %OVERLAY_BEST_EFF% %Sub_Group% %Soft_Park_Latency% 0
Rem Best Efficiency Power Mode: High Qos EPP
powercfg.exe /%AC% %OVERLAY_BEST_EFF% %Sub_Group% %EPP_Small% 60
powercfg.exe /%AC% %OVERLAY_BEST_EFF% %Sub_Group% %EPP_Big% 60
powercfg.exe /%DC% %OVERLAY_BEST_EFF% %Sub_Group% %EPP_Small% 72
powercfg.exe /%DC% %OVERLAY_BEST_EFF% %Sub_Group% %EPP_Big% 72
Rem Best Efficiency Power Mode: Medium Qos EPP & FMax
powercfg.exe /%DC_PROFILE_INDEX% %OVERLAY_BEST_EFF% %Medium_Qos% %EPP_Small% 77
powercfg.exe /%DC_PROFILE_INDEX% %OVERLAY_BEST_EFF% %Medium_Qos% %EPP_Big% 77
powercfg.exe /%DC_PROFILE_INDEX% %OVERLAY_BEST_EFF% %Medium_Qos% %Fmax_Small% 1500
powercfg.exe /%DC_PROFILE_INDEX% %OVERLAY_BEST_EFF% %Medium_Qos% %Fmax_Big% 1500
Rem Best Efficiency Power Mode: Low Qos EPP & FMax & Scheduling Policy
powercfg.exe /%AC_PROFILE_INDEX% %OVERLAY_BEST_EFF% %Low_Qos% %EPP_Small% 70
powercfg.exe /%AC_PROFILE_INDEX% %OVERLAY_BEST_EFF% %Low_Qos% %EPP_Big% 70
powercfg.exe /%DC_PROFILE_INDEX% %OVERLAY_BEST_EFF% %Low_Qos% %EPP_Small% 77
powercfg.exe /%DC_PROFILE_INDEX% %OVERLAY_BEST_EFF% %Low_Qos% %EPP_Big% 77
powercfg.exe /%DC_PROFILE_INDEX% %OVERLAY_BEST_EFF% %Low_Qos% %Fmax_Small% 1500
powercfg.exe /%DC_PROFILE_INDEX% %OVERLAY_BEST_EFF% %Low_Qos% %Fmax_Big% 1500
powercfg.exe /%AC_PROFILE_INDEX% %OVERLAY_BEST_EFF% %Low_Qos% %Short_Scheduling_Policy% 3
powercfg.exe /%DC_PROFILE_INDEX% %OVERLAY_BEST_EFF% %Low_Qos% %Short_Scheduling_Policy% 3
powercfg.exe /%AC_PROFILE_INDEX% %OVERLAY_BEST_EFF% %Low_Qos% %Scheduling_policy% 3
powercfg.exe /%DC_PROFILE_INDEX% %OVERLAY_BEST_EFF% %Low_Qos% %Scheduling_policy% 3
Rem Best Efficiency Power Mode: Eco Qos EPP & FMax & Scheduling Policy
powercfg.exe /%AC_PROFILE_INDEX% %OVERLAY_BEST_EFF% %Eco_Qos% %EPP_Small% 70
powercfg.exe /%AC_PROFILE_INDEX% %OVERLAY_BEST_EFF% %Eco_Qos% %EPP_Big% 70
powercfg.exe /%DC_PROFILE_INDEX% %OVERLAY_BEST_EFF% %Eco_Qos% %EPP_Small% 77
powercfg.exe /%DC_PROFILE_INDEX% %OVERLAY_BEST_EFF% %Eco_Qos% %EPP_Big% 77
powercfg.exe /%DC_PROFILE_INDEX% %OVERLAY_BEST_EFF% %Eco_Qos% %Fmax_Small% 1500
powercfg.exe /%DC_PROFILE_INDEX% %OVERLAY_BEST_EFF% %Eco_Qos% %Fmax_Big% 1500
powercfg.exe /%AC_PROFILE_INDEX% %OVERLAY_BEST_EFF% %Eco_Qos% %Fmax_Small% 1500
powercfg.exe /%AC_PROFILE_INDEX% %OVERLAY_BEST_EFF% %Eco_Qos% %Fmax_Big% 1500
powercfg.exe /%AC_PROFILE_INDEX% %OVERLAY_BEST_EFF% %Eco_Qos% %Short_Scheduling_Policy% 3
powercfg.exe /%DC_PROFILE_INDEX% %OVERLAY_BEST_EFF% %Eco_Qos% %Short_Scheduling_Policy% 3
powercfg.exe /%AC_PROFILE_INDEX% %OVERLAY_BEST_EFF% %Eco_Qos% %Scheduling_policy% 3
powercfg.exe /%DC_PROFILE_INDEX% %OVERLAY_BEST_EFF% %Eco_Qos% %Scheduling_policy% 3
Rem Best Efficiency Power Mode: Multimedia Qos EPP & FMax & Scheduling Policy
powercfg.exe /%AC_PROFILE_INDEX% %OVERLAY_BEST_EFF% %MM_Qos% %EPP_Small% 100
powercfg.exe /%AC_PROFILE_INDEX% %OVERLAY_BEST_EFF% %MM_Qos% %EPP_Big% 100
powercfg.exe /%DC_PROFILE_INDEX% %OVERLAY_BEST_EFF% %MM_Qos% %EPP_Small% 100
powercfg.exe /%DC_PROFILE_INDEX% %OVERLAY_BEST_EFF% %MM_Qos% %EPP_Big% 100
powercfg.exe /%DC_PROFILE_INDEX% %OVERLAY_BEST_EFF% %MM_Qos% %Fmax_Small% 1500
powercfg.exe /%DC_PROFILE_INDEX% %OVERLAY_BEST_EFF% %MM_Qos% %Fmax_Big% 1500
powercfg.exe /%AC_PROFILE_INDEX% %OVERLAY_BEST_EFF% %MM_Qos% %Fmax_Small% 1500
powercfg.exe /%AC_PROFILE_INDEX% %OVERLAY_BEST_EFF% %MM_Qos% %Fmax_Big% 1500
powercfg.exe /%AC_PROFILE_INDEX% %OVERLAY_BEST_EFF% %MM_Qos% %Short_Scheduling_Policy% 3
powercfg.exe /%DC_PROFILE_INDEX% %OVERLAY_BEST_EFF% %MM_Qos% %Short_Scheduling_Policy% 3
powercfg.exe /%AC_PROFILE_INDEX% %OVERLAY_BEST_EFF% %MM_Qos% %Scheduling_policy% 3
powercfg.exe /%DC_PROFILE_INDEX% %OVERLAY_BEST_EFF% %MM_Qos% %Scheduling_policy% 3
Rem Best Efficiency Power Mode: Utility Qos EPP & FMax & Scheduling Policy
powercfg.exe /%AC_PROFILE_INDEX% %OVERLAY_BEST_EFF% %Utility_Qos% %EPP_Small% 70
powercfg.exe /%AC_PROFILE_INDEX% %OVERLAY_BEST_EFF% %Utility_Qos% %EPP_Big% 70
powercfg.exe /%DC_PROFILE_INDEX% %OVERLAY_BEST_EFF% %Utility_Qos% %EPP_Small% 77
powercfg.exe /%DC_PROFILE_INDEX% %OVERLAY_BEST_EFF% %Utility_Qos% %EPP_Big% 77
powercfg.exe /%DC_PROFILE_INDEX% %OVERLAY_BEST_EFF% %Utility_Qos% %Fmax_Small% 1500
powercfg.exe /%DC_PROFILE_INDEX% %OVERLAY_BEST_EFF% %Utility_Qos% %Fmax_Big% 1500
powercfg.exe /%AC_PROFILE_INDEX% %OVERLAY_BEST_EFF% %Utility_Qos% %Fmax_Small% 1500
powercfg.exe /%AC_PROFILE_INDEX% %OVERLAY_BEST_EFF% %Utility_Qos% %Fmax_Big% 1500
powercfg.exe /%AC_PROFILE_INDEX% %OVERLAY_BEST_EFF% %Utility_Qos% %Short_Scheduling_Policy% 3
powercfg.exe /%DC_PROFILE_INDEX% %OVERLAY_BEST_EFF% %Utility_Qos% %Short_Scheduling_Policy% 3
powercfg.exe /%AC_PROFILE_INDEX% %OVERLAY_BEST_EFF% %Utility_Qos% %Scheduling_policy% 3
powercfg.exe /%DC_PROFILE_INDEX% %OVERLAY_BEST_EFF% %Utility_Qos% %Scheduling_policy% 3



Rem Balanced Power Mode: SMT Unpark Policy
powercfg.exe /%DC% SCHEME_CURRENT %Sub_Group% %SMT_Unpark_Policy% 3
Rem Balanced Power Mode: Module Unpark Policy
powercfg.exe /%AC% SCHEME_CURRENT %Sub_Group% %Module_Unpark_Policy% 2
powercfg.exe /%DC% SCHEME_CURRENT %Sub_Group% %Module_Unpark_Policy% 2
Rem Balanced Power Mode: CpConcurrency
powercfg.exe /%AC% SCHEME_CURRENT %Sub_Group% %CP_Concurrency% 90
powercfg.exe /%DC% SCHEME_CURRENT %Sub_Group% %CP_Concurrency% 90
Rem Balanced Power Mode: CpHeadroom
powercfg.exe /%AC% SCHEME_CURRENT %Sub_Group% %CP_Headroom% 60
powercfg.exe /%DC% SCHEME_CURRENT %Sub_Group% %CP_Headroom% 60
Rem Balanced Power Mode: CpIncreaseTime
powercfg.exe /%AC% SCHEME_CURRENT %Sub_Group% %CP_Increase_Time% 4
powercfg.exe /%DC% SCHEME_CURRENT %Sub_Group% %CP_Increase_Time% 4
Rem Balanced Power Mode: CpDecreaseTime
powercfg.exe /%AC% SCHEME_CURRENT %Sub_Group% %CP_Decrease_Time% 1
powercfg.exe /%DC% SCHEME_CURRENT %Sub_Group% %CP_Decrease_Time% 1
Rem Balanced Power Mode: Hetero policy
powercfg.exe /%AC% SCHEME_CURRENT %Sub_Group% %HETERO_GUID% 4
powercfg.exe /%DC% SCHEME_CURRENT %Sub_Group% %HETERO_GUID% 4
Rem Balanced Power Mode: CPMincores
powercfg.exe /%AC% SCHEME_CURRENT %Sub_Group% %CP_Min_Cores_Small% 4
powercfg.exe /%DC% SCHEME_CURRENT %Sub_Group% %CP_Min_Cores_Small% 4
powercfg.exe /%AC% SCHEME_CURRENT %Sub_Group% %CP_Min_Cores_Big% 4
powercfg.exe /%DC% SCHEME_CURRENT %Sub_Group% %CP_Min_Cores_Big% 4
Rem Balanced Power Mode: Softparklatency
powercfg.exe /%AC% SCHEME_CURRENT %Sub_Group% %Soft_Park_Latency% 0
powercfg.exe /%DC% SCHEME_CURRENT %Sub_Group% %Soft_Park_Latency% 0
Rem Balanced Power Mode: High Qos EPP
powercfg.exe /%AC% SCHEME_CURRENT %Sub_Group% %EPP_Small% 45
powercfg.exe /%AC% SCHEME_CURRENT %Sub_Group% %EPP_Big% 45
powercfg.exe /%DC% SCHEME_CURRENT %Sub_Group% %EPP_Small% 60
powercfg.exe /%DC% SCHEME_CURRENT %Sub_Group% %EPP_Big% 60
Rem Balanced Power Mode: Medium Qos EPP & FMax
powercfg.exe /%DC_PROFILE_INDEX% SCHEME_CURRENT %Medium_Qos% %EPP_Small% 77
powercfg.exe /%DC_PROFILE_INDEX% SCHEME_CURRENT %Medium_Qos% %EPP_Big% 77
Rem Balanced Power Mode: Low Qos EPP & FMax & Scheduling Policy
powercfg.exe /%AC_PROFILE_INDEX% SCHEME_CURRENT %Low_Qos% %EPP_Small% 70
powercfg.exe /%AC_PROFILE_INDEX% SCHEME_CURRENT %Low_Qos% %EPP_Big% 70
powercfg.exe /%DC_PROFILE_INDEX% SCHEME_CURRENT %Low_Qos% %EPP_Small% 77
powercfg.exe /%DC_PROFILE_INDEX% SCHEME_CURRENT %Low_Qos% %EPP_Big% 77
powercfg.exe /%DC_PROFILE_INDEX% SCHEME_CURRENT %Low_Qos% %Fmax_Small% 1500
powercfg.exe /%DC_PROFILE_INDEX% SCHEME_CURRENT %Low_Qos% %Fmax_Big% 1500
powercfg.exe /%AC_PROFILE_INDEX% SCHEME_CURRENT %Low_Qos% %Short_Scheduling_Policy% 3
powercfg.exe /%DC_PROFILE_INDEX% SCHEME_CURRENT %Low_Qos% %Short_Scheduling_Policy% 3
powercfg.exe /%AC_PROFILE_INDEX% SCHEME_CURRENT %Low_Qos% %Scheduling_policy% 3
powercfg.exe /%DC_PROFILE_INDEX% SCHEME_CURRENT %Low_Qos% %Scheduling_policy% 3
Rem Balanced Power Mode: Eco Qos EPP & FMax & Scheduling Policy
powercfg.exe /%AC_PROFILE_INDEX% SCHEME_CURRENT %Eco_Qos% %EPP_Small% 70
powercfg.exe /%AC_PROFILE_INDEX% SCHEME_CURRENT %Eco_Qos% %EPP_Big% 70
powercfg.exe /%DC_PROFILE_INDEX% SCHEME_CURRENT %Eco_Qos% %EPP_Small% 77
powercfg.exe /%DC_PROFILE_INDEX% SCHEME_CURRENT %Eco_Qos% %EPP_Big% 77
powercfg.exe /%DC_PROFILE_INDEX% SCHEME_CURRENT %Eco_Qos% %Fmax_Small% 1500
powercfg.exe /%DC_PROFILE_INDEX% SCHEME_CURRENT %Eco_Qos% %Fmax_Big% 1500
powercfg.exe /%AC_PROFILE_INDEX% SCHEME_CURRENT %Eco_Qos% %Fmax_Small% 1500
powercfg.exe /%AC_PROFILE_INDEX% SCHEME_CURRENT %Eco_Qos% %Fmax_Big% 1500
powercfg.exe /%AC_PROFILE_INDEX% SCHEME_CURRENT %Eco_Qos% %Short_Scheduling_Policy% 3
powercfg.exe /%DC_PROFILE_INDEX% SCHEME_CURRENT %Eco_Qos% %Short_Scheduling_Policy% 3
powercfg.exe /%AC_PROFILE_INDEX% SCHEME_CURRENT %Eco_Qos% %Scheduling_policy% 3
powercfg.exe /%DC_PROFILE_INDEX% SCHEME_CURRENT %Eco_Qos% %Scheduling_policy% 3
Rem Balanced Power Mode: Multimedia Qos EPP & FMax & Scheduling Policy
powercfg.exe /%AC_PROFILE_INDEX% SCHEME_CURRENT %MM_Qos% %EPP_Small% 100
powercfg.exe /%AC_PROFILE_INDEX% SCHEME_CURRENT %MM_Qos% %EPP_Big% 100
powercfg.exe /%DC_PROFILE_INDEX% SCHEME_CURRENT %MM_Qos% %EPP_Small% 100
powercfg.exe /%DC_PROFILE_INDEX% SCHEME_CURRENT %MM_Qos% %EPP_Big% 100
powercfg.exe /%DC_PROFILE_INDEX% SCHEME_CURRENT %MM_Qos% %Fmax_Small% 1500
powercfg.exe /%DC_PROFILE_INDEX% SCHEME_CURRENT %MM_Qos% %Fmax_Big% 1500
powercfg.exe /%AC_PROFILE_INDEX% SCHEME_CURRENT %MM_Qos% %Fmax_Small% 1500
powercfg.exe /%AC_PROFILE_INDEX% SCHEME_CURRENT %MM_Qos% %Fmax_Big% 1500
powercfg.exe /%AC_PROFILE_INDEX% SCHEME_CURRENT %MM_Qos% %Short_Scheduling_Policy% 3
powercfg.exe /%DC_PROFILE_INDEX% SCHEME_CURRENT %MM_Qos% %Short_Scheduling_Policy% 3
powercfg.exe /%AC_PROFILE_INDEX% SCHEME_CURRENT %MM_Qos% %Scheduling_policy% 3
powercfg.exe /%DC_PROFILE_INDEX% SCHEME_CURRENT %MM_Qos% %Scheduling_policy% 3
Rem Balanced Power Mode: Utility Qos EPP & FMax & Scheduling Policy
powercfg.exe /%AC_PROFILE_INDEX% SCHEME_CURRENT %Utility_Qos% %EPP_Small% 70
powercfg.exe /%AC_PROFILE_INDEX% SCHEME_CURRENT %Utility_Qos% %EPP_Big% 70
powercfg.exe /%DC_PROFILE_INDEX% SCHEME_CURRENT %Utility_Qos% %EPP_Small% 77
powercfg.exe /%DC_PROFILE_INDEX% SCHEME_CURRENT %Utility_Qos% %EPP_Big% 77
powercfg.exe /%DC_PROFILE_INDEX% SCHEME_CURRENT %Utility_Qos% %Fmax_Small% 1500
powercfg.exe /%DC_PROFILE_INDEX% SCHEME_CURRENT %Utility_Qos% %Fmax_Big% 1500
powercfg.exe /%AC_PROFILE_INDEX% SCHEME_CURRENT %Utility_Qos% %Fmax_Small% 1500
powercfg.exe /%AC_PROFILE_INDEX% SCHEME_CURRENT %Utility_Qos% %Fmax_Big% 1500
powercfg.exe /%AC_PROFILE_INDEX% SCHEME_CURRENT %Utility_Qos% %Short_Scheduling_Policy% 3
powercfg.exe /%DC_PROFILE_INDEX% SCHEME_CURRENT %Utility_Qos% %Short_Scheduling_Policy% 3
powercfg.exe /%AC_PROFILE_INDEX% SCHEME_CURRENT %Utility_Qos% %Scheduling_policy% 3
powercfg.exe /%DC_PROFILE_INDEX% SCHEME_CURRENT %Utility_Qos% %Scheduling_policy% 3
Rem Balanced Power Mode: HeteroIncreaseThreshold & HeteroDecreaseThreshold
reg add "HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Power\PowerSettings\54533251-82be-4824-96c1-47b60b740d00\b000397d-9b0b-483d-98c9-692a6060cfbf\254" /v SettingValue /t REG_DWORD /d 0x64646464 /f
reg add "HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Power\PowerSettings\54533251-82be-4824-96c1-47b60b740d00\f8861c27-95e7-475c-865b-13c0cb3f9d6b\254" /v SettingValue /t REG_DWORD /d 0x32323232 /f



Rem Max Performance Power Mode: SMT Unpark Policy
powercfg.exe /%DC% %OVERLAY_BEST_PERF% %Sub_Group% %SMT_Unpark_Policy% 3
Rem Max Performance Power Mode: Module Unpark Policy
powercfg.exe /%AC% %OVERLAY_BEST_PERF% %Sub_Group% %Module_Unpark_Policy% 2
powercfg.exe /%DC% %OVERLAY_BEST_PERF% %Sub_Group% %Module_Unpark_Policy% 2
Rem Max Performance Power Mode: CpConcurrency
powercfg.exe /%AC% %OVERLAY_BEST_PERF% %Sub_Group% %CP_Concurrency% 95
powercfg.exe /%DC% %OVERLAY_BEST_PERF% %Sub_Group% %CP_Concurrency% 95
Rem Max Performance Power Mode: CpHeadroom
powercfg.exe /%AC% %OVERLAY_BEST_PERF% %Sub_Group% %CP_Headroom% 50
powercfg.exe /%DC% %OVERLAY_BEST_PERF% %Sub_Group% %CP_Headroom% 50
Rem Max Performance Power Mode: CpIncreaseTime
powercfg.exe /%AC% %OVERLAY_BEST_PERF% %Sub_Group% %CP_Increase_Time% 1
powercfg.exe /%DC% %OVERLAY_BEST_PERF% %Sub_Group% %CP_Increase_Time% 1
Rem Max Performance Power Mode: CpDecreaseTime
powercfg.exe /%AC% %OVERLAY_BEST_PERF% %Sub_Group% %CP_Decrease_Time% 3
powercfg.exe /%DC% %OVERLAY_BEST_PERF% %Sub_Group% %CP_Decrease_Time% 3
Rem Max Performance Power Mode: Hetero policy
powercfg.exe /%AC% %OVERLAY_BEST_PERF% %Sub_Group% %HETERO_GUID% 4
powercfg.exe /%DC% %OVERLAY_BEST_PERF% %Sub_Group% %HETERO_GUID% 4
Rem Max Performance Power Mode: CPMincores
powercfg.exe /%AC% %OVERLAY_BEST_PERF% %Sub_Group% %CP_Min_Cores_Small% 4
powercfg.exe /%DC% %OVERLAY_BEST_PERF% %Sub_Group% %CP_Min_Cores_Small% 4
powercfg.exe /%AC% %OVERLAY_BEST_PERF% %Sub_Group% %CP_Min_Cores_Big% 4
powercfg.exe /%DC% %OVERLAY_BEST_PERF% %Sub_Group% %CP_Min_Cores_Big% 4
Rem Max Performance Power Mode: Softparklatency
powercfg.exe /%AC% %OVERLAY_BEST_PERF% %Sub_Group% %Soft_Park_Latency% 0
powercfg.exe /%DC% %OVERLAY_BEST_PERF% %Sub_Group% %Soft_Park_Latency% 0
Rem Max Performance Power Mode: High Qos EPP
powercfg.exe /%AC% %OVERLAY_BEST_PERF% %Sub_Group% %EPP_Small% 25
powercfg.exe /%AC% %OVERLAY_BEST_PERF% %Sub_Group% %EPP_Big% 25
powercfg.exe /%DC% %OVERLAY_BEST_PERF% %Sub_Group% %EPP_Small% 33
powercfg.exe /%DC% %OVERLAY_BEST_PERF% %Sub_Group% %EPP_Big% 33
Rem Max Performance Power Mode: Medium Qos EPP & FMax
powercfg.exe /%DC_PROFILE_INDEX% %OVERLAY_BEST_PERF% %Medium_Qos% %EPP_Small% 50
powercfg.exe /%DC_PROFILE_INDEX% %OVERLAY_BEST_PERF% %Medium_Qos% %EPP_Big% 50
Rem Max Performance Power Mode: Low Qos EPP & FMax & Scheduling Policy
powercfg.exe /%AC_PROFILE_INDEX% %OVERLAY_BEST_PERF% %Low_Qos% %EPP_Small% 50
powercfg.exe /%AC_PROFILE_INDEX% %OVERLAY_BEST_PERF% %Low_Qos% %EPP_Big% 50
powercfg.exe /%DC_PROFILE_INDEX% %OVERLAY_BEST_PERF% %Low_Qos% %EPP_Small% 50
powercfg.exe /%DC_PROFILE_INDEX% %OVERLAY_BEST_PERF% %Low_Qos% %EPP_Big% 50
powercfg.exe /%DC_PROFILE_INDEX% %OVERLAY_BEST_PERF% %Low_Qos% %Fmax_Small% 0
powercfg.exe /%DC_PROFILE_INDEX% %OVERLAY_BEST_PERF% %Low_Qos% %Fmax_Big% 0
powercfg.exe /%AC_PROFILE_INDEX% %OVERLAY_BEST_PERF% %Low_Qos% %Short_Scheduling_Policy% 4
powercfg.exe /%DC_PROFILE_INDEX% %OVERLAY_BEST_PERF% %Low_Qos% %Short_Scheduling_Policy% 4
powercfg.exe /%AC_PROFILE_INDEX% %OVERLAY_BEST_PERF% %Low_Qos% %Scheduling_policy% 4
powercfg.exe /%DC_PROFILE_INDEX% %OVERLAY_BEST_PERF% %Low_Qos% %Scheduling_policy% 4
Rem Max Performance Power Mode: Eco Qos EPP & FMax & Scheduling Policy
powercfg.exe /%AC_PROFILE_INDEX% %OVERLAY_BEST_PERF% %Eco_Qos% %EPP_Small% 50
powercfg.exe /%AC_PROFILE_INDEX% %OVERLAY_BEST_PERF% %Eco_Qos% %EPP_Big% 50
powercfg.exe /%DC_PROFILE_INDEX% %OVERLAY_BEST_PERF% %Eco_Qos% %EPP_Small% 50
powercfg.exe /%DC_PROFILE_INDEX% %OVERLAY_BEST_PERF% %Eco_Qos% %EPP_Big% 50
powercfg.exe /%DC_PROFILE_INDEX% %OVERLAY_BEST_PERF% %Eco_Qos% %Fmax_Small% 1500
powercfg.exe /%DC_PROFILE_INDEX% %OVERLAY_BEST_PERF% %Eco_Qos% %Fmax_Big% 1500
powercfg.exe /%AC_PROFILE_INDEX% %OVERLAY_BEST_PERF% %Eco_Qos% %Fmax_Small% 1500
powercfg.exe /%AC_PROFILE_INDEX% %OVERLAY_BEST_PERF% %Eco_Qos% %Fmax_Big% 1500
powercfg.exe /%AC_PROFILE_INDEX% %OVERLAY_BEST_PERF% %Eco_Qos% %Short_Scheduling_Policy% 4
powercfg.exe /%DC_PROFILE_INDEX% %OVERLAY_BEST_PERF% %Eco_Qos% %Short_Scheduling_Policy% 4
powercfg.exe /%AC_PROFILE_INDEX% %OVERLAY_BEST_PERF% %Eco_Qos% %Scheduling_policy% 4
powercfg.exe /%DC_PROFILE_INDEX% %OVERLAY_BEST_PERF% %Eco_Qos% %Scheduling_policy% 4
Rem Max Performance Power Mode: Multimedia Qos EPP & FMax & Scheduling Policy
powercfg.exe /%AC_PROFILE_INDEX% %OVERLAY_BEST_PERF% %MM_Qos% %EPP_Small% 100
powercfg.exe /%AC_PROFILE_INDEX% %OVERLAY_BEST_PERF% %MM_Qos% %EPP_Big% 100
powercfg.exe /%DC_PROFILE_INDEX% %OVERLAY_BEST_PERF% %MM_Qos% %EPP_Small% 100
powercfg.exe /%DC_PROFILE_INDEX% %OVERLAY_BEST_PERF% %MM_Qos% %EPP_Big% 100
powercfg.exe /%DC_PROFILE_INDEX% %OVERLAY_BEST_PERF% %MM_Qos% %Fmax_Small% 1500
powercfg.exe /%DC_PROFILE_INDEX% %OVERLAY_BEST_PERF% %MM_Qos% %Fmax_Big% 1500
powercfg.exe /%AC_PROFILE_INDEX% %OVERLAY_BEST_PERF% %MM_Qos% %Fmax_Small% 1500
powercfg.exe /%AC_PROFILE_INDEX% %OVERLAY_BEST_PERF% %MM_Qos% %Fmax_Big% 1500
powercfg.exe /%AC_PROFILE_INDEX% %OVERLAY_BEST_PERF% %MM_Qos% %Short_Scheduling_Policy% 4
powercfg.exe /%DC_PROFILE_INDEX% %OVERLAY_BEST_PERF% %MM_Qos% %Short_Scheduling_Policy% 4
powercfg.exe /%AC_PROFILE_INDEX% %OVERLAY_BEST_PERF% %MM_Qos% %Scheduling_policy% 4
powercfg.exe /%DC_PROFILE_INDEX% %OVERLAY_BEST_PERF% %MM_Qos% %Scheduling_policy% 4
Rem Max Performance Power Mode: Utility Qos EPP & FMax
powercfg.exe /%AC_PROFILE_INDEX% %OVERLAY_BEST_PERF% %Utility_Qos% %EPP_Small% 100
powercfg.exe /%AC_PROFILE_INDEX% %OVERLAY_BEST_PERF% %Utility_Qos% %EPP_Big% 100
powercfg.exe /%DC_PROFILE_INDEX% %OVERLAY_BEST_PERF% %Utility_Qos% %EPP_Small% 100
powercfg.exe /%DC_PROFILE_INDEX% %OVERLAY_BEST_PERF% %Utility_Qos% %EPP_Big% 100
powercfg.exe /%DC_PROFILE_INDEX% %OVERLAY_BEST_PERF% %Utility_Qos% %Fmax_Small% 1500
powercfg.exe /%DC_PROFILE_INDEX% %OVERLAY_BEST_PERF% %Utility_Qos% %Fmax_Big% 1500
powercfg.exe /%AC_PROFILE_INDEX% %OVERLAY_BEST_PERF% %Utility_Qos% %Fmax_Small% 1500
powercfg.exe /%AC_PROFILE_INDEX% %OVERLAY_BEST_PERF% %Utility_Qos% %Fmax_Big% 1500
powercfg.exe /%AC_PROFILE_INDEX% %OVERLAY_BEST_PERF% %Utility_Qos% %Short_Scheduling_Policy% 3
powercfg.exe /%DC_PROFILE_INDEX% %OVERLAY_BEST_PERF% %Utility_Qos% %Short_Scheduling_Policy% 3
powercfg.exe /%AC_PROFILE_INDEX% %OVERLAY_BEST_PERF% %Utility_Qos% %Scheduling_policy% 3
powercfg.exe /%DC_PROFILE_INDEX% %OVERLAY_BEST_PERF% %Utility_Qos% %Scheduling_policy% 3


powercfg.exe /setactive SCHEME_CURRENT
powercfg.exe /OverlaySetActive %OVERLAY_BALANCED%

echo OS settings changes completed

