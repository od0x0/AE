// This code could have been cleaner, particularly with an array "class"

static AEVBO* AEVBONew(void){
	return (AEVBO*)calloc(1,sizeof(AEVBO));
}

unsigned char AEVBOAddAllocateFreq=100;

////  Really ugly code, this is the part where I say, "It works"
static unsigned int AEVBOAddVert(AEVBO* vbo,AEVBOVert* v){
	if(v==NULL){//  Resize the array to fit
		vbo->vallocated=0;
		if(vbo->hasNormals){
			vbo->verts=(AEVBOVert*)realloc(vbo->verts,sizeof(AEVBOVert)*vbo->vcount);
		}else{
			vbo->verts=(AEVBOVert*)realloc(vbo->verts,sizeof(AEVBOVertWithoutNormal)*vbo->vcount);
		}
		return 0xbaadf00d;//Junk return, you shouldn't do anything with this anyway
	}
	
	//Check for pre-existing, return it if it is
	unsigned int index=0;
	if(vbo->hasNormals){
		index=AELinearSearch(v,vbo->verts,vbo->vcount);
	}else{
		AEVBOVertWithoutNormal vert={v->t,v->v};
		index=AELinearSearch(&vert,((AEVBOVertWithoutNormal*) vbo->verts),vbo->vcount);
	}
	
	if(index) return index-1;
	//Add if it doesn't exist
	//Resize array
	index=vbo->vcount++;
	if(vbo->vallocated==0 || vbo->vallocated <= (vbo->vcount+1)){//Allocates more than needed to avoid expensive realloc calls
		vbo->vallocated=vbo->vcount+AEVBOAddAllocateFreq;
		if(vbo->hasNormals){
			vbo->verts=(AEVBOVert*)realloc(vbo->verts,sizeof(AEVBOVert)*vbo->vallocated);
		}else{
			vbo->verts=(AEVBOVert*)realloc(vbo->verts,sizeof(AEVBOVertWithoutNormal)*vbo->vallocated);
		}
	}
	//Make assignment
	if(vbo->hasNormals){
		vbo->verts[index]=*v;
	}else{
		AEVBOVertWithoutNormal vert={v->t,v->v};
		((AEVBOVertWithoutNormal*) vbo->verts)[index]=vert;
	}
	
	return index;
}

void AEVBOAdd(AEVBO* vbo,AEVBOVert* v){//Pretty much the same concept as above, just for indices, and wraps the above
	if(v==NULL){
		vbo->iallocated=0;
		vbo->indices=(unsigned int*)realloc(vbo->indices,sizeof(int)*vbo->icount);
		AEVBOAddVert(vbo,v);
		return;
	}
	
	if(vbo->iallocated==0 || vbo->iallocated <= (vbo->icount+1)){
		vbo->iallocated=vbo->icount+AEVBOAddAllocateFreq;
		vbo->indices=(unsigned int*)realloc(vbo->indices,sizeof(int)*vbo->iallocated);
	}
	
	vbo->indices[vbo->icount++]=AEVBOAddVert(vbo,v);
}

void AEVBODraw(AEVBO* vbo){
	if(!vbo) return;
	glBindBuffer(GL_ARRAY_BUFFER,vbo->vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vbo->ibo);
	
	glInterleavedArrays(vbo->hasNormals?GL_T2F_N3F_V3F:GL_T2F_V3F,0,vbo->vbo?NULL:vbo->verts);
	
	if(vbo->indices || vbo->ibo) glDrawElements(GL_TRIANGLES, vbo->icount, GL_UNSIGNED_INT, vbo->ibo?NULL:vbo->indices);
	else glDrawArrays(GL_TRIANGLES,vbo->vcount,GL_UNSIGNED_INT);
}

void AEVBOCompile(AEVBO* vbo,unsigned int* usages){
	if(vbo->iallocated||vbo->vallocated) AEVBOAdd(vbo,NULL);//Implicit
	
	unsigned int defaultUsages[2]={GL_STATIC_DRAW,GL_STATIC_DRAW};
	if(usages==NULL) usages=defaultUsages;
	
	//Vertices
	glGenBuffers(1,(GLuint*)&vbo->vbo);
	glBindBuffer(GL_ARRAY_BUFFER,vbo->vbo);
	if(vbo->hasNormals) glBufferData(GL_ARRAY_BUFFER,vbo->vcount*sizeof(AEVBOVert),vbo->verts,usages[0]);
	else glBufferData(GL_ARRAY_BUFFER,vbo->vcount*sizeof(AEVBOVertWithoutNormal),vbo->verts,usages[0]);
	
	//free(vbo->verts);
	//vbo->verts=NULL;
	//vbo->vallocated=0;
	
	//Indices
	glGenBuffers(1,(GLuint*)&vbo->ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vbo->ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,vbo->icount*sizeof(int),vbo->indices,usages[1]);
	
	//free(vbo->indices);
	//vbo->indices=NULL;
	//vbo->vallocated=0;
}

AEVBO* AEVBOLoad(const char* filename,int isStatic,int hasNormals){
	if(filename==NULL) return AEVBONew();
	AEMesh* m=AEMeshLoad(filename);
	AEVBO* vbo=AEMeshAsVBO(m,isStatic,hasNormals);
	AEMeshDelete(m);
	return vbo;
}

void AEVBODelete(AEVBO* vbo){
	if(vbo==NULL) return;
	glDeleteBuffers(1,(GLuint*)&vbo->vbo);
	glDeleteBuffers(1,(GLuint*)&vbo->ibo);
	free(vbo->verts);
	free(vbo->indices);
	free(vbo);
}