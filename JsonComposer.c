#include "JsonComposer.h"

#include <stdio.h>
#include <string.h>

#include "Text.h"

char JSON_NULL_LITERAL[] = "null";
char JSON_FALSE_LITERAL[] = "false";
char JSON_TRUE_LITERAL[] = "true";


char* EMPTY_OBJECT_SOURCE = "{}";


void AppendQuoted(StringBuffer* buffer, const char* string)
{
    size_t i;
    size_t length = strlen(string);
    AppendCharacter(buffer, '\"');
    for (i = 0; i < length; ++i)
    {
        char character = string[i];
        if (IsControl(character))
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
        StringBuffer* buffer = CreateStringBuffer();
        bool empty = object->source == EMPTY_OBJECT_SOURCE;
        AppendSubstring(buffer, object->source, 0, strlen(object->source) - 1);
        if (! empty)
        {
            AppendCharacter(buffer, ',');
        }
        AppendQuoted(buffer,  name);
        AppendCharacter(buffer, ':');
        if (quoted)
        {
            AppendQuoted(buffer,  value);
        }
        else
        {
            AppendString(buffer, value);
        }
        AppendCharacter(buffer, '}');
        if (! empty)
        {
            free(object->source);
        }
        object->source = DetachString(buffer);
        return JSON_OK;
    }
    else
    {
        return JSON_OBJECT_EXPECTED;
    }
}


/*
** Interface
*/

JsonStatus ComposeObject(JsonNode* object)
{
    JsonStatus status = JSON_OK;
    object->source = EMPTY_OBJECT_SOURCE;
    object->type = JSON_OBJECT;
    object->offset = 0;
    object->length = 2;
    return status;
}


JsonStatus PutStringMember(JsonNode* object, const char* name, const char* value)
{
    return PutMember(object, name, value, TRUE);
}


JsonStatus PutIntegerMember(JsonNode* object, const char* name, long value)
{
    char string[16];
    sprintf(string, "%d", value);
    PutMember(object, name, string, FALSE);
}


JsonStatus PutRealMember(JsonNode* object, const char* name, double value)
{
    char string[32];
    sprintf(string, "%f", value);
    PutMember(object, name, string, FALSE);
}


JsonStatus PutBooleanMember(JsonNode* object, const char* name, bool value)
{
    PutMember(object, name, (value) ? JSON_TRUE_LITERAL : JSON_FALSE_LITERAL, FALSE);
}


JsonStatus PutNullMember(JsonNode* object, const char* name)
{
    PutMember(object, name, JSON_NULL_LITERAL, FALSE);
}
