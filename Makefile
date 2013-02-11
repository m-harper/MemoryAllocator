all:
	clear; gcc -Wall -lm src/header.c src/freelist.c src/ackerman.c src/mem_test.c src/util.c src/my_allocator.c -o bin/mem_test

performance:
	clear; gcc -Wall -O3 -lm src/header.c src/freelist.c src/ackerman.c src/mem_test.c src/util.c src/my_allocator.c -o bin/mem_test
