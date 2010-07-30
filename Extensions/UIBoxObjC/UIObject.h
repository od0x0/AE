#pragma once

#include <objc/objc.h>
#include <objc/Object.h>
#include "stdlib.h"


@interface AEUIObject : Object {
	size_t refcount;
}
-init;
-retain;
-(void)release;
-(void)dealloc;
@end
