#! /bin/bash

find ./ \
	-type f \( -name "*.cpp" -o -name "*.c" -o -name "*.h" \) \
| while read -r fname; do
	clang-format "$fname" -style=file -i
done
