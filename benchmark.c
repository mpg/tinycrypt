#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <inttypes.h>

#include "tinycrypt/ecc_dh.h"
#include "tinycrypt/ecc_dsa.h"

/* test version based on stdlib - never do this in production! */
int test_prng(uint8_t *output, unsigned output_size)
{
    for (unsigned i = 0; i < output_size; i++) {
        output[i] = (uint8_t) rand();
    }

    return 1;
}

static uint64_t usec(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (uint64_t) tv.tv_sec * 1000000 + (uint64_t) tv.tv_usec;
}

#define TIMES   100

#define TIMEIT(NAME, CODE)                                          \
do {                                                                \
    const uint64_t start = usec();                                  \
    for (unsigned i = 0; i < TIMES; i++) {                          \
        CODE;                                                       \
    }                                                               \
    const uint64_t ellapsed = usec() - start;                       \
    printf("%s: %4"PRIu64" us\n", NAME, ellapsed / TIMES);          \
    if( ret != 1)                                                   \
        printf("FAILED\n");                                         \
} while (0)

int main(void)
{
    int ret;
    uint8_t priv[32], pub[64], secret[32], sig[64], hash[32];

    uECC_Curve p256 = uECC_secp256r1();
    uECC_set_rng(&test_prng);

    TIMEIT("Keygen", ret = uECC_make_key(pub, priv, p256));
    TIMEIT("ECDH", ret = uECC_shared_secret(pub, priv, secret, p256));
    TIMEIT("Sign", ret = uECC_sign(priv, hash, sizeof hash, sig, p256));
    TIMEIT("Verify", ret = uECC_verify(pub, hash, sizeof hash, sig, p256));

    return 0;
}
