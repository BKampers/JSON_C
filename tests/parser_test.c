#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "JsonParser.h" 

#include "Simple.h"


#define EXPECT_NODE_SOURCE(EXPECTED, NODE) EXPECT_EQUAL_INT(0, strncmp(EXPECTED, NODE.source + NODE.offset, NODE.length))


void ExpectValidInteger(char* value, int expected)
{
    char source[64];
    JsonNode object;
    int retrieved;
    sprintf(source, "{\"int\":%s}", value);
    Initialize(source, &object);
    GetInt(&object, "int", &retrieved);
    EXPECT_EQUAL_INT(expected, retrieved);
}


void ExpectValidReal(char* value, double expected)
{
    char source[64];
    JsonNode object;
    double retrieved;
    sprintf(source, "{\"double\":%s}", value);
    Initialize(source, &object);
    GetDouble(&object, "double", &retrieved);
    EXPECT_EQUAL_DOUBLE(expected, retrieved, 0.0);
}


void ExpectInvalidString(char* source)
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


/*
** JSON Parser Test Suite
*/

void ValidIntegerTest() {
    ExpectValidInteger("123", 123);
    ExpectValidInteger("0", 0);
    ExpectValidInteger("-0", 0);
    ExpectValidInteger("-8", -8);
    ExpectValidInteger("-75", -75);
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


void ValidRealTest()
{
    ExpectValidReal("0", 0.0);
    ExpectValidReal("12.3", 12.3);
    ExpectValidReal("-0.005", -0.005);
    ExpectValidReal("6.789e+1", 6.789e+1);
    ExpectValidReal("98E-20", 98E-20);
    ExpectValidReal("12e003", 12e003);
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


void InvalidEscapeTest()
{
    ExpectInvalidString("{\"invalid\":\"\\q\"}");
    ExpectInvalidString("{\"invalid\":\"\\u1\"}");
    ExpectInvalidString("{\"invalid\":\"\\u1a\"}");
    ExpectInvalidString("{\"invalid\":\"\\u1ae\"}");
    ExpectInvalidString("{\"invalid\":\"\\u1a2x\"}");
}


void NestedSourcesTest()
{
    JsonNode root, array, objectElement, object, objectMember, arrayMember, arrayElement;
    float real;
    int integer;
    JsonStatus status;
    Initialize("{\"Object\":{\"a\":[{\"1\":1,\"2\":2},{\"3\":3,\"4\":4}],\"o\":{}},\"Array\":[[5.0],[]]}", &root);
    status = GetObject(&root, "Object", &object);
    EXPECT_EQUAL_INT(JSON_OK, status);
    EXPECT_EQUAL_INT(JSON_OBJECT, object.type);
    status = GetArray(&object, "a", &arrayMember);
    EXPECT_EQUAL_INT(JSON_OK, status);
    EXPECT_EQUAL_INT(JSON_ARRAY, arrayMember.type);
    status = GetObjectAt(&arrayMember, 0, &objectElement);
    EXPECT_EQUAL_INT(JSON_OK, status);
    EXPECT_EQUAL_INT(JSON_OBJECT, objectElement.type);
    status = GetInt(&objectElement, "1", &integer);
    EXPECT_EQUAL_INT(JSON_OK, status);
    EXPECT_EQUAL_INT(1, integer);
    status = GetInt(&objectElement, "2", &integer);
    EXPECT_EQUAL_INT(JSON_OK, status);
    EXPECT_EQUAL_INT(2, integer);
    status = GetObjectAt(&arrayMember, 1, &objectElement);
    EXPECT_EQUAL_INT(JSON_OK, status);
    EXPECT_EQUAL_INT(JSON_OBJECT, objectElement.type);
    status = GetInt(&objectElement, "3", &integer);
    EXPECT_EQUAL_INT(JSON_OK, status);
    EXPECT_EQUAL_INT(3, integer);
    status = GetInt(&objectElement, "4", &integer);
    EXPECT_EQUAL_INT(JSON_OK, status);
    EXPECT_EQUAL_INT(4, integer);
    status = GetObjectAt(&arrayMember, 2, &objectElement);
    EXPECT_EQUAL_INT(JSON_OUT_OF_BOUNDS, status);
    status = GetObject(&object, "o", &objectMember);
    EXPECT_EQUAL_INT(JSON_OK, status);
    EXPECT_EQUAL_INT(JSON_OBJECT, objectMember.type);
    status = GetArray(&root, "Array", &array);
    EXPECT_EQUAL_INT(JSON_OK, status);
    EXPECT_EQUAL_INT(JSON_ARRAY, array.type);
    status = GetArrayAt(&array, 0, &arrayElement);
    EXPECT_EQUAL_INT(JSON_OK, status);
    EXPECT_EQUAL_INT(JSON_ARRAY, arrayElement.type);
    status = GetFloatAt(&arrayElement, 0, &real);
    EXPECT_EQUAL_INT(JSON_OK, status);
    EXPECT_EQUAL_DOUBLE(5.0, real, 0.0);
    status = GetFloatAt(&arrayElement, 1, &real);
    EXPECT_EQUAL_INT(JSON_OUT_OF_BOUNDS, status);
    status = GetArrayAt(&array, 1, &arrayElement);
    EXPECT_EQUAL_INT(JSON_OK, status);
    EXPECT_EQUAL_INT(JSON_ARRAY, arrayElement.type);
    status = GetFloatAt(&arrayElement, 0, &real);
    EXPECT_EQUAL_INT(JSON_OUT_OF_BOUNDS, status);
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

    start("NestedsourcesTest");
    NestedSourcesTest();
    finish();

    finishSuite();

    return EXIT_SUCCESS;
}
