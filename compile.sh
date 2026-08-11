#!/bin/bash

OPTIONS=-O2
# OPTIONS=-g3

rm -rf *.o

g++ -o main.o           -c main.cpp           $OPTIONS -fPIC -Wall -Wextra || exit;
g++ -o PackageBuilder.o -c PackageBuilder.cpp $OPTIONS -fPIC -Wall -Wextra || exit;

echo "Success"

