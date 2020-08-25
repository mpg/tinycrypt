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

#define SUCCESS     1
#define TIMES       100
#define TIMEIT(N, CODE)                                             \
do {                                                                \
    if (CODE != SUCCESS)                                            \
        printf("%s failed\n", names[N]);                            \
    const uint64_t start = usec();                                  \
    for (unsigned i = 0; i < TIMES; i++) {                          \
        CODE;                                                       \
    }                                                               \
    results[N][i] = (usec() - start) / TIMES;                       \
} while (0)

#define RUNS 5

int cmp_u64(const void *a, const void *b) {
    uint64_t x = *((uint64_t *) a);
    uint64_t y = *((uint64_t *) b);
    if (x < y)
        return -1;
    if (x > y)
        return 1;
    return 0;
}

int main(void)
{
    uint8_t priv[32], pub[64], secret[32], sig[64], hash[32];
    uint64_t results[4][RUNS], total = 0;
    const char * names[4] = {"Keygen", "ECDH", "Sign", "Verify"};

    uECC_Curve p256 = uECC_secp256r1();
    uECC_set_rng(&test_prng);

    for (unsigned i = 0; i < RUNS; i++) {
        TIMEIT(0, uECC_make_key(pub, priv, p256));
        TIMEIT(1, uECC_shared_secret(pub, priv, secret, p256));
        TIMEIT(2, uECC_sign(priv, hash, sizeof hash, sig, p256));
        TIMEIT(3, uECC_verify(pub, hash, sizeof hash, sig, p256));
    }

    for (unsigned n = 0; n < 4; n++) {
        qsort(results[n], RUNS, sizeof results[n][0], cmp_u64);
        uint64_t median = results[n][RUNS / 2];
        printf("%s: %"PRIu64" us\n", names[n], median);
        total += median;
    }
    printf("%s: %"PRIu64" us\n", "Total", total);

    return 0;
}
