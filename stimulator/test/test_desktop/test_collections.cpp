#include "testing.h"
#include <collections\smart_list.h>

using namespace ams::collections;

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
