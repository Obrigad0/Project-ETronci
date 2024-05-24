#IDIR=../hdr
IDIR=.
ODIR=../obj
BDIR=../bin

CC=g++
# per connettersi a PostgreSQL
#CFLAGS=-std=c++11 -I. -I$(IDIR) -I/usr/include/postgresql -lpq -lm
CFLAGS=-std=c++20 -g -ggdb -fpermissive -L/usr/local/lib -Wall -I. -I$(IDIR) -lm -lhiredis

DEPS = $(wildcard $(IDIR)/*.h)
#OBJ = $(wildcard $(ODIR)/*.o)
SRC = $(wildcard *.cpp)
OBJ = $(patsubst %.cpp, $(ODIR)/%.o, $(SRC))

utils_OBJ=$(wildcard ../shared/obj/*.o)

#all:	$(ODIR)/%.o

all:	create_dirs utils $(ODIR)/$(OBJ)

create_dirs:
	mkdir -p $(ODIR)

utils:
	$(MAKE) -C ../shared/

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ $(IDIR)/*~ core