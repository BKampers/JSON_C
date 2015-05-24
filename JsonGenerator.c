#include "JsonGenerator.h"

#include <stdio.h>
#include <string.h>

#include "Text.h"

#define CAPACITY_INCREMENT 8


char NULL_LITERAL[] = "null";
char FALSE_LITERAL[] = "false";
char TRUE_LITERAL[] = "true";

void ToObjectString(const JsonObject* object, StringBuffer* stringBuffer) ;
void ToArraySring(const JsonArray* array, StringBuffer* stringBuffer);


void AppendQuotedString(const char* string, StringBuffer* stringBuffer)
{
    AppendString(stringBuffer, "\"");
    AppendString(stringBuffer, string);
    AppendString(stringBuffer, "\"");
}


void AppendValueString(const JsonValue* value, StringBuffer* stringBuffer)
{
    switch (value->type)
    {
        case JSON_STRING:
            AppendQuotedString((char*) value->data, stringBuffer);
            break;
        case JSON_NUMBER:
        case JSON_BOOLEAN:
        case JSON_NULL:
            AppendString(stringBuffer, (char*) value->data);
            break;
        case JSON_OBJECT:
            ToObjectString((JsonObject*) value->data, stringBuffer);
            break;
        case JSON_ARRAY:
            ToArraySring((JsonArray*) value->data, stringBuffer);
            break;
        default:
            break;
   }
}


void ToObjectString(const JsonObject* object, StringBuffer* stringBuffer) 
{
    int i;
    AppendString(stringBuffer, "{");
    for (i = 0; i < object->elementCount; ++i)
    {
        JsonPair* pair = &(object->elements[i]);
        if (i > 0)
        {
            AppendString(stringBuffer, ",");
        }
        AppendQuotedString(pair->name, stringBuffer);
        AppendString(stringBuffer, ":");
        AppendValueString(&(pair->value), stringBuffer);
    }
    AppendString(stringBuffer, "}");
}

 
void ToArraySring(const JsonArray* array, StringBuffer* stringBuffer)
{
    int i;
    AppendString(stringBuffer, "[");
    for (i = 0; i < array->elementCount; ++i)
    {
        if (i > 0)
        {
            AppendString(stringBuffer, ",");
        }
        AppendValueString(&(array->elements[i]), stringBuffer);
    }
    AppendString(stringBuffer, "]");    
}


char* AllocIntegerValue(int value)
{
    char* string = malloc(16 * sizeof(char));
    sprintf(string, "%d", value);
    return string;
}


char* AllocRealValue(double value)
{
    char* string = malloc(32 * sizeof(char));
    sprintf(string, "%f", value);
    return string;
}


char* AllocStringValue(const char* value)
{
    char* string = malloc(strlen(value) + 1);
    strcpy(string, value);
    return string;
}


void PutPair(JsonObject* object, const char* name, JsonType type, void* value)
{
    if (object->capacity <= object->elementCount)
    {
        object->capacity += CAPACITY_INCREMENT;
        object->elements = realloc(object->elements, object->capacity * sizeof(JsonPair));
    }
    JsonPair* pair = &(object->elements[object->elementCount]);
    pair->name = AllocStringValue(name);
    pair->value.type = type;
    pair->value.data = value;
    object->elementCount++;
}


void AddElement(JsonArray* array, JsonType type, void* element)
{
    if (array->capacity <= array->elementCount)
    {
        array->capacity += CAPACITY_INCREMENT;
        array->elements = realloc(array->elements, array->capacity * sizeof(JsonValue));
    }
    JsonValue* value = &(array->elements[array->elementCount]);
    value->type = type;
    value->data = element;
    array->elementCount++;
}


/*
** Interface
*/

JsonObject* CreateJsonObject()
{
    JsonObject* object = malloc(sizeof(JsonObject));
    object->elementCount = 0;
    object->capacity = 0;
    object->elements = NULL;
    return object;
}


void ClearJsonObject(JsonObject* object)
{
    int i;
    for (i = 0; i < object->elementCount; ++i)
    {
        JsonPair* pair = &(object->elements[i]);
        free(pair->name);
        switch (pair->value.type) 
        {
            case JSON_NUMBER:
            case JSON_STRING:
                free(pair->value.data);
                break;
            default:
                break;
        }
    }
    object->elementCount = 0;
    free(object->elements);
    object->capacity = 0;
}


void PutObject(JsonObject* object, const char* name, JsonObject* value)
{
    PutPair(object, name, JSON_OBJECT, value);
}


void PutArray(JsonObject* object, const char* name, JsonArray* value)
{
    PutPair(object, name, JSON_ARRAY, value);
}


void PutBoolean(JsonObject* object, const char* name, int value)
{
    PutPair(object, name, JSON_BOOLEAN, (value) ? TRUE_LITERAL : FALSE_LITERAL);
}


void PutInteger(JsonObject* object, const char* name, int value)
{
    PutPair(object, name, JSON_NUMBER, AllocIntegerValue(value));
}


void PutReal(JsonObject* object, const char* name, double value)
{
    PutPair(object, name, JSON_NUMBER, AllocRealValue(value));
}


void PutString(JsonObject* object, const char* name, char* value)
{
    PutPair(object, name, JSON_STRING, AllocStringValue(value));
}


void PutNull(JsonObject* object, const char* name)
{
    PutPair(object, name, JSON_NULL, NULL_LITERAL);
}


JsonArray* CreateJsonArray()
{
    JsonArray* array = malloc(sizeof(JsonArray));
    array->elementCount = 0;
    array->capacity = 0;
    array->elements = NULL;
    return array;
}


void ClearJsonArray(JsonArray* array)
{
    int i;
    for (i = 0; i < array->elementCount; ++i)
    {
        JsonValue* element = &(array->elements[i]);
        switch (element->type) 
        {
            case JSON_NUMBER:
            case JSON_STRING:
                free(element->data);
                break;
            default:
                break;
        }
    }
    array->elementCount = 0;
    free(array->elements);
    array->capacity = 0;
}


void AddObject(JsonArray* array, JsonObject* element)
{
    AddElement(array, JSON_OBJECT, element);
}


void AddArray(JsonArray* array, JsonArray* element)
{
    AddElement(array, JSON_ARRAY, element);
}


void AddBoolean(JsonArray* array, int element)
{
    AddElement(array, JSON_BOOLEAN, (element) ? TRUE_LITERAL : FALSE_LITERAL);
}


void AddInteger(JsonArray* array, int element)
{
    AddElement(array, JSON_NUMBER, AllocIntegerValue(element));
}


void AddReal(JsonArray* array, double element)
{
    AddElement(array, JSON_NUMBER, AllocRealValue(element));
}


void AddString(JsonArray* array, char* element)
{
    AddElement(array, JSON_STRING, AllocStringValue(element));
}


void AddNull(JsonArray* array)
{
    AddElement(array, JSON_NULL, NULL_LITERAL);
}


char* ToString(const JsonObject* object)
{
    StringBuffer stringBuffer;
    InitializeStringBuffer(&stringBuffer);
    ToObjectString(object, &stringBuffer);
    return DetachString(&stringBuffer);
}
