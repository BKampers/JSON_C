#ifndef __JSONCOMPOSER_H__
#define	__JSONCOMPOSER_H__

#include "JsonParser.h"

JsonStatus ComposeObject(JsonNode* object);
JsonStatus PutStringMember(JsonNode* object, const char* name, const char* value);
JsonStatus PutIntegerMember(JsonNode* object, const char* name, long value);
JsonStatus PutRealMember(JsonNode* object, const char* name, double value);
JsonStatus PutBooleanMember(JsonNode* object, const char* name, bool value);
JsonStatus PutNullMember(JsonNode* object, const char* name);

#endif	/* __JSONCOMPOSER_H__ */

