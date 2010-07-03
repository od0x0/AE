#include "../AE/Core.h"
#include "../AE/Camera.h"
#include "../AE/VBO.h"
#include "../AE/RawMesh.h"
#include "../AE/VertexIndexer.h"
//#include "../AE/FOI.h"

AEVBO* Model=NULL;
unsigned int Texture=0;
AEVec3 Positions[]={{0,0,-10}};
AERawMesh* Mesh=NULL;
AEVA TriangleVA,TriangleIndices;

void perframe(float step){
	AETextureBind(Texture);
	//AEVBOBind(Model);
	//puts("Bound");
	static float delta=0;
	delta+=10*step;
	for(int x=0;x<6;x++){
		for(int y=0;y<6;y++){
			glPushMatrix();
			glTranslatef(x*2-3,y*2-3,0);
			glRotatef(delta,0,1,0);
			//AEVBODraw(Model);
			glPopMatrix();
		}
	}
	//AEVBODraw(Model);
	//AEVBODraw(Model);
	//puts("Drew it");
	//AEFOIRender(Model,Texture,1,Positions);
	//glTranslatef(0,2,0);
	//glRotatef(delta,0,1,0);
	//AEVABindInterleavedTV(&TriangleVA);
	//AEVABindIndices(&TriangleIndices);
	//AEVADraw(0,TriangleIndices.length);
	AEVABindInterleavedTV(&(Model->va));
	//AEVABindIndices(&(Model->indices));
	AEVABindIndices(NULL);
	AEVADraw(0,Model->indices.length);
	//AEVBODraw(Model);
	//AERawMeshDraw(Mesh);
}

/*void VBOCompile(AEVBO* vbo){
	if(vbo==NULL) return;
	//AEVBOAdd(vbo,NULL);//Implicit
	
	void* data=NULL;
	
	float* vertData=AEVAMap(&(vbo->va),AEListLength(vbo->vlist),GL_WRITE_ONLY);
	int numberoffloats=vbo->hasNormals*3+vbo->texUnitCount*2+3;
	printf("%i\n",numberoffloats);
	for(unsigned int i=0;i<(AEListLength(vbo->vlist)*numberoffloats);i++){
		vertData[i]=AEListAsArray(vbo->vlist,float)[i];
	}
	//memcpy(data,vbo->vlist->data,vbo->vlist->typeSize*vbo->vlist->length);
	AEVAUnmap(&(vbo->va));
	
	data=AEVAMap(&(vbo->indices),AEListLength(vbo->ilist),GL_WRITE_ONLY);
	//memcpy(data,vbo->ilist->data,vbo->ilist->typeSize*vbo->ilist->length);
	AEVAUnmap(&(vbo->indices));
}
*/

void VBOCompile(AEVBO* vbo,size_t length,float* vdata,size_t ilength,unsigned int* idata){
	if(vbo==NULL) return;
	
	vbo->indices.length=(length/vbo->elementSize)/3;//AEListLength(list)/3;
	
	if(vbo->usesIndices){
		//AEList* indicesList=AEListRemoveDuplicatesAndReturnIndices(list);
		unsigned int* data=AEVAMap(&(vbo->indices),ilength,GL_WRITE_ONLY);
		
		for(size_t i=0;i<ilength;i++)
			data[i]=idata[i];
		
		AEVAUnmap(&(vbo->indices));
		
		//AEListDelete(indicesList);
	}
	
	float* data=AEVAMap(&(vbo->va),length,GL_READ_WRITE);
	printf("");
	memcpy(data,vdata,length);
	/*float* vdata=AEListAsArray(list,float);
	for(size_t i=0;i<AEListLength(list);i++)
			data[i]=vdata[i];*/
	AEVAUnmap(&(vbo->va));
	
	//AEListDelete(list);
}

int main(int argc,char** argv){
	printf("%s() Line %i\n",__func__,__LINE__);
	AEInit("Window",800,500);
//	glDisable(GL_CULL_FACE);
	printf("%s() Line %i\n",__func__,__LINE__);
	glClearColor(1,1,1,1);
	//Code here
	printf("%s() Line %i\n",__func__,__LINE__);
	AECamera* cam=AECameraActiveGet();
	cam->z=10;
	//AECameraActive.z=10;
	printf("%s() Line %i\n",__func__,__LINE__);
	//Model=AEVBOLoad("/Users/Oliver/Documents/Dev/Projects/AE/Junk/Old/Tests/Data/TexturedCube.obj",0,1,0,"static");
	//AEVBOLoad("/Users/Oliver/Documents/Dev/Projects/AE/Junk/Old/Tests/Data/TexturedCube.obj","v: 1 n: 1 t: 1 i: 1 static");
	//AEVBOLoad("/Users/Oliver/Documents/Dev/Projects/AE/Junk/Old/Tests/Data/TexturedCube.obj",1,1);
	//AEVBOCompile(Model);//,NULL);
	printf("%s() Line %i\n",__func__,__LINE__);
	Texture=AETextureLoad("/Users/Oliver/Documents/Dev/Projects/AE/Junk/Old/Tests/Data/Texture.png");
	printf("%s() Line %i\n",__func__,__LINE__);
	Mesh=AERawMeshLoad("/Users/Oliver/Documents/Dev/Projects/AE/Junk/Old/Tests/Data/TexturedCube.obj");
	printf("%s() Line %i\n",__func__,__LINE__);
	
	
	unsigned int indices[]={2,1,0,0,3,2};
	float vertexdata[]={0,0, 0,0,0, 0,1, 0,1,0, 1,1, 1,1,0, 1,0, 1,0,0};
	
	printf("%i %i",sizeof(float),sizeof(unsigned int));
	
	Model=AEVBONew(0,1,1,"static");
	AEVertexIndexerData* vidata=AEVertexIndexerDataLoad("/Users/Oliver/Documents/Dev/Projects/AE/Junk/Old/Tests/Data/TexturedCube.obj",1,0);
	VBOCompile(Model,vidata->vcount*vidata->elementSize,vidata->verts,vidata->icount,vidata->indices);
	
	/*float v[5];
	AEList* vlist=AEListNewWithTypeSize(AEVBOVertexTypeSize(Model));
	for(int i=0;i<(sizeof(indices)/sizeof(indices[0]));i++){
		int j=indices[i];
		for(int k=0;k<5;k++)
			v[k]=vertexdata[j*5+k];
		
		float* to=AEListCheck(vlist);
		for(int byte=0;byte<5;byte++){
			to[byte]=v[byte];
		}
		
		//AEListAddBytes(vlist,v);
	}
	//VBOCompile(Model,vlist->data,(sizeof(indices)/sizeof(indices[0]))*5);
	//AEVBOCompileVertexList(Model,vlist);
	
	Model->indices.length=AEListLength(vlist)/3;*/
	
	
	
	//TriangleVA.vbotype=AEVAVBOTypeStream;
	//volatile void* memory=AEVAMap(&TriangleVA,Model->indices.length,GL_WRITE_ONLY);
	//memcpy(memory,vertexdata,sizeof(vertexdata));
	//memcpy(memory,vlist->data,AEListLength(vlist)*AEListTypeSize(vlist));
	//AEVAUnmap(&TriangleVA);
	//Model->va=TriangleVA;
	
	/*TriangleIndices.vbotype=AEVAVBOTypeStream;
	memory=AEVAMap(&TriangleIndices,sizeof(indices)/sizeof(unsigned int),GL_WRITE_ONLY);
	memcpy(memory,indices,sizeof(indices));
	AEVAUnmap(&TriangleIndices);
	Model->indices=TriangleIndices;*/
	
	AEStart(perframe);
	return 1;
}
