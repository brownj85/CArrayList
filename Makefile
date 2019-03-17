CC=gcc
CFLAGS= -Wall

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

all: array_list.o util.o

clean:
	rm *.o
