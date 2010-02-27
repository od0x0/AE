#include <math.h>

//AEVFCulling
// Based off of http://www.crownandcutlass.com/features/technicaldetails/AEVFrustum.html
float AEVFrustum[6][4];

float AEVFCheck(AEVec3* p,float radius){
  float d=0;
  for(char i= 0; i < 6; i++ ){
     d = AEVFrustum[i][0] * p->x +AEVFrustum[i][1] * p->y + AEVFrustum[i][2] * p->z + AEVFrustum[i][3];
     if( d <= -radius )
        return 0;
  }
  return d + radius;
}

void AEVFSetup(void){
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
  AEVFrustum[0][0] = clip[ 3] - clip[ 0];
  AEVFrustum[0][1] = clip[ 7] - clip[ 4];
  AEVFrustum[0][2] = clip[11] - clip[ 8];
  AEVFrustum[0][3] = clip[15] - clip[12];

  /* Normalize the result */
  t = sqrtf( AEVFrustum[0][0] * AEVFrustum[0][0] + AEVFrustum[0][1] * AEVFrustum[0][1] + AEVFrustum[0][2] * AEVFrustum[0][2] );
  AEVFrustum[0][0] /= t;
  AEVFrustum[0][1] /= t;
  AEVFrustum[0][2] /= t;
  AEVFrustum[0][3] /= t;

  /* Extract the numbers for the LEFT plane */
  AEVFrustum[1][0] = clip[ 3] + clip[ 0];
  AEVFrustum[1][1] = clip[ 7] + clip[ 4];
  AEVFrustum[1][2] = clip[11] + clip[ 8];
  AEVFrustum[1][3] = clip[15] + clip[12];

  /* Normalize the result */
  t = sqrtf( AEVFrustum[1][0] * AEVFrustum[1][0] + AEVFrustum[1][1] * AEVFrustum[1][1] + AEVFrustum[1][2] * AEVFrustum[1][2] );
  AEVFrustum[1][0] /= t;
  AEVFrustum[1][1] /= t;
  AEVFrustum[1][2] /= t;
  AEVFrustum[1][3] /= t;

  /* Extract the BOTTOM plane */
  AEVFrustum[2][0] = clip[ 3] + clip[ 1];
  AEVFrustum[2][1] = clip[ 7] + clip[ 5];
  AEVFrustum[2][2] = clip[11] + clip[ 9];
  AEVFrustum[2][3] = clip[15] + clip[13];

  /* Normalize the result */
  t = sqrtf( AEVFrustum[2][0] * AEVFrustum[2][0] + AEVFrustum[2][1] * AEVFrustum[2][1] + AEVFrustum[2][2] * AEVFrustum[2][2] );
  AEVFrustum[2][0] /= t;
  AEVFrustum[2][1] /= t;
  AEVFrustum[2][2] /= t;
  AEVFrustum[2][3] /= t;

  /* Extract the TOP plane */
  AEVFrustum[3][0] = clip[ 3] - clip[ 1];
  AEVFrustum[3][1] = clip[ 7] - clip[ 5];
  AEVFrustum[3][2] = clip[11] - clip[ 9];
  AEVFrustum[3][3] = clip[15] - clip[13];

  /* Normalize the result */
  t = sqrtf( AEVFrustum[3][0] * AEVFrustum[3][0] + AEVFrustum[3][1] * AEVFrustum[3][1] + AEVFrustum[3][2] * AEVFrustum[3][2] );
  AEVFrustum[3][0] /= t;
  AEVFrustum[3][1] /= t;
  AEVFrustum[3][2] /= t;
  AEVFrustum[3][3] /= t;

  /* Extract the FAR plane */
  AEVFrustum[4][0] = clip[ 3] - clip[ 2];
  AEVFrustum[4][1] = clip[ 7] - clip[ 6];
  AEVFrustum[4][2] = clip[11] - clip[10];
  AEVFrustum[4][3] = clip[15] - clip[14];

  /* Normalize the result */
  t = sqrtf( AEVFrustum[4][0] * AEVFrustum[4][0] + AEVFrustum[4][1] * AEVFrustum[4][1] + AEVFrustum[4][2] * AEVFrustum[4][2] );
  AEVFrustum[4][0] /= t;
  AEVFrustum[4][1] /= t;
  AEVFrustum[4][2] /= t;
  AEVFrustum[4][3] /= t;

  /* Extract the NEAR plane */
  AEVFrustum[5][0] = clip[ 3] + clip[ 2];
  AEVFrustum[5][1] = clip[ 7] + clip[ 6];
  AEVFrustum[5][2] = clip[11] + clip[10];
  AEVFrustum[5][3] = clip[15] + clip[14];

  /* Normalize the result */
  t = sqrtf( AEVFrustum[5][0] * AEVFrustum[5][0] + AEVFrustum[5][1] * AEVFrustum[5][1] + AEVFrustum[5][2] * AEVFrustum[5][2] );
  AEVFrustum[5][0] /= t;
  AEVFrustum[5][1] /= t;
  AEVFrustum[5][2] /= t;
  AEVFrustum[5][3] /= t;
}
