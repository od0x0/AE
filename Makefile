#my second attempt at using makefiles

LIBS= -framework SDL -framework OpenGL
FLAGS= -std=c99 -O3 -fstack-protector
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
	
	#-I AECoreSource/ODE/ -I AECoreSource/ODE/src/
	#$(CCXX) -c $(shell find AECoreSource -name "*.cpp") -arch ppc -Os
	$(CC) -c $(shell find AECoreSource -name "*.c") $(FLAGS) -arch ppc $(INCLUDEPATHS) 
	#$(CC) -c $(shell find AECoreSource -name "*.m") $(FLAGS) -arch ppc $(INCLUDEPATHS) $(OBJCFLAGS)
	$(CC) -c Extra/SDL\ WM/AESDLMain.m $(FLAGS) -arch ppc -Os -I /Library/Frameworks/SDL.framework/Headers -I AECoreSource
	ar cr libAEppc.a *.o
	rm -f *.o
	
	#$(CCXX) -c $(shell find AECoreSource -name "*.cpp") -arch i386 -Os
	$(CC) -c $(shell find AECoreSource -name "*.c") $(FLAGS) -arch i386 $(INCLUDEPATHS) 
	#$(CC) -c $(shell find AECoreSource -name "*.m") $(FLAGS) -arch i386 $(INCLUDEPATHS) $(OBJCFLAGS)
	$(CC) -c Extra/SDL\ WM/AESDLMain.m $(FLAGS) -arch i386 -Os -I /Library/Frameworks/SDL.framework/Headers -I AECoreSource
	ar cr libAEintel.a *.o
	rm -f *.o
	lipo -create libAEintel.a libAEppc.a -output libAE.a

install:
	mkdir -p AE
	#mkdir -p AE/ode
	#mkdir -p AE/lua
	cp libAE.a AE
	cp AECoreSource/*.h AE
	rm -f AE/AEInternalRawMesh.h AE/image_DXT.h AE/image_helper.h AE/stb_image_aug.h AE/stbi_DDS_aug_c.h AE/stbi_DDS_aug.h
	#cp AECoreSource/SOIL/SOIL.h AE
	#cp AECoreSource/ode/*.h AE/ode
	#cp AECoreSource/lua/*.h AE/lua
	#cp AECoreSource/*.ooc AE

sweep:
	rm -f *.o
	rm -f *.a

clean:
	make sweep
	cd Examples && make clean
	rm -rf AE

%:
	cd Examples && make $@