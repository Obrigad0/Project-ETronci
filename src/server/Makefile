#IDIR=../hdr
IDIR=.
ODIR= ./obj
BDIR= ./bin

CC=g++
# use this when connecting to Postgresql (library lpq)
CFLAGS=-std=c++11 -I. -I$(IDIR) -I/usr/include/postgresql -lpq -lm
# CFLAGS=-std=c++20 -g -ggdb -fpermissive -L/usr/local/lib -Wall -I. -I$(IDIR) -lm -lhiredis

# use this when no connection to Postgresql (library lpq) is needed
#CFLAGS=-std=c++11 -I. -I$(IDIR) -lm

DEPS = $(wildcard $(IDIR)/*.h)
#OBJ = $(wildcard $(ODIR)/*.o)
SRC = $(wildcard *.cpp)
OBJ = $(patsubst %.cpp, $(ODIR)/%.o, $(SRC))

con2db_OBJ=$(wildcard ../service/database/*.o)
con2redis_OBJ=$(wildcard ../service/redis/obj/*.o)
classes_OBJ=$(wildcard ../classes/obj/*.o)
utils_OBJ=$(wildcard ../shared/obj/*.o)

#all:	$(ODIR)/%.o

all:	create_dirs database redis classes shared $(ODIR)/$(OBJ)

create_dirs:
	mkdir -p $(BDIR)
	mkdir -p $(ODIR)

database:
	$(MAKE) -C ../service/database/

redis:
	$(MAKE) -C ../service/redis/

classes:
	$(MAKE) -C ../classes/

shared:
	$(MAKE) -C ../shared/

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ $(IDIR)/*~ core