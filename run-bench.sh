#!/bin/bash

tag=$1
cxx=$2
bdir=release-$cxx
function fail() {
 echo "error: $@" >2
 exit 1
}

function run_test() {
 if [ ! -f build.ninja ]; then 
  cmake -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=$cxx  .. || fail "cmake"
 fi 
 cmdline="./utf_utils_test -dd ../test_data -al phoyd,kewb-fast,kewb-fast-unrolled,kewb-sse"
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
	run_test $@ | tee result.tmp
	grep '^>' result.tmp | sed 's/^>//g' >result.md
        cd ..
}

run
echo "** output in $bdir/result.md"


