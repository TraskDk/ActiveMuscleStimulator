#include "abstract/cpu_stats.h"
#include "abstract/movement_dummy.h"

#ifdef ESP
extern "C" void app_main()
{
    do_cpu_stats(10);
    do_movement_dummy();
}
#endif

#ifdef NATIVE
extern "C" int main(int argc, char ** argv)
{
    do_cpu_stats(10);
    do_movement_dummy();
}
#endif
