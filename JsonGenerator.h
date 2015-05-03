#ifndef _JSONGENERATOR_H_
#define	_JSONGENERATOR_H_

#include <stdlib.h>

#include "JsonParser.h"


typedef struct
{
    JsonType type;
    void* data;
} JsonValue;


typedef struct
{
    char* name;
    JsonValue value;
} JsonPair;


typedef struct
{
    JsonPair* elements;
    size_t capacity;
    size_t elementCount;
} JsonObject;


typedef struct
{
    JsonValue* elements;
    size_t capacity;
    size_t elementCount;
} JsonArray;


JsonObject* CreateJsonObject();
void ClearJsonObject(JsonObject* object);

void PutObject(JsonObject* object, const char* name, JsonObject* value);
void PutArray(JsonObject* object, const char* name, JsonArray* value);
void PutBoolean(JsonObject* object, const char* name, int value);
void PutInteger(JsonObject* object, const char* name, int value);
void PutReal(JsonObject* object, const char* name, double value);
void PutString(JsonObject* object, const char* name, char* value);
void PutNull(JsonObject* object, const char* name);


JsonArray* CreateJsonArray();
void ClearJsonArray(JsonArray* array);

void AddObject(JsonArray* array, JsonObject* element);
void AddArray(JsonArray* array, JsonArray* element);
void AddBoolean(JsonArray* array, int element);
void AddInteger(JsonArray* array, int element);
void AddReal(JsonArray* array, double element);
void AddString(JsonArray* array, char* element);
void AddNull(JsonArray* array);


char* ToString(const JsonObject* object);

#endif	/* _JSONGENERATOR_H_ */

