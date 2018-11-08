#!/bin/bash

tag=$1
prefix=$2

function run_test() {
 cmdline="${prefix}/utf_utils_test -dd test_data -al phoyd,kewb-fast,kewb-fast-unrolled,kewb-sse"
 echo "# $tag"
 echo "command line: $cmdline"
 echo 
 ninja -C ${prefix} >/dev/null 
 ${cmdline}
}

function run()
{
	shift
	run_test $@ | tee $tag.tmp
	grep '^>' $tag.tmp | sed 's/^>//g' >$tag.md
}

run
echo "** output in $tag.md"


