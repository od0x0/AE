#include "../VA.h"

static void* CurrentIterationData=NULL;

static size_t AEVAVertexByteSize(AEVA* va){
	size_t stride=sizeof(AEVec2)*va->tunit+sizeof(AEVec3);
	switch (va->dataformat) {
		case AEVADataFormat3VF:
			break;
		case AEVADataFormat3NF3VF:
			stride+=sizeof(AEVec3);
			break;
		case AEVADataFormat4CUB3VF:
			stride+=sizeof(char[4]);
			break;
		case AEVADataFormat4CUB3NF3VF:
			stride+=sizeof(AEVec3)+sizeof(char[4]);
			break;
		default:
			break;
	}
	return stride;
}

void AEVAIterationBegin(AEVA* va){
	CurrentIterationData=AEVAMap(va, va->length, GL_READ_WRITE);
}

void AEVAIterationEnd(AEVA* va){
	AEVAUnmap(va);
	CurrentIterationData=NULL;
}

AEVec3* AEVAIteratorVertexNext(AEVA* va, AEVec3* old){
	size_t stride=AEVAVertexByteSize(va);
	size_t offset=stride-sizeof(AEVec3);
	if(old==NULL) old=CurrentIterationData+offset;
	old+=stride;
	if(((void*)old) > (CurrentIterationData + va->length*sizeof(float))) old=NULL;
	return old;
}

AEVec3* AEVAIteratorNormalNext(AEVA* va, AEVec3* old){
	size_t stride=AEVAVertexByteSize(va);
	size_t offset=stride-sizeof(AEVec3)-sizeof(AEVec3);
	if(old==NULL) old=CurrentIterationData+offset;
	old+=stride;
	if(((void*)old) > (CurrentIterationData + va->length*sizeof(float))) old=NULL;
	return old;
}

AEVec2* AEVAIteratorTexCoordNext(AEVA* va, int unit, AEVec2* old){
	return NULL;
}

GLuint* AEVAIteratorIndexNext(AEVA* va, GLuint* old){
	return NULL;
}

void AEVAInit(AEVA* va,int isAnIndexArray,int vbotype,int tuint, int dataformat){
	memset(va,0,sizeof(AEVA));
	va->isAnIndexArray=isAnIndexArray;
	va->vbotype=vbotype;
	va->tunit=tuint;
	va->dataformat=dataformat;
}

void AEVADeinit(AEVA* va){
	if(va->length){
		if(va->vbotype)
			glDeleteBuffers(1, &va->data.vbo);
		else
			free(va->data.data);
	}
	va->length=0;
	memset(va, 0, sizeof(AEVA));
}

void AEVAInitCopy(AEVA* vato,AEVA* vafrom){
	AEVAInit(vato,vafrom->isAnIndexArray, vafrom->vbotype, vafrom->tunit, vafrom->dataformat);
	
	//A workaround for me being uncertain on whether I can map multiple vbos at the same time
	
	void* memorytemp = malloc(sizeof(float) * vafrom->length);
	
	void* memoryfrom = AEVAMap(vafrom, vafrom->length, GL_READ_ONLY);
	memcpy(memorytemp, memoryfrom, sizeof(float) * vafrom->length);
	AEVAUnmap(vafrom);
	
	void* memoryto = AEVAMap(vato, vafrom->length, GL_WRITE_ONLY);
	memcpy(memoryto, memorytemp, sizeof(float) * vafrom->length);
	AEVAUnmap(vato);
	
	free(memorytemp);
}

void* AEVAMap(AEVA* va, unsigned int length,unsigned int writereadmode){
	unsigned int arrayType=va->isAnIndexArray?GL_ELEMENT_ARRAY_BUFFER:GL_ARRAY_BUFFER;
	if(sizeof(GLfloat)!=sizeof(char[4])) AEError("A GLfloat is not equal to 4 bytes on your system!  This is very, very, bad.");
	//if(va->length && va->length!=length) AEError("You are trying to access a va with a different length than the length it actually has.");
	if(va->length==0){
		va->length=length;
		if(va->vbotype){
			glGenBuffers(1, &va->data.vbo);
			//glBindBuffer(GL_ARRAY_BUFFER, va->data.vbo);
			
			unsigned int vbotype=0;
			
			switch(va->vbotype){
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
	if(va->vbotype){
		glBindBuffer(arrayType, va->data.vbo);
		
		return glMapBuffer(arrayType, writereadmode);
	}
	return va->data.data;
}

void AEVAUnmap(AEVA* va){
	if(va->vbotype){
		unsigned int arrayType=va->isAnIndexArray?GL_ELEMENT_ARRAY_BUFFER:GL_ARRAY_BUFFER;
		glBindBuffer(arrayType, va->data.vbo);
		glUnmapBuffer(arrayType);
	}
}

void AEVABindVertex(AEVA* va){
	if(va==NULL){
		glDisableClientState(GL_VERTEX_ARRAY);
		return;
	}
	glEnableClientState(GL_VERTEX_ARRAY);
	void* offset = NULL;
	if(va->vbotype) glBindBuffer(GL_ARRAY_BUFFER, va->data.vbo);
	else{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		offset=va->data.data;
	}
	glVertexPointer(3, GL_FLOAT, 0, offset);
}

void AEVABindTexcoord(AEVA* va){
	if(va==NULL){
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		return;
	}
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	void* offset = NULL;
	if(va->vbotype) glBindBuffer(GL_ARRAY_BUFFER, va->data.vbo);
	else{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		offset=va->data.data;
	}
	glTexCoordPointer(2, GL_FLOAT, 0, offset);
}

void AEVABindNormal(AEVA* va){
	if(va==NULL){
		glDisableClientState(GL_NORMAL_ARRAY);
		return;
	}
	glEnableClientState(GL_NORMAL_ARRAY);
	void* offset = NULL;
	if(va->vbotype) glBindBuffer(GL_ARRAY_BUFFER, va->data.vbo);
	else{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		offset=va->data.data;
	}
	glNormalPointer(GL_FLOAT, 0, offset);
}

//You might be wondering why I'm using glInterleavedArrays, but the truth is because it doesn't require I enable/disable client state (which shouldn't be slow or anything, but I like the less code required)  Ideally, we'd have a AEVABindInterleavedWith(AEVA* va,int t, int n, int v)

void AEVABindInterleaved(AEVA* va,int tcount,bool hasNormals){
	if(va==NULL) return;
	void* offset=NULL;
	if(va->vbotype) glBindBuffer(GL_ARRAY_BUFFER, va->data.vbo);
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
}

void AEVABindInterleavedTNV(AEVA* va){
	if(va==NULL) return;
	void* offset=NULL;
	if(va->vbotype) glBindBuffer(GL_ARRAY_BUFFER, va->data.vbo);
	else{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		offset=va->data.data;
	}
	glInterleavedArrays(GL_T2F_N3F_V3F,0,offset);
}

void AEVABindInterleavedTV(AEVA* va){
	if(va==NULL) return;
	void* offset=NULL;
	if(va->vbotype) glBindBuffer(GL_ARRAY_BUFFER, va->data.vbo);
	else{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		offset=va->data.data;
	}
	glInterleavedArrays(GL_T2F_V3F,0,offset);
}

static unsigned long* CurrentIndices;
//static unsigned int IndicesCount=0;
static bool UseIndices=false;

void AEVABindIndices(AEVA* va){
	//IndicesCount=va->length;
	
	if(va==NULL){
		glDisableClientState(GL_INDEX_ARRAY);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		UseIndices=false;
		return;
	}
	glEnableClientState(GL_INDEX_ARRAY);
	CurrentIndices=NULL;
	if(va->vbotype) glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, va->data.vbo);
	else CurrentIndices=va->data.data;
	UseIndices=true;
}

void AEVABind(AEVA* va){
	if(va->isAnIndexArray) AEVABindIndices(va);
	else AEVABindInterleaved(va, va->tunit, va->dataformat);
}

void AEVADraw(unsigned long start, unsigned long end){
	//if(start==0 && end==0) end=IndicesCount;
	
	if(UseIndices) glDrawElements(GL_TRIANGLES, end - start, GL_UNSIGNED_INT,CurrentIndices+start);
	else glDrawArrays(GL_TRIANGLES,start,end);
}

void AEVASerializeToFILE(AEVA* va,FILE* file){
	uint8_t buffer[16];
	
	uint64_t version=1;
	AEUInt64To8Bytes(version, buffer, true);
	fwrite(buffer, 1, sizeof(uint64_t), file);
	
	uint64_t length=va->length;
	AEUInt64To8Bytes(length, buffer, true);
	fwrite(buffer, 1, sizeof(uint64_t), file);
	
	buffer[0]=va->isAnIndexArray;
	buffer[1]=va->vbotype;
	buffer[2]=va->tunit;
	buffer[3]=va->dataformat;
	fwrite(buffer, 1, 4, file);
	
	void* memory=AEVAMap(va, va->length, GL_READ_ONLY);
	
	if(not va->isAnIndexArray)
		fwrite(memory, sizeof(float), va->length, file);
	else{
		uint32_t* ints=memory;
		for (size_t i=0; i<va->length; i++) {
			AEUInt32To4Bytes(ints[i], buffer, true);
			fwrite(buffer, sizeof(uint32_t), 1, file);
		}
	}
	
	AEVAUnmap(va);
}

void AEVAUnserializeFromFILE(AEVA* va,FILE* file){
	uint8_t buffer[16];
	
	fread(buffer, 1, sizeof(uint64_t), file);
	uint64_t version=AEUInt64From8Bytes(buffer, true);
	if(version not_eq 1)
		AEError("Invalid version, only 1 is accepted.");
	AEVADeinit(va);
	memset(va, 0, sizeof(AEVA));
	
	fread(buffer, 1, sizeof(uint64_t), file);
	uint64_t length=AEUInt64From8Bytes(buffer, true);
	
	fread(buffer, 1, 4, file);
	va->isAnIndexArray=buffer[0];
	va->vbotype=buffer[1];
	va->tunit=buffer[2];
	va->dataformat=buffer[3];
	
	
	void* memory=AEVAMap(va, length, GL_WRITE_ONLY);
	
	if(not va->isAnIndexArray)
		fread(memory, sizeof(float), va->length, file);
	else{
		uint32_t* ints=memory;
		for (size_t i=0; i<va->length; i++) {
			fread(buffer, sizeof(uint32_t), 1, file);
			ints[i]=AEUInt32From4Bytes(buffer, true);
		}
	}
	
	AEVAUnmap(va);
}