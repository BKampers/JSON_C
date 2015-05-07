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
    PrintTestResult(pass, message);
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
    PrintTestResult(pass, message);
}


void ExpectValidInteger(const char* source, const char* expected)
{
    JsonNode object, pair, value;
    Initialize(source, &object);
    ParseFirst(&object, &pair);
    GetValue(&pair, &value);
    ExpectEqualInteger(JSON_NUMBER, value.type);
    ExpectEqualString(expected, &value);
}


void ValidIntegerTest() {
    ExpectValidInteger("{\"int\":123}", "123");
    ExpectValidInteger("{\"int\":9}", "9");
    ExpectValidInteger("{\"int\":0}", "0");
    ExpectValidInteger("{\"int\":-0}", "-0");
    ExpectValidInteger("{\"int\":-8}", "-8");
    ExpectValidInteger("{\"int\":-75}", "-75");
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


void ExpectValidReal(const char* source, const char* expected)
{
    JsonNode object, pair, value;
    Initialize(source, &object);
    ParseFirst(&object, &pair);
    GetValue(&pair, &value);
    ExpectEqualInteger(JSON_NUMBER, value.type);
    ExpectEqualString(expected, &value);    
}


void ValidRealTest()
{
    ExpectValidReal("{\"real\":12.3}",  "12.3");
    ExpectValidReal("{\"real\":-0.005}",  "-0.005");
    ExpectValidReal("{\"real\":6.789e+1}",  "6.789e+1");
    ExpectValidReal("{\"real\":98E-20}",  "98E-20");
    ExpectValidReal("{\"real\":12e003}",  "12e003");
}


void InvalidRealTest()
{
    JsonNode object;
    Initialize("{\"invalid\":.1}", &object);
    ExpectEqualInteger(JSON_INVALID, object.type);
    Initialize("{\"invalid\":2.}", &object);
    ExpectEqualInteger(JSON_INVALID, object.type);
    Initialize("{\"invalid\":3.a}", &object);
    ExpectEqualInteger(JSON_INVALID, object.type);
    Initialize("{\"invalid\":4E5.6}", &object);
    ExpectEqualInteger(JSON_INVALID, object.type);
    Initialize("{\"invalid\":33.e4}", &object);
    ExpectEqualInteger(JSON_INVALID, object.type);
    Initialize("{\"invalid\":45.6E}", &object);
    ExpectEqualInteger(JSON_INVALID, object.type);
}


void ValidStringTest()
{
    JsonNode object, pair, value;
    Initialize("{\"String\":\"\\\"\\f\\n\\r\\t\\\\\\/\\uAc01\"}", &object);
    ParseFirst(&object, &pair);
    GetValue(&pair, &value);
    ExpectEqualString("\"\\\"\\f\\n\\r\\t\\\\\\/\\uAc01\"", &value);
}


void InvalidStringTest()
{
    JsonNode object;
    Initialize("{\"invalid\":\"\\q\"}", &object);
    ExpectEqualInteger(JSON_INVALID, object.type);
    Initialize("{\"invalid\":\"\\u1\"}", &object);
    ExpectEqualInteger(JSON_INVALID, object.type);
    Initialize("{\"invalid\":\"\\u1a\"}", &object);
    ExpectEqualInteger(JSON_INVALID, object.type);
    Initialize("{\"invalid\":\"\\u1ae\"}", &object);
    ExpectEqualInteger(JSON_INVALID, object.type);
    Initialize("{\"invalid\":\"\\u1a2x\"}", &object);
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

    StartTest("ValidRealTest");
    ValidRealTest();
    FinishTest();

    StartTest("InvalidRealTest");
    InvalidRealTest();
    FinishTest();

    StartTest("ValidStringTest");
    ValidStringTest();
    FinishTest();

    StartTest("InvalidStringTest");
    InvalidStringTest();
    FinishTest();

    printf("%%SUITE_FINISHED%% time=0\n");

    return (EXIT_SUCCESS);
}
