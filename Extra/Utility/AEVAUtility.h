#pragma once
#include "AE.h"

/*static inline uint64_t AEVAFormatTo64Bits(AEVAFormat* self){
	uint64_t bits=0;
	bits |= self->indexType << 0;
	bits |= self->storageType << 2;
	bits |= self->textureCoordsPerVertex << (2+2);
	bits |= self->hasNormals << (2+2+4);
	bits |= self->hasColors << (2+2+4+1);
	bits |= self->hasVertices << (2+2+4+1+1);
	bits |= self->isQuads << (2+2+4+1+1+1);
	return bits;
}

static inline void AEVAFormatFrom64Bits(AEVAFormat* self, uint64_t bits){
	self->indexType = (bits >> 0) & 2;
	self->storageType = (bits >> 2) & 2;
	self->textureCoordsPerVertex = (bits >> (2+2)) & 4;
	self->hasNormals = (bits >> (2+2+4)) & 1;
	self->hasColors = (bits >> (2+2+4+1)) & 1;
	self->hasVertices = (bits >> (2+2+4+1+1)) & 1;
	self->isQuads = (bits >> (2+2+4+1+1+1)) & 1;
}

static inline size_t AEVAFormatVertex32Count(AEVAFormat* self){
	size_t count=0;
	count+=self->textureCoordsPerVertex*2;
	if(self->hasNormals) count+=3;
	if(self->hasColors) count++;
	if(self->hasVertices) count+=3;
	return count;
}
*/
static inline void AEVAFormatSetVertex(const AEVAFormat* self, void* outdata, AEVec2* ts, AERGBA* c, AEVec3* n, AEVec3* v){
	char* data=(char*)outdata;
	for (int i=0; i<self->textureCoordsPerVertex; i++) {
		const size_t size=sizeof(float[2]);
		if(ts) {
			memcpy(data, ts, size);
			ts+=2;
		}
		data+=size;
	}
	if (self->hasColors) {
		const size_t size=sizeof(char[4]);
		if(c) memcpy(data, c, size);
		data+=size;
	}
	if (self->hasNormals) {
		const size_t size=sizeof(float[3]);
		if(n) memcpy(data, n, size);
		data+=size;
	}
	if (self->hasVertices) {
		const size_t size=sizeof(float[3]);
		if(v) memcpy(data, v, size);
		data+=size;
	}
}

static inline void AEVAFormatGetVertex(const AEVAFormat* self, void* outdata, AEVec2* ts, AERGBA* c, AEVec3* n, AEVec3* v){
	char* data=(char*)outdata;
	for (int i=0; i<self->textureCoordsPerVertex; i++) {
		const size_t size=sizeof(float[2]);
		if(ts) {
			memcpy(ts, data, size);
			ts+=2;
		}
		data+=size;
	}
	if (self->hasColors) {
		const size_t size=sizeof(char[4]);
		if(c) memcpy(c, data, size);
		data+=size;
	}
	if (self->hasNormals) {
		const size_t size=sizeof(float[3]);
		if(n) memcpy(n, data, size);
		data+=size;
	}
	if (self->hasVertices) {
		const size_t size=sizeof(float[3]);
		if(v) memcpy(v, data, size);
		data+=size;
	}
}

static inline GLuint AEVAFormatGetIndex(const AEVAFormat* self, void* data) {
	GLushort value16bit=0;
	GLuint value32bit=0;
	switch (self->indexType) {
		case AEVAFormatIndexType16Bit: 
			memcpy(& value16bit, data, sizeof(GLushort));
			return value16bit;
		case AEVAFormatIndexType32Bit: 
			memcpy(& value32bit, data, sizeof(GLuint));
			return value32bit;
	}
	return 0;
}

static inline void AEVAFormatSetIndex(const AEVAFormat* self, void* data, GLuint value32bit) {
	GLushort value16bit=index;
	printf("To %p ", data);
	switch (self->indexType) {
		case AEVAFormatIndexType16Bit: 
			memcpy(data, & value16bit, sizeof(GLushort));
			break;
		case AEVAFormatIndexType32Bit: 
			memcpy(data, & value32bit, sizeof(GLuint));
			break;
	}
}
/*
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
*/
//A little note about serialization...  By the nature of serialization, the data that is serialized is highly unportable.  While it is possible to make serialized index arrays cross platform (which deal with integers), it is another matter for vertex arrays (which deal with 32bit floats), as the C standard makes no real guarrantee about the format of a floating point number.  As such, in order for this to be used properly, you should serialize the vertex array for every possible architecture you plan to run on.

void AEVALoadFromObj(AEVA* va, AEVA* ia, const char* objfilename);

//A few notes about loading from .obj's, 1) It will only pickup triangulated meshes, there are just too many variants to deal with for it to bother with triangulating your stuff, you should be manually triangulating anyway.  2) Leaving the "ia" argument as NULL will generate va as ready for glDrawArrays() 3) This loader will only pickup normals, positions, and texture coordinates, if you want more, write your own loader. 4) It will not handle relative indices (negatives) for the face command
