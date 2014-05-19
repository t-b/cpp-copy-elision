#!/bin/bash

function doit
{
  if [ -e "$1/g++" ]
  then
    CXX=$1/g++
  elif [ -e "$1/clang++" ]
  then
    CXX=$1/clang++
  elif [ -e "$1" ]
  then
    CXX=$1
  fi

  CFLAGS="$2 -O3"

  $CXX --version | head -n1
  $CXX $CFLAGS cpp-copy-elision-NRVO-URVO.cpp && ./a.out 1>/dev/null | xargs echo -n
  echo ""
}

export LIBRARY_PATH=/usr/lib/x86_64-linux-gnu

path=~/devel/compiler-builds/inst

for i in gcc-3.4.6 gcc-4.0.4 gcc-4.1.2 gcc-4.2.4
do
  doit "$path/$i/usr/local/bin"
done
 
for i in gcc-4.3.6 gcc-4.4.7 gcc-4.5.4 gcc-4.6.6
do
  doit "$path/$i/usr/local/bin" "-std=c++0x -DALLOW_MOVE"
done

for i in gcc-4.7.3 gcc-4.8.2 gcc-4.9.0 gcc-trunk llvm-3.1 llvm-3.2 llvm-3.3 llvm-3.4 llvm-trunk
do
  doit "$path/$i/usr/local/bin" "-std=c++11 -DALLOW_MOVE"
done

doit "/opt/intel/bin/icc" "-std=c++11 -DALLOW_MOVE"
