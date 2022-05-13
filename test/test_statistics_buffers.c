#include "unity.h"
#include "statistics_buffers.h"
void test_3_input_mean_stdev(void)
{
    STATISTICS_ROLLING_AVE ra;
    initStatRollAve(&ra);
    pushStatSample(&ra,1.0);
    pushStatSample(&ra,2.0);
    pushStatSample(&ra,3.0);
    TEST_ASSERT_FALSE(ra.full);
    TEST_ASSERT_EQUAL_FLOAT(2.0,getStatRollAve(&ra));
    TEST_ASSERT_EQUAL_FLOAT(0.816496580927726,getStatRollStdDev(&ra));
}

void test_clear_mean_stdev(void)
{
    STATISTICS_ROLLING_AVE ra;
    initStatRollAve(&ra);
    pushStatSample(&ra,1.0);
    pushStatSample(&ra,2.0);
    pushStatSample(&ra,3.0);
    clearStatRollAve(&ra);
    pushStatSample(&ra,3.0);
    pushStatSample(&ra,5.0);
    pushStatSample(&ra,7.0);    
    TEST_ASSERT_FALSE(ra.full);
    TEST_ASSERT_EQUAL_FLOAT(5.0,getStatRollAve(&ra));
    TEST_ASSERT_EQUAL_FLOAT(1.632993161855452,getStatRollStdDev(&ra));
}


void test_large_mean_stdev(void)
{
    STATISTICS_ROLLING_AVE ra;
    initStatRollAve(&ra);
    int i=0;
    for (i=0;i<80;i++)
    {
        pushStatSample(&ra,((float)i)+0.5);
    }
    TEST_ASSERT_FALSE(ra.full);
    TEST_ASSERT_EQUAL_FLOAT(40.0,getStatRollAve(&ra));
    TEST_ASSERT_EQUAL_FLOAT(23.092206477510977,getStatRollStdDev(&ra));
}

void test_rollover_mean_stdev(void)
{
    STATISTICS_ROLLING_AVE ra;
    initStatRollAve(&ra);
    int i=0;
    for (i=0;i<240;i++)
    {
        pushStatSample(&ra,((float)i)+0.5);
    }
    TEST_ASSERT_TRUE(ra.full);
    TEST_ASSERT_EQUAL_FLOAT(180.0,getStatRollAve(&ra));
    TEST_ASSERT_EQUAL_FLOAT(34.63981331743384,getStatRollStdDev(&ra));
}