Collect emon data on MTL:
1. Copy run_emon.cmd & meteorlake_events_private.txt to a folder on the system. (c:\tools\emon)
2. run this command from admin cmd as close as to the workload run, so we don't collect events during idle time -> run_emon.cmd meteorlake_events_private.txt
3. Ctrl+C to stop the collection and run "emon -stop"

Post Process the emon data:
pre requisites: follow readme.md at C:\Program Files (x86)\IntelSWTools\sep\config\edp\pyedp and complete the setup
copy _MTL_postprocess.cmd & emon input file (emonInput.txt) to "C:\Program Files (x86)\IntelSWTools\sep\config\edp\pyedp"
Run this command -> _MTL_postprocess.cmd emonInput.txt
You should see an output excel file