all : a.out

a.out : src/*.cpp
	mkdir bin; g++ $^ -o bin/a.out

bb : 
	mkdir -p build

build/%.o : src/%.cpp | bb
	g++ -c -fPIC $< -o $@

ll : 
	mkdir -p libs

libs/static_scalar.a : build/add.o build/subs.o build/muls.o build/divs.o build/addm.o build/subm.o build/mulm.o build/transm.o build/detm.o | ll
	ar rcs -o $@ $^

libs/dynamic_matrix.a : 
	g++ -shared -o libs/matOp.so build/addm.o build/subm.o build/mulm.o build/transm.o build/detm.o 

libs: libs/static_scalar.a libs/dynamic_matrix.a

tests : test1 test2 test3 test4

test1 : 
	@ bin/a.out -a -T < tests/sample1.in > output.out ; ./check.sh output.out 1 ; rm output.out

test2 : 
	@ bin/a.out -M < tests/sample2.in > output.out ; ./check.sh output.out 2 ; rm output.out

test3 : 
	@ bin/a.out -A -M -M -X < tests/sample3.in > output.out ; ./check.sh output.out 3 ; rm output.out

test4 : 
	@ bin/a.out -a -M -T < tests/sample4.in > output.out ; ./check.sh output.out 4 ; rm output.out

run : 
	./runFile.sh

clean: 
	rm -rf bin build libs Log.txt
