extern "C"
{
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
}
#include "abstract/cpu_stats.h"

static TickType_t cpu_task_delay;
char stats_buffer[1024];

static void cpu_task( void * pvParameters )
{
    while(true)
    {
        vTaskDelay(cpu_task_delay);
        vTaskGetRunTimeStats(stats_buffer);
        printf("# STATS\n");
        printf(stats_buffer);
    }
}

void do_cpu_stats(int display_secs)
{
    cpu_task_delay = display_secs * 1000 / portTICK_RATE_MS;
    xTaskCreatePinnedToCore(cpu_task, "cpu_stats_0", 2048, NULL, 1, NULL, 0);
}