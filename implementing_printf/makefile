#	sets c compiler variable to GCC
CC=gcc

#	compiler flags variable- to add more, separate by spaces
CFLAGS=-g

default: main.c sub-print_functions.c my_printf.c 
	$(CC) -o main main.c my_printf.c sub-print_functions.c -g

#	what happens when you use the "clean" command.

#	rm - "remove" or delete files command

#	rm -f - force delete all specified files, even ones which are write-protected and would
#	request delete confirmation

#	*.o - specifies all files with the .o file format, regardless of name

clean:
	rm -f *.o 
