#!/bin/sh

set -eu

cd lib/source

arm-none-eabi-gcc -DNO_PTR_CALL -fdump-rtl-dfinish -fstack-usage \
    -Os -fomit-frame-pointer -mthumb -mcpu=cortex-m0 \
    -I ../include ecc.c ecc_dh.c ecc_dsa.c -c

python3 wcs.py | egrep 'uECC_(sign|verify|make_key|shared_secret)\>'

rm *.o *.su *.dfinish
