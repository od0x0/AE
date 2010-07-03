#include "../Camera.h"
#include <math.h>

AECamera AECameraActive;

void AECameraActiveSet(AECamera* cam){
	AECameraActive=*cam;
	
	glViewport(0,0,cam->w,cam->h);
	glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		if(cam->fov==0) glOrtho(0,cam->w,0,cam->h,cam->near,cam->far);
		else gluPerspective (cam->fov, (float)cam->w/(float)cam->h, cam->near, cam->far);
	glMatrixMode(GL_MODELVIEW);
	
	glLoadIdentity();
	glRotatef(-cam->rotation.x,	1,0,0);
	glRotatef(-cam->rotation.y,	0,1,0);
	glRotatef(-cam->rotation.z,	0,0,1);
	glTranslatef(-cam->x,-cam->y,-cam->z);
	
	AECameraVFCalculate(cam);
}

AECamera* AECameraActiveGet(void){return &AECameraActive;}

// Based off of http://www.crownandcutlass.com/features/technicaldetails/frustum.html
//Slow, only here until I get a faster method

void AECameraVFCalculate(AECamera* cam){
	float   proj[16];
	float   modl[16];
	float   clip[16];
	float   t;
	
	/* Get the current PROJECTION matrix from OpenGL */
	glGetFloatv( GL_PROJECTION_MATRIX, proj );
	
	/* Get the current MODELVIEW matrix from OpenGL */
	glGetFloatv( GL_MODELVIEW_MATRIX, modl );
	
	/* Combine the two matrices (multiply projection by modelview) */
	clip[ 0] = modl[ 0] * proj[ 0] + modl[ 1] * proj[ 4] + modl[ 2] * proj[ 8] + modl[ 3] * proj[12];
	clip[ 1] = modl[ 0] * proj[ 1] + modl[ 1] * proj[ 5] + modl[ 2] * proj[ 9] + modl[ 3] * proj[13];
	clip[ 2] = modl[ 0] * proj[ 2] + modl[ 1] * proj[ 6] + modl[ 2] * proj[10] + modl[ 3] * proj[14];
	clip[ 3] = modl[ 0] * proj[ 3] + modl[ 1] * proj[ 7] + modl[ 2] * proj[11] + modl[ 3] * proj[15];
	
	clip[ 4] = modl[ 4] * proj[ 0] + modl[ 5] * proj[ 4] + modl[ 6] * proj[ 8] + modl[ 7] * proj[12];
	clip[ 5] = modl[ 4] * proj[ 1] + modl[ 5] * proj[ 5] + modl[ 6] * proj[ 9] + modl[ 7] * proj[13];
	clip[ 6] = modl[ 4] * proj[ 2] + modl[ 5] * proj[ 6] + modl[ 6] * proj[10] + modl[ 7] * proj[14];
	clip[ 7] = modl[ 4] * proj[ 3] + modl[ 5] * proj[ 7] + modl[ 6] * proj[11] + modl[ 7] * proj[15];
	
	clip[ 8] = modl[ 8] * proj[ 0] + modl[ 9] * proj[ 4] + modl[10] * proj[ 8] + modl[11] * proj[12];
	clip[ 9] = modl[ 8] * proj[ 1] + modl[ 9] * proj[ 5] + modl[10] * proj[ 9] + modl[11] * proj[13];
	clip[10] = modl[ 8] * proj[ 2] + modl[ 9] * proj[ 6] + modl[10] * proj[10] + modl[11] * proj[14];
	clip[11] = modl[ 8] * proj[ 3] + modl[ 9] * proj[ 7] + modl[10] * proj[11] + modl[11] * proj[15];
	
	clip[12] = modl[12] * proj[ 0] + modl[13] * proj[ 4] + modl[14] * proj[ 8] + modl[15] * proj[12];
	clip[13] = modl[12] * proj[ 1] + modl[13] * proj[ 5] + modl[14] * proj[ 9] + modl[15] * proj[13];
	clip[14] = modl[12] * proj[ 2] + modl[13] * proj[ 6] + modl[14] * proj[10] + modl[15] * proj[14];
	clip[15] = modl[12] * proj[ 3] + modl[13] * proj[ 7] + modl[14] * proj[11] + modl[15] * proj[15];
	
	/* Extract the numbers for the RIGHT plane */
	cam->vfPlanes[0][0] = clip[ 3] - clip[ 0];
	cam->vfPlanes[0][1] = clip[ 7] - clip[ 4];
	cam->vfPlanes[0][2] = clip[11] - clip[ 8];
	cam->vfPlanes[0][3] = clip[15] - clip[12];
	
	/* Normalize the result */
	t = sqrtf( cam->vfPlanes[0][0] * cam->vfPlanes[0][0] + cam->vfPlanes[0][1] * cam->vfPlanes[0][1] + cam->vfPlanes[0][2] * cam->vfPlanes[0][2] );
	cam->vfPlanes[0][0] /= t;
	cam->vfPlanes[0][1] /= t;
	cam->vfPlanes[0][2] /= t;
	cam->vfPlanes[0][3] /= t;
	
	/* Extract the numbers for the LEFT plane */
	cam->vfPlanes[1][0] = clip[ 3] + clip[ 0];
	cam->vfPlanes[1][1] = clip[ 7] + clip[ 4];
	cam->vfPlanes[1][2] = clip[11] + clip[ 8];
	cam->vfPlanes[1][3] = clip[15] + clip[12];
	
	/* Normalize the result */
	t = sqrtf( cam->vfPlanes[1][0] * cam->vfPlanes[1][0] + cam->vfPlanes[1][1] * cam->vfPlanes[1][1] + cam->vfPlanes[1][2] * cam->vfPlanes[1][2] );
	cam->vfPlanes[1][0] /= t;
	cam->vfPlanes[1][1] /= t;
	cam->vfPlanes[1][2] /= t;
	cam->vfPlanes[1][3] /= t;
	
	/* Extract the BOTTOM plane */
	cam->vfPlanes[2][0] = clip[ 3] + clip[ 1];
	cam->vfPlanes[2][1] = clip[ 7] + clip[ 5];
	cam->vfPlanes[2][2] = clip[11] + clip[ 9];
	cam->vfPlanes[2][3] = clip[15] + clip[13];
	
	/* Normalize the result */
	t = sqrtf( cam->vfPlanes[2][0] * cam->vfPlanes[2][0] + cam->vfPlanes[2][1] * cam->vfPlanes[2][1] + cam->vfPlanes[2][2] * cam->vfPlanes[2][2] );
	cam->vfPlanes[2][0] /= t;
	cam->vfPlanes[2][1] /= t;
	cam->vfPlanes[2][2] /= t;
	cam->vfPlanes[2][3] /= t;
	
	/* Extract the TOP plane */
	cam->vfPlanes[3][0] = clip[ 3] - clip[ 1];
	cam->vfPlanes[3][1] = clip[ 7] - clip[ 5];
	cam->vfPlanes[3][2] = clip[11] - clip[ 9];
	cam->vfPlanes[3][3] = clip[15] - clip[13];
	
	/* Normalize the result */
	t = sqrtf( cam->vfPlanes[3][0] * cam->vfPlanes[3][0] + cam->vfPlanes[3][1] * cam->vfPlanes[3][1] + cam->vfPlanes[3][2] * cam->vfPlanes[3][2] );
	cam->vfPlanes[3][0] /= t;
	cam->vfPlanes[3][1] /= t;
	cam->vfPlanes[3][2] /= t;
	cam->vfPlanes[3][3] /= t;
	
	/* Extract the FAR plane */
	cam->vfPlanes[4][0] = clip[ 3] - clip[ 2];
	cam->vfPlanes[4][1] = clip[ 7] - clip[ 6];
	cam->vfPlanes[4][2] = clip[11] - clip[10];
	cam->vfPlanes[4][3] = clip[15] - clip[14];
	
	/* Normalize the result */
	t = sqrtf( cam->vfPlanes[4][0] * cam->vfPlanes[4][0] + cam->vfPlanes[4][1] * cam->vfPlanes[4][1] + cam->vfPlanes[4][2] * cam->vfPlanes[4][2] );
	cam->vfPlanes[4][0] /= t;
	cam->vfPlanes[4][1] /= t;
	cam->vfPlanes[4][2] /= t;
	cam->vfPlanes[4][3] /= t;
	
	/* Extract the NEAR plane */
	cam->vfPlanes[5][0] = clip[ 3] + clip[ 2];
	cam->vfPlanes[5][1] = clip[ 7] + clip[ 6];
	cam->vfPlanes[5][2] = clip[11] + clip[10];
	cam->vfPlanes[5][3] = clip[15] + clip[14];
	
	/* Normalize the result */
	t = sqrtf( cam->vfPlanes[5][0] * cam->vfPlanes[5][0] + cam->vfPlanes[5][1] * cam->vfPlanes[5][1] + cam->vfPlanes[5][2] * cam->vfPlanes[5][2] );
	cam->vfPlanes[5][0] /= t;
	cam->vfPlanes[5][1] /= t;
	cam->vfPlanes[5][2] /= t;
	cam->vfPlanes[5][3] /= t;
}

int AECameraVFCheckSphere(AECamera* cam,float x,float y,float z,float r){
	float d=0;
	for(char i= 0; i < 6; i++ ){
		d = cam->vfPlanes[i][0] * x +cam->vfPlanes[i][1] * y + cam->vfPlanes[i][2] * z + cam->vfPlanes[i][3];
		if( d <= -r )
			return 0;
	}
	return d + r;
}