#include <stdlib.h>
#include <string.h>

#include "Text.h"


#define STRING_BUFFER_INCREMENT 64


StringBuffer* CreateStringBuffer()
{
    StringBuffer* stringBuffer = malloc(sizeof(StringBuffer));
    stringBuffer->buffer = NULL;
    stringBuffer->capacity = 0;
    stringBuffer->length = 0;
    return stringBuffer;
}


void AppendString(StringBuffer* stringBuffer, const char* string)
{
    AppendSubstring(stringBuffer, string, 0, strlen(string));
}


void AppendSubstring(StringBuffer* stringBuffer, const char* string, size_t offset, size_t count)
{
    stringBuffer->length += count;
    if (stringBuffer->capacity < stringBuffer->length)
    {
        stringBuffer->capacity = (stringBuffer->length / STRING_BUFFER_INCREMENT + 1) * STRING_BUFFER_INCREMENT;
        if (stringBuffer->buffer == NULL)
        {
            stringBuffer->buffer = malloc(stringBuffer->capacity * sizeof(char));
            stringBuffer->buffer[0] = '\0';
        }
        else
        {
            stringBuffer->buffer = realloc(stringBuffer->buffer, stringBuffer->capacity * sizeof(char));
        }
    }
    strncat(stringBuffer->buffer, string + offset, count);    
}


void AppendCharacter(StringBuffer* stringBuffer, char character)
{
    AppendSubstring(stringBuffer, &character, 0 , 1);
}


char* DetachString(StringBuffer* stringBuffer)
{
    if (stringBuffer->buffer != NULL)
    {
        char* string = malloc(stringBuffer->length + 1);
        strcpy(string, stringBuffer->buffer);
        free(stringBuffer->buffer);
        free(stringBuffer);
        return string;
    }
    else
    {
        return NULL;
    }
}
