#include "AEQuadPatch.h"

//There are a couple optimizations we could make here, if I cared enough.  We could have made optimizations such as Streamed Vertex Buffer Objects, separating calculation of billboarding from rendering, using point sprites, using shaders, and so on... ...but, I didn't, so suck it.

void AEQuadPatchInit(AEQuadPatch* self, bool cylindrical, size_t quadCount, AEVec3* centers, AEVec3* sizes, AERGBAub* colors/*Perface color, which is translated to pervertex*/){
	memset(self, 0, sizeof(AEQuadPatch));
	self->cylindrical=cylindrical;
	self->quadCount=quadCount;
	
	self->quads=calloc(quadCount, sizeof(AEQuadPatchQuad));
	self->vertices=calloc(quadCount*4, sizeof(AEQuadPatchVertex));
	
	for(size_t i=0;i < quadCount;i++){
		if(centers) self->quads[i].center=centers[i];
		if(sizes) self->quads[i].size=sizes[i];
		else self->quads[i].size=AEVec3From3(1, 1, 1);
		const AEVec2 tout[4]={{0,0},{1,0},{1,1},{0,1}};
		for(char j=0;j<4;j++){
			self->vertices[i*4+j].texcoord=tout[j];
			self->vertices[i*4+j].color.r= colors ? colors[i].r : 255;
			self->vertices[i*4+j].color.g= colors ? colors[i].g : 255;
			self->vertices[i*4+j].color.b= colors ? colors[i].b : 255;
			self->vertices[i*4+j].color.a= 255;
		}
	}
}

//This may potentially be unsafe
/*static int SortQuads(const AEQuadPatchQuad* q, const AEQuadPatchQuad* q2, AEVec3* cameraPosition){
	return q2->distance-q->distance;
}*/

static int SortQuads(const void* vq, const void* vq2){
	const AEQuadPatchQuad* q=vq;
	const AEQuadPatchQuad* q2=vq2;
	return q2->distance - q->distance;
}

void AEQuadPatchUpdate(AEQuadPatch* self, const AEVec3 cameraPosition, const AEVec3 up, const AEVec3 right){
	//Some of this code was made experimentally, so it's not the cleanest, nor the fastest, but it works.
	if(not self) return;
	
	if(self->depthSort){
		for (size_t i=0; i<self->quadCount; i++) {
			AEVec3 difference=AEVec3Sub(self->quads[i].center, cameraPosition);
			self->quads[i].distance=AEVec3Dot(difference,difference);
		}
		qsort(self->quads, self->quadCount, sizeof(AEQuadPatchQuad), SortQuads);
		//qsort_withuserdata(self->quads, self->quadCount, sizeof(AEQuadPatchQuad), (int(*)(const void*,const void*,void*))SortQuads, &position);
	}
	
	const AEVec3 modifiedUp=self->cylindrical ? AEVec3From3(0, 1, 0) : up;
	
	for(size_t i=0; i < self->quadCount; i++){
		const AEVec3 center=self->quads[i].center;
		const AEVec3 size=self->quads[i].size;
		const AEVec3 scaledRight = AEVec3Mul(right, AEVec3From1(size.x));
		const AEVec3 scaledUp = AEVec3Mul(modifiedUp, AEVec3From1(size.y));
		const AEVec3 rightPlusUp=AEVec3Add(scaledRight, scaledUp);
		const AEVec3 rightMinusUp=AEVec3Sub(scaledRight, scaledUp);
		self->vertices[i*4+0].position=AEVec3Sub(center, rightPlusUp);
		self->vertices[i*4+1].position=AEVec3Add(center, rightMinusUp);
		self->vertices[i*4+2].position=AEVec3Add(center, rightPlusUp);
		self->vertices[i*4+3].position=AEVec3Sub(center, rightMinusUp);
	}
}

void AEQuadPatchRender(AEQuadPatch* self){
	glDisable(GL_CULL_FACE);
	if(self->alphaCutoff){
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GEQUAL, self->alphaCutoff);
	}
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glInterleavedArrays(GL_T2F_C4UB_V3F, 0, self->vertices);
	
	glDrawArrays(GL_QUADS,0,self->quadCount*4);
	
	glEnable(GL_CULL_FACE);
	if(self->alphaCutoff) glDisable(GL_ALPHA_TEST);
}

void AEQuadPatchDeinit(AEQuadPatch* self){
	if(not self) return;
	free(self->quads);
	free(self->vertices);
	memset(self, 0, sizeof(AEQuadPatch));
}

void AEQuadPatchAlterImageToSetupForAlphaTest(AEImage* image){
	size_t pixelCount=image->w*image->h;
	if(image->channels==4){
		unsigned char max=0,min=0;
		for(size_t i=0;i<pixelCount;i++){
			max=AEMax(max,image->pixels[i*4+3]);
			min=AEMin(min,image->pixels[i*4+3]);
		}
		
		for(size_t i=0;i<pixelCount;i++){
			unsigned char alpha=image->pixels[i*4+3];
			image->pixels[i*4+3]=(alpha==max) ? 255 : 0;
		}
	}
}
