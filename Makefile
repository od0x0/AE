#my second attempt at using makefiles

LIBS= -framework SDL -framework OpenGL
FLAGS= -std=c99 -Os -fstack-protector
#-fno-stack-protector
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
	make core
	
	make install
	make sweep

core: 	
	
	#-I AECore/ODE/ -I AECore/ODE/src/
	#$(CCXX) -c $(shell find AECore -name "*.cpp") -arch ppc -Os
	$(CC) -c $(shell find AECore -name "*.c") $(FLAGS) -arch ppc $(INCLUDEPATHS) 
	#$(CC) -c $(shell find AECore -name "*.m") $(FLAGS) -arch ppc $(INCLUDEPATHS) $(OBJCFLAGS)
	$(CC) -c AESDLMain.m $(FLAGS) -arch ppc -Os -I /Library/Frameworks/SDL.framework/Headers -I AECore
	ar cr libAEppc.a *.o
	rm -f *.o
	
	#$(CCXX) -c $(shell find AECore -name "*.cpp") -arch i386 -Os
	$(CC) -c $(shell find AECore -name "*.c") $(FLAGS) -arch i386 $(INCLUDEPATHS) 
	#$(CC) -c $(shell find AECore -name "*.m") $(FLAGS) -arch i386 $(INCLUDEPATHS) $(OBJCFLAGS)
	$(CC) -c AESDLMain.m $(FLAGS) -arch i386 -Os -I /Library/Frameworks/SDL.framework/Headers -I AECore
	ar cr libAEintel.a *.o
	rm -f *.o
	lipo -create libAEintel.a libAEppc.a -output libAE.a

install:
	mkdir -p AE
	mkdir -p AE/ode
	#mkdir -p AE/lua
	cp libAE.a AE
	cp AECore/*.h AE
	cp AECore/SOIL/SOIL.h AE
	#cp AECore/ode/*.h AE/ode
	#cp AECore/lua/*.h AE/lua
	#cp AECore/*.ooc AE

sweep:
	rm -f *.o
	rm -f *.a

clean:
	make sweep
	cd Tests && make clean
	rm -rf AE

%:
	cd Tests && make $@