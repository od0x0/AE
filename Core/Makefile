#my first attempt at using makefiles

LIBS= -framework SDL -framework OpenGL
INTELFLAGS= -arch i386 -std=c99
PPCFLAGS= -arch ppc -std=c99

INCLUDEPATHS= -I /Library/Frameworks/SDL.framework/Headers\
	 -I /System/Library/Frameworks/OpenGL.framework/Headers \
	-I /System/Library/Frameworks/Cocoa.framework/Headers
CC=gcc

all:
	make build
	make cleanButSaveUB

build: 	
	$(CC) -c SDLMain.m $(PPCFLAGS) -Os -I /Library/Frameworks/SDL.framework/Headers
	$(CC) -c SOIL/*.c $(PPCFLAGS) -Os -I /System/Library/Frameworks/OpenGL.framework/Headers -I /System/Library/Frameworks/Cocoa.framework/Headers
	$(CC) -c Core.c $(PPCFLAGS) -O3 $(INCLUDEPATHS)
	ar cr libAECoreppc.a *.o

	$(CC) -c SDLMain.m $(INTELFLAGS) -Os -I /Library/Frameworks/SDL.framework/Headers
	$(CC) -c SOIL/*.c $(INTELFLAGS) -Os -I /System/Library/Frameworks/OpenGL.framework/Headers -I /System/Library/Frameworks/Cocoa.framework/Headers
	$(CC) -c Core.c $(INTELFLAGS) -O3 $(INCLUDEPATHS)
	ar cr libAECoreintel.a *.o

	lipo -create libAECoreintel.a libAECoreppc.a -output libAECore.a

install:
	make cleaninstall
	cp libAECore.a /usr/lib/

	mkdir -p /usr/include/AE
	cp SOIL/SOIL.h /usr/include/AE
	cp Core.h /usr/include/AE

clean:
	rm -f *.o
	rm -f *.a

cleanButSaveUB:
	rm -f *.o
	rm -f libAECoreintel.a
	rm -f libAECoreppc.a

cleaninstall:
	rm -f /usr/lib/libAECore.a
	rm -f /usr/include/AE/*
	rm -f /usr/include/AE

cleanall:
	make clean
	make cleaninstall