#include <stdio.h>
#include <stdlib.h>

#include "JsonGenerator.h"

#include "Simple.h"


/*
 * Generator Test Suite
 */

void GeneratorTest()
{
    char* string;
    JsonObject* root = CreateJsonObject();
    JsonObject* element = CreateJsonObject();
    JsonArray* array = CreateJsonArray();
    AddInteger(array, 0);
    AddReal(array, 0.0);
    AddBoolean(array, FALSE);
    AddObject(array, element);
    AddString(array, "element");
    AddNull(array);
    PutNull(root, "0");
    PutInteger(root, "Year", 1970);
    PutInteger(root, "Month", 3);
    PutInteger(root, "Date", 18);
    PutArray(root, "Array", array);
    PutBoolean(root, "Proposition", TRUE);
    PutString(root, "Text", "Hello World");
    PutReal(root, "Pi", 3.14159265358979323846264338327950288419716939937510582097494459230781640);
    string = ToString(root);
    printf("%s", string);
    printf("\r\n");
    EXPECT_EQUAL_STRING("{\"0\":null,\"Year\":1970,\"Month\":3,\"Date\":18,\"Array\":[0,0.000000,false,{},\"element\",null],\"Proposition\":true,\"Text\":\"Hello World\",\"Pi\":3.141593}", string);
    free(string);
    ClearJsonObject(root);
    ClearJsonArray(array);
    string = ToString(root);
    EXPECT_EQUAL_STRING("{}", string);
    free(string);
}


int main(int argc, char** argv) {
    startSuite("Generator test");

    start("GeneratorTest");
    GeneratorTest();
    finish();
    
    finishSuite();
    return EXIT_SUCCESS;
}
