#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "JsonComposer.h"
 

int main(int argc, char** argv)
{
    JsonNode object;
    ComposeObject(&object);
    PutIntegerMember(&object, "sizeof(char)", sizeof(char));
    PutIntegerMember(&object, "sizeof(short)", sizeof(short));
    PutIntegerMember(&object, "sizeof(int)", sizeof(int));
    PutIntegerMember(&object, "sizeof(size_t)", sizeof(size_t));
    PutIntegerMember(&object, "sizeof(long)", sizeof(long));
    PutIntegerMember(&object, "sizeof(float)", sizeof(float));
    PutIntegerMember(&object, "sizeof(double)", sizeof(double));
    PutIntegerMember(&object, "sizeof(void)", sizeof(void));
    PutIntegerMember(&object, "sizeof(void*)", sizeof(void*));
    PutIntegerMember(&object, "sizeof(JsonStatus)", sizeof(JsonStatus));
    PutIntegerMember(&object, "sizeof(JsonNode)", sizeof(JsonNode));
    printf("%s\r\n", object.source);
    free(object.source);
    return EXIT_SUCCESS;
}
