#Makefile

FLAGS=-arch i386 -arch ppc -std=c99 -Os -framework SDL -framework SDL_ttf -framework Cocoa -I /Library/Frameworks/SDL.framework/Headers -I /Library/Frameworks/SDL_ttf.framework/Headers

all:
	gcc ttf2bmp.c SDLMain.m $(FLAGS) -o ttf2bmp

clean:
	rm -f ttf2bmp