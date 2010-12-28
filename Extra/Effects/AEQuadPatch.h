#pragma once
#include "AE.h"
#include "qsort_withuserdata.h"

typedef struct{
	AEVec2 texcoord;
	AERGBAub color;
	AEVec3 position;
}AEQuadPatchVertex;

typedef struct{
	AEVec3 center;
	AEVec3 size;
	float distance;//Used for sorting
	uint32_t usertag;
}AEQuadPatchQuad;

typedef struct{
	//Per Quad
	AEQuadPatchQuad* quads;
	
	//Per Vertex
	/*AERGBAub* colors;
	AEVec3* vertices;
	AEVec2* texcoords;*/
	
	AEQuadPatchVertex* vertices;
	
	size_t quadCount;
	
	float alphaCutoff;
	bool cylindrical;
	bool depthSort;
}AEQuadPatch;

void AEQuadPatchInit(AEQuadPatch* self, bool cylindrical, size_t quadCount, AEVec3* centers, AEVec3* sizes, AERGBAub* colors/*Perface color, which is translated to pervertex*/);

void AEQuadPatchRender(AEQuadPatch* self);
void AEQuadPatchUpdate(AEQuadPatch* self, const AEVec3 up, const AEVec3 right);
void AEQuadPatchDeinit(AEQuadPatch* self);

void AEQuadPatchAlterImageToSetupForAlphaTest(AEImage* image);
static inline void AEQuadPatchGetUpAndRightBasedOnOpenGLModelViewMatrix(AEVec3* up, AEVec3* right){
	float modelview[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview);

	*right = AEVec3FromCoords(modelview[0], modelview[4], modelview[8]);
	*up = AEVec3FromCoords(modelview[1], modelview[5], modelview[9]);
}