#!/bin/bash

OPTIONS=-O2
# OPTIONS=-g3

rm -rf *.o

g++ -o main.o               -c main.cpp                $OPTIONS -fPIC -Wall -Wextra || exit;
echo "main.o"
g++ -o Package.o            -c Package.cpp             $OPTIONS -fPIC -Wall -Wextra -Wno-write-strings -Wno-unused-parameter || exit;
echo "Package.o"
g++ -o utils_mfc.o          -c utils_mfc.cpp           $OPTIONS -fPIC -Wall -Wextra -I. || exit;
echo "utils_mfc.o"
g++ -o PackagerFilter.o     -c PackagerFilter.cpp      $OPTIONS -fPIC -Wall -Wextra -Wno-unused-parameter -Wno-unused-but-set-parameter -Wno-write-strings -Wno-unused-variable || exit;
echo "PackagerFilter.o"
g++ -o FilterExclude.o      -c FilterExclude.cpp       $OPTIONS -fPIC -Wall -Wextra -Wno-unused-parameter -Wno-write-strings || exit;
echo "FilterExclude.o"
g++ -o FilterUncompressed.o -c FilterUncompressed.cpp  $OPTIONS -fPIC -Wall -Wextra -Wno-unused-parameter -Wno-write-strings || exit;
echo "FilterUncompressed.o"
g++ -o PlatformSDL.o        -c PlatformSDL.cpp         $OPTIONS -fPIC -Wall -Wextra -I. -Wno-unknown-pragmas -Wno-unused-parameter || exit;
echo "PlatformSDL.o"
g++ -o FilterScript.o       -c FilterScript.cpp        $OPTIONS -fPIC -Wall -Wextra -I. -Wno-unknown-pragmas -Wno-write-strings -Wno-unused-parameter || exit;
echo "FilterScript.o"
g++ -o FilterCopy.o         -c FilterCopy.cpp          $OPTIONS -fPIC -Wall -Wextra -Wno-unused-parameter -Wno-write-strings || exit;
echo "FilterCopy.o"
g++ -o PackageBuilder.o     -c PackageBuilder.cpp      $OPTIONS -fPIC -Wall -Wextra -I. -Wno-unused-parameter -Wno-unknown-pragmas -Wno-write-strings || exit;
echo "PackageBuilder.o"

echo "Success"

