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
	
	void* memoryfrom = AEVAMap(from, from->elementCount, GL_READ_ONLY);
	memcpy(memorytemp, memoryfrom, memorySize);
	AEVAUnmap(from);
	
	void* memoryto = AEVAMap(self, from->elementCount, GL_WRITE_ONLY);
	memcpy(memoryto, memorytemp, memorySize);
	AEVAUnmap(self);
	
	free(memorytemp);
}

void* AEVAMap(AEVA* va, unsigned int elementCount,unsigned int writereadmode){
	unsigned int arrayType=va->format.indexType?GL_ELEMENT_ARRAY_BUFFER:GL_ARRAY_BUFFER;
	//if(sizeof(GLfloat) not_eq sizeof(char[4]) and sizeof(GLuint)!=sizeof(char[4])) AEError("A GLfloat or GLuint is not equal to 4 bytes on your system!  This is very, very, bad.");
	if(va->format.indexType == 1 or va->format.indexType == 3) AEError("I dunno if you haven't realized that I b0rk3d ur shiz, but I recently b0rk3d ur shiz.  indexType=AEVAFormatIndexType32Bit is the comparison of isAnIndexArray=true.  It's a whole lot more powerful considering that indexType=AEVAFormatIndexType16Bit saves a ton of space and is probably all you need.  Try the 16 bit version, it works, it's small, and it gives a home to children in Africa (not really).");
	if(va->elementCount and va->elementCount not_eq elementCount) AEError("You are trying to access a VA with a different length than the length it actually has.");
	//if(sizeof(AEVec3) not_eq 3*sizeof(float)) AEError("AEVec3 is padded, this is bad!");
	if(va->elementCount==0){
		va->elementCount=elementCount;
		if(va->format.storageType){
			glGenBuffers(1, &va->data.vbo);
			//glBindBuffer(GL_ARRAY_BUFFER, va->data.vbo);
			
			unsigned int vbotype=0;
			
			switch(va->format.storageType){
				case AEVAFormatStorageTypeStream:
					vbotype=GL_STREAM_DRAW;
					break;
				case AEVAFormatStorageTypeDynamic:
					vbotype=GL_DYNAMIC_DRAW;
					break;
				case AEVAFormatStorageTypeStatic:
					vbotype=GL_STATIC_DRAW;
					break;
			}
			glBindBuffer(arrayType, va->data.vbo);
			glBufferData(arrayType, elementCount * AEVAFormatByteSize(& va->format), NULL,vbotype);
			void* data= glMapBuffer(arrayType, writereadmode);
			if(not data) AEError("glMapBuffer is returning a NULL o/");
			return data;
		}
		else va->data.pointer=malloc(elementCount * AEVAFormatByteSize(& va->format));
	}
	if(va->format.storageType){
		glBindBuffer(arrayType, va->data.vbo);
		
		return glMapBuffer(arrayType, writereadmode);
	}
	return va->data.pointer;
}

void AEVAUnmap(AEVA* va){
	if(va->format.storageType){
		unsigned int arrayType=va->format.indexType?GL_ELEMENT_ARRAY_BUFFER:GL_ARRAY_BUFFER;
		glBindBuffer(arrayType, va->data.vbo);
		glUnmapBuffer(arrayType);
	}
}

//Some might be wondering why I am doing slow things here.  This is not really designed for speed, it is designed to be easy to use.  There was a time that this was designed for speed, it turned out to be fast, but very unforgiving in the event that it was used even the slightest bit incorrectly, the current design is to prevent it from being used improperly.

void AEVADrawRange(const AEVA* va, const AEVA* ia, unsigned long start, unsigned long count){
	char* indexOffset=NULL;
	size_t indexStride=0;
	if(ia){
		glEnableClientState(GL_INDEX_ARRAY);
		indexOffset=NULL;
		if(ia->format.storageType) glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ia->data.vbo);
		else indexOffset=ia->data.pointer;
		indexStride=ia->format.indexType==AEVAFormatIndexType32Bit ? sizeof(GLuint) : sizeof(GLushort);
	}
	
	int tcount=va->format.textureCoordsPerVertex;
	bool hasNormals=va->format.hasNormals;
	
	char* offset=NULL;
	if(va->format.storageType) glBindBuffer(GL_ARRAY_BUFFER, va->data.vbo);
	else{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		offset=va->data.pointer;
	}
	
	size_t stride=AEVAFormatByteSize(& va->format);
	
	for(int i=0;i<tcount;i++){
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
	
	if(hasNormals){
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, stride, offset);
		offset+=sizeof(float[3]);
	}
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, stride, offset);
	
	if(ia) {
		glDrawElements( ia->format.isQuads ? GL_QUADS : GL_TRIANGLES, count, ia->format.indexType==AEVAFormatIndexType32Bit ? GL_UNSIGNED_INT : GL_UNSIGNED_SHORT, indexOffset+indexStride*start);
		/*switch(ia->format.indexType) {
		case AEVAFormatIndexType16Bit:
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT,indexOffset+start);
		break;
		case AEVAFormatIndexType32Bit:
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT,indexOffset+start);
		break;
		default:
			AEError("It's cool and all to have an invalid index type and all, but it doesn't really make sense, really.");
			break;
	}*/
	}
	else glDrawArrays(va->format.isQuads ? GL_QUADS : GL_TRIANGLES, start, count);
	
	for(int i=0;i<tcount;i++){
		glClientActiveTexture(GL_TEXTURE0+i);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}
	glClientActiveTexture(GL_TEXTURE0);
	
	if(va->format.hasColors) glDisableClientState(GL_COLOR_ARRAY);
	
	if(hasNormals) glDisableClientState(GL_NORMAL_ARRAY);
	
	glDisableClientState(GL_VERTEX_ARRAY);
	
	if(ia){
		glDisableClientState(GL_INDEX_ARRAY);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

void AEVADraw(const AEVA* va, const AEVA* ia){
	AEVADrawRange(va, ia, 0, (ia ? ia: va)->elementCount);
}
