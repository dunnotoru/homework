CLFAGS=-fsanitize=leak -fsanitize=address -Wall -Wextra -g
SRC=main.c phonebook.c linked_list.c

all: $(wildcard *.c)
	mkdir -p build
	gcc  $(CLFAGS) $(SRC) -o build/phonebook

debug: all
	gdb -q build/phonebook

run: all
	./build/phonebook