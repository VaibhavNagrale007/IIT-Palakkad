#!/bin/bash

echo ""
echo "*** Note: Enter directory which has grade files (eg ./file.sh dir_path) ***"
echo ""

cd ./$1

# d -> directory, L -> Links, p-> pipes
for original in ./*; do
if [[ ! -d $original ]] && [[ ! -L $original ]] && [[ ! -p $original ]]; then
	
	for file in ./*; do
	if  [[ ! -d $file ]] && [[ ! -L $file ]] && [[ ! -p $file ]]; then
	
		if [[ $original != $file ]]; then
		if cmp --silent -- "$original" "$file"; then
			rm $file
			ln -s "$original" "$file"
		fi
		fi
		
	fi
	done
	
fi
done

echo "soft links created inside given dir_path (not recursevely)"
