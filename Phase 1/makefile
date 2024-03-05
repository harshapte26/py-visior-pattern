CC=g++
AR=ar
CFLAGS=-c

all: greet

hello.o: hello.cc hello.h
	$(CC) $(CFLAGS) hello.cc -o hello.o

hello.a: hello.o
	$(AR) rcs hello.a hello.o

greet.o: greet.cc hello.h
	$(CC) $(CFLAGS) greet.cc -o greet.o

greet: greet.o hello.a
	$(CC) greet.o hello.a -o greet

clean:
	rm -f *.o *.a greet
