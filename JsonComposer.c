#include "JsonComposer.h"

#include <stdio.h>
#include <string.h>

#include "Text.h"


char* EMPTY_OBJECT_SOURCE = "{}";


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
    if ((object != NULL) && (object->type == JSON_OBJECT))
    {
        size_t length = strlen(value);
        size_t i;
        StringBuffer* buffer = CreateStringBuffer();
        AppendSubstringBuffer(buffer, object->source, 0, strlen(object->source) - 1);
        if (object->source != EMPTY_OBJECT_SOURCE)
        {
            AppendStringBuffer(buffer, ",");
        }
        AppendStringBuffer(buffer, "\"");
        AppendStringBuffer(buffer, name);
        AppendStringBuffer(buffer, "\":\"");
        for (i = 0; i < length; ++i)
        {
            char character = value[i];
            if (IsControl(character))
            {
                switch (character)
                {
                    case '\b':
                        AppendStringBuffer(buffer, "\\b");
                        break;
                    case '\f':
                        AppendStringBuffer(buffer, "\\f");
                        break;
                    case '\r':
                        AppendStringBuffer(buffer, "\\r");
                        break;
                    case '\n':
                        AppendStringBuffer(buffer, "\\n");
                        break;
                    case '\t':
                        AppendStringBuffer(buffer, "\\t");
                        break;
                    default:
                    {
                        char u[7];
                        sprintf(u, "\\u%04x", character);
                        AppendStringBuffer(buffer, u);
                        break;
                    }
                }
            }
            else if (character == '\"')
            {
                AppendStringBuffer(buffer, "\\\"");
            }
            else
            {
                char c[2];
                c[0] = character;
                c[1] = '\0';
                AppendStringBuffer(buffer, c);
            }
        }
        AppendStringBuffer(buffer, "\"}");
        if (object->source != EMPTY_OBJECT_SOURCE)
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
