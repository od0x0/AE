
#Makefile for the demos

FLAGS=  -arch i386 -arch ppc -std=c99 ../AE/libAE.a -framework SDL -framework OpenGL -framework Cocoa -framework SDL_ttf -framework SDL_mixer -I /Library/Frameworks/SDL.framework/Headers -I /Library/Frameworks/Cocoa.framework/Headers -O3 -lstdc++
CC=gcc-4.0 -mmacosx-version-min=10.4 -Wno-pointer-sign

%:
	$(CC) $@.c $(FLAGS) -o $@.binary
	./$@.binary

all: 
	make help

help:
	echo "Type: make nameoftest"
	echo "To run the tests, just do ./nameoftest.binary"

clean:
	rm -f *.binary

luatest:
	$(CC) Lua.m $(FLAGS) -o Lua