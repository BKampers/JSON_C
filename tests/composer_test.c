#include <stdio.h>
#include <stdlib.h>

#include "Simple.h"

#include "JsonComposer.h"


void ObjectTest()
{
    JsonNode object;
    JsonStatus status = ComposeObject(&object);
    ASSERT_EQUAL_INT(JSON_OK, status);
    ASSERT_EQUAL_INT(JSON_OBJECT, object.type);
    ASSERT_EQUAL_STRING("{}", object.source);
    status = PutStringMember(&object, "String", "Test");
    ASSERT_EQUAL_INT(JSON_OK, status);
    ASSERT_EQUAL_STRING("{\"String\":\"Test\"}", object.source);
    status = PutStringMember(&object, "Escapes", "\b \n \r \t \"");
    ASSERT_EQUAL_INT(JSON_OK, status);
    ASSERT_EQUAL_STRING("{\"String\":\"Test\",\"Escapes\":\"\\b \\n \\r \\t \\\"\"}", object.source);
    status = PutIntegerMember(&object, "Year", 2015);
    ASSERT_EQUAL_INT(JSON_OK, status);
    ASSERT_EQUAL_STRING("{\"String\":\"Test\",\"Escapes\":\"\\b \\n \\r \\t \\\"\",\"Year\":2015}", object.source);
    status = PutBooleanMember(&object, "False", FALSE);
    ASSERT_EQUAL_INT(JSON_OK, status);
    EXPECT_EQUAL_STRING("{\"String\":\"Test\",\"Escapes\":\"\\b \\n \\r \\t \\\"\",\"Year\":2015,\"False\":false}", object.source);
    status = PutBooleanMember(&object, "True", TRUE);
    ASSERT_EQUAL_INT(JSON_OK, status);
    EXPECT_EQUAL_STRING("{\"String\":\"Test\",\"Escapes\":\"\\b \\n \\r \\t \\\"\",\"Year\":2015,\"False\":false,\"True\":true}", object.source);
    status = PutNullMember(&object, "Null");
    ASSERT_EQUAL_INT(JSON_OK, status);
    EXPECT_EQUAL_STRING("{\"String\":\"Test\",\"Escapes\":\"\\b \\n \\r \\t \\\"\",\"Year\":2015,\"False\":false,\"True\":true,\"Null\":null}", object.source);
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
    EXPECT_EQUAL_STRING("{\"Controls\":\"\\u0001\\u0002\\u0003\\u0004\\u0005\\u0006\\u0007\\b\\t\\n\\u000b\\f\\r\\u000e\\u000f\\u0010\\u0011\\u0012\\u0013\\u0014\\u0015\\u0016\\u0017\\u0018\\u0019\\u001a\\u001b\\u001c\\u001d\\u001e\\u001f\"}", object.source);
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
    EXPECT_EQUAL_STRING("{\"Object\":{}}", object.source);
    free(member.source);
    free(object.source);
}


void ArrayTest()
{
    JsonNode array, element;
    JsonStatus status = ComposeArray(&array);
    ASSERT_EQUAL_INT(JSON_OK, status);
    ASSERT_EQUAL_INT(JSON_ARRAY, array.type);
    ASSERT_EQUAL_STRING("[]", array.source);
    status = ComposeObject(&element);
    ASSERT_EQUAL_INT(JSON_OK, status);
    status = AddObjectElement(&array, &element);
    ASSERT_EQUAL_INT(JSON_OK, status);
    ASSERT_EQUAL_STRING("[{}]", array.source);
    status = AddStringElement(&array, "String");
    ASSERT_EQUAL_INT(JSON_OK, status);
    ASSERT_EQUAL_STRING("[{},\"String\"]", array.source);
    free(element.source);
    free(array.source);
}


void ArrayNumbersTest()
{
    JsonNode array;
    JsonStatus status = ComposeArray(&array);
    ASSERT_EQUAL_INT(JSON_OK, status);
    ASSERT_EQUAL_INT(JSON_ARRAY, array.type);
    ASSERT_EQUAL_STRING("[]", array.source);
    status = AddIntegerElement(&array, 1234567890);
    ASSERT_EQUAL_INT(JSON_OK, status);
    status = AddRealElement(&array, 0.123456789);
    ASSERT_EQUAL_INT(JSON_OK, status);
    ASSERT_EQUAL_STRING("[1234567890,0.123456789]", array.source);
    free(array.source);
}


void ArrayLiteralsTest()
{
    JsonNode array;
    JsonStatus status = ComposeArray(&array);
    ASSERT_EQUAL_INT(JSON_OK, status);
    ASSERT_EQUAL_INT(JSON_ARRAY, array.type);
    ASSERT_EQUAL_STRING("[]", array.source);
    status = AddBooleanElement(&array, TRUE);
    ASSERT_EQUAL_INT(JSON_OK, status);
    status = AddBooleanElement(&array, FALSE);
    ASSERT_EQUAL_INT(JSON_OK, status);
    status = AddNullElement(&array);
    ASSERT_EQUAL_INT(JSON_OK, status);
    ASSERT_EQUAL_STRING("[true,false,null]", array.source);
    free(array.source);
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
    
    start("ArrayTest");
    ArrayTest();
    finish();

    start("ArrayNumbersTest");
    ArrayNumbersTest();
    finish();

    start("ArrayLiteralsTest");
    ArrayLiteralsTest();
    finish();
    
    finishSuite();
}
