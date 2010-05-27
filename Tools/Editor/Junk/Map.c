typedef struct{
	unsigned int texture,lightmapTexture;
	AEGeoSegment seg;
}MapSegment;

struct{
	AEGeo* geo;
	AEList* segmentList;
}Map;

void MapImportObj(char* filename){
	AEMesh* mesh=AEMeshLoad(filename);

	AEGeoSegment seg={0,0};
	seg.indexStart=Map.geo->indices?Map.geo->icount:Map.geo->vcount/3;
	for(unsigned int i=0;i<vbo->icount;i++){
		unsigned int index=geo2->indices[i];
		AEVBOVert* vbovert=vb->verts+index;
		AEGeoVert v={0,0,0,{0,0,0},vbovert->t,{0,0}};
		if(vbo->n) v.n=vbo->n[index];
		v.x=vbovert->x;
		v.y=vbovert->y;
		v.z=vbovert->z;
		AEGeoAddVert(geo,&v);
	}
	seg.indexStart=geo->indices?geo->icount:geo->vcount/3;
	
	AEListAdd(Map.segmentList,MapSegment,);
}