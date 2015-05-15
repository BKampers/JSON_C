#ifndef _SIMPLE_H_
#define	_SIMPLE_H_

#include <string.h>
#include <time.h>

#include "ApiStatus.h"
#include "Types.h"

time_t suitStartTime;
time_t testStartTime;

char* currentSuiteName = NULL; 
char* currentTestName = NULL;


#define EXPECT_TRUE(ACTUAL) expectTrue(ACTUAL, __LINE__)
#define EXPECT_FALSE(ACTUAL) expectFalse(ACTUAL, __LINE__)
#define EXPECT_NULL(ACTUAL) expectNull(ACTUAL, __LINE__)
#define EXPECT_NOT_NULL(ACTUAL) expectNotNull(ACTUAL, __LINE__)
#define EXPECT_EQUAL_INT(EXPECTED, ACTUAL) expectEqualInt(EXPECTED, ACTUAL, __LINE__)
#define EXPECT_EQUAL_STRING(EXPECTED, ACTUAL) expectEqualString(EXPECTED, ACTUAL, __LINE__)
#define EXPECT_STATUS(EXPECTED, ACTUAL) expectStatus(EXPECTED, ACTUAL, __LINE__)
#define EXPECT_OK(ACTUAL) expectStatus(OK, ACTUAL, __LINE__)

#define ASSERT_NOT_NULL(ACTUAL) EXPECT_NOT_NULL(ACTUAL); if (ACTUAL == NULL) {return;}
#define ASSERT_EQUAL_INT(EXPECTED, ACTUAL) EXPECT_EQUAL_INT(EXPECTED, ACTUAL); if (EXPECTED != ACTUAL) {return;}
#define ASSERT_EQUAL_STRING(EXPECTED, ACTUAL) if (EXPECT_EQUAL_STRING(EXPECTED, ACTUAL)) {return;}


double timeSince(time_t time)
{
    return (clock() - time) / 1000000.0;
}


double testTime()
{
    return timeSince(testStartTime);
}


void startSuite(char* suiteName)
{
    currentSuiteName = suiteName;
    printf("%%SUITE_STARTING%% %s\n", suiteName);
    printf("%%SUITE_STARTED%%\n");
    suitStartTime = clock();
}


void finishSuite()
{
    printf("%%SUITE_FINISHED%% time=%f\n", timeSince(suitStartTime));
    currentSuiteName = NULL;
}


void start(char* testName)
{
    currentTestName = testName;
    printf("%%TEST_STARTED%% %s (%s)\n", currentTestName, currentSuiteName);
    testStartTime = clock();
}


void finish()
{
    printf("%%TEST_FINISHED%% time=%f %s (%s)\n", testTime(), currentTestName, currentSuiteName);
    currentTestName = NULL;
}



void printTestLine(bool pass, int line, char* message)
{
    printf("%s time=%f testname=%s (%s) message=(%d) %s\n",
        (pass) ? "%TEST_PASSED%" : "%TEST_FAILED%",
        testTime(),
        currentTestName,
        currentSuiteName,
        line,
        message
    );
    
}


void expectTrue(bool actual, int line)
{
    printTestLine(actual, line, (actual) ? "TRUE" : "FALSE");
}


void expectFalse(bool actual, int line)
{
    printTestLine(! actual, line, (! actual) ? "FALSE" : "TRUE");
}


void expectNull(void* actual, int line)
{
    bool pass = (actual == NULL);
    printTestLine(pass, line, (pass) ? "NULL" : "NOT NULL");
}


void expectNotNull(void* actual, int line)
{
    bool pass = (actual != NULL);
    printTestLine(pass, line,  (pass) ? "NOT NULL" : "NULL");
}


void expectEqualInt(int expected, int actual, int line)
{
    char message[64];
    bool pass = (expected == actual);
    if (pass)
    {
        sprintf(message, "%d", actual);
    }
    else
    {
        sprintf(message, "expected: %d, actual: %d", expected, actual);
    }
    printTestLine(pass, line, message);    
}


bool expectEqualString(const char* expected, const char* actual, int line)
{
    char message[1024];
    bool pass = (strcmp(expected, actual) == 0);
    if (pass)
    {
        sprintf(message, "%s", actual);
    }
    else
    {
        sprintf(message, "expected: %s, actual: %s", expected, actual);
    }
    printTestLine(pass, line, message);
    return pass;
}


void expectStatus(Status expected, Status actual, int line)
{
    char message[64];
    bool pass = (expected == actual);
    if (pass)
    {
        sprintf(message, "%s", actual);
    }
    else
    {
        sprintf(message, "expected: %s, actual: %s", expected, actual);
    }
}


#endif	/* _SIMPLE_H_ */

