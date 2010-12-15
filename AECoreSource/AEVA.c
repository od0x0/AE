#include "AEVA.h"
#include "AECore.h"
#include "AEArray.h"
#include "AEInternalRawMesh.h"

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

void AEVASerializeToMBuffer(AEVA* va,AEMBuffer* mbuffer){
	
	uint64_t version=1;
	version=AENetU64FromHost(version);
	//fwrite(&version, 1, sizeof(uint64_t), file);
	AEMBufferWrite(mbuffer, &version, sizeof(uint64_t));
	
	uint64_t length=va->length;
	length=AENetU64FromHost(length);
	//fwrite(&length, 1, sizeof(uint64_t), file);
	AEMBufferWrite(mbuffer, &length, sizeof(uint64_t));
	
	uint32_t bytes[1];
	if(sizeof(AEVAFormat) not_eq sizeof(bytes)) AEError("AEVAFormat is not 32 bits!  This is bad.");
	memcpy(bytes, & va->format, sizeof(AEVAFormat));
	//fwrite(bytes, 1, 4, file);
	AEMBufferWrite(mbuffer, bytes, sizeof(bytes));
	
	void* memory=AEVAMap(va, va->length, GL_READ_ONLY);
	
	if(not va->format.isAnIndexArray)
		//fwrite(memory, sizeof(float), va->length, file);
		AEMBufferWrite(mbuffer, memory, va->length*sizeof(float));
	else{
		uint32_t* ints=memory;
		for (size_t i=0; i<va->length; i++) {
			ints[i]=AENetU32FromHost(ints[i]);
			//fwrite(ints+i, sizeof(uint32_t), 1, file);
			AEMBufferWrite(mbuffer, ints+i, sizeof(uint32_t));
		}
	}
	
	AEVAUnmap(va);
}

void AEVAUnserializeFromMBuffer(AEVA* va,AEMBuffer* mbuffer){
	uint64_t version;
	//fread(&version, 1, sizeof(uint64_t), file);
	AEMBufferRead(mbuffer, &version, sizeof(uint64_t));
	version=AEHostU64FromNet(version);
	if(version not_eq 1)
		AEError("Invalid version, only 1 is accepted.");
	AEVADeinit(va);
	memset(va, 0, sizeof(AEVA));
	
	uint64_t length;
	//fread(&length, 1, sizeof(uint64_t), file);
	AEMBufferRead(mbuffer, &length, sizeof(uint64_t));
	length=AEHostU64FromNet(length);
	
	uint32_t bytes[1];
	if(sizeof(AEVAFormat) not_eq sizeof(bytes)) AEError("AEVAFormat is not 32 bits!  This is bad.");
	AEMBufferRead(mbuffer, bytes, sizeof(bytes));
	memcpy(& va->format, bytes, sizeof(AEVAFormat));
	//fread(bytes, 1, 4, file);
	
	void* memory=AEVAMap(va, length, GL_WRITE_ONLY);
	
	if(not va->format.isAnIndexArray)
		//fread(memory, sizeof(float), va->length, file);
		AEMBufferRead(mbuffer, memory, va->length*sizeof(float));
	else{
		uint32_t* ints=memory;
		for (size_t i=0; i<va->length; i++) {
			//fread(ints+i, sizeof(uint32_t), 1, file);
			AEMBufferRead(mbuffer, ints+i, sizeof(uint32_t));
			ints[i]=AEHostU32FromNet(ints[i]);
		}
	}
	
	AEVAUnmap(va);
}

void AEVALoadFromObj(AEVA* va, AEVA* ia, const char* objfilename){
	
	bool hasColors=va->format.hasColors;
	bool hasNormals=va->format.hasNormals;
	int floatcount=hasColors+va->format.textureCoordsPerVertex*2+hasNormals*3+3;
	AEArray(float) vertexList;
	AEArrayInitWithTypeOfSize(&vertexList, sizeof(float)*floatcount);
	AEArray(unsigned int) indexList;
	AEArrayInitWithTypeOfSize(&indexList, sizeof(unsigned int));
	//AEList* vertexList=AEListNewWithTypeSize(floatcount*sizeof(float));
	//AEList* indexList=AEListNew(unsigned int);
	
	AERawMesh* m=AERawMeshLoad(objfilename);
	for(unsigned int i=0;i < m->count.f;i++){
		AERawMeshFace* face=m->f+i;
		for(int j=0;j<3;j++){
			float v[floatcount];
			int size=0;
			if(hasColors){
				unsigned char rgba[4]={255,255,255,255};
				memcpy(v+(size++), rgba, 4);
			}
			for(int k=0;k<(va->format.textureCoordsPerVertex*2);k+=2){
				v[size++]=m->t[face->t[j]].x;
				v[size++]=m->t[face->t[j]].y;
			}
			if(hasNormals){
				v[size++]=m->n[face->n[j]].x;
				v[size++]=m->n[face->n[j]].y;
				v[size++]=m->n[face->n[j]].z;
			}
			v[size++]=m->v[face->v[j]].x;
			v[size++]=m->v[face->v[j]].y;
			v[size++]=m->v[face->v[j]].z;
			if(ia){
				//unsigned int index=AEListAddBytesUnique(vertexList, v);
				//AEListAdd(indexList, unsigned int, index);
				unsigned int index=AEArrayAddBytesUnique(&vertexList, v);
				AEArrayAdd(&indexList, index);
			}
			else AEArrayAddBytes(&vertexList, v);
			//AEListAddBytes(vertexList,v);
		}
	}
	AERawMeshDelete(m);

	void* data=NULL;
	
	if(ia){
		//data=AEVAMap(ia,AEListLength(indexList),GL_WRITE_ONLY);
		//memcpy(data, AEListAsArray(indexList,unsigned int), AEListLengthInSizeofType(indexList,char));
		//AEVAUnmap(ia);
		data=AEVAMap(ia,AEArrayLength(&indexList),GL_WRITE_ONLY);
		memcpy(data, AEArrayAsCArray(&indexList), AEArrayLengthInBytes(&indexList));
		AEVAUnmap(ia);
	}
	
	//data=AEVAMap(va,AEListLengthInSizeofType(vertexList,float),GL_WRITE_ONLY);
	//memcpy(data,AEListAsArray(vertexList,void),AEListLengthInSizeofType(vertexList,char));
	//AEVAUnmap(va);
	data=AEVAMap(va,AEArrayLengthInSizeofType(&vertexList,float),GL_WRITE_ONLY);
	memcpy(data,AEArrayAsCArray(&vertexList),AEArrayLengthInBytes(&vertexList));
	AEVAUnmap(va);
	
	AEArrayDeinit(&indexList);
	AEArrayDeinit(&vertexList);
	
	//AEListDelete(indexList);
	//AEListDelete(vertexList);
}
