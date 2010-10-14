#pragma once
#include <objc/objc.h>

#include "Core.h"
#include "Math3D.h"
#include "VBO.h"
#include "List.h"
#include "Image.h"

#define int32 evint32
#define uint32 evuint32
#define int16 evint16
#define uint16 evuint16
#define int8 evint8
#define uint8 evuint8
#include "enough.h"
#undef int32
#undef uint32
#undef int16
#undef uint16
#undef int8
#undef uint8

#define int32 odeint32
#define uint32 odeuint32
#define int16 odeint16
#define uint16 odeuint16
#define int8 odeint8
#define uint8 odeuint8
#include "ode/ode.h"
#undef int32
#undef uint32
#undef int16
#undef uint16
#undef int8
#undef uint8

@interface AENode : Object{
	void* data;
	ENode* enode;//For binding to Enough
	char name[33];
	unsigned char refcount;//Eases the problem of memory managment
	//unsigned char type;//Don't need this when dealing with ObjC, just use [node class]
}
-retain;
-(void)release;
-(void)dealloc;

-(void)step:(float)stepInSeconds;
-(void)render;

-(void)setName:(char*)newname;
-(char*)name;

-(void)setData:(void*)newdata;
-(void*)data;
@end

@interface AENodeMaterial : AENode{
	unsigned int* textures;
	unsigned int* textureUniforms;
	unsigned char texUnitCount;
	unsigned int program,vshader,fshader;
	//unsigned char color[4];  //unneeded?
}
-(void)dealloc;
-(void)setTextureWithName:(char*)name withImage:(AEImage*)image;
-(void)setShaderWithVertex:(char*)vertexProgram withFragment:(char*)fragmentProgram;
-(void)render;
@end

@interface AENodeVBO : AENode{
	AEVBO* vbo;
	unsigned int start,end;//Range
}
-(void)setVBO:(AEVBO*)newvbo;
-(AEVBO*)VBO;
-(void)render;
-(void)dealloc;
@end

@interface AENodeEnt : AENode{
	AENode* world;
	struct{
		AEVec3 position;
		AEQuat rotation;
	}transform;
	AENode* material;
	AENode* mesh;
	AEList/*<AENode*>*/* children;
	float radius;
	dBodyID body;
	struct{
		float friction;
		float bounciness;
		float bounceVelocity;
		char immovable;
	}bodyProperties;
}
-(void)render;
-(void)dealloc;
@end