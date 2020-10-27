CC = gcc -std=c99
CFLAGS = -g -Wall

all: part1 part2 part3

part1: part1.o command.o
	$(CC) $(CFLAGS) -o part1 part1.o command.o

part1.o: part1.c
	$(CC) $(CFLAGS) -c part1.c

part2: part2.o command.o
	$(CC) $(CFLAGS) -o part2 part2.o command.o

part2.o: part2.c
	$(CC) $(CFLAGS) -c part2.c

part3: part3.o command.o
	$(CC) $(CFLAGS) -o part3 part3.o command.o

part3.o: part3.c
	$(CC) $(CFLAGS) -c part3.c

command.o: command.c command.h
	$(CC) $(CFLAGS) -c command.c

clean:
	$(RM) part1 *.o *~
	$(RM) part2 *.o *~
	$(RM) part3 *.o *~
