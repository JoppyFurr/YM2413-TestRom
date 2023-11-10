#!/bin/sh

CC=gcc
CFLAGS="-std=c11 -O2 -Wall -Werror -I libraries/libspng-0.7.4"


$CC $CFLAGS libraries/libspng-0.7.4/spng.c source/main.c -lm -lz -o Sneptile
