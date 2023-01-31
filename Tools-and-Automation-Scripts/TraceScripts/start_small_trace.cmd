::
:: Batch file to start tracing.
::

::
:: Enable kernel logger
::
xperf -on PROC_THREAD+LOADER+DPC+CSWITCH+POWER+PROFILE+IDLE_STATES+INTERRUPT -clocktype cycle -buffersize 1024 -minbuffers 1024

@echo off

::
:: List of other providers that we are interested in.
::
set providers=Microsoft-Windows-Kernel-Processor-Power

@echo off

::
:: Enable the providers
::
xperf -start power -on %providers% -clocktype cycle -buffersize 1024 -minbuffers 256
xperf -capturestate power %providers%


