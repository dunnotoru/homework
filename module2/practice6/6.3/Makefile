build: main.c
	gcc -g main.c -o build/calculator -ldl

libsum.so: sum.c
	gcc -fPIC -shared sum.c -o build/$@

libsubtract.so: subtract.c
	gcc -fPIC -shared subtract.c -o build/$@

libmultiply.so: multiply.c
	gcc -fPIC -shared multiply.c -o build/$@

libdivide.so: divide.c
	gcc -fPIC -shared divide.c -o build/$@ 

run: build libsum.so libsubtract.so libmultiply.so libdivide.so
	LD_LIBRARY_PATH=($LD_LIBRARY_PATH):$(PWD)/build ./build/calculator

run_only: build
	LD_LIBRARY_PATH=($LD_LIBRARY_PATH):$(PWD)/build ./build/calculator
