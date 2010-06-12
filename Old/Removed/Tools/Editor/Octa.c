#include "Octa.h"

Octa* OctaChildren(Octa* parent){
	if(parent==NULL) return calloc(1,sizeof(Octa));
	if(octa->children) return octa->children;
	
	octa->children=(AEOcta*)calloc(8,sizeof(Octa));
	static float positions[8][3]={{0,0,0},{1,0,0},{0,0,1},{1,0,1},
					{0,1,0},{1,1,0},{0,1,1},{1,1,1}};

	for(char i=0;i<8;i++){
		Octa* child=&octa->children[i];
		child->size=octa->size*0.5f;
		child->x=positions[i][0]*child->size;
		child->y=positions[i][1]*child->size;
		child->z=positions[i][2]*child->size;
	}
	return octa->children;
}

void OctaRemoveChildren(Octa* octa){
	if(octa==NULL) return;
	if(octa->children){
		for(char i=0;i<8;i++) OctaRemoveChildren(&octa->children[i]);
		free(octa->children);
	}
}

static inline int Vec3Between(AEVec3* p,AEVec3*min,AEVec3* max){return (min->x < p->x && min->y < p->y && min->z < p->z && p->x < max->x && p->y < max->y && p->z < max->z);}

Octa* OctaGetAt(Octa* octa,float x,float y,float z){
	if(octa==NULL) return NULL;
	
	AEVec3 points[2]={{x,y,z},{octa->x+octa->size,octa->y+octa->size,octa->z+octa->size}};
	if(!Vec3Between(&points[0],(AEVec3*)octa,&points[1])) return NULL;
	
	if(!octa->children) return octa;
	Octa* subOcta;
	for(char i=0;i<8;i++) if(subOcta=OctaGetAt(&octa->children[i],x,y,z)) return subOcta;
	
	printf("Octa index error for %p at %.2f,%.2f,%.2f\n",octa,x,y,z);
	return NULL;//Logically impossible to get here, but if I messed up, it should return this.
}

void OctaDeform(Octa* octa,AEVec3 center,float radius,AEVec3 strength){
	if(octa->faces) for(char i=0;i<6;i++){
		for(char j=0;j<4;j++){
			AEVec3 dif={octa->x,octa->y,octa->z};
			dif=AEVec3Sub(dif,center);
			float distance=AEVec3LengthInv(dif);
			if(radius<distance) continue;
			AEVec3 amount=AEVec3Mul(,strength);
			
			octa->faces[i].v[j]=AEVec3Add(octa->faces[i].v[j],);
		}
	}
	
	if(octa->children) for(char i=0;i<8;i++)
		OctaDeform(octa->children+i,center,radius,strength);
}

inline int OctaIsVisible(AEOcta* octa){
	AEVec3 center={octa->size/2.0f+octa->x,octa->size/2.0f+octa->y,octa->size/2.0f+octa->z};
	return AEVFCheck(&center,(octa->size/2)*1.74);
}

void OctaRender(Octa* octa){//Brute force and slow
	if(OctaIsVisible(octa)==0) return;
	
	if(octa->faces) for(char i=0;i<6;i++){
		AETextureBind(octa->faces[i].texture);
		glBegin(GL_QUADS);
			for(char j=0;j<4;j++){
				glTexCoord2fv(octa->faces[i].t+j);
				glNormal3fv(octa->faces[i].n+j);
				glVertex3fv(octa->faces[i].v+j);
			}
		glEnd();
	}
	
	if(octa->children) for(char i=0;i<8;i++)
		OctaRender(octa->children+i);
}