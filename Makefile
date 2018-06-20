TARGET  := app
SRC_DIR = src
OBJ_DIR = obj
WARN    := -W -Wall
CFLAGS	:= -std=gnu++11 `pkg-config --cflags --libs opencv` -lpthread -pthread -O4
OPFLAGS	:= -shared -fPIC -shared -c
CC      := g++

all: app

app: app.cpp lib/resize.o
	$(CC) app.cpp $(CFLAGS) -o ${TARGET} lib/resize.o

lib/resize.o: lib/resize.hpp lib/resize.cpp
	$(CC) $(OPFLAGS) lib/resize.cpp -o lib/resize.o

.PHONY: clean

clean:
	rm -f app *.o lib/*.o
