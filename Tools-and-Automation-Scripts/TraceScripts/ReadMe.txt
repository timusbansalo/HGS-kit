"start_small_trace.cmd" is the suggested ETL collection script for the WosHeteroStats post processing tool to work with most of the stats.
"start_complete_trace.cmd" helps to get more events which is good for manual analysis with WPA, but might add an overhead while running along with workloads. 

Steps:
1. Execute from admin prompt, start_*_trace.cmd
2. Wait for the workload execution or any intended time period
3. Execute stop_trace.cmd