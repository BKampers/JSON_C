#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "JsonParser.h" 


/*
** JSON Parser Test Suite
*/


const char* testName = NULL;


void StartTest(const char* name)
{
    testName = name;
    printf("%%TEST_STARTED%% ");
    printf(testName);
    printf(" (JSON Parser Test)\n");
}


void FinishTest()
{
    printf("%%TEST_FINISHED%% time=0 ");
    printf(testName);
    printf(" (JSON Parser Test)\n");
    testName = NULL;
}


void PrintTestResult(bool pass, const char* message)
{
    if (pass)
    {
        printf("%%TEST_PASSED%%");
    }
    else
    {
        printf("%%TEST_FAILED%%");
    }
    printf(" time=0 testname=");
    printf(testName);
    printf(" (JSON Parser Test)");
    if (strlen(message) > 0)
    {
        printf(" message=");
        printf(message);
    }
    printf("\n");
}


void ExpectEqualInteger(int expected, int actual)
{
    char message[256];
    bool pass = expected == actual;
    if (pass)
    {
        message[0] = '\0';
    }
    else
    {
        sprintf(message, "expected = %d, actual = %d", expected, actual);
    }
    PrintTestResult(pass, testName);
}


void ExpectEqualString(const char* expected, const JsonNode* actual)
{
    char message[256];
    bool pass = (strlen(expected) == actual->length) &&(strncmp(expected, actual->source + actual->offset, actual->length) == 0);
    if (pass)
    {
        message[0] = '\0';
    }
    else
    {
        sprintf(message, "expected = %s, actual = ", expected);
        strncat(message, actual->source + actual->offset, actual->length);
    }
    PrintTestResult(pass, testName);
}


void ValidIntegerTest() {
    JsonNode object, pair, value;
    Initialize("{\"int\":123}", &object);
    ParseFirst(&object, &pair);
    GetValue(&pair, &value);
    ExpectEqualInteger(JSON_NUMBER, value.type);
    ExpectEqualString("123", &value);
    Initialize("{\"int\":9}", &object);
    ParseFirst(&object, &pair);
    GetValue(&pair, &value);
    ExpectEqualInteger(JSON_NUMBER, value.type);
    ExpectEqualString("9", &value);
    Initialize("{\"int\":0}", &object);
    ParseFirst(&object, &pair);
    GetValue(&pair, &value);
    ExpectEqualInteger(JSON_NUMBER, value.type);
    ExpectEqualString("0", &value);
    Initialize("{\"int\":-0}", &object);
    ParseFirst(&object, &pair);
    GetValue(&pair, &value);
    ExpectEqualInteger(JSON_NUMBER, value.type);
    ExpectEqualString("-0", &value);
    Initialize("{\"int\":-8}", &object);
    ParseFirst(&object, &pair);
    GetValue(&pair, &value);
    ExpectEqualInteger(JSON_NUMBER, value.type);
    ExpectEqualString("-8", &value);
    Initialize("{\"int\":-75}", &object);
    ParseFirst(&object, &pair);
    GetValue(&pair, &value);
    ExpectEqualInteger(JSON_NUMBER, value.type);
    ExpectEqualString("-75", &value);
}


void InvalidIntegerTest() {
    JsonNode object;
    Initialize("{\"int\":12a}", &object);
    ExpectEqualInteger(JSON_INVALID, object.type);
    Initialize("{\"int\":0123}", &object);
    ExpectEqualInteger(JSON_INVALID, object.type);
    Initialize("{\"int\":+5}", &object);
    ExpectEqualInteger(JSON_INVALID, object.type);
}


int main(int argc, char** argv) {
    printf("%%SUITE_STARTING%% JSON Parser Test\n");
    printf("%%SUITE_STARTED%%\n");

    StartTest("ValidIntegerTest");
    ValidIntegerTest();
    FinishTest();

    StartTest("InvalidIntegerTest");
    InvalidIntegerTest();
    FinishTest();

    printf("%%SUITE_FINISHED%% time=0\n");

    return (EXIT_SUCCESS);
}
