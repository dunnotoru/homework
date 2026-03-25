CFLAGS = -Wall -Wextra -Werror -g

all:
	mkdir -p build
	gcc $(CFLAGS) main.c -o build/fake-chmod

run: all
	./build/fake-chmod