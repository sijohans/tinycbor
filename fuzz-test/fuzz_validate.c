#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "../src/cbor.h"


bool fuzz_one_input(const uint8_t *data, size_t size)
{
    CborParser parser;
    CborValue it;
    CborError err = cbor_parser_init(data, size, 0, &parser, &it);
    if (err == CborNoError) {
        err = cbor_value_validate(&it, CborValidateStrictest);
    }
    return (err == CborNoError);
}
