#!/bin/bash

echo ""
echo "*** Note: Enter directory path which has responses and students_list (eg ./file.sh dir_path) ***"
echo ""

res=$1/responses.csv
stdlist=$1/students_list.csv

# necessary file making checking for repetition
cut -d',' -f3,4 ${res} | sed 's/,""//g' | sed 's/"//g' | sed 1d | sed 's/,/\n/g' > tmp.1
sort tmp.1 | uniq  > resnotrep
sort tmp.1 | uniq -d > resrep
sort ${stdlist} | uniq > slnotrep
sort ${stdlist} | uniq -d > slrep

# not responded(to form for groups) or invalid(filled form but not reg for course)
comm -23 resnotrep slnotrep > invalid
comm -13 resnotrep slnotrep > notrespondedtoform
cut -d',' -f3,4 ${res} | sed 's/,""//g' | sed 's/"//g' | sed 1d | sed 's/,/_/g' | sort > px1

cat invalid | while read line 
do
	echo "$line is invalid / not registered for course"
done
echo ""
cat notrespondedtoform | while read line 
do
	echo "$line not responded to form"
done
echo ""

# multiple entries
cat resrep | while read line 
do
	echo "$line filled form multiple times"
done
echo ""
cat slrep | while read line 
do
	echo "$line registered multiple times"
done
echo ""

# ID creation
cut -d',' -f3,4 ${res} | sed 's/,""//g' | sed 's/"//g' | sed 1d | sed 's/,/ /g' | sort > idfile
grep -v -f invalid idfile > validbutcanrep			# removed invalid IDs
grep -v -f resrep validbutcanrep > corrID			# removed repeated IDs
awk -F' ' '{
	if ($2=="") print $1;
	else if ($1>$2) print $2"_"$1;
	else print $1"_"$2;
}' corrID > correctID

grep -f invalid idfile > px2
awk -F' ' '{
	if ($2=="") print $1;
	else if ($1>$2) print $2"_"$1;
	else print $1"_"$2;
}' px2 > px3
cat px3 | while read line 
do
	echo "$line is invalid ID / not registered for course"
done
echo ""

echo "Correct IDs are present in file (in same folder as sh file) : correctID"
echo ""

rm tmp.1 resnotrep resrep slnotrep slrep invalid notrespondedtoform validbutcanrep idfile corrID px1 px2 px3
