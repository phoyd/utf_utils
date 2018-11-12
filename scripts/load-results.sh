#!/bin/bash

db=$1 
data=$2

sqlite3 $db <<EOF
create table if not exists results(
	project text,
	function text,
	algo text,
	time double,
	src_size int,
	dst_size int,
	dataset text,
	arg text, 
	tag text,
	compiler text,
	gitcommit text); 

.mode csv
.separator ';'
.import $data results
EOF

