#include "cpu_stats.h"
#include "movement/sample_point.h"

extern "C" void app_main()
{
    do_cpu_stats(10);
}