#! /bin/bash
echo "Checking format"

find ../ \
	-type f \( -name "*.cpp" -o -name "*.c" -o -name "*.h" \) \
| while read -r fname; do
	echo $fname
	clang-format "$fname" -style=file -output-replacements-xml \
		| grep -c "<replacement " >/dev/null
	if [ $? -ne 1 ]
	then
		echo "Clang format failure $fname"
		exit 1;
	fi
done
