#my second attempt at using makefiles

LIBS= -framework SDL -framework OpenGL
INTELFLAGS= -arch i386 -std=c99
PPCFLAGS= -arch ppc -std=c99

INCLUDEPATHS= -I /Library/Frameworks/SDL.framework/Headers\
	 -I /System/Library/Frameworks/OpenGL.framework/Headers \
	-I /System/Library/Frameworks/Cocoa.framework/Headers
CC=gcc

all:
	make build
	make install
	make sweep

build: 	
	$(CC) -c SDLMain.m $(PPCFLAGS) -Os -I /Library/Frameworks/SDL.framework/Headers
	$(CC) -c $(shell find Source -name "*.c") $(PPCFLAGS) -O3 $(INCLUDEPATHS) 
	ar cr libAEppc.a *.o
	rm -f *.o

	$(CC) -c SDLMain.m $(INTELFLAGS) -Os -I /Library/Frameworks/SDL.framework/Headers
	$(CC) -c $(shell find Source -name "*.c") $(INTELFLAGS) -O3 $(INCLUDEPATHS) 
	ar cr libAEintel.a *.o
	rm -f *.o

	lipo -create libAEintel.a libAEppc.a -output libAE.a

install:
	mkdir -p AE
	cp libAE.a AE
	cp Source/*.h AE
	cp Source/SOIL/SOIL.h AE
	cp Source/*.ooc AE

sweep:
	rm -f *.o
	rm -f *.a