#include <stdio.h>
#include <stdlib.h>

#include "JsonParser.h" 


/*
** JSON Parser Test Suite
*/


void validIntegerTest() {
    JsonNode object, pair, value;
    printf("JSON validIntegerTest\n");
    Initialize("{\"int\":123}", &object);
    ParseFirst(&object, &pair);
    GetValue(&pair, &value);
    if ((value.type == JSON_NUMBER) && (strncmp(value.source + value.offset, "123", value.length) == 0))
    {
        printf("%%TEST_PASSED%% time=0 testname=validIntegerTest (JSON Parser Test) message=OK\n");
    }
    else
    {
        printf("%%TEST_FAILED%% time=0 testname=validIntegerTest (JSON Parser Test) message=Error\n");
    }
}


void invalidIntegerTest() {
    JsonNode object, pair, value;
    printf("JSON invalidIntegerTest\n");
    Initialize("{\"int\":0123}", &object);
    if (object.type == JSON_INVALID)
    {
        printf("%%TEST_PASSED%% time=0 testname=invalidIntegerTest (JSON Parser Test) message=Object OK\n");
    }
    else
    {
        printf("%%TEST_FAILED%% time=0 testname=invalidIntegerTest (JSON Parser Test) message=Object error\n");
    }
    ParseFirst(&object, &pair);
    if (pair.type == JSON_INVALID)
    {
        printf("%%TEST_PASSED%% time=0 testname=invalidIntegerTest (JSON Parser Test) message=Pair OK\n");
    }
    else
    {
        printf("%%TEST_FAILED%% time=0 testname=invalidIntegerTest (JSON Parser Test) message=Pair error\n");
    }
    GetValue(&pair, &value);
    if (value.type == JSON_INVALID)
    {
        printf("%%TEST_PASSED%% time=0 testname=invalidIntegerTest (JSON Parser Test) message=Value OK\n");
    }
    else
    {
        printf("%%TEST_FAILED%% time=0 testname=invalidIntegerTest (JSON Parser Test) message=Value error\n");
    }
}


int main(int argc, char** argv) {
    printf("%%SUITE_STARTING%% JSON Parser Test\n");
    printf("%%SUITE_STARTED%%\n");

    printf("%%TEST_STARTED%% validIntegerTest (JSON Parser Test)\n");
    validIntegerTest();
    printf("%%TEST_FINISHED%% time=0 validIntegerTest (JSON Parser Test) \n");

    printf("%%TEST_STARTED%% invalidIntegerTest (JSON Parser Test)\n");
    invalidIntegerTest();
    printf("%%TEST_FINISHED%% time=0 invalidIntegerTest (JSON Parser Test) \n");

    printf("%%SUITE_FINISHED%% time=0\n");

    return (EXIT_SUCCESS);
}
