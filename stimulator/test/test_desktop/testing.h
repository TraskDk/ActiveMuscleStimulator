#pragma once
#define UNITY_INCLUDE_EXEC_TIME
#include <unity.h>
#include <stdio.h>

void test_smart_list();
void test_training_log();
void test_model_build();

enum color {
    black = 30,
    red = 31,
    green = 32,
    yellow = 33,
    blue = 34,
    magenta = 35,
    cyan = 36,
    white = 37
};

inline void begin_color(color c)
{
    printf("\033[;%dm", c);
}

inline void end_color()
{
    printf("\033[0m");
}

inline void begin_part(){
    UNITY_EXEC_TIME_START();
}

inline void end_part(const char * method)
{
    UnityPrint(method);
    UNITY_EXEC_TIME_STOP();
    UNITY_PRINT_EXEC_TIME();
    UNITY_PRINT_EOL();
}
