TARGET  := app
SRC_DIR = src
OBJ_DIR = obj
WARN    := -W -Wall
CFLAGS	:= -std=gnu++11 `pkg-config --cflags --libs opencv` -lpthread -pthread
OPFLAGS	:= -fPIC
CC      := g++

all: app

app: app.cpp lib/resize.o lib/catface.o
	$(CC) app.cpp lib/resize.o lib/catface.o $(CFLAGS) -o ${TARGET}

lib/resize.o: lib/resize.hpp lib/resize.cpp
	$(CC) -c lib/resize.cpp $(OPFLAGS) -o lib/resize.o

lib/catface.o: lib/catface.hpp lib/catface.hpp
	$(CC) -c lib/catface.cpp $(OPFLAGS) -o lib/catface.o

.PHONY: clean

clean:
	rm -f app *.o lib/*.o
