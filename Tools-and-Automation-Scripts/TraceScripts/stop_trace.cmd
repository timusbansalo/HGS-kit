xperf -flush
xperf -stop
xperf -flush power
xperf -stop power
xperf -compress -merge \kernel.etl \user.etl mytrace.etl
