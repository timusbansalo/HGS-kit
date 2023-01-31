::
:: Batch file to start tracing.
::

::
:: Enable kernel logger
::
xperf -on LOADER+PROC_THREAD+CSWITCH+DISPATCHER+POWER+DISK_IO+INTERRUPT_STEER+HARD_FAULTS+WDF_INTERRUPT+WDF_DPC+PROFILE+MEMINFO+DPC+INTERRUPT+REGISTRY+IDLE_STATES+PRIORITY+IPI+CLOCKINT+TIMER+SYSCALL+ALL_FAULTS+0xA0000080+0xA0000100 -stackwalk Cswitch+readythread+profile -clocktype cycle -buffersize 1024 -minbuffers 1024

@echo off

::
:: List of other providers that we are interested in.
::
set providers=Microsoft-Windows-Kernel-Interrupt-Steering+Microsoft-Windows-Kernel-Processor-Power+Microsoft-Windows-Kernel-Power+Microsoft-Windows-Kernel-Pep+Microsoft-Windows-Dxgkrnl

@echo off

::
:: Enable the providers
::
xperf -start power -on %providers% -clocktype cycle -buffersize 1024 -minbuffers 256
xperf -capturestate power %providers%


