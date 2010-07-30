#pragma once
#include "HeaderBegin.h"
#include "VBO.h"

//Shamelessly ripped from http://www.lighthouse3d.com/opengl/billboarding/index.php3?billCheat
inline void AEBillboardLoadSphericalMatrix(void){
	float modelview[16];
	// get the current modelview matrix
	glGetFloatv(GL_MODELVIEW_MATRIX , modelview);

	// undo all rotations
	// beware all scaling is lost as well 
	for(int i=0; i<3; i++ ) 
		for(int j=0; j<3; j++ ) {
			if ( i==j )
				modelview[i*4+j] = 1.0;
			else
				modelview[i*4+j] = 0.0;
		}

	// set the modelview with no rotations and scaling
	glLoadMatrixf(modelview);
}

inline void AEBillboardLoadCylindricalMatrix(void){
	float modelview[16];
	// get the current modelview matrix
	glGetFloatv(GL_MODELVIEW_MATRIX , modelview);

	// undo all rotations
	// beware all scaling is lost as well 
	for(int i=0; i<3; i+=2 ) 
		for(int j=0; j<3; j++ ) {
			if ( i==j )
				modelview[i*4+j] = 1.0;
			else
				modelview[i*4+j] = 0.0;
		}

	// set the modelview with no rotations and scaling
	glLoadMatrixf(modelview);
}

inline void AEFOIRender(AEVBO* vbo,unsigned int texture,unsigned int objectCount,AEVec3* positions){
	AETextureBind(texture);
	AEVBOBind(vbo);
	for(unsigned int i=0;i<objectCount;i++){
		glTranslatef(positions[i].x,positions[i].y,positions[i].z);
		AEVBODraw(vbo);
	}
	AEVBOBind(NULL);
}

#include "HeaderEnd.h"