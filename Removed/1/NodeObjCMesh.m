#include "../NodeObjC.h"

@implementation AENodeVBO

-(void)setVBO:(AEVBO*)newvbo{
	vbo=newvbo;
}

-(AEVBO*)VBO{
	return vbo;
}

-(void)render{
	AEVBOBind(vbo);
	AEVBODraw(vbo);
}

-(void)dealloc{
	AEVBODelete(vbo);
}

@end