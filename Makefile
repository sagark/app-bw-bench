
buildpcm:
	cd IntelPerformanceCounterMonitor-V2.11 && make -j16

testbench:
	#disable nmi watchdog to enable counters
	# must run this as root
	sudo echo 0 > /proc/sys/kernel/nmi_watchdog	
