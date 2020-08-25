#include <stdlib.h>
#include <stdint.h>

/* test version based on stdlib - never do this in production! */
int default_CSPRNG(uint8_t *output, unsigned output_size)
{
    for (unsigned i = 0; i < output_size; i++) {
        output[i] = (uint8_t) rand();
    }

    return 1;
}

