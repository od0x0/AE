#include "../Core.h"
#include "../FastMath3D.h"
#include "../enough.h"
#include "../ODE/ode/ode.h"
#include "../List.h"
#include "../Node.h"
#pragma once

/*enum{
	AENodeTypeNone=0,
	AENodeTypeTransform,
	//Defines an orientation in space
	AENodeTypeEnt,
	//Defines an entity that links the other node types
	AENodeTypeBody,
	//Defines collision properties
	AENodeTypeMesh,
	//Defines a mesh
	AENodeTypeMaterial,
	//Defines a material that will be bound before the mesh is rendered
	AENodeTypeEntCluster,
	//Defines a group of Entities
	AENodeTypeEntCamera,
	//Defines a camera
	AENodeTypeEntLight,
	//Defines a light
	AENodeTypeLastPredefined=AENodeTypeEntLight
};

struct AENode;
typedef struct AENode AENode;*/

//This is private, do not access directly
struct AENode{
	unsigned char refcount;//Eases the problem of memory managment
	unsigned char type;
	ENode* enode;//For binding to Enough
	//Warning: The following structs all occupy the same area of memory
	union{
	
		/*struct{
			AEVec3 position
			AEQuat rotation;
		}transform;*/
		
		struct{
			struct{
				AEVec3 position
				AEQuat rotation;
			}transform;
			AEList* nodeIDList;
			float radius;
			int (*collisionCallback)(AENode* node,AENode* node2);
			dBodyID body;
			//Note, exists == non zero
			//unsigned int transformID;
			//Bound, if exists
				//unsigned int light;
				//Bound
				//unsigned int materialID;
				//Bound, if exists
				//unsigned int meshID;
				//Rendered if exists
				//unsigned int cluster;
				//Passed the same event, transform doesn't pass down
			//Transform is then unbound
			
			//unsigned int bodyID;//Unrelated to rendering
		}ent;
		
		struct{
			//unsigned int transformID;
			//int (*collisionCallback)(AENode* node,AENode* node2);
			/////////    <><><><><><><><><><><><><>   To do
		}body;
		
		struct{
			union{
				AEVBO* vbo;
				////////////      <><><><><><><><><><><><> To do when animated vbos come
			}data;
			unsigned int start,end;//Range
			unsigned char type;//Animated, Static, etc, unused for now
		}mesh;
		
		struct{
			//unsigned int transformID;
			//Transforms the texture matrix
			unsigned int* textures;
			unsigned int* textureUniforms;
			void* shader;
			unsigned char texUnitCount;
			
			//unsigned char color[4];  //unneeded?
		}material;
		
		/*struct{
			unsigned int entCount;
			unsigned int* entIDs;
		}cluster;*/
		
		struct{
			//unsigned int transformID;
			float far,near,fov;
			unsigned short x,y,w,h;    //Controls viewport
			///////////////			<><><><><><><><><><><><><><> To do: Add Render-To-Texture stuff
		}camera;
		
		struct{
			//unsigned int transformID;
			unsigned char color[3],intensity;
			float radius;
		}light;

	}data;
};


extern AEList* AENodes;

inline static AENode* AENodeGetForID(unsigned int nodeID){
	return AEListAsArray(AENodes,AENode*)[nodeID-1];
}

void AENodeEntConnect(unsigned int nodeID,unsigned int node2ID); 
//void AENodeBodyConnect(unsigned int nodeID,unsigned int node2ID); 
void AENodeMeshConnect(unsigned int nodeID,unsigned int node2ID); 
void AENodeMaterialConnect(unsigned int nodeID,unsigned int node2ID); 
//void AENodeClusterConnect(unsigned int nodeID,unsigned int node2ID); 
void AENodeCameraConnect(unsigned int nodeID,unsigned int node2ID); 
void AENodeLightConnect(unsigned int nodeID,unsigned int node2ID);

void AENodeEntRenderAction(unsigned int nodeID);