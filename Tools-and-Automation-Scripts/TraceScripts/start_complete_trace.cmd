::
:: Batch file to start tracing.
::

::
:: Enable kernel logger
::
xperf -on LOADER+PROC_THREAD+CSWITCH+DISPATCHER+POWER+DISK_IO+HARD_FAULTS+WDF_INTERRUPT+WDF_DPC+PROFILE+MEMINFO+DPC+INTERRUPT+REGISTRY+IDLE_STATES+PRIORITY+IPI+CLOCKINT+TIMER+SYSCALL+ALL_FAULTS -stackwalk Cswitch+readythread+profile -clocktype cycle -buffersize 1024 -minbuffers 1024

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


