#include "../Geo.h"

AEGeo* AEGeoActive=NULL;

void AEGeoSegmentDraw(AEGeoSegment seg){
	if(AEGeoActive->indices) glDrawElements(GL_TRIANGLES, seg.indexEnd-seg.indexStart, GL_UNSIGNED_INT, (AEGeoActive->ibo?NULL:AEGeoActive->indices)+seg.indexStart);
	else glDrawArrays(GL_TRIANGLES,seg.indexStart*3,(seg.indexEnd-seg.indexStart)*3);
}

void AEGeoBind(AEGeo* geo,AEShader* shader){
	glDisableClientState(GL_NORMAL_ARRAY);
	
	AEGeoActive=geo;
	
	if(geo==NULL){
		glBindBuffer(GL_ARRAY_BUFFER,0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
		return;
	}
	
	glBindBuffer(GL_ARRAY_BUFFER,geo->vbo);
	
	glEnableClientState(GL_NORMAL_ARRAY);
	
	const unsigned int offset=(unsigned int)(geo->vbo?NULL:geo->verts);
	glVertexPointer(3,GL_FLOAT,sizeof(AEGeoVert),(void*)((size_t)offset+(size_t)&(((AEGeoVert*)NULL)->x)));
	glTexCoordPointer(2,GL_FLOAT,sizeof(AEGeoVert),(void*)((size_t)offset+(size_t)&(((AEGeoVert*)NULL)->t.x)));
	glClientActiveTexture(GL_TEXTURE1);
	glTexCoordPointer(2,GL_FLOAT,sizeof(AEGeoVert),(void*)((size_t)offset+(size_t)&(((AEGeoVert*)NULL)->lt.x)));
	
	if(shader){
		glClientActiveTexture(GL_TEXTURE2);
		glTexCoordPointer(2,GL_FLOAT,sizeof(AEGeoVert),(void*)((size_t)offset+(size_t)&(((AEGeoVert*)NULL)->t.x)));
	}
	
	glClientActiveTexture(GL_TEXTURE0);
	glNormalPointer(GL_FLOAT,sizeof(AEGeoVert),(void*)((size_t)offset+(size_t)&(((AEGeoVert*)NULL)->n.x)));
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,geo->ibo);
}

AEGeo* AEGeoLoad(char* path){
	if(path){
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		AEGeo* geo=calloc(1,sizeof(AEGeo));
		geo->indices=NULL;
		return geo;
	}
	
	return NULL;
}

void AEGeoDelete(AEGeo* geo){
	if(geo==NULL) return;
	glDeleteBuffers(1,(GLuint*)&geo->vbo);
	glDeleteBuffers(1,(GLuint*)&geo->ibo);
	free(geo->verts);
	free(geo->indices);
	free(geo);
}

static unsigned int AEGeoAddVert_internal(AEGeo* geo,AEGeoVert* vert){
	if(vert==NULL){//  Resize the array to fit
		geo->vallocated=0;
		geo->verts=(AEGeoVert*)realloc(geo->verts,sizeof(AEGeoVert)*geo->vcount);
		return 0xbaadf00d;//Junk return, you shouldn't do anything with this anyway
	}
	
	//Check for pre-existing, return it if it is
	unsigned int index=0;
	
	if(geo->indices) index=AELinearSearch(vert,geo->verts,geo->vcount);;
	
	if(index) return index-1;
	
	//Add if it doesn't exist
	//Resize array
	index=geo->vcount++;
	if(geo->vallocated==0 || geo->vallocated <= (geo->vcount+1)){//Allocates more than needed to avoid expensive realloc calls
		geo->vallocated=geo->vcount+AEVBOAddAllocateFreq;
		geo->verts=(AEGeoVert*)realloc(geo->verts,sizeof(AEGeoVert)*geo->vallocated);
	}
	//Make assignment
	geo->verts[index]=*vert;
	
	return index;
}

unsigned int AEGeoAddVert(AEGeo* geo,AEGeoVert* vert){
	if(geo==NULL) return 0;
	if(vert==NULL){
		geo->iallocated=0;
		if(geo->indices) geo->indices=(unsigned int*)realloc(geo->indices,sizeof(int)*geo->icount);
		AEGeoAddVert_internal(geo,NULL);
		return 0;
	}
	
	if(geo->indices==NULL) return AEGeoAddVert_internal(geo,vert)/3;
	
	if(geo->iallocated==0 || geo->iallocated <= (geo->icount+1)){
		geo->iallocated=geo->icount+AEVBOAddAllocateFreq;
		geo->indices=(unsigned int*)realloc(geo->indices,sizeof(int)*geo->iallocated);
	}
	
	return 1+(geo->indices[geo->icount++]=AEGeoAddVert_internal(geo,vert));
}

void AEGeoCompile(AEGeo* geo,unsigned int isStreamed){
	if(geo==NULL) return;
	if(geo->iallocated||geo->vallocated) AEGeoAddVert(geo,NULL);//Implicit
	
	geo->indices=malloc(1);
	AEGeoVert* oldVerts=geo->verts;
	unsigned int vcount=geo->vcount;
	geo->vallocated=0;
	geo->vcount=0;
	geo->verts=NULL;
	
	geo->indices=malloc(1);//Just to avoid a null value so AddVerts knows to generate indices
	
	for(unsigned int i=0;i<vcount;i++) AEGeoAddVert(geo,oldVerts+i);
	
	AEGeoAddVert(geo,NULL);
	
	free(oldVerts);
	
	//Vertices
	if(geo->vbo) glGenBuffers(1,(GLuint*)&geo->vbo);
	glBindBuffer(GL_ARRAY_BUFFER,geo->vbo);
	glBufferData(GL_ARRAY_BUFFER,geo->vcount*sizeof(AEGeoVert),geo->verts,isStreamed?GL_STREAM_DRAW:GL_STATIC_DRAW);
	
	//Indices
	if(geo->ibo) glGenBuffers(1,(GLuint*)&geo->ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,geo->ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,geo->icount*sizeof(unsigned int),geo->indices,GL_STATIC_DRAW);
}