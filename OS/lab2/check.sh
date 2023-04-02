#!/bin/bash

f1=${1}
f2="tests/out${2}.out"

if [[ ($(diff -w ${f1} ${f2} | wc -m) == 0) ]]
then
  echo "Testcase $2 Passed!"
else
  echo "Testcase $2 Failed!"
fi