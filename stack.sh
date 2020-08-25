#!/bin/sh

set -eu

cd lib/source

for CPU in m0 m4 a7; do
    echo "*** $CPU ***"
    arm-none-eabi-gcc -DNO_PTR_CALL -fdump-rtl-dfinish -fstack-usage \
        -Os -fomit-frame-pointer -mthumb -mcpu=cortex-$CPU \
        -I ../include ecc.c ecc_dh.c ecc_dsa.c -c

    python3 wcs.py |
        egrep 'uECC_(sign|verify|make_key|shared_secret)\>' |
        sort
done

rm *.o *.su *.dfinish
