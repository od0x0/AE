#pragma once

#ifdef __APPLE_CC__
#import <Cocoa/Cocoa.h>
#define AEObjCObject NSObject
#else
#include <objc/objc.h>
#include <objc/Object.h>
#define AEObjCObject Object
#endif

#include "stdlib.h"


@interface AEUIObject : AEObjCObject {
#ifndef __APPLE_CC__
	size_t refcount;
#endif
}
#ifndef __APPLE_CC__
-init;
-retain;
-(void)release;
-(void)dealloc;
#endif
@end