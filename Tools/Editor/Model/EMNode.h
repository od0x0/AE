#pragma once
#include "AE.h"
#include "EUMemory.h"
#include "EMFlags.h"
#include "verse.h"

typedef enum{
	EMNodeTypeEntity,
	EMNodeTypeGeometry,
	EMNodeTypeMaterial,
	EMNodeTypeTexture,
	EMNodeTypeLight,
	EMNodeTypeCamera,
	EMNodeTypeParticleEmitter
}EMNodeType;

typedef struct EMNode EMNode;
typedef uint32_t EMNodeID;
#define EMNodeIDNull (~((EMNodeID)0))
struct EMNode{
	char* name;
	EMNodeType type;
	EMFlags flags;
	VNodeID verseNodeID;
	union{
		struct{
			AEVec3 position;
			AEVec3 rotation;
			AEVec3 scale;
			//geometry
			EMNodeID geometry;
			//material
			EMNodeID material;
		}entity;
		struct{
			enum{
				EMNodeDataGeometryFormat3VF,
				EMNodeDataGeometryFormat2TF3VF,
				EMNodeDataGeometryFormat2TF3NF3VF
			}format;
			AEArray(float) floats;
			AEArray(GLuint) indices;
		}geometry;
		struct{
			EMNodeID diffuseTexture;
			EMNodeID normalmapTexture;
		}material;
		struct{
			uint64_t w, h, d;
			AERGBAub* pixels;//Always rgba
		}texture;
		struct{
			AEVec3 position;
			float radius;
			float falloff;
			AERGBAub color;//A is unused
		}light;
		struct{
			AEVec3 position;
			AEVec3 rotation;
			float fov;//0 == ortho
			float far;
			float near;
		}camera;
		struct{
			
		}particleEmitter;
	}data;
};