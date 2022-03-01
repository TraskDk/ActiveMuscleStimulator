extern "C"
{
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
}
#include "collections/smart_list.h"
#include "movement/sample_point.h"
#include "movement/movement_translator.h"
#include "movement/symmetric/symmetric_movement_translator.h"

#include "abstract/movement_dummy.h"

using namespace ams::movement;

static const char *TAG = "MAIN";

movement_translator* get_movement_translator()
{
    ESP_LOGI(TAG, "Creating sample points.");
    smart_list<sample_point> samples;
    for(int i=0;i<1500;i++)
    {
        sample_point p{};
        auto tme = i * (TickType_t)10000; 
        if(i % 100 < 10)
            p.act.channels[0] = 1;
        p.vec.values[0] = sin(tme / (3.1415926 * 2000000)); 
        samples.add(p);
    }
    ESP_LOGI(TAG, "Translating points into a model.");
    return create_symmetric_movement_translator(samples);
}

void test_translator(movement_translator * translator)
{
    const int num_test_iterations = 100;

    movement_vector mov{};
    movement_activation act{};
    float progress {};
    float likelihood {};
    while(true)
    {
        auto start_time = esp_timer_get_time();
        auto last = xTaskGetTickCount() - 2;
        for(auto i=0;i<num_test_iterations;i++)
        {
            vTaskDelayUntil(&last, 5);
            auto tme = last * 500;
            if(translator->observe(tme, mov, act, progress, likelihood))
            {
            
            } 
            else
            {

            }
        }
        auto elapsed_time = esp_timer_get_time() - start_time;
        auto time_per_iteration = elapsed_time / num_test_iterations;
        auto iteration_rate = (1000000.0 / time_per_iteration);
        ESP_LOGI(TAG, "Processing rate: %3.1f iterations/sec", iteration_rate);
    }
}

void test_translator_task( void * pvParameters)
{
    test_translator((movement_translator *)pvParameters);
}

void do_movement_dummy(){
    ESP_LOGI(TAG, "Creating translator.");
    auto translator = get_movement_translator();
    ESP_LOGI(TAG, "Testing translator.");
    xTaskCreatePinnedToCore(test_translator_task, "translator_task_1", 64000, translator, 1, NULL, 1);
}
