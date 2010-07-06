#include "PNode.h"

AEPNode* AEPNodeChildren(AEPNode* node,unsigned char count){
	if(node==NULL) return calloc(1,sizeof(AEPNode));
	if(node->children) return node->children;
	node->childCount=count;
	node->children=calloc(node->childCount,sizeof(AEPNode));
	return node->children;
}

void AEPNodeConstructFromPoints(AEPNode* node,unsigned short count,AEVec3* points){
	AEVec3 max,min;
	max=min=AEVec3FromCoords(0,0,0);
	for(int i=0;i<count;i++){
		max=AEVec3Max(points[i],max);
		min=AEVec3Min(points[i],min);
	}
	AEVec3 halfdifference=AEVec3Mul(AEVec3Sub(max,min),AEVec3FromSingle(0.5));
	node->radius=AEVec3Length(halfdifference);
	node->center=AEVec3Add(halfdifference,min);
	
	node->triangles=calloc(count,sizeof(AEVec3));
	memcpy(node->triangles,points,sizeof(AEVec3)*count);
	node->triangleCount=count/3;
}

void AEPNodesDelete(AEPNode* nodes,int count){
	for(int i=0;i<count;i++){
		free(nodes[i].triangles);
		AEPNodesDelete(nodes[i].children,nodes[i].childCount);
	}
	free(nodes);
}

int AEPNodeCollidesWith(AEPNode* node,AEPNode* node2,AEVec3* correction){
	float squaredDistance=AEVec3LengthSQ(AEVec3Sub(node->center,node2->center));
	float squaredRadiusSum=node->radius+node2->radius;
	squaredRadiusSum*=squaredRadiusSum;
	if(squaredDistance>squaredRadiusSum) return 0;
	
	int node1IsSphereOnly=node->triangles==NULL;
	int node2IsSphereOnly=node2->triangles==NULL;
	if(node1IsSphereOnly){
		if(node2IsSphereOnly){
		//If both are spheres
			if(correction){
				AEVec3 difference=AEVec3Sub(node2->center,node->center);
				AEVec3 collisionNormal=AEVec3Normalized(difference);
				float minimumDistance=(1/AESqrtInv(squaredRadiusSum))-(1/AESqrtInv(squaredDistance));
				*correction=AEVec3Mul(collisionNormal,AEVec3FromSingle(minimumDistance));
			}
			return 1;
		}
		
		//Must be a triangle mesh then
		
		
	}
}

inline static int SphereAndTriangleIntersect(AEVec3 center,float radius,AEVec3* triangle){
	AEVec4 plane={0,0,0,0};
	
}

/* Ray-Triangle Intersection Test Routines          */
/* Different optimizations of my and Ben Trumbore's */
/* code from journals of graphics tools (JGT)       */
/* http://www.acm.org/jgt/                          */
/* by Tomas Moller, May 2000                        */

#include <math.h>

#define EPSILON 0.000001
#define CROSS(dest,v1,v2) \
          dest[0]=v1[1]*v2[2]-v1[2]*v2[1]; \
          dest[1]=v1[2]*v2[0]-v1[0]*v2[2]; \
          dest[2]=v1[0]*v2[1]-v1[1]*v2[0];
#define DOT(v1,v2) (v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2])
#define SUB(dest,v1,v2) \
          dest[0]=v1[0]-v2[0]; \
          dest[1]=v1[1]-v2[1]; \
          dest[2]=v1[2]-v2[2]; 

/* the original jgt code */
static int intersect_triangle(double orig[3], double dir[3],
		       double vert0[3], double vert1[3], double vert2[3],
		       double *t, double *u, double *v)
{
   double edge1[3], edge2[3], tvec[3], pvec[3], qvec[3];
   double det,inv_det;

   /* find vectors for two edges sharing vert0 */
   SUB(edge1, vert1, vert0);
   SUB(edge2, vert2, vert0);

   /* begin calculating determinant - also used to calculate U parameter */
   CROSS(pvec, dir, edge2);

   /* if determinant is near zero, ray lies in plane of triangle */
   det = DOT(edge1, pvec);

   if (det > -EPSILON && det < EPSILON)
     return 0;
   inv_det = 1.0 / det;

   /* calculate distance from vert0 to ray origin */
   SUB(tvec, orig, vert0);

   /* calculate U parameter and test bounds */
   *u = DOT(tvec, pvec) * inv_det;
   if (*u < 0.0 || *u > 1.0)
     return 0;

   /* prepare to test V parameter */
   CROSS(qvec, tvec, edge1);

   /* calculate V parameter and test bounds */
   *v = DOT(dir, qvec) * inv_det;
   if (*v < 0.0 || *u + *v > 1.0)
     return 0;

   /* calculate t, ray intersects triangle */
   *t = DOT(edge2, qvec) * inv_det;

   return 1;
}


/* code rewritten to do tests on the sign of the determinant */
/* the division is at the end in the code                    */
static int intersect_triangle1(double orig[3], double dir[3],
			double vert0[3], double vert1[3], double vert2[3],
			double *t, double *u, double *v)
{
   double edge1[3], edge2[3], tvec[3], pvec[3], qvec[3];
   double det,inv_det;

   /* find vectors for two edges sharing vert0 */
   SUB(edge1, vert1, vert0);
   SUB(edge2, vert2, vert0);

   /* begin calculating determinant - also used to calculate U parameter */
   CROSS(pvec, dir, edge2);

   /* if determinant is near zero, ray lies in plane of triangle */
   det = DOT(edge1, pvec);

   if (det > EPSILON)
   {
      /* calculate distance from vert0 to ray origin */
      SUB(tvec, orig, vert0);
      
      /* calculate U parameter and test bounds */
      *u = DOT(tvec, pvec);
      if (*u < 0.0 || *u > det)
	 return 0;
      
      /* prepare to test V parameter */
      CROSS(qvec, tvec, edge1);
      
      /* calculate V parameter and test bounds */
      *v = DOT(dir, qvec);
      if (*v < 0.0 || *u + *v > det)
	 return 0;
      
   }
   else if(det < -EPSILON)
   {
      /* calculate distance from vert0 to ray origin */
      SUB(tvec, orig, vert0);
      
      /* calculate U parameter and test bounds */
      *u = DOT(tvec, pvec);
/*      printf("*u=%f\n",(float)*u); */
/*      printf("det=%f\n",det); */
      if (*u > 0.0 || *u < det)
	 return 0;
      
      /* prepare to test V parameter */
      CROSS(qvec, tvec, edge1);
      
      /* calculate V parameter and test bounds */
      *v = DOT(dir, qvec) ;
      if (*v > 0.0 || *u + *v < det)
	 return 0;
   }
   else return 0;  /* ray is parallell to the plane of the triangle */


   inv_det = 1.0 / det;

   /* calculate t, ray intersects triangle */
   *t = DOT(edge2, qvec) * inv_det;
   (*u) *= inv_det;
   (*v) *= inv_det;

   return 1;
}

/* code rewritten to do tests on the sign of the determinant */
/* the division is before the test of the sign of the det    */
static int intersect_triangle2(double orig[3], double dir[3],
			double vert0[3], double vert1[3], double vert2[3],
			double *t, double *u, double *v)
{
   double edge1[3], edge2[3], tvec[3], pvec[3], qvec[3];
   double det,inv_det;

   /* find vectors for two edges sharing vert0 */
   SUB(edge1, vert1, vert0);
   SUB(edge2, vert2, vert0);

   /* begin calculating determinant - also used to calculate U parameter */
   CROSS(pvec, dir, edge2);

   /* if determinant is near zero, ray lies in plane of triangle */
   det = DOT(edge1, pvec);

   /* calculate distance from vert0 to ray origin */
   SUB(tvec, orig, vert0);
   inv_det = 1.0 / det;
   
   if (det > EPSILON)
   {
      /* calculate U parameter and test bounds */
      *u = DOT(tvec, pvec);
      if (*u < 0.0 || *u > det)
	 return 0;
      
      /* prepare to test V parameter */
      CROSS(qvec, tvec, edge1);
      
      /* calculate V parameter and test bounds */
      *v = DOT(dir, qvec);
      if (*v < 0.0 || *u + *v > det)
	 return 0;
      
   }
   else if(det < -EPSILON)
   {
      /* calculate U parameter and test bounds */
      *u = DOT(tvec, pvec);
      if (*u > 0.0 || *u < det)
	 return 0;
      
      /* prepare to test V parameter */
      CROSS(qvec, tvec, edge1);
      
      /* calculate V parameter and test bounds */
      *v = DOT(dir, qvec) ;
      if (*v > 0.0 || *u + *v < det)
	 return 0;
   }
   else return 0;  /* ray is parallell to the plane of the triangle */

   /* calculate t, ray intersects triangle */
   *t = DOT(edge2, qvec) * inv_det;
   (*u) *= inv_det;
   (*v) *= inv_det;

   return 1;
}

/* code rewritten to do tests on the sign of the determinant */
/* the division is before the test of the sign of the det    */
/* and one CROSS has been moved out from the if-else if-else */
static int intersect_triangle3(double orig[3], double dir[3],
			double vert0[3], double vert1[3], double vert2[3],
			double *t, double *u, double *v)
{
   double edge1[3], edge2[3], tvec[3], pvec[3], qvec[3];
   double det,inv_det;

   /* find vectors for two edges sharing vert0 */
   SUB(edge1, vert1, vert0);
   SUB(edge2, vert2, vert0);

   /* begin calculating determinant - also used to calculate U parameter */
   CROSS(pvec, dir, edge2);

   /* if determinant is near zero, ray lies in plane of triangle */
   det = DOT(edge1, pvec);

   /* calculate distance from vert0 to ray origin */
   SUB(tvec, orig, vert0);
   inv_det = 1.0 / det;
   
   CROSS(qvec, tvec, edge1);
      
   if (det > EPSILON)
   {
      *u = DOT(tvec, pvec);
      if (*u < 0.0 || *u > det)
	 return 0;
            
      /* calculate V parameter and test bounds */
      *v = DOT(dir, qvec);
      if (*v < 0.0 || *u + *v > det)
	 return 0;
      
   }
   else if(det < -EPSILON)
   {
      /* calculate U parameter and test bounds */
      *u = DOT(tvec, pvec);
      if (*u > 0.0 || *u < det)
	 return 0;
      
      /* calculate V parameter and test bounds */
      *v = DOT(dir, qvec) ;
      if (*v > 0.0 || *u + *v < det)
	 return 0;
   }
   else return 0;  /* ray is parallell to the plane of the triangle */

   *t = DOT(edge2, qvec) * inv_det;
   (*u) *= inv_det;
   (*v) *= inv_det;

   return 1;
}