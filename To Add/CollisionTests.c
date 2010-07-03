#include "../Physics.h"

int AEPGeomCollide_AABB_AABB(AEPGeom* g,AEPGeom* g2,AEVec3* correction){
	
	//Really fast AABB collision detection (as compared to the system I used in Toxic, which used a rather quickly coded physics engine)
	
	if((o->size.x+o->size.y+o->size.z)==0) return 0;
	else if((o2->size.x+o2->size.y+o2->size.z)==0) return 0;
	//If the size of either object is 0, assume that they are non-collidable, and return no collision
	
	AEVec3 distance={o->x-o2->x,o->y-o2->y,o->z-o2->z};
	if(distance.x<0) distance.x=-distance.x;
	if(distance.y<0) distance.y=-distance.y;
	if(distance.z<0) distance.z=-distance.z;
	
	AEVec3 combinedSize={o->size.x+o2->size.x,o->size.y+o2->size.y,o->size.z+o2->size.z};
	
	if(distance.x>combinedSize.x) return 0;
	if(distance.y>combinedSize.y) return 0;
	if(distance.z>combinedSize.z) return 0;
	
	if(correction){
		
		AEVec3 penetration;
		penetration.x=fabsf((o->x - g2->x)-(o->size.x+g2->aabb.x));
		penetration.y=fabsf((o->y - g2->y)-(o->size.y+g2->aabb.y));
		penetration.z=fabsf((o->z - g2->z)-(o->size.z+g2->aabb.z));
	
		int axis=0;
		if(penetration.y>penetration.x && penetration.y>penetration.z) axis=1;
		else if(penetration.z>penetration.x && penetration.z>penetration.x) axis=2;		//else if(penetration.y>penetration.x && penetration.z>penetration.x) axis=0;

		//Warning, ugly code, does o[axis]=o2[axis]+o2 size[axis]+o size[axis]
		((float*)correction)[axis]=((float*)g2)[axis]-(float*)g)[axis]+((float*)&(g2->aabb))[axis]+((float*)&(g->aabb))[axis];
	}
	
	return 1;
}

int AEPGeomCollide_AABB_Ray(AEPGeom* g,AEPGeom* g2){
	return 0;
}

int AEPGeomCollide_AABB_Mesh(AEPGeom* g,AEPGeom* g2){
	return 0;
}

int AEPGeomCollide_Mesh_Ray(AEPGeom* g,AEPGeom* g2){
	return 0;
}