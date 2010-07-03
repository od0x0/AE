
#Makefile for the demos

FLAGS=  -arch i386 -arch ppc -std=c99 ../AE/libAE.a -framework SDL -framework OpenGL -framework Cocoa -I /Library/Frameworks/SDL.framework/Headers -O3
CC=gcc

%:
	gcc $@.c $(FLAGS) -o $@.binary

all: 
	make help

help:
	echo "Type: make nameoftest"
	echo "To run the tests, just do ./nameoftest.binary"

clean:
	rm -f *.binary