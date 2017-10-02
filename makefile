# makefile

all: memtest

free_list.o: free_list.c free_list.h
	g++ -c -g free_list.c

my_allocator.o : my_allocator.c my_allocator.h
	g++ -c -g my_allocator.c

main: main_test.c my_allocator.o free_list.o
	g++ -g -o main free_list.o  my_allocator.o  main_test.c 

memtest: memtest.c my_allocator.o free_list.o
	g++ -g -o memtest free_list.o  my_allocator.o memtest.c 

