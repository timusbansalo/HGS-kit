Installation:
	1. Copy files to any local folder 
	2. From the currentfolder\bin
		1. Install bin\WosHwctrs.exe 
		2. Copy bin\WosMSRDll.dll, bin\WosMSRDll64.dll to C:\Program Files (x86)\WosHwCtrs\
	3. Run below on admin cmd 
		1. bcdedit /set testsigning on
		2. bcdedit /set nointegritychecks on
	4. Restart system after above step

Architecture Enumeration:
	1. Run Emulator_Validation.exe -e (from local folder)
	2. Enumeration_output.csv and power.etl are output files

Scenarios:
	1. For Scenarios, xperf is pre-requisite
		1. Download any one of the following
			a. SDK from https://developer.microsoft.com/en-us/windows/downloads/windows-10-sdk
			b. ADK from https://docs.microsoft.com/en-us/windows-hardware/get-started/adk-install
		2. Check "Windows Performance Toolkit", uncheck everything else in the installation option and install it
		3. Restart all the CMD if open
	2. Run Emulator_Validation.exe -e -s
	3. etl/csv files are available in output folder


# For any queries, contact MTC owner
i)  monica.gupta@intel.com
ii) meghana.gudaram@intel.com 