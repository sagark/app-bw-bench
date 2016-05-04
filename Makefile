
PCMLIBS = pcm/cpucounters.o pcm/client_bw.o pcm/pci.o pcm/msr.o

all: logger


buildpcm:
	cd pcm && make -j16

setup:
	#disable nmi watchdog to enable counters
	# must run this as root
	sudo echo 0 > /proc/sys/kernel/nmi_watchdog
	sudo modprobe msr

logger: buildpcm
	g++ -std=c++0x -o perf_log perf_log.cc $(PCMLIBS) -lpthread

buildbandwidth:
	cd bandwidth-1.2.1 && make bandwidth64

saturate:
	./saturate.sh
