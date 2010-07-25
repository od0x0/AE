#my second attempt at using makefiles

LIBS= -framework SDL -framework OpenGL
FLAGS= -std=c99 -fno-stack-protector -Os
#Getting errors without -fno-stack-protector on Snow Leopard
OBJCFLAGS= -fobjc-direct-dispatch -Os
ARCHS= i386 ppc
INCLUDEPATHS= -I /Library/Frameworks/SDL.framework/Headers\
	 -I /System/Library/Frameworks/OpenGL.framework/Headers \
	-I /System/Library/Frameworks/Cocoa.framework/Headers
CC=gcc-4.0 -mmacosx-version-min=10.4 -Wno-pointer-sign
CCXX=g++-4.0 -mmacosx-version-min=10.4

#-4.0 -mmacosx-version-min=10.4

UNAME:=$(shell uname)



all:
	make buildOSXUni
	
	make install
	make sweep

buildOSXUni: 	
	
	#-I Source/ODE/ -I Source/ODE/src/
	$(CCXX) -c $(shell find Source -name "*.cpp") -arch ppc -Os
	$(CC) -c SDLMain.m $(FLAGS) -arch ppc -Os -I /Library/Frameworks/SDL.framework/Headers
	$(CC) -c $(shell find Source -name "*.c") $(FLAGS) -arch ppc $(INCLUDEPATHS) 
	$(CC) -c $(shell find Source -name "*.m") $(FLAGS) -arch ppc $(INCLUDEPATHS) $(OBJCFLAGS)
	ar cr libAEppc.a *.o
	rm -f *.o
	
	$(CCXX) -c $(shell find Source -name "*.cpp") -arch i386 -Os
	$(CC) -c SDLMain.m $(FLAGS) -arch i386 -Os -I /Library/Frameworks/SDL.framework/Headers
	$(CC) -c $(shell find Source -name "*.c") $(FLAGS) -arch i386 $(INCLUDEPATHS) 
	$(CC) -c $(shell find Source -name "*.m") $(FLAGS) -arch i386 $(INCLUDEPATHS) $(OBJCFLAGS)
	ar cr libAEintel.a *.o
	rm -f *.o

	lipo -create libAEintel.a libAEppc.a -output libAE.a

install:
	mkdir -p AE
	mkdir -p AE/ode
	mkdir -p AE/lua
	cp libAE.a AE
	cp Source/*.h AE
	cp Source/SOIL/SOIL.h AE
	cp Source/ode/*.h AE/ode
	cp Source/lua/*.h AE/lua
	#cp Source/*.ooc AE

sweep:
	rm -f *.o
	rm -f *.a

clean:
	make sweep
	cd Tests && make clean
	rm -rf AE

nodetest:
	make
	cd Tests && make Node && ./Node.binary

%:
	cd Tests && make $@