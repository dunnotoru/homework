CLFAGS=-fsanitize=leak -fsanitize=address -Wall -Wextra -g
SRC=main.c phonebook.c binary_tree.c

all:
	mkdir -p build
	gcc  $(CLFAGS) $(SRC) -o build/phonebook

run: all
	./build/phonebook