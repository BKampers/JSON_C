#include "JsonComposer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Text.h"

#define EMPTY_SOURCE_SIZE 3

char JSON_NULL_LITERAL[] = "null";
char JSON_FALSE_LITERAL[] = "false";
char JSON_TRUE_LITERAL[] = "true";


char* EMPTY_OBJECT_SOURCE = "{}";
char* EMPTY_ARRAY_SOURCE = "[]";



void AppendQuoted(StringBuffer* buffer, const char* string)
{
    size_t i;
    size_t length = strlen(string);
    AppendCharacter(buffer, '\"');
    for (i = 0; i < length; ++i)
    {
        char character = string[i];
        if (IsUnicodeControl(character))
        {
            switch (character)
            {
                case '\b':
                    AppendString(buffer, "\\b");
                    break;
                case '\f':
                    AppendString(buffer, "\\f");
                    break;
                case '\r':
                    AppendString(buffer, "\\r");
                    break;
                case '\n':
                    AppendString(buffer, "\\n");
                    break;
                case '\t':
                    AppendString(buffer, "\\t");
                    break;
                default:
                {
                    char u[7];
                    sprintf(u, "\\u%04x", character);
                    AppendString(buffer, u);
                    break;
                }
            }
        }
        else if (character == '\"')
        {
            AppendString(buffer, "\\\"");
        }
        else
        {
            AppendCharacter(buffer, character);
        }
    }
    AppendCharacter(buffer, '\"');
}


JsonStatus PutMember(JsonNode* object, const char* name, const char* value, bool quoted)
{
    if ((object != NULL) && (object->type == JSON_OBJECT))
    {
        StringBuffer buffer;
        InitializeStringBuffer(&buffer);
        bool empty = strcmp(object->source, EMPTY_OBJECT_SOURCE) == 0;
        AppendSubstring(&buffer, object->source, 0, strlen(object->source) - 1);
        if (! empty)
        {
            AppendCharacter(&buffer, ',');
        }
        AppendQuoted(&buffer,  name);
        AppendCharacter(&buffer, ':');
        if (quoted)
        {
            AppendQuoted(&buffer,  value);
        }
        else
        {
            AppendString(&buffer, value);
        }
        AppendCharacter(&buffer, '}');
        free(object->source);
        object->source = DetachString(&buffer);
        return JSON_OK;
    }
    else
    {
        return JSON_OBJECT_EXPECTED;
    }
}


JsonStatus AddArrayElement(JsonNode* array, const char* elementString, size_t length, bool quoted)
{
    if ((array != NULL) && (array->type == JSON_ARRAY))
    {
        StringBuffer buffer;
        InitializeStringBuffer(&buffer);
        AppendSubstring(&buffer, array->source, 0, strlen(array->source) - 1);
        if (strcmp(array->source, EMPTY_ARRAY_SOURCE) != 0)
        {
            AppendCharacter(&buffer, ',');
        }
        if (quoted)
        {
            AppendQuoted(&buffer, elementString);
        }
        else
        {
            AppendSubstring(&buffer, elementString, 0, length);
        }
        AppendCharacter(&buffer, ']');
        free(array->source);
        array->source = DetachString(&buffer);
        return JSON_OK;
    }
    else
    {
        return JSON_ARRAY_EXPECTED;
    }
}


/*
** Interface
*/

JsonStatus ComposeObject(JsonNode* object)
{
    object->source = malloc(EMPTY_SOURCE_SIZE);
    if (object->source != NULL)
    {
        strcpy(object->source, EMPTY_OBJECT_SOURCE);
        object->type = JSON_OBJECT;
        object->offset = 0;
        object->length = EMPTY_SOURCE_SIZE - 1;
        return JSON_OK;
    }
    else
    {
        return JSON_OUT_OF_MEMORY;
    }
}


JsonStatus PutObjectMember(JsonNode* object, const char* name, const JsonNode* value)
{
    JsonStatus status;
    if ((value != NULL) && (value->type == JSON_OBJECT))
    {
        char* valueString = malloc(value->length + 1);
        strncpy(valueString, value->source + value->offset, value->length);
        valueString[value->length] = '\0';
        status = PutMember(object, name, valueString, FALSE);
        free(valueString);
        return status;
    }
    else
    {
        return JSON_OBJECT_EXPECTED;
    }
}


JsonStatus PutStringMember(JsonNode* object, const char* name, const char* value)
{
    return PutMember(object, name, value, TRUE);
}


JsonStatus PutIntegerMember(JsonNode* object, const char* name, long value)
{
    char string[16];
    sprintf(string, "%d", value);
    return PutMember(object, name, string, FALSE);
}


JsonStatus PutRealMember(JsonNode* object, const char* name, double value)
{
    char string[32];
    sprintf(string, "%.15g", value);
    return PutMember(object, name, string, FALSE);
}


JsonStatus PutBooleanMember(JsonNode* object, const char* name, bool value)
{
    return PutMember(object, name, (value) ? JSON_TRUE_LITERAL : JSON_FALSE_LITERAL, FALSE);
}


JsonStatus PutNullMember(JsonNode* object, const char* name)
{
    return PutMember(object, name, JSON_NULL_LITERAL, FALSE);
}


JsonStatus ComposeArray(JsonNode* array)
{
    array->source = malloc(EMPTY_SOURCE_SIZE);
    if (array->source != NULL)
    {
        strcpy(array->source, EMPTY_ARRAY_SOURCE);
        array->type = JSON_ARRAY;
        array->offset = 0;
        array->length = EMPTY_SOURCE_SIZE - 1;
        return JSON_OK;
    }
    else
    {
        return JSON_OUT_OF_MEMORY;
    }
}


JsonStatus AddObjectElement(JsonNode* array, const JsonNode* element)
{
    return AddArrayElement(array, element->source + element->offset, element->length, FALSE);
}


JsonStatus AddStringElement(JsonNode* array, const char* element)
{
    return AddArrayElement(array, element, strlen(element), TRUE);
}


JsonStatus AddIntegerElement(JsonNode* array, const long value)
{
    char valueString[16];
    sprintf(valueString, "%d", value);
    return AddArrayElement(array, valueString, strlen(valueString), FALSE);
}


JsonStatus AddRealElement(JsonNode* array, const double value)
{
    char valueString[32];
    sprintf(valueString, "%.15g", value);
    return AddArrayElement(array, valueString, strlen(valueString), FALSE);
}


JsonStatus AddBooleanElement(JsonNode* array, const bool value)
{
    char* valueString = (value) ? JSON_TRUE_LITERAL : JSON_FALSE_LITERAL;
    return AddArrayElement(array, valueString, strlen(valueString), FALSE);
}


JsonStatus AddNullElement(JsonNode* array)
{
    return AddArrayElement(array, JSON_NULL_LITERAL, strlen(JSON_NULL_LITERAL), FALSE);
}
