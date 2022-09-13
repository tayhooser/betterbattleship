CFLAGS = -pthread
LFLAGS = -lrt -lX11 -lGLU -lGL -lm #-lXrandr

all: bship

bship: bship.o log.o dsimpson.o
	g++ bship.o log.o dsimpson.o libggfonts.a $(LFLAGS) -o bship

bship.o: bship.cpp
	g++ -c bship.cpp

log.o: log.cpp
	g++ -c log.cpp

dsimpson.o: dsimpson.cpp
	g++ -c dsimpson.cpp


clean:
	rm -f bship
	rm -f *.o

