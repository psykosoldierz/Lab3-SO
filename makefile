CC=gcc
route=

all: wave.o wave_funct.o
	$(CC) wave.o wave_funct.o -o wave -pthread -lm -Wall -I.
	rm wave.o wave_funct.o

main.o:  $(route)wave.c
	$(CC) -c $(route)wave.c

funct_main.o: $(route)wave_funct.c
	$(CC) -c $(route)wave_funct.c


