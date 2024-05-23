

CC=g++
CPPFLAGS=-std=c++11 -lm -I. -I/usr/include/postgresql -lpq 
#CFLAGS=-I. -I/usr/include/postgresql -lpq -lm
#CFLAGS=-I. -lm


DEPS = $(wildcard *.h)
objects := $(patsubst %.cpp,%.o,$(wildcard *.cpp))


all:	$(objects)

%.o:	%.cpp $(DEPS) 
	$(CC) -c -o $@ $< $(CPPFLAGS) 

clean:
	rm *.o 