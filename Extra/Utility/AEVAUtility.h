#pragma once
#include "AE.h"
/*
static inline uint64_t AEVAFormatTo64Bits(AEVAFormat* self){
	uint64_t bits=0;
	bits |= self->indexType << 0;
	bits |= self->storageType << 2;
	bits |= self->textureCoordsPerVertex << (2+2);
	bits |= self->hasNormals << (2+2+4);
	bits |= self->hasColors << (2+2+4+1);
	bits |= self->hasVertices << (2+2+4+1+1);
	return bits;
}

static inline void AEVAFormatFrom64Bits(AEVAFormat* self, uint64_t bits){
	self->indexType = (bits >> 0) & 2;
	self->storageType = (bits >> 2) & 2;
	self->textureCoordsPerVertex = (bits >> (2+2)) & 4;
	self->hasNormals = (bits >> (2+2+4)) & 1;
	self->hasColors = (bits >> (2+2+4+1)) & 1;
	self->hasVertices = (bits >> (2+2+4+1+1)) & 1;
}

static inline size_t AEVAFormatVertex32Count(AEVAFormat* self){
	size_t count=0;
	count+=self->textureCoordsPerVertex*2;
	if(self->hasNormals) count+=3;
	if(self->hasColors) count++;
	if(self->hasVertices) count+=3;
	return count;
}

static inline void AEVAFormatSetVertexFromData(AEVAFormat* self, void* outdata, AEVec2* ts, AERGBA* c, AEVec3* n, AEVec3* v){
	float* data=(float*)outdata;
	for (int i=0; i<self->textureCoordsPerVertex; i++) {
		if(ts){
			data[0]=ts->x;
			data[1]=ts->y;
			ts++;
		}
		data+=2;
	}
	if (self->hasColors) {
		if(c) memcpy(data, c, sizeof(char[4]));
		data++;
	}
	if (self->hasNormals) {
		if(n) memcpy(data, n, sizeof(AEVec3));
		data+=3;
	}
	if (self->hasVertices) {
		if(v) memcpy(data, v, sizeof(AEVec3));
		data+=3;
	}
}

static inline void AEVAFormatGetVertexFromData(AEVAFormat* self, void* outdata, AEVec2* ts, AERGBA* c, AEVec3* n, AEVec3* v){
	float* data=(float*)outdata;
	for (int i=0; i<self->textureCoordsPerVertex; i++) {
		if(ts){
			ts->x=data[0];
			ts->y=data[1];
			ts++;
		}
		data+=2;
	}
	if (self->hasColors) {
		if(c) memcpy(c, data, sizeof(char[4]));
		data++;
	}
	if (self->hasNormals) {
		if(n) memcpy(n, data, sizeof(AEVec3));
		data+=3;
	}
	if (self->hasVertices) {
		if(v) memcpy(v, data, sizeof(AEVec3));
		data+=3;
	}
}

static inline void AEVAFormatHostVertexFromNetSwizzle(AEVAFormat* self, void* data){
	uint32_t* data32=(uint32_t*)data;
	AEHostU32FromNetMultiSwizzle(data32, 2*self->textureCoordsPerVertex);
	data32+=2*self->textureCoordsPerVertex;
	if (self->hasColors) data32++;
	if (self->hasNormals) {
		AEHostU32FromNetMultiSwizzle(data32, 3);
		data32+=3;
	}
	if (self->hasVertices) {
		AEHostU32FromNetMultiSwizzle(data32, 3);
		data32+=3;
	}
}

static inline void AEVAFormatNetVertexFromHostSwizzle(AEVAFormat* self, void* data){
	uint32_t* data32=(uint32_t*)data;
	AENetU32FromHostMultiSwizzle(data32, 2*self->textureCoordsPerVertex);
	data32+=2*self->textureCoordsPerVertex;
	if (self->hasColors) data32++;
	if (self->hasNormals) {
		AENetU32FromHostMultiSwizzle(data32, 3);
		data32+=3;
	}
	if (self->hasVertices) {
		AENetU32FromHostMultiSwizzle(data32, 3);
		data32+=3;
	}
}

void AEVASerializeToIO(AEVA* self,AEIO* io);
void AEVAUnserializeFromIO(AEVA* self,AEIO* io);

//A little note about serialization...  By the nature of serialization, the data that is serialized is highly unportable.  While it is possible to make serialized index arrays cross platform (which deal with integers), it is another matter for vertex arrays (which deal with 32bit floats), as the C standard makes no real guarrantee about the format of a floating point number.  As such, in order for this to be used properly, you should serialize the vertex array for every possible architecture you plan to run on.
*/
void AEVALoadFromObj(AEVA* va, AEVA* ia, const char* objfilename);

//A few notes about loading from .obj's, 1) It will only pickup triangulated meshes, there are just too many variants to deal with for it to bother with triangulating your stuff, you should be manually triangulating anyway.  2) Leaving the "ia" argument as NULL will generate va as ready for glDrawArrays() 3) This loader will only pickup normals, positions, and texture coordinates, if you want more, write your own loader.
