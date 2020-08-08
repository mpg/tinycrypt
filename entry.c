#include "tinycrypt/ecc_dh.h"
#include "tinycrypt/ecc_dsa.h"

/* dummy non-random version just to make the linker happy */
int default_CSPRNG(uint8_t *output, unsigned output_size)
{
    for (unsigned i = 0; i < output_size; i++) {
        output[i] = 0;
    }

    return 0;
}

int entry_func(void)
{
    uint8_t priv[32], pub[64], secret[32], sig[64], hash[32];

    uECC_Curve p256 = uECC_secp256r1();

    uECC_make_key(pub, priv, p256);
    uECC_shared_secret(pub, priv, secret, p256);
    uECC_sign(priv, hash, sizeof hash, sig, p256);
    uECC_verify(pub, hash, sizeof hash, sig, p256);

    return 0;
}
