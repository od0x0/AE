#include "../AE/Core.h"
#include "../AE/Camera.h"
#include "../AE/VA.h"
#include "../AE/VBO.h"
#include "../AE/List.h"

unsigned int Texture=0;
AEVBO* VBO=NULL;
AERawMesh* RawMesh=NULL;
AEList* VertexList=NULL;

void perframe(float step){
	static float delta=0;
	delta+=10*step;
	delta=AEActiveState.mouse.y;
	glRotatef(AEActiveState.mouse.y*0.5,1,0,0);
	glRotatef(AEActiveState.mouse.x*0.5,0,1,0);
	AETextureBind(Texture);
	//AERawMeshDraw(RawMesh);
	//glTranslatef(0,2,0);
	AEVBOBind(VBO);
	AEVBODraw(VBO);
	//AEVABindInterleavedTV(&(VBO->va));
	//AEVABindIndices(&(VBO->indices));
	//AEVADraw(0,VBO->indices.length);
}

typedef struct{
	float u,v,x,y,z;
}Vertex;

int main(int argc,char** argv){
	AEInit("Window",800,500);
	//glDisable(GL_CULL_FACE);
	//glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	AECamera* cam=AECameraActiveGet();
	cam->z=10;
	
	Texture=AETextureLoad("/Users/Oliver/Documents/Dev/Projects/AE/Junk/Old/Tests/Data/Texture.png");
	RawMesh=AERawMeshLoad("/Users/Oliver/Documents/Dev/Projects/AE/Junk/Old/Tests/Data/TexturedCube.obj");
	VBO=AEVBOLoad("/Users/Oliver/Documents/Dev/Projects/AE/Junk/Old/Tests/Data/TexturedCube.obj",1,1,0,"static");
	/*
	AERawMesh* m=RawMesh;
	
	//printf("Size of VBO Vertex %i\n",sizeof(Vertex));
	
	printf("Size of Vertex %i\n",sizeof(Vertex));
	
	VertexList=AEListNew(Vertex);
	
	for(unsigned int i=0;i < m->count.f;i++){
		AERawMeshFace* face=m->f+i;
		for(int j=0;j<3;j++){
			//float* v=alloca(AEVBOVertexTypeSize(vbo));
			//int size=0;
			//for(int k=0;k<(vbo->texUnitCount*2);k+=2){
			//	v[size++]=m->t[face->t[j]].x;
			//	v[size++]=m->t[face->t[j]].y;
			//}
			//if(vbo->hasNormals){
			//	v[size++]=m->n[face->n[j]].x;
			//	v[size++]=m->n[face->n[j]].y;
			//	v[size++]=m->n[face->n[j]].z;
			//}
			//v[size++]=m->v[face->v[j]].x;
			//v[size++]=m->v[face->v[j]].y;
			//v[size++]=m->v[face->v[j]].z;
			//AEListAddBytes(VertexList,v);
			
			Vertex v;
			v.u=m->t[face->t[j]].x;
			v.v=m->t[face->t[j]].y;
			v.x==m->v[face->v[j]].x;
			v.y==m->v[face->v[j]].y;
			v.z==m->v[face->v[j]].z;
			AEListAdd(VertexList,Vertex,v);
		}
	}
	
	AEVBO* vbo=AEVBONew(0,1,1,"static");
	size_t length=AEListLengthInSizeofType(VertexList,float);//*vbo->elementSize*AEListLength(VertexList);
	float* data=AEListAsArray(VertexList,float);
	printf("Begin Dump %i:  ",(int)length/5);
	for(size_t i=0;i<length;i++){
		printf("%.2f ",data[i]);
	}
	puts("");
	AEVBOCompileVertexList(vbo,VertexList);
	length=vbo->va.length;
	data=AEVAMap(&(vbo->va),length,GL_READ_WRITE);
	printf("Begin Dump %i:  ",(int)length/5);
	for(size_t i=0;i<length;i++){
		printf("%.2f ",data[i]);
	}
	puts("");
	AEVAUnmap(&(vbo->va));
	length=vbo->indices.length;
	unsigned int* idata=AEVAMap(&(vbo->indices),length,GL_READ_WRITE);
	printf("Begin Index Dump %i:  ",(int)length);
	for(size_t i=0;i<length;i++){
		printf("%i ",idata[i]);
	}
	puts("");
	AEVAUnmap(&(vbo->va));
	VBO=vbo;*/
	//for()
	
	//VBO=AEVBONew(1,);
	
	AEStart(perframe);
	return 1;
}
