/*
 * On-target benchmark program for TinyCrypt ECC using Mbed OS.
 */
#include "mbed.h"

#include "tinycrypt/ecc_dh.h"
#include "tinycrypt/ecc_dsa.h"

Timer t;
int total_ms = 0;

#define FMT "%10s: %5d ms\n"

#define TIMEIT(NAME, CODE)          \
    t.reset();                      \
    t.start();                      \
    CODE;                           \
    t.stop();                       \
    total_ms += t.read_ms();        \
    printf(FMT, NAME, t.read_ms());

int main()
{
    uint8_t priv[32], pub[64], secret[32], sig[64], hash[32];

    uECC_Curve p256 = uECC_secp256r1();
    //uECC_set_rng(&default_CSPRNG);

    puts("\nTinyCrypt ECC benchmark");
    TIMEIT("Keygen", uECC_make_key(pub, priv, p256));
    TIMEIT("ECDH", uECC_shared_secret(pub, priv, secret, p256));
    TIMEIT("Sign", uECC_sign(priv, hash, sizeof hash, sig, p256));
    TIMEIT("Verify", uECC_verify(pub, hash, sizeof hash, sig, p256));

    /* The total is useful for quick comparisons.
     *
     * It also happens to represent the computation time for a mutually
     * authenticated TLS handshake with directly-trusted certs or raw public
     * keys (with actual cert chains there are extra signature verifications).
     */
    printf(FMT, "Total", total_ms);
}
