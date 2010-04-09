
#Makefile for the demos

FLAGS=-arch i386 -arch ppc -std=c99 -O3 ../AE/libAE.a -framework SDL -framework OpenGL -framework Cocoa -I /Library/Frameworks/SDL.framework/Headers -framework SDL_ttf -framework SDL_mixer
CC=gcc

%:
	gcc $@.c $(FLAGS) -o $@.binary

all: 
	make help

help:
	echo "Type: make nameoftest"
	echo "To run the tests, just do ./nameoftest.binary"

ooc:
	ooc Window.ooc +-arch +i386 +-arch +ppc +-std=c99 +-O3 +"../AE/libAE.a" +-framework +SDL +-framework +OpenGL +-framework +Cocoa +-I +/Library/Frameworks/SDL.framework/Headers

clean:
	rm -f *.binary