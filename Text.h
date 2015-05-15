#ifndef __TEXT_H__
#define	__TEXT_H__

typedef struct 
{
    char* buffer;
    size_t capacity;
    size_t length;
} StringBuffer;


StringBuffer* CreateStringBuffer();
void AppendStringBuffer(StringBuffer* stringBuffer, const char* string);
void AppendSubstringBuffer(StringBuffer* stringBuffer, const char* string, size_t offset, size_t count);
char* DetachString(StringBuffer* stringBuffer);


#endif	/* __TEXT_H__ */

