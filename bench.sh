#!/bin/sh

set -eu

gcc --std=c99 -Werror -Wall -Wextra -pedantic \
    -Os -I lib/include lib/source/ecc_platform_specific.c \
    lib/source/ecc.c lib/source/ecc_dh.c lib/source/ecc_dsa.c \
    benchmark.c -o benchmark

./benchmark

rm benchmark
