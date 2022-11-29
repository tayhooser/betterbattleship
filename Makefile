CFLAGS = -pthread
LFLAGS = -Wall -lrt -lX11 -lGLU -lGL -lm #-lXrandr
INCLUDE = cnavarro.cpp dsimpson.cpp dwelch.cpp jrodriguez4.cpp thooser.cpp

all: bship

bship: bship.o log.o $(INCLUDE)
	g++ bship.o log.o $(INCLUDE) libggfonts.a $(LFLAGS) -o bship

bship.o: bship.cpp
	g++ -c bship.cpp

log.o: log.cpp
	g++ -c log.cpp

clean:
	rm -f bship
	rm -f *.o

