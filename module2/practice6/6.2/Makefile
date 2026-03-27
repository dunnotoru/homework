CLFAGS=-Wall -Wextra -g

build: mkdir liblinked_list.so
	gcc $(CLFAGS) main.c phonebook.c -Lbuild -llinked_list -o build/phonebook

liblinked_list.so: linked_list.o
	gcc $(CLFAGS) -shared build/linked_list.o -o build/liblinked_list.so

linked_list.o: mkdir
	gcc $(CLFAGS) -fPIC linked_list.c -c -o build/linked_list.o

mkdir:
	mkdir -p build

run: build
	LD_LIBRARY_PATH=$(LD_LIBRARY_PATH):$(PWD)/build/  ./build/phonebook

run_no_ld:
	./build/phonebook