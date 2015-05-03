#include "JsonParser.h"

#include <string.h>
#include <ctype.h>


#define OBJECT_START '{'
#define OBJECT_END   '}'

#define ARRAY_START '['
#define ARRAY_END   ']'

#define STRING_START '\"'
#define STRING_END   '\"'

#define NAME_VALUE_SEPARATOR ':'
#define ELEMENT_SEPARATOR ','

#define ESCAPE_CHARACTER '\\'
#define UNICODE_CHARACTER 'u'


const char* WHITE_SPACES = " \f\n\r\t";
const char* ESCAPABLE_CHARACTERS = "\"\\/bfnrt";
const char* NUMBER_CHARACTERS = ".+-Ee";


void InitializeNode(JsonNode* node, const char* source)
{
    node->source = source;
    node->type = JSON_INVALID;
    node->offset = 0;
    node->length = 0;
}


bool IsWhiteSpace(char character)
{
    return strchr(WHITE_SPACES, character) != NULL;
}


bool IsControl(char character)
{
    return (character <= (char) 0x1F) || ((char) 0x7F <= character) && (character <= (char) 0x9F);
}


bool IsValue(const JsonNode* node)
{
    switch (node->type)
    {
        case JSON_OBJECT:
        case JSON_ARRAY:
        case JSON_STRING:
        case JSON_NUMBER:
            return TRUE;
        default:
            return FALSE;
    }
}


void ScanNext(const char* source, JsonNode* node);
//void ScanFirstPair(const JsonNode* objectNode, JsonNode* pairNode);
//void ScanNextPair(const JsonNode* objectNode, const JsonNode* offsetPairNode, JsonNode* nextPairNode);
void ScanPair(const char* source, size_t offset, JsonNode* pairNode);


void ScanObject(JsonNode* node)
{
    JsonNode next;
    node->length = 1;
    ScanNext(node->source + node->offset + node->length, &next);
    while (next.type == JSON_STRING)
    {
        ScanPair(next.source, 0, &next);
        if (next.type == JSON_PAIR)
        {
            node->length += next.offset + next.length;
            ScanNext(node->source + node->offset + node->length, &next);
            if (next.type == JSON_ELEMENT_SEPARATOR)
            {
                node->length += next.offset + next.length;
                ScanNext(node->source + node->offset + node->length, &next);            
            }
        }
        else
        {
            next.type = JSON_INVALID;
        }
    }
    if (next.type == JSON_OBJECT_END)
    {
        node->type = JSON_OBJECT;
        node->length += next.offset + next.length;
    }
}


void ScanArray(JsonNode* node)
{
    JsonNode next;
    node->length = 1;
    ScanNext(node->source + node->offset + node->length, &next);
    while (IsValue(&next))
    {
        node->length += next.offset + next.length;
        ScanNext(node->source + node->offset + node->length, &next);
        if (next.type == JSON_ELEMENT_SEPARATOR)
        {
            node->length += next.offset + next.length;
            ScanNext(node->source + node->offset + node->length, &next);            
        }
    }
    if (next.type == JSON_ARRAY_END)
    {
        node->type = JSON_ARRAY;
        node->length += next.offset + next.length;
    }
}


bool ScanUnicode(JsonNode* node)
{
    int i = 1;
    while (i <= 4)
    {
        if (! isxdigit(*(node->source + node->offset + node->length + i)))
        {
            return FALSE;
        }
        i++;
    }
    node->length += 4;
    return TRUE;
}


bool ScanEscape(JsonNode* node)
{
    char escapedCharacter = *(node->source + node->offset + node->length);
    if (strchr(ESCAPABLE_CHARACTERS, escapedCharacter) != NULL)
    {
        node->length++;
        return TRUE;
    }
    else if (escapedCharacter == UNICODE_CHARACTER)
    {
        return ScanUnicode(node);
    }
    else
    {
        return FALSE;
    }
}


void ScanString(JsonNode* node)
{
    bool scanning = TRUE;
    node->length = 1;
    while (scanning)
    {
        char character = *(node->source + node->offset + node->length);
        if (character != '\0')
        {
            node->length++;
            if (character == ESCAPE_CHARACTER)
            {
                scanning = ScanEscape(node);
            }
            else if (IsControl(character))
            {
                scanning = FALSE;
            }
            else if (character == STRING_END)
            {
                node->type = JSON_STRING;
                scanning = FALSE;
            }
        }
        else
        {
            scanning = FALSE;
        }
    }
}


bool ScanMinus(JsonNode* node)
{
    char character = *(node->source + node->offset + node->length);
    if (character == '-')
    {
        node->length++;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


bool ScanZero(JsonNode* node)
{
    char character = *(node->source + node->offset + node->length);
    if (character == '-')
    {
        node->length++;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


bool ScanPositiveDigit(JsonNode* node)
{
    char character = *(node->source + node->offset + node->length);
    if (('1' <= character) && (character <= '9'))
    {
        node->length++;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


bool ScanDigit(JsonNode* node)
{
    if (isdigit(*(node->source + node->offset + node->length)))
    {
        node->length++;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


bool ScanDigits(JsonNode* node)
{
    int count = 0;
    while (ScanDigit(node))
    {
        count++;
    }
    return count > 0;
}


bool ScanInteger(JsonNode* node)
{
    ScanMinus(node);
    if (ScanZero(node))
    {
        node->type = JSON_NUMBER;
    }
    else
    {
        if (ScanPositiveDigit(node))
        {
            node->type = JSON_NUMBER;
            ScanDigits(node);
        }    
    }
    return node->type == JSON_NUMBER;
}


void ScanFraction(JsonNode* node)
{
    
}


void ScanNumber(JsonNode* node)
{
    ScanInteger(node);
//    char character;
//    bool scanning = TRUE;
//    while (scanning)
//    {
//        character = *(node->source + node->offset + node->length);
//        if (isdigit(character) || (strchr(NUMBER_CHARACTERS, character) != NULL))
//        {
//            node->length++;
//        }
//        else
//        {
//            scanning = FALSE;
//        }
//    }
//    if (node->length > 0)
//    {
//        node->type = JSON_NUMBER;
//    }
}


void ScanNext(const char* source, JsonNode* node)
{
    char character;
    InitializeNode(node, source);
    while (IsWhiteSpace(*(source + node->offset)))
    {
        node->offset++;
    }
    character = *(source + node->offset);
    if (character == OBJECT_START)
    {
        ScanObject(node);
    }
    else if (character == ARRAY_START)
    {
        ScanArray(node);
    }
    else if (character == STRING_START)
    {
        ScanString(node);
    }
    else if ((character == '-') || isdigit(character))
    {
        ScanNumber(node);
    }
    else if (character == NAME_VALUE_SEPARATOR)
    {
        node->type = JSON_NAME_VALUE_SEPARATOR;
        node->length = 1;
    }
    else if (character == ELEMENT_SEPARATOR)
    {
        node->type = JSON_ELEMENT_SEPARATOR;
        node->length = 1;
    }
    else if (character == ARRAY_END)
    {
        node->type = JSON_ARRAY_END;
        node->length = 1;
    }
    else if (character == OBJECT_END)
    {
        node->type = JSON_OBJECT_END;
        node->length = 1;
    }
}


void ScanPair(const char* source, size_t offset, JsonNode* pairNode)
{
    JsonNode nameNode;
    ScanNext(source + offset, &nameNode);
    if (nameNode.type == JSON_STRING)
    {
        JsonNode separatorNode;
        ScanNext(nameNode.source + nameNode.offset + nameNode.length, &separatorNode);
        if (separatorNode.type == JSON_NAME_VALUE_SEPARATOR)
        {
            JsonNode valueNode;
            ScanNext(separatorNode.source + separatorNode.offset + separatorNode.length, &valueNode);
            if (IsValue(&valueNode))
            {
                pairNode->type = JSON_PAIR;
                pairNode->offset = nameNode.offset + offset;
                pairNode->length = nameNode.length + separatorNode.offset + separatorNode.length + valueNode.offset + valueNode.length;
            }
        }
    }
}


void ScanFirstPair(const JsonNode* objectNode, JsonNode* pairNode)
{
    InitializeNode(pairNode, objectNode->source);
    ScanPair(objectNode->source, 1, pairNode);
}


void ScanNextPair(/*const JsonNode* objectNode,*/ const JsonNode* offsetPairNode, JsonNode* nextPairNode)
{
    JsonNode elementSeparatorNode;
    InitializeNode(nextPairNode, offsetPairNode->source);
    ScanNext(offsetPairNode->source + offsetPairNode->offset + offsetPairNode->length, &elementSeparatorNode);
    if (elementSeparatorNode.type == JSON_ELEMENT_SEPARATOR)
    {
        ScanPair(offsetPairNode->source, offsetPairNode->offset + offsetPairNode->length + elementSeparatorNode.offset + elementSeparatorNode.length, nextPairNode);
    }
    else if (elementSeparatorNode.type == JSON_OBJECT_END)
    {
        *nextPairNode = elementSeparatorNode;
    }
}


void ScanFirstElement(const JsonNode* containerNode, JsonNode* elementNode)
{
    ScanNext(containerNode->source + 1, elementNode);
}


//void ScanNextElement(const JsonNode* containerNode, const JsonNode* offsetNode, JsonNode* nextNode)
//{
//    JsonNode elementSeparatorNode;
//    InitializeNode(nextNode, containerNode->source);
//    ScanNext(offsetNode->source + offsetNode->offset + offsetNode->length, &elementSeparatorNode);
//    if (elementSeparatorNode.type == JSON_ELEMENT_SEPARATOR)
//    {
//        ScanNext(containerNode->source + offsetNode->offset + offsetNode->length + elementSeparatorNode.offset + elementSeparatorNode.length, nextNode);
//    }    
//}


/* 
** Interface
*/

void Initialize(const char* source, JsonNode* node)
{
    ScanNext(source, node);
}


void ParseFirst(const JsonNode* parent, JsonNode* first)
{
    if (parent->type == JSON_OBJECT)
    {
        ScanFirstPair(parent, first);
    }
    else if (parent->type == JSON_ARRAY)
    {
        ScanFirstElement(parent, first);
    }
    else
    {
        InitializeNode(first, parent->source);
    }
}


void ParseNext(const JsonNode* sibling, JsonNode* next)
{
    if (sibling->type == JSON_PAIR)
    {
        ScanNextPair(sibling, next);
    }    
}


void GetValue(const JsonNode* pair, JsonNode* value)
{
    InitializeNode(value, pair->source);
    if ((pair != NULL) && (pair->type == JSON_PAIR))
    {
        JsonNode name;
        ScanNext(pair->source + pair->offset, &name);
        if (name.type == JSON_STRING)
        {
            JsonNode separator;
            ScanNext(name.source + name.offset + name.length, &separator);
            if (separator.type == JSON_NAME_VALUE_SEPARATOR)
            {
                ScanNext(separator.source + separator.offset + separator.length, value);
            }
        }
    }
}
