
#IDIR=../hdr
IDIR=.
ODIR=../obj
BDIR=../bin

CC=g++
# use this when connecting to Postgresql (library lpq)
#CFLAGS=-std=c++11 -I. -I$(IDIR) -I/usr/include/postgresql -lpq -lm
CFLAGS=-std=c++20 -I. -I$(IDIR) -I/usr/include/postgresql -lpq -lm -lhiredis

# use this when no connection to Postgresql (library lpq) is needed
#CFLAGS=-std=c++11 -I. -I$(IDIR) -lm

DEPS = $(wildcard $(IDIR)/*.h)
#OBJ = $(wildcard $(ODIR)/*.o)
SRC = $(wildcard *.cpp)
OBJ = $(patsubst %.cpp, $(ODIR)/%.o, $(SRC))

# con2db_OBJ=$(wildcard ../../con2db/*.o)
con2db_OBJ=$(wildcard ../database/*.o)
con2redis_OBJ=$(wildcard ../redis/obj/*.o)
classes_OBJ=$(wildcard ../../classes/obj/*.o)
utils_OBJ=$(wildcard ../../shared/obj/*.o)

all:	create_dirs con2db con2redis classes utils $(BDIR)/main

create_dirs:
	mkdir -p $(BDIR)
	mkdir -p $(ODIR)

con2db:
	$(MAKE) -C ../database/

con2redis:
	$(MAKE) -C ../redis/

classes:
	$(MAKE) -C ../../classes/

utils:
	$(MAKE) -C ../../shared/

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(BDIR)/main: $(OBJ) $(DEPS)
	$(CC) -o $@ $(OBJ) $(con2db_OBJ) $(con2redis_OBJ) $(classes_OBJ) $(utils_OBJ) $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ $(IDIR)/*~ core
