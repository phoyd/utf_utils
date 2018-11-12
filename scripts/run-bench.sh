#!/bin/bash

export tag=$1
export cxx=$2
export rev=$(git rev-parse HEAD)
bdir=release-$cxx
function fail() {
 echo "error: $@" >2
 exit 1
}

function run_test() {
 if [ ! -f build.ninja ]; then 
  cmake -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=$cxx  .. || fail "cmake"
 fi 
 cmdline="./utf_utils_test -dd ../test_data -rx 29 -al phoyd,kewb-fast,kewb-fast-unrolled,kewb-sse"
# cmdline="./utf_utils_test -dd ../test_data -rx 31 -al phoyd,kewb-fast,kewb-fast-unrolled,kewb-sse"
 echo "# $tag"
 echo "command line: $cmdline"
 echo 
  
 ninja || fail "ninja!"
 ${cmdline} || fail "command"
}

function run()
{
	[ -d  "$bdir" ]  || mkdir -p "$bdir"
        cd $bdir
	run_test $@  | grep --line-buffered '^@' | while read l
	do 
		echo "$l;-rx 31;$tag;$cxx;$rev"
        done
        cd ..
}

run


