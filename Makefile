CC = gcc
CFLAGS = -Wall
EXECUTABLE = screenshot-file-format

all: $(EXECUTABLE)

$(EXECUTABLE): main.o
	$(CC) $(CFLAGS) main.o -o $(EXECUTABLE)

main.o: main.c
	$(CC) -c main.c

clean:
	rm *.o
