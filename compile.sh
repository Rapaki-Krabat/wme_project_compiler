#!/bin/bash

OPTIONS=-O2
# OPTIONS=-g3

rm -rf *.o

g++ -o main.o           -c main.cpp           $OPTIONS -fPIC -Wall -Wextra || exit;
g++ -o Package.o        -c Package.cpp        $OPTIONS -fPIC -Wall -Wextra -Wno-write-strings -Wno-unused-parameter || exit;
g++ -o utils_mfc.o      -c utils_mfc.cpp      $OPTIONS -fPIC -Wall -Wextra -I. || exit;
g++ -o PackagerFilter.o -c PackagerFilter.cpp $OPTIONS -fPIC -Wall -Wextra -Wno-unused-parameter || exit;
g++ -o PackageBuilder.o -c PackageBuilder.cpp $OPTIONS -fPIC -Wall -Wextra -I. || exit;

echo "Success"

