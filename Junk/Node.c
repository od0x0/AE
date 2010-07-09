#include "../Core.h"
#include "../FastMath3D.h"
#include "../enough.h"
#include "../ODE/ode/ode.h"
#include "../List.h"
#include "../Node.h"
#pragma once

struct{
	unsigned int* textures;
	unsigned int* textureUniforms;
	void* shader;
	unsigned char texUnitCount;
}AENodeMaterial;

struct{
	union{
		AEVBO* vbo
	}data;
	unsigned char type;
}AENodeMesh;

struct{
	AENodeMesh* mesh;
	AENodeMaterial* material;
}AENodeObject;

void AENodeObjectRenderAction(void* node){
	glPushMatrix();

	//double pos[3];
	//e_nso_get_pos(enode,pos,NULL,NULL,NULL,NULL,NULL);
	glTranslatef(pos[0],pos[1],pos[2]);
	//VQuat64 rot;
	//e_nso_get_rot(enode,&rot,NULL,NULL,NULL,NULL,NULL);
	//AEQuat q={rot.x,rot.y,rot.z,rot.w};
	float m[16];
	AEQuatToMatrix4x4(&q,m);
	glMultMatrix(m);
	
	
	
	glPopMatrix();
}