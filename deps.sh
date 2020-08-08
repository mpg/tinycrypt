#!/bin/sh

# print dependencies on libc / compiler's runtime, with sizes

set -eu

TC_SYM_RE='uECC_|EccPoint_|XYcZ_|apply_z|bits2int|cond_set|curve_|double_|default_|entry_func|g_rng_|regularize_k|vli_|x_side_'

for CPU in m0 m4 a5; do
    printf "\n*** %s ***\n" $CPU
    arm-none-eabi-gcc -Os -mthumb -mcpu=cortex-$CPU entry.c \
        -I lib/include lib/source/ecc_platform_specific.c \
        lib/source/ecc.c lib/source/ecc_dh.c lib/source/ecc_dsa.c \
        --entry=entry_func -nostartfiles -o linked.elf
    nm --print-size --radix=d linked.elf |
        awk "/^[0-9]{8} [0-9]{8} . / && !/ . $TC_SYM_RE/ \
            "'{print $2, $4; tot += $2} END {print "total: " tot}'
done

rm linked.elf
