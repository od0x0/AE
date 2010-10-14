#include "VA.h"
#include "Math3D.h"

typedef struct{
	AEVec3 position;
	unsigned int parent;
	char name[16];//Null terminated, so it can hold 15 chars
}AEAnimeshBone;

typedef struct{
	AEVA vertexArray, texCoordArray, indexArray;
	short* vertexBones;
	AEList/*<AEAnimeshBone>*/* bones;
}AEAnimesh;

void AEAnimeshBind(AEAnimesh* mesh){
	AEVABindTexcoord(&(mesh->texCoordArray));
	AEVABindVertex(&(mesh->vertexArray));
	AEVABindIndices(&(mesh->indexArray));
}

void AEAnimeshDraw(AEAnimesh* mesh){
	AEVADraw(0,mesh->indexArray.length);
}

size_t AEAnimeshBoneLookup(AEAnimesh* mesh,char* name){
	for(size_t i=0;i<AEListLength();i++){
		if(strncmp(AEListAsArray(mesh->bones,AEAnimeshBone)[i].name,name,sizeof(char)*16)==0) return i+1;
	}
	return 0;
}

void AEAnimeshBoneNameSet(AEAnimesh* mesh,int boneID,char* name){
	strncpy(AEListAsArray(mesh->bones,AEAnimeshBone)[i].name,name,sizeof(char)*16);
}

const char* AEAnimeshBoneNameGet(AEAnimesh* mesh,int boneID){
	return AEListAsArray(mesh->bones,AEAnimeshBone)[i].name;
}
/*
void AEAnimeshBoneRotationSet(AEAnimesh* mesh,int boneID,AEQuat* rotation){
	
}
*/

void AEAnimeshBonePositionSet(AEAnimesh* mesh,int boneID,float x,float y,float z){
	if(boneID==0) return;
	
	AEAnimeshBone* bone=AEListAsArray(mesh->bones,AEAnimeshBone)+(boneID-1);
	AEAnimeshBone* parent=AEListAsArray(mesh->bones,AEAnimeshBone)+(bone->parent-1);
	
	AEVec3 newposition={x,y,z};
	AEVec3 oldposition=bone->position;
	AEVec3 parentposition=parent->position;
	
	AEVec3 newdifference=AEVec3Sub(newposition,parentposition);
	AEVec3 olddifference=AEVec3Sub(oldposition,parentposition);
	
	AEVec3 angleToRotate=AEVec3Sub(AEVec3AngleTo(newposition),AEVec3AngleTo(oldposition));
	AEQuat rotationq=AEQuatFromEuler(angleToRotate);
	
	float matrix[16];
	AEQuatToMatrix4x4(rotationq,matrix);
	
	AEVec3* v=AEVAMap(&(mesh->vertexArray),mesh->vertexArray.length,GL_READ_WRITE);
	
	for(size_t i=0;i<(mesh->vertexArray.length/3);i++){
		if(mesh->vertexBones[i]==boneID){
			AEVec3 p=AEVec3Sub(v[i],parentposition);
			v[i]=AEVec3Add(AEMatrix4x4MulVec3(matrix,p),parentposition);
		}
	}
	
	AEVAUnmap(&(mesh->vertexArray));
	
	//AEQuat rotation=AEQuatBetween(newposition,oldposition);
	
	//AEVec3 newDifference=AEVec3Sub(bone->position,parent->position);
	//AEVec3 newAngleFromParentToThis=AEVec3AngleTo(newDifference);
//	float newLengthinv=AEVec3LengthInvAccurate(newDifference);
	
	
}

void AEAnimeshStep(AEAnimesh* mesh,float step){
	
}