#include <stdio.h>
#include <stdlib.h>

#include "Simple.h"

#include "Text.h"


void AppendTest()
{
    StringBuffer buffer;
    TextStatus status;
    char* string;
    InitializeStringBuffer(&buffer);
    status = AppendString(&buffer, "Buffer");
    ASSERT_EQUAL_INT(TEXT_OK, status);
    EXPECT_EQUAL_INT(6, buffer.length);
    EXPECT_EQUAL_INT(64, buffer.capacity);
    ASSERT_EQUAL_STRING("Buffer", buffer.buffer);
    status = AppendString(&buffer, " test");
    ASSERT_EQUAL_INT(TEXT_OK, status);
    EXPECT_EQUAL_INT(11, buffer.length);
    EXPECT_EQUAL_INT(64, buffer.capacity);
    ASSERT_EQUAL_STRING("Buffer test", buffer.buffer);
    string = DetachString(&buffer);
    EXPECT_EQUAL_STRING("Buffer test", string);
    free(string);
}


void LongTextTest()
{
    StringBuffer buffer;
    TextStatus status;
    char* longString = malloc(65);
    memset(longString, '.', 64);
    longString[64] = '\0';
    InitializeStringBuffer(&buffer);
    status = AppendString(&buffer, longString);
    ASSERT_EQUAL_INT(TEXT_OK, status);
    ASSERT_EQUAL_INT(64, buffer.length);
    ASSERT_EQUAL_INT(64 * 2, buffer.capacity);
    status = AppendString(&buffer, longString);
    ASSERT_EQUAL_INT(TEXT_OK, status);
    EXPECT_EQUAL_INT(128, buffer.length);
    EXPECT_EQUAL_INT(64 * 3, buffer.capacity);
    free(buffer.buffer);
}


void AppendSubstringTest()
{
    StringBuffer buffer;
    TextStatus status;
    char* string;
    InitializeStringBuffer(&buffer);
    status = AppendSubstring(&buffer, "Buffer", 2, 2);
    ASSERT_EQUAL_INT(TEXT_OK, status);
    EXPECT_EQUAL_INT(2, buffer.length);
    string = DetachString(&buffer);
    EXPECT_EQUAL_STRING("ff", string);
    free(string);
}


void AppendCharacterTest()
{
    StringBuffer buffer;
    TextStatus status;
    char* string;
    InitializeStringBuffer(&buffer);
    status = AppendString(&buffer, "Buffer");
    ASSERT_EQUAL_INT(TEXT_OK, status);
    status = AppendCharacter(&buffer, '.');
    ASSERT_EQUAL_INT(TEXT_OK, status);
    EXPECT_EQUAL_INT(7, buffer.length);
    string = DetachString(&buffer);
    EXPECT_EQUAL_STRING("Buffer.", string);
    free(string);
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

