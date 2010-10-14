#include "SDL.h"
#include "../AE/Core.h"
#include "../AE/VBO.h"
#include "../AE/Node.h"
AEVBO* VBO;
AETexture Texture;
AENode* World;
dBodyID DynamicBody;

#define MAXIMUM_FRAME_RATE 120
#define MINIMUM_FRAME_RATE 15
#define UPDATE_INTERVAL (1.0 / MAXIMUM_FRAME_RATE)
#define MAX_CYCLES_PER_FRAME (MAXIMUM_FRAME_RATE / MINIMUM_FRAME_RATE)

void perframe(float step){
	static double lastFrameTime = 0.0;
	static double cyclesLeftOver = 0.0;
	double currentTime;
	double updateIterations;
	
	currentTime = SDL_GetTicks();
	updateIterations = ((currentTime - lastFrameTime) + cyclesLeftOver);
	
	if (updateIterations > (MAX_CYCLES_PER_FRAME * UPDATE_INTERVAL)) {
		updateIterations = (MAX_CYCLES_PER_FRAME * UPDATE_INTERVAL);
	}
	
	while (updateIterations > UPDATE_INTERVAL) {
		updateIterations -= UPDATE_INTERVAL;
		AENodeStep(World,UPDATE_INTERVAL);
		const dReal* pos=dBodyGetPosition(DynamicBody);
		//printf("(%f, %f, %f)\n",(float)pos[0],(float)pos[1],(float)pos[2]);
	}
	
	cyclesLeftOver = updateIterations;
	lastFrameTime = currentTime;
	
	AENodeRender(World);
}

int main(int argc,char** argv){
	AEInit("Window",800,500);
	//Code here
	AECameraPositionSet(AECameraActiveGet(),0,0,7);
	Texture=AETextureLoad("Data/Texture.png");
	VBO=AEVBOLoad("Data/TexturedCube.obj","tnvi","stream");
	
	World=AENodeNew(AENodeTypeWorld,"World");
	dWorldDestroy(AENodeWorldODEWorldGet(World));
	
	dSpaceID staticSpace=dHashSpaceCreate(0);
	dSpaceID dynamicSpace=dHashSpaceCreate(0);
	AENodeWorldStaticSpaceSet(World,staticSpace);
	AENodeWorldDynamicSpaceSet(World,dynamicSpace);
	
	AENode* entnode=AENodeNew(AENodeTypeEnt,"Ent");
	AENodeWorldAddChild(World,entnode);
	AENodeEntBodyPropertySet(entnode,AENodeEntBodyPropertyMovable,false);
	
	AENode* entnode2=AENodeNew(AENodeTypeEnt,"Ent");
	AENodeWorldAddChild(World,entnode2);
	AENodeEntBodyPropertySet(entnode2,AENodeEntBodyPropertyMovable,true);
	
	AENode* meshnode=AENodeNew(AENodeTypeMesh,"Mesh");
	AENodeEntAddChild(entnode,meshnode);
	AENodeEntAddChild(entnode2,meshnode);
	AENodeMeshVBOSet(meshnode,VBO);
	
	AENode* materialnode=AENodeNew(AENodeTypeMaterial,"Material");
	AENodeEntAddChild(entnode,materialnode);
	AENodeEntAddChild(entnode2,materialnode);
	//AEImage* diffuseimage=AEImageFromFile("Data/Texture.png");
	AENodeMaterialTextureSet(materialnode,"Diffuse",AEImageFromFile("Data/Texture.png"));
	//AEImageDelete(diffuseimage);
	
	AENodeWorldODEWorldSet(World,dWorldCreate());
	dWorldSetGravity(AENodeWorldODEWorldGet(World),0,-10,0);
	
	dGeomID geom=dCreateBox(0,2,2,2);
	dBodyID body=AENodeEntBodyGet(entnode);//dBodyCreate(AENodeWorldODEWorldGet(World));
	dGeomSetBody(geom,body);
	dSpaceAdd(staticSpace,geom);
	//AENodeEntBodySet(entnode,body);
	
	dGeomID geom2=dCreateBox(0,2,2,2);
	dBodyID body2=DynamicBody=AENodeEntBodyGet(entnode2);//dBodyCreate(AENodeWorldODEWorldGet(World));
	dGeomSetBody(geom2,body2);
	dSpaceAdd(dynamicSpace,geom2);
	//AENodeEntBodySet(entnode2,body2);
	dMass mass;
	dMassSetBoxTotal(&mass,2,2,2,1);
	dBodySetMass(body2,&mass);
	
	AENodeEntPositionSet(entnode2,0,5,0);
	AENodeEntVelocitySet(entnode2,0,-1,0);
	
	AEStart(perframe);
	return 1;
}