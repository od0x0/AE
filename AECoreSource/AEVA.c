#include "AEVA.h"
#include "AECore.h"
#include "AEArray.h"

//This has a lot of ugly things that need to be cleaned up sometime.

size_t AEVABytesPerVertex(AEVA* self){
	size_t stride=sizeof(AEVec3);
	stride+=sizeof(AEVec2)*self->format.textureCoordsPerVertex;
	if(self->format.hasNormals) stride+=sizeof(AEVec3);
	if(self->format.hasColors) stride+=sizeof(char[4]);
	return stride;
}
#define AEVAVertexByteSize AEVABytesPerVertex

void AEVAInit(AEVA* va){
	memset(va,0,sizeof(AEVA));
}

void AEVADeinit(AEVA* self){
	if(self->length){
		if(self->format.storageType)
			glDeleteBuffers(1, &self->data.vbo);
		else
			free(self->data.data);
	}
	self->length=0;
	memset(self, 0, sizeof(AEVA));
}

void AEVAInitCopy(AEVA* self,AEVA* from){
	AEVAInit(self);
	memcpy(& self->format, & from->format, sizeof(AEVAFormat));
	
	//A workaround for me being uncertain on whether I can map multiple vbos at the same time
	
	void* memorytemp = malloc(sizeof(float) * from->length);
	
	void* memoryfrom = AEVAMap(from, from->length, GL_READ_ONLY);
	memcpy(memorytemp, memoryfrom, sizeof(float) * from->length);
	AEVAUnmap(from);
	
	void* memoryto = AEVAMap(self, from->length, GL_WRITE_ONLY);
	memcpy(memoryto, memorytemp, sizeof(float) * from->length);
	AEVAUnmap(self);
	
	free(memorytemp);
}

void* AEVAMap(AEVA* va, unsigned int length,unsigned int writereadmode){
	unsigned int arrayType=va->format.isAnIndexArray?GL_ELEMENT_ARRAY_BUFFER:GL_ARRAY_BUFFER;
	if(sizeof(GLfloat)!=sizeof(char[4]) and sizeof(GLuint)!=sizeof(char[4])) AEError("A GLfloat or GLuint is not equal to 4 bytes on your system!  This is very, very, bad.");
	if(va->length && va->length!=length) AEError("You are trying to access a VA with a different length than the length it actually has.");
	if(va->length==0){
		va->length=length;
		if(va->format.storageType){
			glGenBuffers(1, &va->data.vbo);
			//glBindBuffer(GL_ARRAY_BUFFER, va->data.vbo);
			
			unsigned int vbotype=0;
			
			switch(va->format.storageType){
				case AEVAVBOTypeStream:
					vbotype=GL_STREAM_DRAW;
					break;
				case AEVAVBOTypeDynamic:
					vbotype=GL_DYNAMIC_DRAW;
					break;
				case AEVAVBOTypeStatic:
					vbotype=GL_STATIC_DRAW;
					break;
			}
			glBindBuffer(arrayType, va->data.vbo);
			glBufferData(arrayType, length * sizeof(char[4]), NULL,vbotype);
			void* data= glMapBuffer(arrayType, writereadmode);
			if(not data) AEError("glMapBuffer is returning a NULL o/");
			return data;
		}
		else va->data.data=malloc(sizeof(char[4])*length);
	}
	if(va->format.storageType){
		glBindBuffer(arrayType, va->data.vbo);
		
		return glMapBuffer(arrayType, writereadmode);
	}
	return va->data.data;
}

void AEVAUnmap(AEVA* va){
	if(va->format.storageType){
		unsigned int arrayType=va->format.isAnIndexArray?GL_ELEMENT_ARRAY_BUFFER:GL_ARRAY_BUFFER;
		glBindBuffer(arrayType, va->data.vbo);
		glUnmapBuffer(arrayType);
	}
}

//Some might be wondering why I am doing slow things here, the truth is, this is not really designed for speed, it is designed to be easier to use.  There was a time that this was designed for speed, it turned out to be fast, but very unforgiving in the event that it was used even the slightest bit incorrectly, the current design is to prevent it from being used improperly.

void AEVADrawRange(AEVA* va, AEVA* ia, unsigned long start, unsigned long end){
	GLuint* indexOffset=NULL;
	if(ia){
		glEnableClientState(GL_INDEX_ARRAY);
		indexOffset=NULL;
		if(ia->format.storageType) glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ia->data.vbo);
		else indexOffset=ia->data.data;
	}
	
	int tcount=va->format.textureCoordsPerVertex;
	bool hasNormals=va->format.hasNormals;
	
	void* offset=NULL;
	if(va->format.storageType) glBindBuffer(GL_ARRAY_BUFFER, va->data.vbo);
	else{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		offset=va->data.data;
	}
	
	size_t stride=tcount*sizeof(float[2]) + sizeof(float[3]);
	stride += hasNormals ? sizeof(float[3]) : 0;
	
	for(int i=0;i<tcount;i++){
		glClientActiveTexture(GL_TEXTURE0+i);
		glTexCoordPointer(2, GL_FLOAT, stride, offset);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		offset+=sizeof(float[2]); 
	}
	glClientActiveTexture(GL_TEXTURE0);
	
	if(hasNormals){
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, stride, offset);
		offset+=sizeof(float[3]);
	}
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, stride, offset);

	if(ia) glDrawElements(GL_TRIANGLES, end - start, GL_UNSIGNED_INT,indexOffset+start);
	else glDrawArrays(GL_TRIANGLES,start,end);
	
	for(int i=0;i<tcount;i++){
		glClientActiveTexture(GL_TEXTURE0+i);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}
	glClientActiveTexture(GL_TEXTURE0);
	
	if(hasNormals) glDisableClientState(GL_NORMAL_ARRAY);
	
	glDisableClientState(GL_VERTEX_ARRAY);
	
	if(ia){
		glDisableClientState(GL_INDEX_ARRAY);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

void AEVADraw(AEVA* va, AEVA* ia){
	AEVADrawRange(va, ia, 0, (ia ? ia: va)->length);
}
