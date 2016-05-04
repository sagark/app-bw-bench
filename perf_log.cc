#include "perf_count_pcm.h"

using namespace std;

int main()
{
    PerfCount *perf = new PerfCount();
    perf->Start();
    std::cout << "test" << std::endl;
    perf->Stop(); 
    perf->PrintAll();
}
