///////////////////////////////////
The x64(release) .exe file is in ~\x64\Release\HGSharnessapp.exe
///////////////////////////////////////


to run the program call it with the parameters:
-c x1,y1 x2,y2 x3,y3 ...  (space is not allowed for each pair x,y)
such that xi is class index and yi is duration in Milli-seconds

note: time=0 means the class wil run forever (unless there is time out for the whole program)

by default, the scenario will run in a circuler manner.
to set a time out for the program use the flag:
-t x
such that x is time out in Milli-seconds

note: not using -t flag or using it with time =0 will make the program run forver

*there are no specific order for the flags, just note that if there are multipe -t flags, the program will only consider the last one

example:
./HGSharnessapp.exe -c 0,1000 2,2000 1,4000 -t 20000

for 20 seconds do:
start:		1 sec class0
		2 sec class2
		4 sec class1
goto start

one iteration is 7 seconds, so 2 iterations will be executed (14 sec), and will start the third one and will be cut at the end
the program will exit after 20 seconds


.....................................................................................
How to run harness:

HGSharnessapp.exe -c <class,duration_ms> <class,duration_ms> ... [-t <timeout_ms>]

Ex:
HGSharnessapp.exe -c  0,1000 1,1000 2,1000 3,1000 -t 500000

This will run class 0 for 1000ms, then class 1 for 1000 ms and so on, for 500000 ms time.
If you don’t use timeout then it will run forever.

Supported classes: 
0
1
2 – can use “V”/”v” for VNNI part, or “I”/”i” for INT8, default is VNNI (char instead of the class number)
3 – can use “P”/”p” for Pause instruction, or “U”/”u” for UserMWait, default is Pause (char instead of the class number)

