#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "JsonParser.h" 

#include "Simple.h"


/*
** JSON Parser Test Suite
*/


#define EXPECT_NODE_SOURCE(EXPECTED, NODE) EXPECT_EQUAL_INT(0, strncmp(EXPECTED, NODE.source + NODE.offset, NODE.length))

void ExpectValidInteger(char* source, const char* expected)
{
    JsonNode object, pair, value;
    Initialize(source, &object);
    ParseFirst(&object, &pair);
    GetValue(&pair, &value);
    EXPECT_EQUAL_INT(JSON_NUMBER, value.type);
    EXPECT_NODE_SOURCE(expected, value);
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
    EXPECT_EQUAL_INT(JSON_INVALID, object.type);
    Initialize("{\"int\":0123}", &object);
    EXPECT_EQUAL_INT(JSON_INVALID, object.type);
    Initialize("{\"int\":+5}", &object);
    EXPECT_EQUAL_INT(JSON_INVALID, object.type);
}


void ExpectValidReal(char* source, const char* expected)
{
    JsonNode object, pair, value;
    Initialize(source, &object);
    ParseFirst(&object, &pair);
    GetValue(&pair, &value);
    EXPECT_EQUAL_INT(JSON_NUMBER, value.type);
    EXPECT_NODE_SOURCE(expected, value);    
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
    EXPECT_EQUAL_INT(JSON_INVALID, object.type);
    Initialize("{\"invalid\":2.}", &object);
    EXPECT_EQUAL_INT(JSON_INVALID, object.type);
    Initialize("{\"invalid\":3.a}", &object);
    EXPECT_EQUAL_INT(JSON_INVALID, object.type);
    Initialize("{\"invalid\":4E5.6}", &object);
    EXPECT_EQUAL_INT(JSON_INVALID, object.type);
    Initialize("{\"invalid\":33.e4}", &object);
    EXPECT_EQUAL_INT(JSON_INVALID, object.type);
    Initialize("{\"invalid\":45.6E}", &object);
    EXPECT_EQUAL_INT(JSON_INVALID, object.type);
}


void ValidStringTest()
{
    JsonNode object, pair, value;
    Initialize("{\"String\":\"\\\"\\f\\n\\r\\t\\\\\\/\\uAc01\"}", &object);
    ParseFirst(&object, &pair);
    GetValue(&pair, &value);
    EXPECT_NODE_SOURCE("\"\\\"\\f\\n\\r\\t\\\\\\/\\uAc01\"", value);
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
    EXPECT_TRUE(JSON_OK == status);
    if (status == JSON_OK)
    {
        EXPECT_TRUE(strcmp("Text", string) == 0);
        free(string);
    }
    status = GetInt(&object, "Zero", &integer);
    EXPECT_EQUAL_INT(0, integer);
    status = GetInt(&object, "Integer", &integer);
    EXPECT_EQUAL_INT(99, integer);
    status = GetInt(&object, "Real", &integer);
    EXPECT_EQUAL_INT(-123, integer);
    status = GetInt(&object, "Mega", &integer);
    EXPECT_EQUAL_INT(1000000000, integer);
    status = GetInt(&object, "Milli", &integer);
    EXPECT_EQUAL_INT(0, integer);
    status = GetDouble(&object, "Zero", &real);
    EXPECT_EQUAL_DOUBLE(0.0, real, 0.0);
    status = GetDouble(&object, "Integer", &real);
    EXPECT_EQUAL_DOUBLE(99.0, real, 0.0);
    status = GetDouble(&object, "Real", &real);
    EXPECT_EQUAL_DOUBLE(-123.4, real, 0.0);
    status = GetDouble(&object, "Mega", &real);
    EXPECT_EQUAL_DOUBLE(1000000000.0, real, 0.0);
    status = GetDouble(&object, "Milli", &real);
    EXPECT_EQUAL_DOUBLE(0.001, real, 0.0);
    status = GetObject(&object, "Empty", &sub);
    EXPECT_EQUAL_INT(JSON_OBJECT, sub.type);
    status = GetArray(&object, "Array", &sub);
    EXPECT_EQUAL_INT(JSON_ARRAY, sub.type);
}


void UnicodeTest()
{
    char* string;
    JsonNode object;
    Initialize("{\"Unicode\":\"\\u0041\\u0042\\u0043\\u005A\"}", &object);
    AllocateString(&object, "Unicode", &string);
    EXPECT_EQUAL_STRING("ABCZ", string);
    free(string);
}


void ControlCharactersTest()
{
    char* string;
    int i;
    JsonNode object;
    Initialize("{\"Controls\":\"\\u0001\\u0002\\u0003\\u0004\"}", &object);
    AllocateString(&object, "Controls", &string);
    for (i = 0; i < 4; ++i)
    {
        EXPECT_EQUAL_INT(i + 1, string[i]);
    }
    free(string);
}


void EscapeCharactersTest()
{
    char* string;
    JsonNode object;
    Initialize("{\"Escaped\":\"\\\"\\\\\\/\\b\\f\\r\\n\\t\"}", &object);
    AllocateString(&object, "Escaped", &string);
    EXPECT_EQUAL_STRING("\"\\/\b\f\r\n\t", string);
    free(string);
}


void ExpectInvalidString(const char* source)
{
    JsonNode object;
    char* string;
    JsonStatus status;
    Initialize(source, &object);
    EXPECT_EQUAL_INT(JSON_OBJECT, object.type);
    status = AllocateString(&object, "invalid", &string);
    EXPECT_EQUAL_INT(JSON_INVALID_STRING, status);
    free(string);
    
}


void InvalidEscapeTest()
{
    ExpectInvalidString("{\"invalid\":\"\\q\"}");
    ExpectInvalidString("{\"invalid\":\"\\u1\"}");
    ExpectInvalidString("{\"invalid\":\"\\u1a\"}");
    ExpectInvalidString("{\"invalid\":\"\\u1ae\"}");
    ExpectInvalidString("{\"invalid\":\"\\u1a2x\"}");
}


int main(int argc, char** argv)
{
    startSuite("JSON Parser Test");

    start("ValidIntegerTest");
    ValidIntegerTest();
    finish();

    start("InvalidIntegerTest");
    InvalidIntegerTest();
    finish();

    start("ValidRealTest");
    ValidRealTest();
    finish();

    start("InvalidRealTest");
    InvalidRealTest();
    finish();

    start("ValidStringTest");
    ValidStringTest();
    finish();

    start("GetValueTest");
    GetValueTest();
    finish();

    start("UnicodeTest");
    UnicodeTest();
    finish();

    start("ControlCharactersTest");
    ControlCharactersTest();
    finish();

    start("EscapeCharactersTest");
    EscapeCharactersTest();
    finish();

    start("InvalidEscapeTest");
    InvalidEscapeTest();
    finish();

    finishSuite();

    return (EXIT_SUCCESS);
}
