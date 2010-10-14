#include "../NodeObjC.h"

@implementation AENode

/*{
	void* data;
	ENode* enode;//For binding to Enough
	char name[33];
	unsigned char refcount;//Eases the problem of memory managment
	//unsigned char type;//Don't need this when dealing with ObjC, just use [node class]
}*/

-retain{
	refcount++;
}

-(void)release{
	if(refcount>0) refcount--;
	if(refcount) return;
	[self dealloc];
}

-(void)dealloc{}

-(void)step:(float)stepInSeconds{}

-(void)render{}

-(void)setName:(char*)newname{
	snprintf(name,"%31s",newname);
}

-(char*)name{
	return name;
}

-(void)setData:(void*)newdata{
	data=newdata;
}

-(void*)data{
	return data;
}

@end