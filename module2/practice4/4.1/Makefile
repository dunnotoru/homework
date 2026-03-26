CLFAGS=-Wall -Wextra -g
SRC=main.c phonebook.c linked_list.c

all:
	mkdir -p build
	gcc  $(CLFAGS) $(SRC) -o build/phonebook

run: all
	./build/phonebook