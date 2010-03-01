#include "../Core.h"
#include "../Octa.h"
#include "../VFChecking.h"
#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL_opengl.h>

static char AEOctaIsVisible(AEOcta* octa){
	AEVec3 center={octa->size/2.0f+octa->x,octa->size/2.0f+octa->y,octa->size/2.0f+octa->z};
	return AEVFCheck(&center,(octa->size/2)*1.74);
}

void AEOctaDraw(AEOcta* octa){
	for(unsigned char i=0;i<octa->count;i++) {
		AETextureBind(octa->textures[i]);
		AEVBODraw(octa->vbos[i]);
	}
}

void AEOctaRender(AEOcta* octa){
	if(octa==NULL){
		AEObjectsSignal(AEObjectEventRender,octa);
		return;
	}
	
	if(AEOctaIsVisible(octa)){
		AEOctaDraw(octa);
		if(octa->children) for(char i=0;i<8;i++) AEOctaRender(&octa->children[i]);
	}
}

static inline void VBOAppend(AEVBO* vbo,AEVBO* vbo2){
//	printf("vbo2->indices %p\n",vbo2->indices);
//	printf("vbo2->verts %p",vbo2->verts);
	
	if(!(vbo->hasNormals&&vbo2->hasNormals)) {
		puts("VBOAppend: Both must have normals!!!");
		return;
	}
	printf("File %s Line %i Reached\n",__FILE__,__LINE__);
	AEVBOVert* verts=vbo2->verts;
	unsigned int* indices=vbo2->indices;
	if(!verts){	
		printf("File %s Line %i Reached\n",__FILE__,__LINE__);
		glBindBuffer(GL_ARRAY_BUFFER,vbo2->vbo);
		printf("File %s Line %i Reached\n",__FILE__,__LINE__);
		verts=glMapBuffer(GL_ARRAY_BUFFER,GL_READ_ONLY);
		printf("File %s Line %i Reached\n",__FILE__,__LINE__);
	}
	if(!indices){	
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vbo2->ibo);
		printf("File %s Line %i Reached\n",__FILE__,__LINE__);
		indices=glMapBuffer(GL_ELEMENT_ARRAY_BUFFER,GL_READ_ONLY);
		printf("File %s Line %i Reached\n",__FILE__,__LINE__);
	}
	
	for(unsigned int i=0;i<vbo2->icount;i++)
		AEVBOAdd(vbo,&verts[indices[i]]);
		printf("File %s Line %i Reached\n",__FILE__,__LINE__);
		
	if(!vbo2->verts){	
		glBindBuffer(GL_ARRAY_BUFFER,vbo2->vbo);
		printf("File %s Line %i Reached\n",__FILE__,__LINE__);
		glUnmapBuffer(GL_ARRAY_BUFFER);
		printf("File %s Line %i Reached\n",__FILE__,__LINE__);
	}
	if(!vbo2->indices){	
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vbo2->ibo);
		printf("File %s Line %i Reached\n",__FILE__,__LINE__);
		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
		printf("File %s Line %i Reached\n",__FILE__,__LINE__);
	}
	//AEVBOAdd(vbo,NULL);
	//if(vbo->vbo){
		//glDeleteBuffers(1,(GLuint*)&vbo->vbo);
		//glDeleteBuffers(1,(GLuint*)&vbo->ibo);
		//AEVBOCompile(vbo,usages);
	//}
}

void AEOctaCompile(AEOcta* octa,unsigned int* vboUsages){
	for(int i=0;i<octa->count;i++){
		AEVBO* vbo=octa->vbos[i];
		AEVBOAdd(vbo,NULL);
		if(vbo->vbo){
			glDeleteBuffers(1,(GLuint*)&vbo->vbo);
			glDeleteBuffers(1,(GLuint*)&vbo->ibo);
			AEVBOCompile(vbo,vboUsages);
		}
	}
}

void AEOctaAddVBO(AEOcta* octa,unsigned int texture,AEVBO* vbo){
	
	unsigned int index=0;
	/*for(int i=0;i<octa->count;i++) if(texture==octa->textures[i]){index=i+1;break;}
	
	if(index){
		index--;
		
		VBOAppend(octa->vbos[index],vbo);
		
		return;
	}*/
	
	index=octa->count++;
	
	octa->textures=(unsigned int*)realloc(octa->textures,octa->count*sizeof(int));
	
	octa->textures[index]=texture;
	
	octa->vbos=(AEVBO**)realloc(octa->vbos,octa->count*sizeof(AEVBO*));
	
	octa->vbos[index]=vbo;//AEVBOLoad(NULL,1,1);
	
	
	/*AEVBO* temp=octa->vbos[index];
	temp->hasNormals=1;  //AEVBOLoad(NULL,1,1); doesn't set hasNormals!
	
	VBOAppend(temp,vbo);*/
	
}

AEOcta* AEOctaAddChildren(AEOcta* octa){
	if(octa==NULL) return (AEOcta*)calloc(1,sizeof(AEOcta));
	if(octa->children) return octa->children;
	octa->children=(AEOcta*)calloc(8,sizeof(AEOcta));
	static float positions[8][3]={{0,0,0},{1,0,0},{0,0,1},{1,0,1},
					{0,1,0},{1,1,0},{0,1,1},{1,1,1}};

	for(char i=0;i<8;i++){
		AEOcta* child=&octa->children[i];
		child->size=octa->size*0.5f;
		child->x=positions[i][0]*child->size;
		child->y=positions[i][1]*child->size;
		child->z=positions[i][2]*child->size;
	}
	return octa->children;
}
 
void AEOctaDelete(AEOcta* octa){
	if(!octa) return;
	for(unsigned char i=0;i<octa->count;i++)  AEVBODelete(octa->vbos[i]);
	free(octa->vbos);
	free(octa->textures);
	
	for(char i=0;i<8;i++) AEOctaDelete(&octa->children[i]);
	if(octa->children) free(octa->children);
}

static inline int Vec3Between(AEVec3* p,AEVec3*min,AEVec3* max){return (min->x < p->x && min->y < p->y && min->z < p->z && p->x < max->x && p->y < max->y && p->z < max->z);}
AEOcta* AEOctaGetAt(AEOcta* octa,float x,float y,float z){
	AEVec3 points[2]={{x,y,z},{octa->x+octa->size,octa->y+octa->size,octa->z+octa->size}};
	if(!Vec3Between(&points[0],(AEVec3*)octa,&points[1])) return NULL;
	
	if(!octa->children) return octa;
	AEOcta* subOcta;
	for(char i=0;i<8;i++) if(subOcta=AEOctaGetAt(&octa->children[i],x,y,z)) return subOcta;
	
	printf("AEOcta index error for %p at %.2f,%.2f,%.2f\n",octa,x,y,z);
	return NULL;//Logically impossible to get here, but if I messed up, it should return this.
}