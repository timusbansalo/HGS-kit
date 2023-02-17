Details on Gatherton Buildup/Drip
It’s a kernel provided to us by @Hernandez, Eli (from Nikhil’s team). 
You can find the original test in the attached email, but please do approach Eli and let him know you’re planning to use the test in case he has updates.

Kernel includes two tests:
•	Buildup (expected IPC Ratio ~2.4x, Class 1)
•	Drip (expected IPC Ratio ~3.3x, Class 2)

The way we used it was by making 10 iteration of each:
Gatherton -b -r10
Gatherton -d -r10

You can either use the affinitze parameter (-c) or adjust the affinity the same way you’re doing it for the HarnessApp.



======================

Here is the new executable usage. 
Use the “-t” option to run MT loops with OpenMP.

Usage: Gatherton [-t] <-b|-d> -r<num>
         -t: to run MT mode, (Default is ST)
         -b: to execute Buildup sample code, or
         -d: to execute Drip sample code
         -r<num>: number of runs to execute b or d sample code, (Default 1 run)

          -c<core num> option (hidden): to set affinity for the Gatherton process to a given core.

