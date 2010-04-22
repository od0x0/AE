#include "../Octa.h"
#include "../VFChecking.h"

inline int AEOctaIsVisible(AEOcta* octa){
	AEVec3 center={octa->size/2.0f+octa->x,octa->size/2.0f+octa->y,octa->size/2.0f+octa->z};
	return AEVFCheck(&center,(octa->size/2)*1.74);
}

void AEOctaRender(AEOcta* octa){
	if(octa==NULL) return;
	
	if(AEOctaIsVisible(octa)){
		//TODO: Call mesh render function
		
		//Render children
		if(octa->children) for(char i=0;i<8;i++) AEOctaRender(&octa->children[i]);
	}
}

AEOcta* AEOctaAddChildren(AEOcta* octa){
	if(octa==NULL) return (AEOcta*)calloc(1,sizeof(AEOcta));
	if(octa->children) return octa->children;
	octa->children=(AEOcta*)calloc(8,sizeof(AEOcta));
	static float positions[8][3]={{0,0,0},{1,0,0},{0,0,1},{1,0,1},
					{0,1,0},{1,1,0},{0,1,1},{1,1,1}};

	for(char i=0;i<8;i++){
		AEOcta* child=&octa->children[i];
		child->size=octa->size*0.5f;
		child->x=positions[i][0]*child->size;
		child->y=positions[i][1]*child->size;
		child->z=positions[i][2]*child->size;
	}
	return octa->children;
}
 
void AEOctaDelete(AEOcta* octa){
	if(!octa) return;
	
	for(char i=0;i<8;i++) AEOctaDelete(&octa->children[i]);
	if(octa->children) free(octa->children);
}

static inline int Vec3Between(AEVec3* p,AEVec3*min,AEVec3* max){return (min->x < p->x && min->y < p->y && min->z < p->z && p->x < max->x && p->y < max->y && p->z < max->z);}

AEOcta* AEOctaGetAt(AEOcta* octa,float x,float y,float z){
	AEVec3 points[2]={{x,y,z},{octa->x+octa->size,octa->y+octa->size,octa->z+octa->size}};
	if(!Vec3Between(&points[0],(AEVec3*)octa,&points[1])) return NULL;
	
	if(!octa->children) return octa;
	AEOcta* subOcta;
	for(char i=0;i<8;i++) if(subOcta=AEOctaGetAt(&octa->children[i],x,y,z)) return subOcta;
	
	printf("AEOcta index error for %p at %.2f,%.2f,%.2f\n",octa,x,y,z);
	return NULL;//Logically impossible to get here, but if I messed up, it should return this.
}