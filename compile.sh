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
g++ -o ScEngine.o           -c engine_core/wme_base/ScEngine.cpp            $OPTIONS -fPIC -Wall -Wextra -I. -Iexternal_lib/compiler -Wno-unknown-pragmas -Wno-write-strings -Wno-unused-parameter || exit;
echo "PackageBuilder.o"
g++ -o BBase.o              -c engine_core/wme_base/BBase.cpp               $OPTIONS -fPIC -Wall -Wextra -I. -Wno-unknown-pragmas -Wno-write-strings -Wno-unused-parameter -Wno-unused-variable || exit;
echo "BBase.o"
g++ -o BGame.o              -c engine_core/wme_base/BGame.cpp               $OPTIONS -fPIC -Wall -Wextra -I. -Wno-unknown-pragmas -Wno-write-strings -Wno-unused-parameter -Wno-unused-variable || exit;
echo "BGame.o"
g++ -o ScValue.o            -c engine_core/wme_base/ScValue.cpp             $OPTIONS -fPIC -Wall -Wextra -I. -Wno-unknown-pragmas -Wno-write-strings -Wno-unused-parameter -Wno-unused-variable || exit;
echo "ScValue.o"
g++ -o ScScript.o           -c engine_core/wme_base/ScScript.cpp            $OPTIONS -fPIC -Wall -Wextra -I. -Wno-unknown-pragmas -Wno-write-strings -Wno-unused-parameter -Wno-unused-variable || exit;
echo "ScScript.o"
g++ -o ScStack.o            -c engine_core/wme_base/ScStack.cpp             $OPTIONS -fPIC -Wall -Wextra -I. -Wno-unknown-pragmas -Wno-write-strings -Wno-unused-parameter -Wno-unused-variable || exit;
echo "ScStack.o"
g++ -o BObject.o            -c engine_core/wme_base/BObject.cpp             $OPTIONS -fPIC -Wall -Wextra -I. -Wno-unknown-pragmas -Wno-write-strings -Wno-unused-parameter -Wno-unused-variable || exit;
echo "BObject.o"
g++ -o BScriptable.o        -c engine_core/wme_base/BScriptable.cpp         $OPTIONS -fPIC -Wall -Wextra -I. -Wno-unknown-pragmas -Wno-write-strings -Wno-unused-parameter -Wno-unused-variable || exit;
echo "BScriptable.o"
g++ -o BScriptHolder.o      -c engine_core/wme_base/BScriptHolder.cpp       $OPTIONS -fPIC -Wall -Wextra -I. -Wno-unknown-pragmas -Wno-write-strings -Wno-unused-parameter -Wno-unused-variable || exit;
echo "BScriptHolder.o"
gcc -o DirectoryOperations.o -c DirectoryOperations.c $OPTIONS -fPIC -Wall -Wextra || exit;
echo "DirectoryOperations.o"
gcc -o FileOperations.o      -c FileOperations.c      $OPTIONS -fPIC -Wall -Wextra || exit;
echo "FileOperations.o"
gcc -o ConvertUTF.o          -c ConvertUTF.c          $OPTIONS -fPIC -Wall -Wextra || exit;
echo "ConvertUTF.o"
g++ -o PathUtil.o         -c PathUtil.cpp         $OPTIONS -fPIC -Wall -Wextra -I. -Iengine_core/wme_base/ -Wno-unknown-pragmas -Wno-write-strings -Wno-unused-parameter -Wno-unused-variable || exit;
echo "PathUtil.o"
g++ -o StringUtil.o         -c StringUtil.cpp         $OPTIONS -fPIC -Wall -Wextra -I. -Iengine_core/wme_base/ -Wno-unknown-pragmas -Wno-write-strings -Wno-unused-parameter -Wno-unused-variable || exit;
echo "StringUtil.o"
g++ -o BFileManager.o       -c engine_core/wme_base/BFileManager.cpp        $OPTIONS -fPIC -Wall -Wextra -I. -Wno-unknown-pragmas -Wno-write-strings -Wno-unused-parameter -Wno-unused-variable || exit;
echo "BFileManager.o"
g++ -o utils.o              -c engine_core/wme_base/utils.cpp             $OPTIONS -fPIC -Wall -Wextra -I. -Wno-unknown-pragmas -Wno-write-strings -Wno-unused-parameter -Wno-unused-variable || exit;
echo "utils.o"

g++ -o packagebuilder -lz -ldcscomp -Lexternal_lib/compiler \
main.o Package.o utils_mfc.o  PackagerFilter.o   FilterExclude.o FilterUncompressed.o PlatformSDL.o FilterScript.o FilterCopy.o PackageBuilder.o  ScEngine.o BBase.o BGame.o \
ScValue.o  ScScript.o    ScStack.o BObject.o  BScriptable.o   BScriptHolder.o FileOperations.o DirectoryOperations.o BFileManager.o StringUtil.o ConvertUTF.o PathUtil.o utils.o


echo "Success"

