#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "JsonParser.h" 


/*
** JSON Parser Test Suite
*/


const char* suiteName = NULL;
const char* testName = NULL;


void StartSuite(const char* name)
{
    suiteName = name;
    printf("%%SUITE_STARTING%% ");
    printf(suiteName);
    printf("\n");
    printf("%%SUITE_STARTED%%\n");
}


void FinishSuite()
{
    printf("%%SUITE_FINISHED%% time=0\n");
    suiteName = NULL;
}


void StartTest(const char* name)
{
    testName = name;
    printf("%%TEST_STARTED%% ");
    printf(testName);
    printf(" (");
    printf(suiteName);
    printf(")\n");
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
    printf(" time=0");
    printf(" testname=");
    printf(testName);
    printf(" (JSON Parser Test)");
    if (strlen(message) > 0)
    {
        printf(" message=");
        printf(message);
    }
    printf("\n");
}


void ExpectEqualBoolean(bool expected, bool actual)
{
    char message[256];
    bool pass = (expected) ? actual : ! actual;
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


void ExpectTrue(bool actual)
{
    ExpectEqualBoolean(TRUE, actual);
}


void ExpectEqualInteger(int expected, int actual)
{
    char message[256];
    bool pass = (expected == actual);
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


void ExpectEqualDouble(double expected, double actual)
{
    char message[256];
    bool pass = (expected == actual);
    if (pass)
    {
        message[0] = '\0';
    }
    else
    {
        sprintf(message, "expected = %f, actual = %f", expected, actual);
    }
    PrintTestResult(pass, message);
}


void ExpectEqualString(const char* expected, const char* actual)
{
    char message[256];
    bool pass = (strcmp(expected, actual) == 0);
    if (pass)
    {
        message[0] = '\0';
    }
    else
    {
        sprintf(message, "expected = %s, actual = %s", expected, actual);
    }
    PrintTestResult(pass, message);
}


void ExpectNodeText(const char* expected, const JsonNode* actual)
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
    ExpectNodeText(expected, &value);
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
    ExpectNodeText(expected, &value);    
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
    ExpectNodeText("\"\\\"\\f\\n\\r\\t\\\\\\/\\uAc01\"", &value);
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


void GetValueTest()
{
    JsonStatus status;
    JsonNode object, sub;
    char* string;
    double real;
    int integer;
    
    Initialize("{\"Zero\":0,\"Integer\":99,\"Real\":-123.4,\"Mega\":1e9,\"Milli\":1E-3,\"String\":\"Text\",\"Empty\":{},\"Array\":[0,\"one\",{},[]]}", &object);
    status = AllocateString(&object, "String", &string);
    ExpectTrue(JSON_OK == status);
    if (status == JSON_OK)
    {
        ExpectTrue(strcmp("Text", string) == 0);
        free(string);
    }
    status = GetInt(&object, "Zero", &integer);
    ExpectEqualInteger(0, integer);
    status = GetInt(&object, "Integer", &integer);
    ExpectEqualInteger(99, integer);
    status = GetInt(&object, "Real", &integer);
    ExpectEqualInteger(-123, integer);
    status = GetInt(&object, "Mega", &integer);
    ExpectEqualInteger(1000000000, integer);
    status = GetInt(&object, "Milli", &integer);
    ExpectEqualInteger(0, integer);
    status = GetDouble(&object, "Zero", &real);
    ExpectEqualDouble(0.0, real);
    status = GetDouble(&object, "Integer", &real);
    ExpectEqualDouble(99.0, real);
    status = GetDouble(&object, "Real", &real);
    ExpectEqualDouble(-123.4, real);
    status = GetDouble(&object, "Mega", &real);
    ExpectEqualDouble(1000000000.0, real);
    status = GetDouble(&object, "Milli", &real);
    ExpectEqualDouble(0.001, real);
    status = GetObject(&object, "Empty", &sub);
    ExpectEqualInteger(JSON_OBJECT, sub.type);
    status = GetArray(&object, "Array", &sub);
    ExpectEqualInteger(JSON_ARRAY, sub.type);
}


void UnicodeTest()
{
    char* string;
    JsonNode object;
    Initialize("{\"Unicode\":\"\\u0041\\u0042\\u0043\\u005A\"}", &object);
    AllocateString(&object, "Unicode", &string);
    ExpectEqualString("ABCZ", string);
    free(string);
}


void EscapeCharactersTest()
{
    char* string;
    JsonNode object;
    Initialize("{\"Escaped\":\"\\\"\\\\\\/\\b\\f\\r\\n\\t\"}", &object);
    AllocateString(&object, "Escaped", &string);
    ExpectEqualString("\"\\/\b\f\r\n\t", string);
    free(string);
}


int main(int argc, char** argv) {
    StartSuite("JSON Parser Test");

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

    StartTest("GetValueTest");
    GetValueTest();
    FinishTest();

    StartTest("UnicodeTest");
    UnicodeTest();
    FinishTest();

    StartTest("EscapeCharactersTest");
    EscapeCharactersTest();
    FinishTest();

    FinishSuite();

    return (EXIT_SUCCESS);
}
