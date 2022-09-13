CFLAGS = -pthread
LFLAGS = -lrt -lX11 -lGLU -lGL -lm #-lXrandr

all: bship

bship: bship.o log.o dwelch.o
	g++ bship.o log.o dwelch.o libggfonts.a $(LFLAGS) -o bship 

bship.o: bship.cpp
	g++ -c bship.cpp

log.o: log.cpp
	g++ -c log.cpp

dwelch.o: dwelch.cpp
	g++ -c dwelch.cpp
clean:
	rm -f bship
	rm -f *.o

