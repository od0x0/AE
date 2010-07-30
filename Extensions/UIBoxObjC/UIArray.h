#pragma once
#include "UIObject.h"
#include "List.h"


@interface AEUIArray : AEUIObject {
	AEList* list;
}

-init;
-add:(id)object;
-remove:(id)object;
-(void)dealloc;
-(size_t)length;
-(id*)asCArray;
-at:(size_t)index;
-(void)set:(size_t)index to:(id)object;
-(size_t)find:(id)object;//Returns index+1, 0 if search failed
-(void)swap:(id)object with:(id)object2;

@end
