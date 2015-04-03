column_number=$1
input_file=$2

result=""

intdex=0
Old_IFS="$IFS"
IFS=','
while read -r line || [[ -n $line ]]
do
	line_nospace=$(echo -e "${line}" | tr -d '[[:space:]]')
	read -a items <<< "${line_nospace}"
	if [ ! -z $items ]
	then
		if [[ $index > 0 ]]
		then
			result+=" "
		fi
		result+='"'
		result+=${items[$column_number]}
		result+='"'
		((index++))
	fi
done <$input_file
IFS="$Old_IFS"

# result+=")"

echo $result
