#include "testing.h"

int main(int argc, char **argv)
{
    UNITY_BEGIN();
    RUN_TEST(test_smart_list);
    RUN_TEST(test_training_log);
    RUN_TEST(test_model_build);
    UNITY_END();
    return 0;
}