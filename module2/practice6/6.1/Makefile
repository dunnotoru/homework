CLFAGS=-Wall -Wextra -g

build: mkdir linked_list.a 
	gcc $(CLFAGS) main.c phonebook.c -Lbuild -llinked_list -o build/phonebook

linked_list.a: linked_list.o
	ar rcs build/liblinked_list.a build/linked_list.o 

linked_list.o: mkdir
	gcc $(CLFAGS) linked_list.c -c -o build/linked_list.o

mkdir:
	mkdir -p build

run: build
	./build/phonebook