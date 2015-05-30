#include <stdio.h>
#include <stdlib.h>

#include "Simple.h"

#include "JsonComposer.h"


#define EXPECT_NODE_SOURCE(EXPECTED, NODE) EXPECT_EQUAL_INT(0, strncmp(EXPECTED, NODE.source, NODE.length))


void ObjectTest()
{
    JsonNode object;
    JsonStatus status = ComposeObject(&object);
    ASSERT_EQUAL_INT(JSON_OK, status);
    ASSERT_EQUAL_INT(JSON_OBJECT, object.type);
    EXPECT_NODE_SOURCE("{}", object);
    status = PutStringMember(&object, "String", "Test");
    ASSERT_EQUAL_INT(JSON_OK, status);
    EXPECT_NODE_SOURCE("{\"String\":\"Test\"}", object);
    status = PutStringMember(&object, "Escapes", "\b \n \r \t \"");
    ASSERT_EQUAL_INT(JSON_OK, status);
    EXPECT_NODE_SOURCE("{\"String\":\"Test\",\"Escapes\":\"\\b \\n \\r \\t \\\"\"}", object);
    status = PutIntegerMember(&object, "Year", 2015);
    ASSERT_EQUAL_INT(JSON_OK, status);
    EXPECT_NODE_SOURCE("{\"String\":\"Test\",\"Escapes\":\"\\b \\n \\r \\t \\\"\",\"Year\":2015}", object);
    status = PutBooleanMember(&object, "False", FALSE);
    ASSERT_EQUAL_INT(JSON_OK, status);
    EXPECT_NODE_SOURCE("{\"String\":\"Test\",\"Escapes\":\"\\b \\n \\r \\t \\\"\",\"Year\":2015,\"False\":false}", object);
    status = PutBooleanMember(&object, "True", TRUE);
    ASSERT_EQUAL_INT(JSON_OK, status);
    EXPECT_NODE_SOURCE("{\"String\":\"Test\",\"Escapes\":\"\\b \\n \\r \\t \\\"\",\"Year\":2015,\"False\":false,\"True\":true}", object);
    status = PutNullMember(&object, "Null");
    ASSERT_EQUAL_INT(JSON_OK, status);
    EXPECT_NODE_SOURCE("{\"String\":\"Test\",\"Escapes\":\"\\b \\n \\r \\t \\\"\",\"Year\":2015,\"False\":false,\"True\":true,\"Null\":null}", object);
    free(object.source);
}


void ControlCharactersTest()
{
    JsonNode object;
    JsonStatus status = ComposeObject(&object);
    char string[0x20];
    int i;
    for (i = 0; i < 0x1F; ++i)
    {
        string[i] = (char) i + 1;
    }
    string[0x1F] = '\0';
    status = PutStringMember(&object, "Controls", string);
    ASSERT_EQUAL_INT(JSON_OK, status);
    EXPECT_NODE_SOURCE("{\"Controls\":\"\\u0001\\u0002\\u0003\\u0004\\u0005\\u0006\\u0007\\b\\t\\n\\u000b\\f\\r\\u000e\\u000f\\u0010\\u0011\\u0012\\u0013\\u0014\\u0015\\u0016\\u0017\\u0018\\u0019\\u001a\\u001b\\u001c\\u001d\\u001e\\u001f\"}", object);
    free(object.source);
}


void PutObjectTest()
{
    JsonNode object, member;
    JsonStatus status = ComposeObject(&object);
    ASSERT_EQUAL_INT(JSON_OK, status);
    status = ComposeObject(&member);
    ASSERT_EQUAL_INT(JSON_OK, status);
    status = PutObjectMember(&object, "Object", &member);
    ASSERT_EQUAL_INT(JSON_OK, status);
    EXPECT_NODE_SOURCE("{\"Object\":{}}", object);
    free(member.source);
    free(object.source);
}


void PutArrayTest()
{
    JsonNode object, member;
    JsonStatus status = ComposeObject(&object);
    ASSERT_EQUAL_INT(JSON_OK, status);
    status = ComposeArray(&member);
    ASSERT_EQUAL_INT(JSON_OK, status);
    status = PutArrayMember(&object, "Array", &member);
    ASSERT_EQUAL_INT(JSON_OK, status);
    EXPECT_NODE_SOURCE("{\"Array\":[]}", object);
    free(member.source);
    free(object.source);
}


void ArrayTest()
{
    JsonNode array, element;
    JsonStatus status = ComposeArray(&array);
    ASSERT_EQUAL_INT(JSON_OK, status);
    ASSERT_EQUAL_INT(JSON_ARRAY, array.type);
    EXPECT_NODE_SOURCE("[]", array);
    status = ComposeObject(&element);
    ASSERT_EQUAL_INT(JSON_OK, status);
    status = AddObjectElement(&array, &element);
    ASSERT_EQUAL_INT(JSON_OK, status);
    EXPECT_NODE_SOURCE("[{}]", array);
    status = AddStringElement(&array, "String");
    ASSERT_EQUAL_INT(JSON_OK, status);
    EXPECT_NODE_SOURCE("[{},\"String\"]", array);
    free(element.source);
    free(array.source);
}


void ArrayNumbersTest()
{
    JsonNode array;
    JsonStatus status = ComposeArray(&array);
    ASSERT_EQUAL_INT(JSON_OK, status);
    ASSERT_EQUAL_INT(JSON_ARRAY, array.type);
    EXPECT_NODE_SOURCE("[]", array);
    status = AddIntegerElement(&array, 1234567890);
    ASSERT_EQUAL_INT(JSON_OK, status);
    status = AddRealElement(&array, 0.123456789);
    ASSERT_EQUAL_INT(JSON_OK, status);
    EXPECT_NODE_SOURCE("[1234567890,0.123456789]", array);
    free(array.source);
}


void ArrayLiteralsTest()
{
    JsonNode array;
    JsonStatus status = ComposeArray(&array);
    ASSERT_EQUAL_INT(JSON_OK, status);
    ASSERT_EQUAL_INT(JSON_ARRAY, array.type);
    EXPECT_NODE_SOURCE("[]", array);
    status = AddBooleanElement(&array, TRUE);
    ASSERT_EQUAL_INT(JSON_OK, status);
    status = AddBooleanElement(&array, FALSE);
    ASSERT_EQUAL_INT(JSON_OK, status);
    status = AddNullElement(&array);
    ASSERT_EQUAL_INT(JSON_OK, status);
    EXPECT_NODE_SOURCE("[true,false,null]", array);
    free(array.source);
}


void ArrayArrayTest()
{
    JsonNode array, member;
    JsonStatus status = ComposeArray(&array);
    ASSERT_EQUAL_INT(JSON_OK, status);
    status = ComposeArray(&member);
    ASSERT_EQUAL_INT(JSON_OK, status);
    status = AddStringElement(&member, "String");
    ASSERT_EQUAL_INT(JSON_OK, status);
    EXPECT_NODE_SOURCE("[\"String\"]", member);
    status = AddArrayElement(&array, &member);
    ASSERT_EQUAL_INT(JSON_OK, status);
    EXPECT_NODE_SOURCE("[[\"String\"]]", array);
    free(member.source);
    free(array.source);
}


void InvalidNodesTest()
{
    JsonNode object, array, invalid;
    JsonStatus status = ComposeObject(&object);
    ASSERT_EQUAL_INT(JSON_OK, status);
    status = ComposeArray(&array);
    ASSERT_EQUAL_INT(JSON_OK, status);
    status = PutObjectMember(&array, "Object", &object);
    EXPECT_EQUAL_INT(JSON_OBJECT_EXPECTED, status);
    status = AddIntegerElement(&object, -1);
    EXPECT_EQUAL_INT(JSON_ARRAY_EXPECTED, status);
    invalid.type = JSON_INVALID;
    status = PutObjectMember(&object, "Invalid", &invalid);
    EXPECT_EQUAL_INT(JSON_INVALID_PARAMETER, status);
    status = AddObjectElement(&array, &invalid);
    EXPECT_EQUAL_INT(JSON_INVALID_PARAMETER, status);
    free(array.source);
    free(object.source);
}


int main(int argc, char** argv)
{
    startSuite("Composer test");
    
    start("ObjectTest");
    ObjectTest();
    finish();
    
    start("ControlCharactersTest");
    ControlCharactersTest();
    finish();
    
    start("PutObjectTest");
    PutObjectTest();
    finish();
    
    start("PutArrayTest");
    PutObjectTest();
    finish();
    
    start("ArrayTest");
    ArrayTest();
    finish();

    start("ArrayNumbersTest");
    ArrayNumbersTest();
    finish();

    start("ArrayLiteralsTest");
    ArrayLiteralsTest();
    finish();
    
    start("ArrayArrayTest");
    ArrayArrayTest();
    finish();
    
    start("InvalidNodesTest");
    InvalidNodesTest();
    finish();
    
    finishSuite();
}
