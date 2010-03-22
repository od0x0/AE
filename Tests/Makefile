
#Makefile for the demos

FLAGS=-arch i386 -arch ppc -std=c99 -O3 ../AE/libAE.a -framework SDL -framework OpenGL -framework Cocoa -I /Library/Frameworks/SDL.framework/Headers
CC=gcc

all: 
	gcc Window.c $(FLAGS) -o Window.binary
	gcc MoveCamera.c $(FLAGS) -o MoveCamera.binary
	gcc Object.c $(FLAGS) -o Object.binary
	gcc Nodes.c $(FLAGS) -o Nodes.binary
	make help

help:
	echo "Type: make nameoftest"
	echo "To run the tests, just do ./nameoftest.binary"

ooc:
	ooc Window.ooc +-arch +i386 +-arch +ppc +-std=c99 +-O3 +"../AE/libAE.a" +-framework +SDL +-framework +OpenGL +-framework +Cocoa +-I +/Library/Frameworks/SDL.framework/Headers

clean:
	rm -f *.binary

testing:
	gcc Image.c $(FLAGS) -o Image.binary