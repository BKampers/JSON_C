#ifndef __TEXT_H__
#define	__TEXT_H__

typedef struct 
{
    char* buffer;
    size_t capacity;
    size_t length;
} StringBuffer;


StringBuffer* CreateStringBuffer();
void AppendString(StringBuffer* stringBuffer, const char* string);
void AppendSubstring(StringBuffer* stringBuffer, const char* string, size_t offset, size_t count);
void AppendCharacter(StringBuffer* stringBuffer, char character);
char* DetachString(StringBuffer* stringBuffer);


#endif	/* __TEXT_H__ */

