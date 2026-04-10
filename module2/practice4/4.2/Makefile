CFLAGS=-fsanitize=address -g -Wall -Wextra
SRC=main.c priority_queue.c

build: $(SRC)
	mkdir -p build
	gcc $(CFLAGS) $(SRC) -o build/app

run: build
	./build/app