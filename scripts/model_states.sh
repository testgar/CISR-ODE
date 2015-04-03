#!/bin/bash

me=`basename $0`
in_file=$1
out_file=$2

if [ ! "$1" ]
then
	echo "no input file is received ($me)"
	exit 1
fi

if [ ! "$2" ]
then
	echo "no output file is received ($me)"
	exit 1
fi

if [ ! -f $in_file ]
then
	echo "error: $in_file not found. ($me)"
	exit 1234	
fi

echo "">$out_file
./scripts/autogen_code_header.sh  >>$out_file
echo "// path: $out_file">>$out_file
echo "">>$out_file
echo "#pragma once">>$out_file
echo "">>$out_file
echo "namespace states">>$out_file
echo "{">>$out_file

vector=($(./scripts/read_column.sh 0 $in_file))
for index in ${!vector[*]}
do
	echo -e "\t// x$index\t${vector[index]//\"/}">>$out_file
done
echo "">>$out_file
for index in ${!vector[*]}
do
	echo -e "\tconst int ${vector[index]//\"/}=$index;">>$out_file
done

echo "">>$out_file
echo -e "\t// total size of states vector">>$out_file
echo -e "\tconst int state_size=${#vector[@]};">>$out_file
echo "}">>$out_file

echo "$out_file updated"
