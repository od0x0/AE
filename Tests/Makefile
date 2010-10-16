
#Makefile for the demos

FLAGS=  -arch i386 -arch ppc -std=c99 ../AE/libAE.a -framework SDL -framework OpenGL -framework Cocoa -framework SDL_ttf -framework SDL_mixer -I /Library/Frameworks/SDL.framework/Headers -I /Library/Frameworks/Cocoa.framework/Headers -I /Library/Frameworks/OpenGL.framework/Headers -O0 -lstdc++ -lobjc -I ../AE/ -fstack-protector -g
CC=gcc-4.0 -mmacosx-version-min=10.4 -Wno-pointer-sign

%:
	$(CC) $@.c $(FLAGS) -o $@.binary
	./$@.binary
	rm -f $@.binary

all: 
	make help

help:
	echo "Type: make nameoftest"
	echo "To run the tests, just do ./nameoftest.binary"

clean:
	rm -f *.binary

nodetest:
	$(CC) BasicObjC.m $(FLAGS) -o BasicObjC.binary
	./BasicObjC.binary