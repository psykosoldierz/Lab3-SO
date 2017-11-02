CC=gcc
route=

all: main.o funct_main.o
	$(CC) main.o funct_main.o -o lab2 -pthread -Wall -I.
	rm main.o funct_main.o

main.o:  $(route)main.c
	$(CC) -c $(route)main.c

funct_main.o: $(route)funct_main.c
	$(CC) -c $(route)funct_main.c


