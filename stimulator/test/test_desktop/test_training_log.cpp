#include "testing.h"
#include <collections\smart_list.h>
#include <movement\sample_point_log.h>

using namespace std;
using namespace ams::collections;
using namespace ams::movement;

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
