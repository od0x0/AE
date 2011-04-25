#pragma once
#include "AE.h"

typedef struct{
	AEVec2 texcoord;
	AERGBAub color;
	AEVec3 position;
}AEQuadPatchVertex;

typedef struct{
	AEVec3 center;
	AEVec3 size;
	float distance;//Used for sorting
	uint16_t usertag;
}AEQuadPatchQuad;

typedef struct {
	AEVec3 velocity;
	float timeLeft;
}AEQuadPatchQuadDynamics;

typedef struct{
	//Per Quad
	AEQuadPatchQuad* quads;
	AEQuadPatchQuadDynamics* quadDynamics;
	
	//Per Vertex
	/*AERGBAub* colors;
	AEVec3* vertices;
	AEVec2* texcoords;*/
	
	AEQuadPatchVertex* vertices;
	
	size_t quadCount;
	
	//float alphaCutoff;
	bool cylindrical;
	bool depthSort;
	
	AEVA va;
}AEQuadPatch;

void AEQuadPatchInit(AEQuadPatch* self, bool cylindrical, size_t quadCount, AEVec3* centers, AEVec3* sizes, AERGBAub* colors/*Perface color, which is translated to pervertex*/, bool hasDynamics);

void AEQuadPatchStep(AEQuadPatch* self, float seconds, const AEVec3 cameraPosition, const AEVec3 up, const AEVec3 right);
void AEQuadPatchDeinit(AEQuadPatch* self);

static inline const AEVA* AEQuadPatchGetVA(AEQuadPatch* self){
	memset(& self->va.format, 0, sizeof(AEVAFormat));
	self->va.format.textureCoordsPerVertex=1;
	self->va.format.hasColors=true;
	self->va.format.hasVertices=true;
	self->va.format.isQuads=true;
	self->va.data.pointer=self->vertices;
	self->va.elementCount=self->quadCount*4;
	return & self->va;
}

static inline void AEQuadPatchUtilityRenderVA(const AEVA* va, float alphaCutoff){
	glDisable(GL_CULL_FACE);
	if(alphaCutoff){
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GEQUAL, alphaCutoff);
	}
	
	/*glBindBuffer(GL_ARRAY_BUFFER, 0);
	glInterleavedArrays(GL_T2F_C4UB_V3F, 0, self->vertices);
	
	glDrawArrays(GL_QUADS,0,self->quadCount*4);*/
	
	AEVADraw(va, NULL);
	
	glEnable(GL_CULL_FACE);
	if(alphaCutoff) glDisable(GL_ALPHA_TEST);
}

static inline void AEQuadPatchRender(const AEQuadPatch* self, float alphaCutoff){
	glDisable(GL_CULL_FACE);
	if(alphaCutoff){
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GEQUAL, alphaCutoff);
	}
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glInterleavedArrays(GL_T2F_C4UB_V3F, 0, self->vertices);
	
	glDrawArrays(GL_QUADS,0,self->quadCount*4);
	
	glEnable(GL_CULL_FACE);
	if(alphaCutoff) glDisable(GL_ALPHA_TEST);
}

void AEQuadPatchAlterImageToSetupForAlphaTest(AEImage* image);
static inline void AEQuadPatchGetUpAndRightFromMatrix(AEVec3* up, AEVec3* right, float* modelview){
	*right = AEVec3From3(modelview[0], modelview[4], modelview[8]);
	*up = AEVec3From3(modelview[1], modelview[5], modelview[9]);
}
