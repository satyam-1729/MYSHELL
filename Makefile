CC = gcc
CFLAG = -I.

myshell: assignment1.o
	$(CC) -o myshell assignment1.o