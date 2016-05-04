#include "perf_count_pcm.h"

using namespace std;

int main()
{
    char c;
    std::cout << "Started logging...\npress enter to quit" << std::endl;
    PerfCount *perf = new PerfCount();
    perf->Start();
    // wait for user to stop
    cin.get();
    perf->Stop(); 
    perf->PrintAll();
}
