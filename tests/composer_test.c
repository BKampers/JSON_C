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
    EXPECT_EQUAL_STRING("{}", object.source);
    status = PutStringMember(&object, "String", "Test");
    ASSERT_EQUAL_INT(JSON_OK, status);
    EXPECT_EQUAL_STRING("{\"String\":\"Test\"}", object.source);
    status = PutStringMember(&object, "Escapes", "\b \n \r \t \"");
    ASSERT_EQUAL_INT(JSON_OK, status);
    EXPECT_EQUAL_STRING("{\"String\":\"Test\",\"Escapes\":\"\\b \\n \\r \\t \\\"\"}", object.source);
}


int main(int argc, char** argv)
{
    startSuite("Composer test");
    
    start("ObjectTest");
    ObjectTest();
    finish();
    
    finishSuite();
}