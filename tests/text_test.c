#include <stdio.h>
#include <stdlib.h>

#include "Simple.h"

#include "Text.h"


void AppendTest()
{
    StringBuffer* buffer = CreateStringBuffer();
    char* string;
    AppendString(buffer, "Buffer");
    EXPECT_EQUAL_INT(6, buffer->length);
    EXPECT_EQUAL_INT(64, buffer->capacity);
    ASSERT_EQUAL_STRING("Buffer", buffer->buffer);
    AppendString(buffer, " test");
    EXPECT_EQUAL_INT(11, buffer->length);
    EXPECT_EQUAL_INT(64, buffer->capacity);
    ASSERT_EQUAL_STRING("Buffer test", buffer->buffer);
    string = DetachString(buffer);
    EXPECT_EQUAL_STRING("Buffer test", string);
    free(string);
    free(buffer);
}


void LongTextTest()
{
    StringBuffer* buffer = CreateStringBuffer();
    char* longString = malloc(65);
    memset(longString, '.', 64);
    longString[64] = '\0';
    AppendString(buffer, longString);
    ASSERT_EQUAL_INT(64, buffer->length);
    ASSERT_EQUAL_INT(64 * 2, buffer->capacity);
    AppendString(buffer, longString);
    EXPECT_EQUAL_INT(128, buffer->length);
    EXPECT_EQUAL_INT(64 * 3, buffer->capacity);
    free(buffer->buffer);
    free(buffer);
}


void AppendSubstringTest()
{
    StringBuffer* buffer = CreateStringBuffer();
    char* string;
    AppendSubstring(buffer, "Buffer", 2, 2);
    EXPECT_EQUAL_INT(2, buffer->length);
    string = DetachString(buffer);
    EXPECT_EQUAL_STRING("ff", string);
    free(string);
    free(buffer);        
}


void AppendCharacterTest()
{
    StringBuffer* buffer = CreateStringBuffer();
    char* string;
    AppendString(buffer, "Buffer");
    AppendCharacter(buffer, '.');
    EXPECT_EQUAL_INT(7, buffer->length);
    string = DetachString(buffer);
    EXPECT_EQUAL_STRING("Buffer.", string);
    free(string);
    free(buffer);    
}


int main(int argc, char** argv)
{
    startSuite("Text test");
    
    start("AppendTest");
    AppendTest();
    finish();
    
    start("LongTextTest");
    LongTextTest();
    finish();
    
    start("AppendCharacterTest");
    AppendCharacterTest();
    finish();
    
    start("AppendSubstringTest");
    AppendSubstringTest();
    finish();

    finishSuite();
}

