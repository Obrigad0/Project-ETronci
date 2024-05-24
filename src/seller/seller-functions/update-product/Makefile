IDIR=.
ODIR=../obj
BDIR=../bin

CC=g++
# per connettersi a PostgreSQL
#CFLAGS=-std=c++11 -I. -I$(IDIR) -I/usr/include/postgresql -lpq -lm 
CFLAGS=-std=c++20 -I. -I$(IDIR) -I/usr/include/postgresql -lpq -lm -lhiredis

DEPS = $(wildcard $(IDIR)/*.h)
SRC = $(wildcard *.cpp)
#OBJ = $(wildcard $(ODIR)/*.o)
OBJ = $(patsubst %.cpp, $(ODIR)/%.o, $(SRC))

DBConn_OBJ = $(wildcard ../../../service/database/*.o)
redConn_OBJ = $(wildcard ../../../service/redis/*.o)
classes_OBJ = $(wildcard ../../../classes/*.o)
shared_OBJ = $(wildcard ../../../shared/*.o)
server_OBJ = $(wildcard ../../../server/*.o)

all: create_dirs con2db con2redis classes utils $(BDIR)/main


create_dirs:
	mkdir -p $(BDIR)
	mkdir -p $(ODIR)

DBConn:
	$(MAKE) -C ../../../service/database

redConn:
	$(MAKE) -C ../../../service/redis

classes:
	$(MAKE) -C ../../../classes

shared:
	$(MAKE) -C ../../../shared

server:
	$(MAKE) -C ../../../server

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(BDIR)/main: $(OBJ) $(DEPS)
	$(CC) -o $@ $(OBJ) $(DBConn_OBJ) $(redConn_OBJ) $(classes_OBJ) $(shared_OBJ) $(server_OBJ) $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ $(IDIR)/*~ core 