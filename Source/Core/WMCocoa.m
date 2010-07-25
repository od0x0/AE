#ifdef __APPLE__

#include "../WM.h"
#include <Cocoa/Cocoa.h>
//#include <objc/objc.h>

AEWMEventFunc AEWMEventFilter;

typedef struct{
	NSWindow* window;
}AEWMCocoa;

AEWM* AEWMOpen(char* title,int w,int h){
	AEWM* wm=calloc(1,sizeof(AEWM));

	NSRect frame = NSMakeRect(0, 0, w, h);
	
	printf("Reached %s Line %i\n",__func__,__LINE__);
	
	NSWindow* window=[NSWindow alloc];
	
	printf("Reached %s Line %i\n",__func__,__LINE__);
	
	window  = [window initWithContentRect:frame
                                        styleMask:NSTitledWindowMask | NSClosableWindowMask | NSTexturedBackgroundWindowMask
                                        backing:NSBackingStoreBuffered
                                        defer:NO];
										
	printf("Reached %s Line %i\n",__func__,__LINE__);
	
	[window setBackgroundColor:[NSColor whiteColor]];
	printf("Reached %s Line %i\n",__func__,__LINE__);
	[window makeKeyAndOrderFront:NSApp];
	printf("Reached %s Line %i\n",__func__,__LINE__);
	[window setReleasedWhenClosed:YES];
	printf("Reached %s Line %i\n",__func__,__LINE__);
	
	[window center];
	[window setLevel:NSFloatingWindowLevel];
	
	NSString* string=[[NSString  alloc] initWithCString:title encoding:NSASCIIStringEncoding];
	[window setTitle:string];
	[string release];
	
	wm->data=calloc(1,sizeof(AEWMCocoa));
	AEWMCocoa* cocoawm=wm->data;
	cocoawm->window=window;
	
	return wm;
}

void AEWMReset(AEWM* wm);
void AEWMClose(AEWM* wm){
	AEWMCocoa* cocoawm=wm->data;
	[cocoawm->window release];
	free(wm->data);
}

void AEWMPoll(AEWM* wm);

unsigned char* AEWMGetKeys(void){}

int* AEWMMouse(int* newPosition);//Returns static array of 2 ints (x,y), sets mouse pos if arg is not null

double AEWMGetSeconds(void);
void AEWMResetSeconds(double to);

#endif