#include "../VA.h"

AEVA AEVANew(int isAnIndexArray,int vbotype){
	AEVA va;
	memset(&va,0,sizeof(AEVA));
	va.isAnIndexArray=1;
	va.vbotype=vbotype;
	return va;
}

void* AEVAMap(AEVA* va, unsigned int length,unsigned int writereadmode){
	unsigned int arrayType=va->isAnIndexArray?GL_ELEMENT_ARRAY_BUFFER:GL_ARRAY_BUFFER;
	if(va->length==0){
		va->length=length;
		if(va->vbotype){
			glGenBuffers(1, &va->data.vbo);
			glBindBuffer(GL_ARRAY_BUFFER, va->data.vbo);
			
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
			return glMapBuffer(arrayType, writereadmode);
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
	if(va->vbotype) glUnmapBuffer(va->isAnIndexArray?GL_ELEMENT_ARRAY_BUFFER:GL_ARRAY_BUFFER);
}

void AEVAClear(AEVA* va){
	if(va->length){
		if(va->vbotype)
			glDeleteBuffers(1, &va->data.vbo);
		else
			free(va->data.data);
	}
	va->length=0;
}

void AEVABindVertex(AEVA* va){
	if(va==NULL){
		glDisableClientState(GL_VERTEX_ARRAY);
		return;
	}
	glEnableClientState(GL_VERTEX_ARRAY);
	void* offset = NULL;
	if(va->vbotype) glBindBuffer(GL_ARRAY_BUFFER, va->data.vbo);
	else offset=va->data.data;
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
	else offset=va->data.data;
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
	else offset=va->data.data;
	glNormalPointer(GL_FLOAT, 0, offset);
}

void AEVABindInterleavedTNV(AEVA* va){
	if(va==NULL) return;
	void* offset=NULL;
	if(va->vbotype) glBindBuffer(GL_ARRAY_BUFFER, va->data.vbo);
	else offset=va->data.data;
	glInterleavedArrays(GL_T2F_N3F_V3F,0,offset);
}

void AEVABindInterleavedTV(AEVA* va){
	if(va==NULL) return;
	void* offset=NULL;
	if(va->vbotype) glBindBuffer(GL_ARRAY_BUFFER, va->data.vbo);
	else offset=va->data.data;
	glInterleavedArrays(GL_T2F_V3F,0,offset);
}

static unsigned long* CurrentIndices;
//static unsigned int IndicesCount=0;
static char UseIndices=0;

void AEVABindIndices(AEVA* va){
	//IndicesCount=va->length;
	
	if(va==NULL){
		glDisableClientState(GL_INDEX_ARRAY);
		UseIndices=0;
		return;
	}
	glEnableClientState(GL_INDEX_ARRAY);
	CurrentIndices=NULL;
	if(va->vbotype) glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, va->data.vbo);
	else CurrentIndices=va->data.data;
	UseIndices=1;
}

void AEVADraw(unsigned long start, unsigned long end){
	//if(start==0 && end==0) end=IndicesCount;
	
	if(UseIndices) glDrawElements(GL_TRIANGLES, end - start, GL_UNSIGNED_INT,CurrentIndices+start);
	else glDrawArrays(GL_TRIANGLES,start,end);
}
