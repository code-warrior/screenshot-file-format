CC = gcc
CFLAGS = -Wall -ansi -pedantic
EXECUTABLE = screenshot-file-format

all: $(EXECUTABLE)

$(EXECUTABLE): main.o
	$(CC) main.o -o $(EXECUTABLE)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

clean:
	rm *.o a.out
