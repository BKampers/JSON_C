#ifndef __JSONCOMPOSER_H__
#define	__JSONCOMPOSER_H__

#include "JsonParser.h"

JsonStatus ComposeObject(JsonNode* object);
JsonStatus PutStringMember(JsonNode* object, const char* name, const char* value);

#endif	/* __JSONCOMPOSER_H__ */

