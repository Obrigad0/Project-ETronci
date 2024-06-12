#IDIR=../hdr
IDIR=.
ODIR=./obj
BDIR=./bin

CC=g++
CFLAGS=-std=c++20 -I. -I$(IDIR) -I/usr/include/postgresql -lpq -lm -lhiredis

DEPS = $(wildcard $(IDIR)/*.h)
#OBJ = $(wildcard $(ODIR)/*.o)
SRC = $(wildcard *.cpp)
OBJ = $(patsubst %.cpp, $(ODIR)/%.o, $(SRC))


DBConn_OBJ = $(wildcard ../service/database/*.o)
redConn_OBJ = $(wildcard ../service/redis/obj/*.o)
classes_OBJ = $(wildcard ../classes/obj/*.o)
shared_OBJ = $(wildcard ../shared/obj/*.o)

all:	create_dirs con2db con2redis classes utils $(BDIR)/main

create_dirs:
	mkdir -p $(BDIR)
	mkdir -p $(ODIR)

con2db:
	$(MAKE) -C ../service/database

con2redis:
	$(MAKE) -C ../service/redis

classes:
	$(MAKE) -C ../classes/

utils:
	$(MAKE) -C ../shared/

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(BDIR)/main: $(OBJ) $(DEPS)
	$(CC) -o $@ $(OBJ) $(DBConn_OBJ) $(redConn_OBJ) $(classes_OBJ) $(shared_OBJ) $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ $(IDIR)/*~ core 