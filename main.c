#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Types.h"

#include "JsonParser.h"
#include "JsonGenerator.h"


char* validSource = "{ \"Number\" :  -0.123456789e+12, \"String\"   :   \"\\\"\\f\\n\\r\\t\\\\\\/\\uAc01\" ,  \"EmptyObject\":{}, \"Object\":{\"E1\":1,\"E2\":2}, \"Array\" : [1,23,456,7890]}";
char* controlStringSource = "{ \"Control\" : \"\t\" }";
char* invalidUnicodeSource = "{ \"Unicode\" : \"\\u123q\" }";
char* invalidNumberSource = "{ \"Number\" :  0123 }";

void dumpNode(JsonNode* node)
{
    int i;
    printf("%2d: ", node->type);
    for (i = node->offset; i < node->offset + node->length; ++i)
    {
        printf("%c", node->source[i]);
    }
    printf("| %d\r\n", node->length);
}


void ParseValidSourceTest()
{
    JsonNode object, pair, name, separator, value;
    ScanNext(validSource, &object);
    dumpNode(&object);
    ParseFirst(&object, &pair);
    while (pair.type == JSON_PAIR)
    {
        JsonNode next;
        dumpNode(&pair);
        if (pair.type == JSON_PAIR)
        {
            GetValue(&pair, &value);
            if (value.type == JSON_OBJECT)
            {
                JsonNode element, nextElement;
                ParseFirst(&value, &element);
                while (element.type == JSON_PAIR)
                {
                    dumpNode(&element);
                    ParseNext(&element, &nextElement);
                    element = nextElement;
                }
            }
        }
        ParseNext(&pair, &next);
        if (next.type == JSON_ARRAY)
        {
            JsonNode element;
            ParseFirst(&next, &element);
            dumpNode(&element);
        }
        pair = next;
    }
}


void ParseInvalidStringsTest()
{
    JsonNode object;
    ScanNext(controlStringSource, &object);
    printf("Control String Test: ");
    if (object.type == JSON_INVALID)
    {
        printf("OK");
    }
    else
    {
        printf("Error = %d", object.type);
    }
    printf("\r\n");
    ScanNext(invalidUnicodeSource, &object);
    printf("Invalid Unicode Test: ");
    if (object.type == JSON_INVALID)
    {
        printf("OK");
    }
    else
    {
        printf("Error = %d", object.type);
    }
    printf("\r\n");
}


void ParseInvalidNumberTest()
{
    JsonNode object;
    ScanNext(invalidNumberSource, &object);
    printf("Invalid Number Test: ");
    if (object.type == JSON_INVALID)
    {
        printf("OK");
    }
    else
    {
        printf("Error = %d", object.type);
    }
    printf("\r\n");
}


void GeneratorTest()
{
    char* string;
    JsonObject* root = CreateJsonObject();
    JsonObject* sizes = CreateJsonObject();
    JsonObject* element = CreateJsonObject();
    JsonArray* array = CreateJsonArray();
    AddInteger(array, 0);
    AddReal(array, 0.0);
    AddBoolean(array, FALSE);
    AddObject(array, element);
    AddString(array, "element");
    AddNull(array);
    PutInteger(sizes, "void", sizeof(void));
    PutInteger(sizes, "char", sizeof(char));
    PutInteger(sizes, "short", sizeof(short));
    PutInteger(sizes, "int", sizeof(int));
    PutInteger(sizes, "long", sizeof(long));
    PutInteger(sizes, "size_t", sizeof(size_t));
    PutInteger(sizes, "enum", sizeof(JsonType));
    PutInteger(sizes, "float", sizeof(float));
    PutInteger(sizes, "double", sizeof(double));
    PutInteger(sizes, "*", sizeof(int*));
    PutNull(root, "0");
    PutInteger(root, "Year", 1970);
    PutInteger(root, "Month", 3);
    PutInteger(root, "Date", 18);
    PutObject(root, "Sizes", sizes);
    PutArray(root, "Array", array);
    PutBoolean(root, "Proposition", TRUE);
    PutString(root, "Text", "Hello World");
    PutReal(root, "Pi", 3.14159265358979323846264338327950288419716939937510582097494459230781640);
    string = ToString(root);
    printf(string);
    free(string);
    printf("\r\n");
    ClearJsonObject(root);
    ClearJsonObject(sizes);
    ClearJsonArray(array);
    string = ToString(root);
    printf(string);
    free(string);
    printf("\r\n");
}


int main(int argc, char** argv)
{
    GeneratorTest();
    ParseValidSourceTest();
    ParseInvalidStringsTest();
    ParseInvalidNumberTest();
    return EXIT_SUCCESS;
}
