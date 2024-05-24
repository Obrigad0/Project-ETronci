#IDIR=../hdr
IDIR=.
ODIR= ./obj
BDIR= ./bin

CC=g++
# per connettersi a PostgreSQL
#CFLAGS=-std=c++11 -I. -I$(IDIR) -I/usr/include/postgresql -lpq -lm 
CFLAGS=-std=c++20 -I. -I$(IDIR) -I/usr/include/postgresql -lpq -lm -lhiredis

DEPS = $(wildcard $(IDIR)/*.h)
#OBJ = $(wildcard $(ODIR)/*.o)
SRC = $(wildcard *.cpp)
OBJ = $(patsubst %.cpp, $(ODIR)/%.o, $(SRC))

DBConn_OBJ = $(wildcard ../../service/database/*.o)
redConn_OBJ = $(wildcard ../../service/redis/obj/*.o)
classes_OBJ = $(wildcard ../../classes/obj/*.o)
shared_OBJ = $(wildcard ../../shared/obj/*.o)
server_OBJ = $(wildcard ../../server/obj/*.o)

all: create_dirs database redis classes shared server $(BDIR)/main

create_dirs:
	@echo "\n-----------------qua 1------------------\n"
	mkdir -p $(BDIR)
	mkdir -p $(ODIR)
	@echo "\n-----------------FINE 1------------------\n"

database:
	@echo "\n-----------------qua 2------------------\n"
	$(MAKE) -C ../../service/database
	@echo "\n-----------------FINE 2------------------\n"

redis:
	@echo "\n-----------------qua 3------------------\n"
	$(MAKE) -C ../../service/redis
	@echo "\n-----------------FINE 3------------------\n"

classes:
	@echo "\n-----------------qua 4------------------\n"
	$(MAKE) -C ../../classes
	@echo "\n-----------------FINE 4------------------\n"

shared:
	@echo "\n-----------------qua 5------------------\n"
	$(MAKE) -C ../../shared
	@echo "\n-----------------FINE 5------------------\n"

server:
	@echo "\n-----------------INIZIO SERVER 6 ------------------\n"
	$(MAKE) -C ../../server/
	@echo "\n-----------------FINE 6------------------\n"

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(BDIR)/main: $(OBJ) $(DBConn_OBJ) $(redConn_OBJ) $(classes_OBJ) $(shared_OBJ) $(server_OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ $(IDIR)/*~ core
