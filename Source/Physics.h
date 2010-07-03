#pragma once
#include "Core.h"
#include "HeaderBegin.h"
#include "FastMath3D.h"

typedef struct AEPProperties AEPProperties;
typedef struct AEPMesh AEPMesh;
typedef struct AEPEnv AEPEnv;
typedef struct AEPGeom AEPGeom;
typedef struct AEPContact AEPContact;

struct AEPProperties{
	AEVec3 velocity;
	float mass;
	unsigned char airResistance;
};

struct AEPMesh{
	unsigned int vcount,icount;
	AEVec3 *vs;
	unsigned short* indices;//Max of 2^16 range, for both size and performance reasons, (You're crazy if you want to collide against 65000 or so verts at once)
};

struct AEPEnv{
	AEPGeom* geoms;
	unsigned int geomCount;
	AEVec3 globalForce;
	float globalResistance;
};

struct AEPGeom{
	float x,y,z;
	AEVec3 aabb;
	union{
		AEPEnv* env;
		AEVec3 rayDirection;
		AEPMesh* mesh;
	}geom;
	unsigned char type,surfaceResistance,surfaceBounciness;
	AEPProperties* movementProperties;
	void (*collisionCallback)(AEPContact*);
	void* data;
};

#define AEPGeomTypeAABB 1
#define AEPGeomTypeMesh 2
#define AEPGeomTypeRay 3
#define AEPGeomTypeEnv 4

struct AEPContact{
	AEPGeom *g,*g2;
	AEVec3 correction;
};

#include "HeaderEnd.h"