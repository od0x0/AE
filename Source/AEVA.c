#include "AEVA.h"
#include "AECore.h"
#include "AEArray.h"

//This has a lot of ugly things that need to be cleaned up sometime.

/*size_t AEVABytesPerVertex(AEVA* self){
	size_t stride=sizeof(AEVec3);
	stride+=sizeof(AEVec2)*self->format.textureCoordsPerVertex;
	if(self->format.hasNormals) stride+=sizeof(AEVec3);
	if(self->format.hasColors) stride+=sizeof(char[4]);
	return stride;
}
#define AEVAVertexByteSize AEVABytesPerVertex
*/

void AEVAInit(AEVA* va){
	memset(va,0,sizeof(AEVA));
}

void AEVADeinit(AEVA* self){
	if(self->elementCount){
		if(self->format.storageType)
			glDeleteBuffers(1, &self->data.vbo);
		else
			free(self->data.pointer);
	}
	memset(self, 0, sizeof(AEVA));
}

void AEVAInitCopy(AEVA* self,AEVA* from){
	AEVAInit(self);
	memcpy(& self->format, & from->format, sizeof(AEVAFormat));
	
	//A workaround for me being uncertain on whether I can map multiple vbos at the same time
	
	size_t memorySize=AEVAFormatByteSize(& from->format) * from->elementCount;
	
	void* memorytemp = malloc(memorySize);
	
	void* memoryfrom = AEVAMap(from, from->elementCount, AEVAMapFlagReadable);
	memcpy(memorytemp, memoryfrom, memorySize);
	AEVAUnmap(from);
	
	void* memoryto = AEVAMap(self, from->elementCount, AEVAMapFlagWritable);
	memcpy(memoryto, memorytemp, memorySize);
	AEVAUnmap(self);
	
	free(memorytemp);
}

void* AEVAMap(AEVA* va, unsigned int elementCount, int writereadmode){
	unsigned int writereadmodeOGL = 0;
	switch (writereadmode) {
		case AEVAMapFlagWritable:
			#ifdef AEiOS
			writereadmodeOGL=GL_WRITE_ONLY_OES;
			#else
			writereadmodeOGL=GL_WRITE_ONLY;
			#endif
			break;
		case AEVAMapFlagReadable:
			#ifdef AEiOS
			return NULL;
			#else
			writereadmodeOGL=GL_READ_ONLY;
			#endif
			break;
		default:
			#ifdef AEiOS
			return NULL;
			#else
			writereadmodeOGL=GL_READ_WRITE;
			#endif
			break;
	}
	#ifdef AEiOS
	if(va->format.indexType==AEVAFormatIndexType32Bit) va->format.indexType=AEVAFormatIndexType16Bit;
	#endif
	unsigned int arrayType=va->format.indexType?GL_ELEMENT_ARRAY_BUFFER:GL_ARRAY_BUFFER;
	//if(sizeof(GLfloat) not_eq sizeof(char[4]) and sizeof(GLuint)!=sizeof(char[4])) AEError("A GLfloat or GLuint is not equal to 4 bytes on your system!  This is very, very, bad.");
	if(va->elementCount and va->elementCount not_eq elementCount) return NULL;
	//if(sizeof(AEVec3) not_eq 3*sizeof(float)) AEError("AEVec3 is padded, this is bad!");
	if(va->elementCount==0){
		va->elementCount=elementCount;
		#ifdef AEiOS
		if(va->format.storageType==AEVAFormatStorageTypeStream) va->format.storageType=0;
		#endif
		if(va->format.storageType){
			glGenBuffers(1, &va->data.vbo);
			//glBindBuffer(GL_ARRAY_BUFFER, va->data.vbo);
			
			unsigned int vbotype=0;

			switch(va->format.storageType){
				#ifndef AEiOS
				case AEVAFormatStorageTypeStream:
					vbotype=GL_STREAM_DRAW;
					break;
				#endif
				case AEVAFormatStorageTypeDynamic:
					vbotype=GL_DYNAMIC_DRAW;
					break;
				case AEVAFormatStorageTypeStatic:
					vbotype=GL_STATIC_DRAW;
					break;
			}
			glBindBuffer(arrayType, va->data.vbo);
			glBufferData(arrayType, elementCount * AEVAFormatByteSize(& va->format), NULL,vbotype);
			#ifdef AEiOS
			void* data = glMapBufferOES(arrayType, writereadmodeOGL);
			#else
			void* data = glMapBuffer(arrayType, writereadmodeOGL);
			#endif
			return data;
		}
		else va->data.pointer=malloc(elementCount * AEVAFormatByteSize(& va->format));
	}
	if(va->format.storageType){
		glBindBuffer(arrayType, va->data.vbo);
		#ifdef AEiOS
		return glMapBufferOES(arrayType, writereadmodeOGL);
		#else
		return glMapBuffer(arrayType, writereadmodeOGL);
		#endif
	}
	return va->data.pointer;
}

void AEVAUnmap(AEVA* va){
	if(va->format.storageType){
		unsigned int arrayType=va->format.indexType?GL_ELEMENT_ARRAY_BUFFER:GL_ARRAY_BUFFER;
		glBindBuffer(arrayType, va->data.vbo);
		#ifdef AEiOS
		glUnmapBufferOES(arrayType);
		#else
		glUnmapBuffer(arrayType);
		#endif
	}
}

//Some might be wondering why I am doing slow things here.  This is not really designed for speed, it is designed to be easy to use.  There was a time that this was designed for speed, it turned out to be fast, but very unforgiving in the event that it was used even the slightest bit incorrectly, the current design is to prevent it from being used improperly.

//#define AEVADrawRangeUnoptimized
#ifdef AEVADrawRangeUnoptimized
void AEVADrawRange(const AEVA* va, const AEVA* ia, GLuint start, GLuint count){
	
	if(not va) return;
	if(count==0) count = (ia ? ia : va)->elementCount;
	char* offset=NULL;
	if(va->format.storageType) glBindBuffer(GL_ARRAY_BUFFER, va->data.vbo);
	else{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		offset=va->data.pointer;
	}
	
	size_t stride=AEVAFormatByteSize(& va->format);
	
	for(int i=0;i<va->format.textureCoordsPerVertex;i++){
		glClientActiveTexture(GL_TEXTURE0+i);
		glTexCoordPointer(2, GL_FLOAT, stride, offset);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		offset+=sizeof(float[2]); 
	}
	glClientActiveTexture(GL_TEXTURE0);
	
	if(va->format.hasColors) {
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(4, GL_UNSIGNED_BYTE, stride, offset);
		offset+=sizeof(GLubyte[4]);
	}
	
	if(va->format.hasNormals){
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, stride, offset);
		offset+=sizeof(float[3]);
	}
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, stride, offset);
	
	
	if(ia) {
		char* indexOffset=NULL;
		size_t indexStride=0;
		//glEnableClientState(GL_INDEX_ARRAY);
		if(ia->format.storageType) glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ia->data.vbo);
		else indexOffset=ia->data.pointer;
		indexStride=ia->format.indexType==AEVAFormatIndexType32Bit ? sizeof(GLuint) : sizeof(GLushort);
		GLuint indexType=GL_UNSIGNED_SHORT;
		#ifndef AEiOS
		if(ia->format.indexType==AEVAFormatIndexType32Bit) indexType=GL_UNSIGNED_INT;
		#endif
		glDrawElements(ia->format.isQuads ? GL_QUADS : GL_TRIANGLES, count, indexType, indexOffset+indexStride*start);
		//glDisableClientState(GL_INDEX_ARRAY);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	else glDrawArrays(va->format.isQuads ? GL_QUADS : GL_TRIANGLES, start, count);
	
	for(int i=0;i<va->format.textureCoordsPerVertex;i++){
		glClientActiveTexture(GL_TEXTURE0+i);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}
	glClientActiveTexture(GL_TEXTURE0);
	
	if(va->format.hasColors) glDisableClientState(GL_COLOR_ARRAY);
	
	if(va->format.hasNormals) glDisableClientState(GL_NORMAL_ARRAY);
	
	glDisableClientState(GL_VERTEX_ARRAY);
}

#else
static inline bool AEVAsAreEqual(const AEVA* va, const AEVA* va2) {
	return 0==memcmp(va, va2, sizeof(AEVA));
}

static GLuint IndexType=0, IndexStride=0;
static char* IndexOffset=NULL;


static inline void AEVABind(const AEVA* va, const AEVA* ia){
	static AEVA lastVA, lastIA, blank;
	
	if(not ia){
		lastIA = blank;
		//glDisableClientState(GL_INDEX_ARRAY);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	else if(not AEVAsAreEqual(ia, & lastIA)){
		//glEnableClientState(GL_INDEX_ARRAY);
		
		if(ia->format.storageType) {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ia->data.vbo);
			IndexOffset=NULL;
		}
		else {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			IndexOffset=ia->data.pointer;
		}
		
		IndexStride=ia->format.indexType==AEVAFormatIndexType32Bit ? sizeof(GLuint) : sizeof(GLushort);
		
		IndexType=GL_UNSIGNED_SHORT;
		#ifndef AEiOS
		if(ia->format.indexType==AEVAFormatIndexType32Bit) IndexType=GL_UNSIGNED_INT;
		#endif
		lastIA = *ia;
		
		//puts("Binding Indices.");
	}
	
	if(not va or not AEVAsAreEqual(& lastIA, va)){
		for(int i=0;i<lastVA.format.textureCoordsPerVertex;i++){
			glClientActiveTexture(GL_TEXTURE0+i);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}
		glClientActiveTexture(GL_TEXTURE0);
	
		if(lastVA.format.hasColors) glDisableClientState(GL_COLOR_ARRAY);
	
		if(lastVA.format.hasNormals) glDisableClientState(GL_NORMAL_ARRAY);
	
		glDisableClientState(GL_VERTEX_ARRAY);
		
		lastVA=blank;
		if(not va) return;
		
		lastVA =  *va;
		
		char* offset=NULL;
		if(va->format.storageType) glBindBuffer(GL_ARRAY_BUFFER, va->data.vbo);
		else{
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			offset=va->data.pointer;
		}
		size_t stride=AEVAFormatByteSize(& va->format);
		for(int i=0;i<va->format.textureCoordsPerVertex;i++){
			glClientActiveTexture(GL_TEXTURE0+i);
			glTexCoordPointer(2, GL_FLOAT, stride, offset);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			offset+=sizeof(float[2]); 
		}
		glClientActiveTexture(GL_TEXTURE0);
		if(va->format.hasColors) {
			glEnableClientState(GL_COLOR_ARRAY);
			glColorPointer(4, GL_UNSIGNED_BYTE, stride, offset);
			offset+=sizeof(GLubyte[4]);
		}
		if(va->format.hasNormals){
			glEnableClientState(GL_NORMAL_ARRAY);
			glNormalPointer(GL_FLOAT, stride, offset);
			offset+=sizeof(float[3]);
		}
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, stride, offset);
		
		//puts("Binding Vertices.");
	}
}

void AEVADrawRange(const AEVA* va, const AEVA* ia, GLuint start, GLuint count){
	AEVABind(va, ia);
	if(not va) return;
	if(count==0) count = (ia ? ia : va)->elementCount;
	#ifdef AEiOS
	if(ia) glDrawElements(GL_TRIANGLES, count, IndexType, IndexOffset+IndexStride*start);
	else glDrawArrays(GL_TRIANGLES, start, count);
	#else
	if(ia) glDrawElements(ia->format.isQuads ? GL_QUADS : GL_TRIANGLES, count, IndexType, IndexOffset+IndexStride*start);
	else glDrawArrays(va->format.isQuads ? GL_QUADS : GL_TRIANGLES, start, count);
	#endif
}
#endif
/*
void AEVADraw(const AEVA* va, const AEVA* ia){
	GLuint count=0;
	if(ia) count=ia->elementCount;
	else if(va) count=va->elementCount;
	AEVADrawRange(va, ia, 0, count);
}*/
