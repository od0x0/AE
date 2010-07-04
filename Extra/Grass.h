#pragma once
#include "../AE/Core.h"
#include "../AE/VA.h"

typedef struct{
	float u,v,x,y,z;
}GrassVertex;

typedef struct{
	//GrassVertex* templateVerts;
	//unsigned short templateVertCount;
	int locationCount;
	float fractionUsed;
	AEVec3* locations;
	unsigned int texture;
	AEVA va;
}GrassPatch;

static GrassVertex GrassTemplate[]={
		{0,0,-0.5,0,0},{1,0,0.5,0,0},{1,1,0.5,1,0},{0,1,-0.5,1,0},
		{0,0,0,0,-0.5},{1,0,0,0,0.5},{1,1,0,1,0.5},{0,1,0,1,-0.5}
};
static int GrassVertCount=8;//sizeof(GrassTemplate)/sizeof(GrassVertex);

GrassPatch* GrassPatchNew(AEVec3* locations,int locationCount,unsigned int texture){
	GrassPatch* patch=calloc(1,sizeof(GrassPatch));
	patch->va=AEVANew(0,0);//AEVAVBOTypeDynamic);
	patch->locationCount=locationCount;
	patch->locations=locations;
	patch->texture=texture;
	patch->fractionUsed=1;
	AEVAMap(&(patch->va),5*GrassVertCount*locationCount,GL_WRITE_ONLY);
	AEVAUnmap(&(patch->va));
	return patch;
}

static inline void GrassPatchUpdate(GrassPatch* patch,float fractionUsed){
	int numberofverts=(patch->va.length/5)/*fractionUsed*/;
	//patch->fractionUsed=fractionUsed;
	printf("%s() %i\n",__func__,__LINE__);
	GrassVertex* vaverts=AEVAMap(&(patch->va),patch->va.length,GL_WRITE_ONLY);
	printf("%s() %i\n",__func__,__LINE__);
	for(int i=0;i<numberofverts;i++){
		AEVec3 location=patch->locations[i];
		for(int j=0;j<GrassVertCount;j++){
			vaverts[i*GrassVertCount+j]=GrassTemplate[j];
			vaverts[i*GrassVertCount+j].x+=location.x;
			vaverts[i*GrassVertCount+j].y+=location.y;
			vaverts[i*GrassVertCount+j].z+=location.z;
		}
	}
	printf("%s() %i\n",__func__,__LINE__);
	AEVAUnmap(&(patch->va));
}

static inline void GrassPatchRender(GrassPatch* patch){
	AEVABindIndices(NULL);
	AEVABindInterleavedTV(&(patch->va));
	
	unsigned int quadCount=(patch->va.length/5)/4;
	
	glDrawArrays(GL_QUADS,0, quadCount /* patch->fractionUsed*/);
}