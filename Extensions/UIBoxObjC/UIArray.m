#include "UIArray.h"


@implementation AEUIArray
-init{
	self=[super init];
	if(self) list=AEListNew(id);
	return self;
}

-add:(id)object{
	AEListAdd(list,id,[object retain]);
	return self;
}

-remove:(id)object{
	[object release];
	//Not yet implemented
	return self;
}

-(void)dealloc{
	size_t length=[self length];
	for(size_t i=0;i<length;i++)
		[[self at:i] release];
	AEListDelete(list);
	[super dealloc];
}

-(size_t)length{
	return AEListLength(list);
}

-(id*)asCArray{
	return AEListAsArray(list,id);
}

-at:(size_t)index{
	if(index>[self length]) return nil;
	return AEListAsArray(list,id)[index];
}

-(void)set:(size_t)index to:(id)object{
	if(index>[self length]) return;
	AEListAsArray(list,id)[index]=object;
}

-(size_t)find:(id)object{//Returns index+1, 0 if search failed
	size_t length=[self length];
	for(size_t i=0;i<length;i++)
		if([self at:i]==object) return i+1;
	return 0;
}

-(void)swap:(id)object with:(id)object2{
	size_t location1=[self find:object];
	if(location1) return;
	location1--;
	size_t location2=[self find:object2];
	if(location2) return;
	location2--;
	
	[self set:location1 to:object2];
	[self set:location2 to:object];
}

@end
