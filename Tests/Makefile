
#Makefile for the demos

FLAGS=-arch i386 -arch ppc -std=c99 -O3 -lAECore -framework SDL -framework OpenGL -framework Cocoa -I /Library/Frameworks/SDL.framework/Headers
CC=gcc

all: 
	gcc Window.c $(FLAGS) -o Window
	gcc MoveCamera.c $(FLAGS) -o MoveCamera
	gcc Object.c $(FLAGS) -o Object
	make help

help:
	echo "Type: make nameoftest"
	echo "To run the tests, just do ./nameoftest"

ooc:
	ooc Window.ooc +-arch +i386 +-arch +ppc +-std=c99 +-O3 +-lAECore +-framework +SDL +-framework +OpenGL +-framework +Cocoa +-I +/Library/Frameworks/SDL.framework/Headers

clean:
	rm -f *.o
	rm -f MoveCamera
	rm -f Window
	rm -f Object