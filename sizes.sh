#!/bin/sh

set -eu

cd lib/source
rm -f ecc*.o

for CPU in m0 m4 a5; do
    printf "$CPU: "
    arm-none-eabi-gcc -Werror -Wall -Wextra -pedantic --std=c99 \
        -Os -fomit-frame-pointer -mthumb -mcpu=cortex-$CPU \
        -I ../include ecc.c ecc_dh.c ecc_dsa.c -c
    arm-none-eabi-size -t ecc*.o | grep TOTALS | awk '{print $4}'
done
rm -f ecc*.o
