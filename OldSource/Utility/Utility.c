#include "../Utility.h"
#include <SDL/SDL_opengl.h>
#include <SDL/SDL.h>
#include "../FastMath3D.h"

void AECrosshairDraw(float size){
	 AERefreshViewport(1);
	 glPushMatrix();
	 glLoadIdentity();
	 glDisable(GL_DEPTH_TEST);
	 AETextureBind(0);
	 glLineWidth(2);
	 glBegin(GL_LINES);
		 glVertex3f(AEScreen.w/2-size,AEScreen.h/2,-1);
		 glVertex3f(AEScreen.w/2+ size,AEScreen.h/2,-1);
		 glVertex3f(AEScreen.w/2,AEScreen.h/2-size,-1);
		 glVertex3f(AEScreen.w/2,AEScreen.h/2+ size,-1);
	 glEnd();
	 glEnable(GL_DEPTH_TEST);
	 glPopMatrix();
	 AERefreshViewport(0);
}

void AEFPSCameraMouseMoveSetup(float speed,float upmax,float downmax){
	if((speed+upmax+downmax)==0){
		AEVec2i screenCenter={AEScreen.w/2,AEScreen.h/2};
		SDL_WarpMouse(screenCenter.x,screenCenter.y);
		return;
	}
	AEVec2i screenCenter={AEScreen.w/2,AEScreen.h/2};
	AECamera.rotation.y-=(AEMouse.x-screenCenter.x)*speed;
	AECamera.rotation.x-=(AEMouse.y-screenCenter.y)*speed;
	SDL_WarpMouse(screenCenter.x,screenCenter.y);
	
	if(AECamera.rotation.x>upmax) AECamera.rotation.x=upmax;
	if(AECamera.rotation.x<-downmax) AECamera.rotation.x=-downmax;
}

void AEObjectApplyFriction(AEObject* o,float amount){
	float factor=1-amount;
	o->velocity.x*=factor;
	o->velocity.y*=factor;
	o->velocity.z*=factor;
}
void AEObjectApply3DFriction(AEObject* o,AEVec3 amount){
	AEVec3 factor=AEVec3Sub(AEVec3FromSingle(1),amount);
	o->velocity=AEVec3Mul(o->velocity,factor);
}

//#pragma once
////
////////////////////////////////////////////////////////////////////////////
/********************************************************/
/* AABB-triangle overlap test code                      */
/* by Tomas Akenine-Möller                              */
/* Function: int triBoxOverlap(float boxcenter[3],      */
/*          float boxhalfsize[3],float triverts[3][3]); */
/* History:                                             */
/*   2001-03-05: released the code in its first version */
/*   2001-06-18: changed the order of the tests, faster */
/*                                                      */
/* Acknowledgement: Many thanks to Pierre Terdiman for  */
/* suggestions and discussions on how to optimize code. */
/* Thanks to David Hunt for finding a ">="-bug!         */
/********************************************************/
#include <math.h>
#include <stdio.h>

#define X 0
#define Y 1
#define Z 2

#define CROSS(dest,v1,v2) \
          dest[0]=v1[1]*v2[2]-v1[2]*v2[1]; \
          dest[1]=v1[2]*v2[0]-v1[0]*v2[2]; \
          dest[2]=v1[0]*v2[1]-v1[1]*v2[0]; 

#define DOT(v1,v2) (v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2])

#define SUB(dest,v1,v2) \
          dest[0]=v1[0]-v2[0]; \
          dest[1]=v1[1]-v2[1]; \
          dest[2]=v1[2]-v2[2]; 

#define FINDMINMAX(x0,x1,x2,min,max) \
  min = max = x0;   \
  if(x1<min) min=x1;\
  if(x1>max) max=x1;\
  if(x2<min) min=x2;\
  if(x2>max) max=x2;

static int planeBoxOverlap(float normal[3],float d, float maxbox[3])
{
  int q;
  float vmin[3],vmax[3];
  for(q=X;q<=Z;q++)
  {
    if(normal[q]>0.0f)
    {
      vmin[q]=-maxbox[q];
      vmax[q]=maxbox[q];
    }
    else
    {
      vmin[q]=maxbox[q];
      vmax[q]=-maxbox[q];
    }
  }
  if(DOT(normal,vmin)+d>0.0f) return 0;
  if(DOT(normal,vmax)+d>=0.0f) return 1;
  
  return 0;
}


/*======================== X-tests ========================*/
#define AXISTEST_X01(a, b, fa, fb)			   \
	p0 = a*v0[Y] - b*v0[Z];			       	   \
	p2 = a*v2[Y] - b*v2[Z];			       	   \
        if(p0<p2) {min=p0; max=p2;} else {min=p2; max=p0;} \
	rad = fa * boxhalfsize[Y] + fb * boxhalfsize[Z];   \
	if(min>rad || max<-rad) return 0;

#define AXISTEST_X2(a, b, fa, fb)			   \
	p0 = a*v0[Y] - b*v0[Z];			           \
	p1 = a*v1[Y] - b*v1[Z];			       	   \
        if(p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;} \
	rad = fa * boxhalfsize[Y] + fb * boxhalfsize[Z];   \
	if(min>rad || max<-rad) return 0;

/*======================== Y-tests ========================*/
#define AXISTEST_Y02(a, b, fa, fb)			   \
	p0 = -a*v0[X] + b*v0[Z];		      	   \
	p2 = -a*v2[X] + b*v2[Z];	       	       	   \
        if(p0<p2) {min=p0; max=p2;} else {min=p2; max=p0;} \
	rad = fa * boxhalfsize[X] + fb * boxhalfsize[Z];   \
	if(min>rad || max<-rad) return 0;

#define AXISTEST_Y1(a, b, fa, fb)			   \
	p0 = -a*v0[X] + b*v0[Z];		      	   \
	p1 = -a*v1[X] + b*v1[Z];	     	       	   \
        if(p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;} \
	rad = fa * boxhalfsize[X] + fb * boxhalfsize[Z];   \
	if(min>rad || max<-rad) return 0;

/*======================== Z-tests ========================*/

#define AXISTEST_Z12(a, b, fa, fb)			   \
	p1 = a*v1[X] - b*v1[Y];			           \
	p2 = a*v2[X] - b*v2[Y];			       	   \
        if(p2<p1) {min=p2; max=p1;} else {min=p1; max=p2;} \
	rad = fa * boxhalfsize[X] + fb * boxhalfsize[Y];   \
	if(min>rad || max<-rad) return 0;

#define AXISTEST_Z0(a, b, fa, fb)			   \
	p0 = a*v0[X] - b*v0[Y];				   \
	p1 = a*v1[X] - b*v1[Y];			           \
        if(p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;} \
	rad = fa * boxhalfsize[X] + fb * boxhalfsize[Y];   \
	if(min>rad || max<-rad) return 0;

int AETriBoxOverlap(float boxcenter[3],float boxhalfsize[3],float triverts[3][3]){

  /*    use separating axis theorem to test overlap between triangle and box */
  /*    need to test for overlap in these directions: */
  /*    1) the {x,y,z}-directions (actually, since we use the AABB of the triangle */
  /*       we do not even need to test these) */
  /*    2) normal of the triangle */
  /*    3) crossproduct(edge from tri, {x,y,z}-directin) */
  /*       this gives 3x3=9 more tests */
   float v0[3],v1[3],v2[3];
 //  float axis[3];
   float min,max,d,p0,p1,p2,rad,fex,fey,fez;  
   float normal[3],e0[3],e1[3],e2[3];

   /* This is the fastest branch on Sun */
   /* move everything so that the boxcenter is in (0,0,0) */
   SUB(v0,triverts[0],boxcenter);
   SUB(v1,triverts[1],boxcenter);
   SUB(v2,triverts[2],boxcenter);

   /* compute triangle edges */
   SUB(e0,v1,v0);      /* tri edge 0 */
   SUB(e1,v2,v1);      /* tri edge 1 */
   SUB(e2,v0,v2);      /* tri edge 2 */

   /* Bullet 3:  */
   /*  test the 9 tests first (this was faster) */
   fex = fabs(e0[X]);
   fey = fabs(e0[Y]);
   fez = fabs(e0[Z]);
   AXISTEST_X01(e0[Z], e0[Y], fez, fey);
   AXISTEST_Y02(e0[Z], e0[X], fez, fex);
   AXISTEST_Z12(e0[Y], e0[X], fey, fex);

   fex = fabs(e1[X]);
   fey = fabs(e1[Y]);
   fez = fabs(e1[Z]);
   AXISTEST_X01(e1[Z], e1[Y], fez, fey);
   AXISTEST_Y02(e1[Z], e1[X], fez, fex);
   AXISTEST_Z0(e1[Y], e1[X], fey, fex);

   fex = fabs(e2[X]);
   fey = fabs(e2[Y]);
   fez = fabs(e2[Z]);
   AXISTEST_X2(e2[Z], e2[Y], fez, fey);
   AXISTEST_Y1(e2[Z], e2[X], fez, fex);
   AXISTEST_Z12(e2[Y], e2[X], fey, fex);

   /* Bullet 1: */
   /*  first test overlap in the {x,y,z}-directions */
   /*  find min, max of the triangle each direction, and test for overlap in */
   /*  that direction -- this is equivalent to testing a minimal AABB around */
   /*  the triangle against the AABB */

   /* test in X-direction */
   FINDMINMAX(v0[X],v1[X],v2[X],min,max);
   if(min>boxhalfsize[X] || max<-boxhalfsize[X]) return 0;

   /* test in Y-direction */
   FINDMINMAX(v0[Y],v1[Y],v2[Y],min,max);
   if(min>boxhalfsize[Y] || max<-boxhalfsize[Y]) return 0;

   /* test in Z-direction */
   FINDMINMAX(v0[Z],v1[Z],v2[Z],min,max);
   if(min>boxhalfsize[Z] || max<-boxhalfsize[Z]) return 0;

   /* Bullet 2: */
   /*  test if the box intersects the plane of the triangle */
   /*  compute plane equation of triangle: normal*x+d=0 */
   CROSS(normal,e0,e1);
   d=-DOT(normal,v0);  /* plane eq: normal.x+d=0 */
   if(!planeBoxOverlap(normal,d,boxhalfsize)) return 0;

   return 1;   /* box and triangle overlaps */
}
#undef X
#undef Y
#undef Z

#undef CROSS

#undef DOT

#undef SUB

#undef FINDMINMAX

int AEObjectCollidesWithVBO(AEObject* o,AEVBO* vbo,AEVec3 vboPos){
	for(unsigned int i=0;i<vbo->icount;i+=3){
		float triverts[3][3];
		for(char j=0;j<3;j++ ) *((AEVec3*)&triverts[j])=AEVec3Add(vbo->verts[vbo->indices[i+j]].v,vboPos);
		if(AETriBoxOverlap((float*)o,(float*)&o->size,triverts)) return 1;
	}
	return 0;
}

/*int AEObjectCollidesWithOcta(AEObject* o,AEOcta* octa){
	return 0;
}*/

/*AEVec3 AEVBOCalculateOffset(AEVBO* vbo){
	AEVec3 total=AEVec3FromSingle(0);
	for(unsigned int i=0;i<vbo->icount;i++)
		total=AEVec3Add(vbo->verts[vbo->indices[i]].v,total);
	return AEVec3Div(total,AEVec3FromSingle(vbo->icount));
}
AEVec3 AEVBOCalculateSize(AEVBO* vbo){
	AEVec3 total=AEVec3FromSingle(0);
	AEVec3 offset=AEVBOCalculateOffset(vbo);
	for(unsigned int i=0;i<vbo->icount;i++){
		AEVec3 temp=AEVec3Sub(vbo->verts[vbo->indices[i]].v,offset);
		temp=AEVec3FromCoords(AEAbs(temp.x),AEAbs(temp.y),AEAbs(temp.z));
		total=AEVec3Add(temp,total);
	}
	return AEVec3Div(total,AEVec3FromSingle(2));
}*/

AEVec3 AEVBOCenter(AEVBO* vbo){
	AEVec3 max,min;
	max=min=vbo->verts[0].v;

	for(char i=0;i<vbo->vcount;i++){
		max=AEVec3Max(max,vbo->verts[i].v);
		min=AEVec3Min(min,vbo->verts[i].v);
	}

	return AEVec3Mul(AEVec3Add(max,min),AEVec3FromSingle(0.5));
}

AEVec3 AEVBOAABB(AEVBO* vbo){
	AEVec3 max,min;
	max=min=vbo->verts[0].v;

	for(char i=0;i<vbo->vcount;i++){
		max=AEVec3Max(max,vbo->verts[i].v);
		min=AEVec3Min(min,vbo->verts[i].v);
	}

	return AEVec3Mul(AEVec3Sub(max,min),AEVec3FromSingle(0.5));
}


void AEVBOTranslate(AEVBO* vbo,AEVec3 move){
	for(unsigned int i=0;i<vbo->vcount;i++)
		vbo->verts[i].v=AEVec3Sub(vbo->verts[i].v,move);
}

void AEObjectAimVelocityAt(AEObject* o,AEVec3* point,AEVec3 speed){
	o->velocity=AEVec3Mul(AEVec3Normalized(AEVec3Sub(*point,*((AEVec3*)o))),speed);
}

char* AEFileAsString(const char* filename){//Used for loading shaders
	FILE* file=fopen(filename,"r");
	if(!file) return NULL;
	fseek (file,0,SEEK_END);
	int length=ftell(file);
	rewind(file);
	char* string=calloc(length+1,1);
	fread(string,1,length,file);
	fclose(file);
	return string;
}