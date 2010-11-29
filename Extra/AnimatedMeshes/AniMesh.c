#include "AniMesh.h"

//Potential optimization: Use 4x4 Matrices for multiplication? (ie. generate matrix on interpolation for use in multiplying vertices against)

//The t in r=a+t*(b-a)
void AEAniJointsInterpolate(AEAniJoint* outjoints, AEAniJoint* joints1, AEAniJoint* joints2, float t, int jointCount){
	for(int i=0;i<jointCount;++i){
		outjoints[i].position=AEVec3Lerp(joints1[i].position,joints2[i].position,t);
		outjoints[i].rotation=AEQuatSlerp(joints1[i].rotation, joints2[i].rotation, t);
		outjoints[i].parentID=joints1[i].parentID;
    }
}

void AEAniMeshCalculateVertices(const AEAniMesh* mesh, const AEAniJoint* joints, AEVec3* vertices){
	//For each vertex
	for (int i = 0; i < mesh->vertexCount; ++i){
		//For each weight of said vertex
		AEVec3 result={0,0,0};
		for (int j = 0; j < mesh->vertices[i].weightCount; ++j){
			//Eases typing
			const AEAniWeight* weight = mesh->weights + (mesh->vertices[i].startingWeight + j);
			const AEAniJoint* joint = joints + weight->jointID;

			//Rotate the position of the weight by it's joint's rotation
			const AEVec3 transformedVertexPosition=AEQuatMulVec3(joint->rotation, weight->offsetFromJoint);
			
			//Obvious code is obvious
			const AEVec3 sum=AEVec3Add(transformedVertexPosition, joint->position);
			
			//Now, compound it on, the biases should evenually sum to 1.0
			result.x += sum.x * weight->bias;
			result.y += sum.y * weight->bias;
			result.z += sum.z * weight->bias;
		}
		vertices[i]=result;
	}
}