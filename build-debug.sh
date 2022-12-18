#!/bin/sh

./configure CPPFLAGS=-DDEBUG CFLAGS="-g -O0" && make
