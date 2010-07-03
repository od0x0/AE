#include "../VertexIndexer.h"

// This code could have been cleaner, particularly with an array "class"

static AEVertexIndexerData* AEVertexIndexerDataNew(void){
	return (AEVertexIndexerData*)calloc(1,sizeof(AEVertexIndexerData));
}

unsigned char AEVertexIndexerDataAddAllocateFreq=100;

////  Really ugly code, this is the part where I say, "It works"
static unsigned int AEVertexIndexerDataAddVert(AEVertexIndexerData* vid,float* v){
	if(v==NULL){//  Resize the array to fit
		vid->vallocated=0;
		vid->verts=realloc(vid->verts,sizeof(float)*vid->elementSize*vid->vcount);
		return 0xbaadf00d;//Junk return, you shouldn't do anything with this anyway
	}
	
	//Check for pre-existing, return it if it is
	unsigned int index=0;
	/*index=AELinearSearch(&vert,vbo->verts,vbo->vcount);
	if(vbo->hasNormals){
		index=AELinearSearch(&(v->n),vbo->v,vbo->vcount);
	}*/
	for(unsigned int i=0;i<vid->vcount;i++){
		if(memcmp(vid->verts+i*vid->elementSize,v,sizeof(float)*vid->elementSize*vid->vcount)==0){
			index=i+1;
			break;
		}
	}
	
	if(index) return index-1;
	//Add if it doesn't exist
	//Resize array
	index=vid->vcount++;
	if(vid->vallocated==0 || vid->vallocated <= (vid->vcount+1)){//Allocates more than needed to avoid expensive realloc calls
		vid->vallocated=vid->vcount+AEVertexIndexerDataAddAllocateFreq;
		vid->verts=realloc(vid->verts,sizeof(float)*vid->elementSize*vid->vallocated);
	}
	//Make assignment
	memcpy(vid->verts+vid->elementSize*index,v,sizeof(float)*vid->elementSize);
	
	return index;
}

void AEVertexIndexerDataAdd(AEVertexIndexerData* vid,float* v){//Pretty much the same concept as above, just for indices, and wraps the above
	if(vid==NULL) return;
	if(v==NULL){
		vid->iallocated=0;
		vid->indices=(unsigned int*)realloc(vid->indices,sizeof(unsigned int)*vid->icount);
		AEVertexIndexerDataAddVert(vid,v);
		return;
	}
	
	if(vid->iallocated==0 || vid->iallocated <= (vid->icount+1)){
		vid->iallocated=vid->icount+AEVertexIndexerDataAddAllocateFreq;
		vid->indices=(unsigned int*)realloc(vid->indices,sizeof(int)*vid->iallocated);
	}
	
	vid->indices[vid->icount++]=AEVertexIndexerDataAddVert(vid,v);
}

static AEVertexIndexerData* AEVertexIndexerDataFromRawMesh(AERawMesh* m,int texCoordCount,int hasNormals){
	AEVertexIndexerData* vid=AEVertexIndexerDataLoad(NULL,0,0);
	vid->hasNormals=hasNormals;
	vid->texCoordCount=texCoordCount;
	for(int i=0; i<m->count.f; i++){
		AERawMeshFace* f=&m->f[i];
		for (char j = 0; j<3; j++) {
			//AEVertexIndexerDataVertWithNormal v;
			float* v=alloca(sizeof(float)*vid->elementSize);
			int size=0;
			if(vid->texCoordCount){
				v[size++]=m->t[f->t[j]].x;
				v[size++]=m->t[f->t[j]].y;
				for(int k=0;k<(vid->texCoordCount-1);k++) size+=2;
			}
			
			if(vid->hasNormals){
				v[size++]=m->n[f->n[j]].x;
				v[size++]=m->n[f->n[j]].y;
				v[size++]=m->n[f->n[j]].z;
			}
			
			v[size++]=m->v[f->v[j]].x;
			v[size++]=m->n[f->n[j]].y;
			v[size++]=m->n[f->n[j]].z;
			
			AEVertexIndexerDataAdd(vid,v);
		}
	}
	AEVertexIndexerDataAdd(vid,NULL);
	return vid;
}


AEVertexIndexerData* AEVertexIndexerDataLoad(const char* filename,int texCoordCount,int hasNormals){
	if(filename==NULL){
		AEVertexIndexerData* vid=AEVertexIndexerDataNew();
		vid->hasNormals=hasNormals;
		vid->texCoordCount=texCoordCount;
		return vid;
	}
	//puts(".obj loading not yet implemented");
	//return NULL;
	AERawMesh* m=AERawMeshLoad(filename);
	AEVertexIndexerData* vid=AEVertexIndexerDataFromRawMesh(m,texCoordCount,hasNormals);
	AERawMeshDelete(m);
	return vid;
}

void AEVertexIndexerDataDelete(AEVertexIndexerData* vid){
	if(vid==NULL) return;
	free(vid->verts);
	free(vid->indices);
	free(vid);
}