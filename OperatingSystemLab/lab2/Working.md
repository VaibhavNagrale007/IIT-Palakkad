# Working:

M -> martix <br>
S -> scalar <br>
>
eg. File has S0 S1 M0 M1 M2 <br>
[ -a -a -A ] <br>
[S0+M0]   = M00 <br>
[M00+S1]  = M001 <br>
[M001+M1] = M0011 (final output) <br>
>>
{ <br>
    * initially on -a, S0 and M0 gets used up to form M00 <br>
    * then on -a, again S1 and M00 gets used up to form M001 <br>
    * then on -A, M001 and M1 gets used up to form M0011 <br>
    * thus finally S0, S1, M0 and M1 will be removed as
      they are used up and ans will be M0011 <br>
    * if after this used  <br>
      (-a will cause error since no scalar, all are used up) <br>
      (-A will use M0011 and M2 since M2 left) ... so on <br>
} <br>

### Files:
files       |   folder/use
------------|------------
./a.out     | in bin
.cpp        | in src
.out & .in  | in tests
.o          | in build
library     | in libs
.h          | in include
check.sh    | bash file for checking input and output
runFile.sh  | bash file run command
Log.txt     | logger
makefile    | make command