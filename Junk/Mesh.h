#include "AABB.h"
#include "../List.h"

//Binary tree

typedef struct AEAABBTree{
	AEAABB aabb;
	void* tag;
	AEList*/*AEAABBTree*/ childrenList;
}AEAABBTree;

static AEAABBTree* AEAABBTreeCollide(AEAABBTree* tree,AEAABB* aabb){
	if(AEAABBCollide(aabb,&(tree->aabb))==0) return NULL;
	unsigned int length=AEListLength(tree->childrenList);
	AEAABBTree* children=AEListAsArray(tree->childrenList,AEAABBTree);
	for(unsigned int i=0;i<length;i++){
		void* result=AEAABBTreeCollide(children+i,aabb);
		if(result) return result;
	}
	return NULL;
}

void AEAABBTreeDeleteChildren(AEAABBTree* tree){
	unsigned int length=AEListLength(tree->childrenList);
	AEAABBTree* children=AEListAsArray(tree->childrenList,AEAABBTree);
	for(unsigned int i=0;i<length;i++){
		AEAABBTreeDeleteChildren(children+i);
	}
	if(tree->childrenList) AEListDelete(tree->childrenList);
}

typedef struct{
	AEVec3* verts;
	AEAABBTree tree;//A tree with a depth of 1
}AECollisionMesh;

AECollisionMesh* AECollisionMeshNew(AEVec3* verts,unsigned int vertCount){
	AECollisionMesh* mesh=calloc(1,sizeof(AECollisionMesh));
	AEAABBFromTriangles(&(mesh->tree.aabb),verts,vertCount);
	for(unsigned int i=0;i<vertCount;i+=3){
		AEAABB aabb;
		AEAABBFromTriangles(&aabb,verts,3);
		AEAABTree subtree;
		subtree.aabb=aabb;
		tag=verts+i;
		subtree.childrenList=NULL;
		AEListAdd(mesh.tree,AEAABB);
	}
	return mesh;
}