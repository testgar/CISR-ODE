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
echo -e "using namespace arma;">>$out_file
echo -e "">>$out_file
echo -e "void Model::observer(">>$out_file
echo -e "\tconst state_type \t\t\t&x ,">>$out_file
echo -e "\tconst double \t\t\t\t&t,">>$out_file
echo -e "\tobserver_type \t\t\t\t&ymat,">>$out_file
echo -e "\tconst state_type \t\t\t&last_observed_x,">>$out_file
echo -e "\tconst time_type \t\t\t&last_observed_t,">>$out_file
echo -e "\tinput_type\t\t\t\t\t&u">>$out_file
echo -e "\t)">>$out_file
echo -e "{">>$out_file
# echo -e "\tinput_type u;">>$out_file
echo -e "\tintermediate_type mid;">>$out_file
# echo -e "\tnext_sudden_change_time=input(t,u);">>$out_file
echo -e "\tintermediates(u,x,mid,t,last_observed_x,last_observed_t);">>$out_file
echo "">>$out_file

vector=($(./scripts/read_column.sh 0 $in_file))
for index in ${!vector[*]}
do
	echo -e "\t// o$index\t${vector[index]//\"/}">>$out_file
done
echo "">>$out_file
for index in ${!vector[*]}
do
	echo -e "\tymat(outputs::${vector[index]//\"/})=mid(mids::${vector[index]//\"/});">>$out_file
done

if [ ${#vector[@]} -eq 0 ]
then
	echo -e "\t_unused(ymat);">>$out_file
fi

for item in ${output_names[*]}
do
	echo -e "\t// $item">>$out_file
done

echo "">>$out_file

for item in ${output_names[*]}
do
	echo -e "\tymat(outputs::$item)=mid(mids::$item);">>$out_file
done

echo "}">>$out_file

echo "$out_file updated"
