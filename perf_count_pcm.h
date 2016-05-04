#ifndef PERF_COUNT_PCM
#define PERF_COUNT_PCM

#include <iostream>

#include "pcm/cpucounters.h"

using namespace std;

class PerfCount {
  PCM* pcm;
  SystemCounterState before;
  SystemCounterState after;
  uint64 before_time;
  uint64 after_time;

  void SafelyExit(int return_code) {
    ReleasePCM();
    exit(return_code);
  }

  void ReleasePCM() {
    if (pcm != NULL) {
      pcm->cleanup();
      delete pcm;
    }
  }


public:
  PerfCount() {
    pcm = PCM::getInstance();
    PCM::ErrorCode status = pcm->program();
    if (status == PCM::PMUBusy) {
      cerr << "Reseting PMU configuration" << endl;
      pcm->resetPMU();
      status = pcm->program();
    }
    if (status != PCM::Success)
      SafelyExit(-2);
    cerr << "System: " << pcm->getCPUBrandString() << " (";
    cerr << pcm->getUArchCodename() << ")" << endl;
  }

  ~PerfCount() {
    ReleasePCM();
  }

  void Reset() {}

  void Start() {
    #pragma omp single
    StartParallel();
  }

  void Stop() {
    #pragma omp barrier
    #pragma omp single
    StopParallel();
  }

  void StartParallel(bool reset=true) {
    before = getSystemCounterState();
    before_time = pcm->getTickCount(1000000);
  }

  void StopParallel(bool reset=true) {
    after = getSystemCounterState();
    after_time = pcm->getTickCount(1000000);
  }

  void PrintAll() {
    double seconds = ((double) after_time - before_time)/1e6;
    double ghz = getActiveAverageFrequency(before,after)/1e9;
    long   cycles = getCycles(before, after);
    long   insts = getInstructionsRetired(before, after);
    double ipc = getIPC(before, after);
    long   mem_reqs = getBytesReadFromMC(before,after)/64;
    double mem_reqs_s = (double) mem_reqs/seconds;
    long   write_reqs = getBytesWrittenToMC(before,after)/64;
    double l3_hit_rate = getL3CacheHitRatio(before,after);
    long   l3_misses = getL3CacheMisses(before,after);
    double prefetch_guess = (double) (mem_reqs-l3_misses) / l3_misses;
    cout << setprecision(3) << fixed;
    cout << "Time:   " << setw(15) << seconds << " s" << endl;
    cout << "Freq:   " << setw(15) << ghz << " GHz" << endl;
    cout << "Cycles: " << setw(15) << cycles << endl;
    cout << "Insts:  " << setw(15) << insts << endl;
    cout << "IPC:    " << setw(15) << ipc << endl;
    cout << "Mreqs:  " << setw(15) << mem_reqs << endl;
    cout << "Wreqs:  " << setw(15) << write_reqs << endl;
    cout << "Mreqs/s:" << setw(15) << mem_reqs_s/1e6 << " M/s" << endl;
    cout << "L3 Hit: " << setw(15) << 100*l3_hit_rate << " %" << endl;
    cout << "L3 Miss:" << setw(15) << l3_misses << endl;
    cout << "Prefetch:" << setw(14) << 100*prefetch_guess << " %" << endl;
  }
};

#endif // PERF_COUNT_PCM
