#!/bin/bash

#name,plot
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
echo "namespace outputs">>$out_file
echo "{">>$out_file
echo "	const std::string output_header[]={">>$out_file

vector=($(./scripts/read_column.sh 0 $in_file))
for index in ${!vector[*]}
do
	echo -e "\t\t"'"'${vector[index]//\"/}'",'>>$out_file
done

echo "	};">>$out_file
echo "">>$out_file

for index in ${!vector[*]}
do
	echo -e "\t// o$index\t${vector[index]//\"/}">>$out_file
done
echo "">>$out_file
for index in ${!vector[*]}
do
	echo -e "\tconst int ${vector[index]//\"/}=$index;">>$out_file
done

echo "">>$out_file
echo -e "\t// total size of output vector">>$out_file
echo -e "\tconst int output_size=${#vector[@]};">>$out_file

echo "">>$out_file
echo -e "\t//Export list">>$out_file
echo -e "\tstruct ExpList">>$out_file
echo -e "\t{">>$out_file
echo -e "\t\tstd::string file_suffix; // no space is accepted in file suffix name">>$out_file
echo -e "\t\tint output_index;">>$out_file
echo -e "\t} figure_list[]={">>$out_file

plot=($(./scripts/read_column.sh 1 $in_file))
for index in ${!plot[*]}
do
	if [ "${plot[index]//\"/}" == "plot" ]
	then
		echo -e "\t\t{\"${vector[index]//\"/}\",outputs::${vector[index]//\"/}},">>$out_file
	fi
done

echo -e "\t};">>$out_file
echo "}">>$out_file

echo "$out_file updated"