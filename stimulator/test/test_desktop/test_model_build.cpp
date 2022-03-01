#include "testing.h"
#include <collections\smart_list.h>
#include <movement\sample_point_log.h>
#include <movement\analysis\analysis.h>
#include <movement\symmetric\symmetric_movement_translator.h>

using namespace std;
using namespace ams::collections;
using namespace ams::movement;
using namespace ams::movement::symmetric;

const char training_data_filename[] = "../data/training_data.csv";

void test_model_build()
{
    begin_color(blue);
    
    const int NumProgressValues = 50;

    smart_list<sample_point> points;
    begin_part();
    sample_point_log::read(training_data_filename, points);
    end_part("Loading points");
    TEST_ASSERT_EQUAL_INT32(1487, points.length());

    begin_part();
    auto translator = symmetric_movement_translator(points);
    end_part("Creating translator");

    begin_part();
    const int cnt = points.length();
    int matchedPoints = 0;
    int correctPoints = 0;
    float matchedPointsMass = 0;
    float correctPointsMass = 0;
    for(int i=0;i<cnt;i++)
    {
        const sample_point & pt = points[i];
        movement_activation act;
        float progress;
        float likelihood;
        if(translator.observe(pt.time_us, pt.vec, act, progress, likelihood))
        {
            matchedPoints++;
            matchedPointsMass+=likelihood;
            if(act.channels[0]==pt.act.channels[0])
            {
                correctPoints++;
                correctPointsMass+=likelihood;
            }
        }
        if((i % 5)==0){
            
            printf("%d %4.2f %4.2f [%d,%d]\n", i, progress, likelihood, act.channels[0], pt.act.channels[0]);
        }
    }
    end_part("Checking points");

    auto matchedAverage = matchedPointsMass / matchedPoints;
    auto correctAverage = correctPointsMass / correctPoints;
    auto accuracy = 100 * correctPoints / (float)matchedPoints;
    printf("points=%d, matched=%d [l=%4.2f], correct=%d [l=%4.2f], accuracy=%3.1f%\n", cnt, matchedPoints, matchedAverage, correctPoints, correctAverage, accuracy);

    auto model = translator.model();
    printf("model_size=%d bytes\n", sizeof(model));

    end_color();
}

