#define UNITY_INCLUDE_EXEC_TIME
#include <unity.h>
#include <collections\smart_list.h>
#include <movement\sample_point_log.h>
#include <movement\analysis\analysis.h>
#include <movement\symmetric\symmetric_movement_translator.h>

using namespace std;
using namespace ams::collections;
using namespace ams::movement;
using namespace ams::movement::symmetric;

void test_smart_list()
{
    const int element_count = 1000;

    smart_list<int> list;
    TEST_ASSERT_EQUAL_INT32(0, list.length());
    for(auto i=0;i<element_count;i++)
        list.add(i);
    TEST_ASSERT_EQUAL_INT32(element_count, list.length());
    for(auto i=0;i<element_count;i++)
        TEST_ASSERT_EQUAL_INT32(i, list[i]);  
}

int num_groups(const read_only_collection<sample_point>& points)
{
    auto isPositive = [] (const sample_point & pt) { return pt.act.channels[0]>0; };
    auto isNegative = [] (const sample_point & pt) { return pt.act.channels[0]==0; };

    int cnt = 0;
    int idx = 0;
    while(true){
        idx = points.index_of(isPositive, idx);
        if(idx<0) break;
        idx = points.index_of(isNegative, idx);
        if(idx<0) break;
        cnt++;
    }
    return cnt;
}

const char training_data_filename[] = "../data/training_data.csv";

void test_training_log()
{
    smart_list<sample_point> points;
    sample_point_log::read(training_data_filename, points);

    auto positives = points.count([] (const sample_point & pt) { return pt.act.channels[0]>0; });
    auto groups = num_groups(points);

    TEST_ASSERT_EQUAL_INT32(1487, points.length());
    TEST_ASSERT_EQUAL_INT32(512, positives);
    TEST_ASSERT_EQUAL_INT32(29, groups);
}

void test_model_build()
{
    const int NumProgressValues = 50;

    UNITY_EXEC_TIME_START();
    smart_list<sample_point> points;
    sample_point_log::read(training_data_filename, points);
    UNITY_EXEC_TIME_STOP(); 
    printf("Loading points");
    UNITY_PRINT_EXEC_TIME();
    UNITY_PRINT_EOL();

    UNITY_EXEC_TIME_START();
    auto translator = symmetric_movement_translator(points);
    UNITY_EXEC_TIME_STOP(); 
    printf("Creating translator");
    UNITY_PRINT_EXEC_TIME();
    UNITY_PRINT_EOL();

    UNITY_EXEC_TIME_START();
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
    }
    UNITY_EXEC_TIME_STOP(); 
    printf("Checking points");
    UNITY_PRINT_EXEC_TIME();
    UNITY_PRINT_EOL();


    auto matchedAverage = matchedPointsMass / matchedPoints;
    auto correctAverage = correctPointsMass / correctPoints;
    auto accuracy = 100 * correctPoints / (float)matchedPoints;
    printf("points=%d, matched=%d [l=%4.2f], correct=%d [l=%4.2f], accuracy=%3.1f%\n", cnt, matchedPoints, matchedAverage, correctPoints, correctAverage, accuracy);

    auto model = translator.model();
    printf("model_size=%d bytes\n", sizeof(model));
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();
    RUN_TEST(test_smart_list);
    RUN_TEST(test_training_log);
    RUN_TEST(test_model_build);
    UNITY_END();
    return 0;
}