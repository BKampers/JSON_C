#ifndef __JSONPARSER_H__
#define	__JSONPARSER_H__

#include <stdlib.h>

#include "Types.h"


typedef enum
{
    JSON_OK,
    JSON_OBJECT_EXPECTED,
    JSON_NAME_NOT_PRESENT,
    JSON_TYPE_MISMATCH,
} JsonStatus;


typedef enum
{ 
    JSON_INVALID,
    JSON_PAIR,
    JSON_NAME,
    JSON_VALUE,
    JSON_STRING,
    JSON_NUMBER,
    JSON_OBJECT,
    JSON_ARRAY,
    JSON_BOOLEAN,
    JSON_NULL,
    JSON_ELEMENT_SEPARATOR,
    JSON_NAME_VALUE_SEPARATOR,
    JSON_ARRAY_END,
    JSON_OBJECT_END
} JsonType;


typedef struct 
{
    const char* source;
    JsonType type;
    size_t offset;
    size_t length;
} JsonNode;


void Initialize(const char* source, JsonNode* node);

void ParseFirst(const JsonNode* parent, JsonNode* first);
void ParseNext(const JsonNode* sibling, JsonNode* next);

void GetValue(const JsonNode* pair, JsonNode* value);

JsonStatus AllocateString(JsonNode* object, const char* name, char** value);
JsonStatus GetInt(JsonNode* object, const char* name, int* value);
JsonStatus GetDouble(JsonNode* object, const char* name, double* value);

#endif	/* JSONPARSER_H */

