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
}


int main(int argc, char** argv)
{
    startSuite("Composer test");
    
    start("ObjectTest");
    ObjectTest();
    finish();
    
    finishSuite();
}
