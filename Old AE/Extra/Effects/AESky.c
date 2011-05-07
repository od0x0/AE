#include "AESky.h"

void AESkyUtilityRenderBoxWithCubemap(AEVec3 min, AEVec3 max, GLuint cubemap){
	//glEnable(GL_TEXTURE_CUBE_MAP);
	//glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
	//glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, cubemap);
	/*GLfloat texcoordsAndVertices[24*2*4]={
		1,1,1,0, max.x,max.y,max.z,1, -1,1,1,0, min.x,max.y,max.z,1, -1,-1,1,0, min.x,min.y,max.z,1, -1,-1,1,0, max.x,min.y,max.z,1, //v0-v1-v2-v3
		1,1,1,0, max.x,max.y,max.z,1, 1,-1,1,0, max.x,min.y,max.z,1, 1,-1,-1,0, max.x,min.y,min.z,1, 1,1,-1,0, max.x,max.y,min.z,1, //v0-v3-v4-v5
		1,1,1,0, max.x,max.y,max.z,1, 1,1,-1,0, max.x,max.y,min.z,1, -1,1,-1,0, min.x,max.y,min.z,1, -1,1,1,0, min.x,max.y,max.z,1, //v0-v5-v6-v1
		-1,1,1,0, min.x,max.y,max.z,1, -1,1,-1,0, min.x,max.y,min.z,1, -1,-1,-1,0, min.x,min.y,min.z,1, -1,-1,1,0, min.x,min.y,max.z,1, //v1-v6-v7-v2
		-1,-1,-1,0, min.x,min.y,min.z,1, 1,-1,-1,0, max.x,min.y,min.z,1, 1,-1,1,0, max.x,min.y,max.z,1, -1,-1,1,0, min.x,min.y,max.z,1, //v7-v4-v3-v2
		1,-1,-1,0, max.x,min.y,min.z,1, -1,-1,-1,0, min.x,min.y,min.z,1, -1,1,-1,0, min.x,max.y,min.z,1, 1,1,-1,0, max.x,max.y,min.z,1//v4-v7-v6-v5;
	};*/
	/*GLfloat normalsAndVertices[4*6*2*3]={
		1,1,1, max.x,max.y,max.z, -1,1,1, min.x,max.y,max.z, -1,-1,1, min.x,min.y,max.z, -1,-1,1, max.x,min.y,max.z, //v0-v1-v2-v3
		1,1,1, max.x,max.y,max.z, 1,-1,1, max.x,min.y,max.z, 1,-1,-1, max.x,min.y,min.z, 1,1,-1, max.x,max.y,min.z, //v0-v3-v4-v5
		1,1,1, max.x,max.y,max.z, 1,1,-1, max.x,max.y,min.z, -1,1,-1, min.x,max.y,min.z, -1,1,1, min.x,max.y,max.z, //v0-v5-v6-v1
		-1,1,1, min.x,max.y,max.z, -1,1,-1, min.x,max.y,min.z, -1,-1,-1, min.x,min.y,min.z, -1,-1,1, min.x,min.y,max.z, //v1-v6-v7-v2
		-1,-1,-1, min.x,min.y,min.z, 1,-1,-1, max.x,min.y,min.z, 1,-1,1, max.x,min.y,max.z, -1,-1,1, min.x,min.y,max.z, //v7-v4-v3-v2
		1,-1,-1, max.x,min.y,min.z, -1,-1,-1, min.x,min.y,min.z, -1,1,-1, min.x,max.y,min.z, 1,1,-1, max.x,max.y,min.z//v4-v7-v6-v5;
	};*/
	
	/*glInterleavedArrays(GL_T4F_V4F, 0, texcoordsAndVertices);
	glDrawArrays(GL_QUADS, 0, 6*4);*/
	//glDisable(GL_TEXTURE_CUBE_MAP);
	/*GLfloat data[]={};
	for (size_t i=0; i<6; i++) {
		glInterleavedArrays(GL_T2F_V3F, 0, data+(i*5*));
		glDrawArrays(GL_QUADS, 0, 4);
	}*/
}
/*
GLfloat texcoordsAndVertices[]={
		1,1,1,0, max.x,max.y,max.z,1, -1,1,1,0, min.x,max.y,max.z,1, -1,-1,1,0, min.x,min.y,max.z,1, min.x,-1,1,0, 1,-1,1,1, //v0-v1-v2-v3
		1,1,1,0, 1,1,1,1, 1,-1,1,0, 1,-1,1,1, 1,-1,-1,0, 1,-1,-1,1, 1,1,-1,0, 1,1,-1,1, //v0-v3-v4-v5
		1,1,1,0, 1,1,1,1, 1,1,-1,0, 1,1,-1,1, -1,1,-1,0, -1,1,-1,1, -1,1,1,0, -1,1,1,1, //v0-v5-v6-v1
		-1,1,1,0, -1,1,1,1, -1,1,-1,0, -1,1,-1,1, -1,-1,-1,0, -1,-1,-1,1, -1,-1,1,0, -1,-1,1,1, //v1-v6-v7-v2
		-1,-1,-1,0, -1,-1,-1,1, 1,-1,-1,0, 1,-1,-1,1, 1,-1,1,0, 1,-1,1,1, -1,-1,1,0, -1,-1,1,1, //v7-v4-v3-v2
		1,-1,-1,0, 1,-1,-1,1, -1,-1,-1,0, -1,-1,-1,1, -1,1,-1,0, -1,1,-1,1, 1,1,-1,0, 1,1,-1,1//v4-v7-v6-v5;
	};

GLfloat texcoordsAndVertices[]={
		1,1,1,0, 1,1,1,1, -1,1,1,0, -1,1,1,1, -1,-1,1,0, -1,-1,1,1, 1,-1,1,0, 1,-1,1,1, //v0-v1-v2-v3
		1,1,1,0, 1,1,1,1, 1,-1,1,0, 1,-1,1,1, 1,-1,-1,0, 1,-1,-1,1, 1,1,-1,0, 1,1,-1,1, //v0-v3-v4-v5
		1,1,1,0, 1,1,1,1, 1,1,-1,0, 1,1,-1,1, -1,1,-1,0, -1,1,-1,1, -1,1,1,0, -1,1,1,1, //v0-v5-v6-v1
		-1,1,1,0, -1,1,1,1, -1,1,-1,0, -1,1,-1,1, -1,-1,-1,0, -1,-1,-1,1, -1,-1,1,0, -1,-1,1,1, //v1-v6-v7-v2
		-1,-1,-1,0, -1,-1,-1,1, 1,-1,-1,0, 1,-1,-1,1, 1,-1,1,0, 1,-1,1,1, -1,-1,1,0, -1,-1,1,1, //v7-v4-v3-v2
		1,-1,-1,0, 1,-1,-1,1, -1,-1,-1,0, -1,-1,-1,1, -1,1,-1,0, -1,1,-1,1, 1,1,-1,0, 1,1,-1,1//v4-v7-v6-v5;
	};
*/

void AESkyUtilityRenderBoxWithSixTextures(AEVec3 min, AEVec3 max, GLuint* textures){
	//glDisable(GL_CULL_FACE);
	AETextureBind(textures[0]);
	// Render the Right quad
	glBegin(GL_QUADS);
		glTexCoord2f(1, 0); glVertex3f(  max.x, min.y, min.z );
		glTexCoord2f(0, 0); glVertex3f(  max.x, min.y,  max.z );
		glTexCoord2f(0, 1); glVertex3f(  max.x,  max.y,  max.z );
		glTexCoord2f(1, 1); glVertex3f(  max.x,  max.y, min.z );
	glEnd();
	
	AETextureBind(textures[1]);
	// Render the left quad
	glBegin(GL_QUADS);
		glTexCoord2f(1, 0); glVertex3f( min.x, min.y,  max.z );
		glTexCoord2f(0, 0); glVertex3f( min.x, min.y, min.z );
		glTexCoord2f(0, 1); glVertex3f( min.x,  max.y, min.z );
		glTexCoord2f(1, 1); glVertex3f( min.x,  max.y,  max.z );
	glEnd();
	
	AETextureBind(textures[2]);
	// Render the Top quad
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex3f( min.x,  max.y,  max.z );
		glTexCoord2f(0, 1); glVertex3f( min.x,  max.y, min.z );
		glTexCoord2f(1, 1); glVertex3f(  max.x,  max.y, min.z );
		glTexCoord2f(1, 0); glVertex3f(  max.x,  max.y,  max.z );
	glEnd();
	
	AETextureBind(textures[3]);
	// Render the Bottom quad
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex3f( min.x, min.y, min.z );
		glTexCoord2f(0, 1); glVertex3f( min.x, min.y,  max.z );
		glTexCoord2f(1, 1); glVertex3f(  max.x, min.y,  max.z );
		glTexCoord2f(1, 0); glVertex3f(  max.x, min.y, min.z );
	glEnd();
	
	AETextureBind(textures[4]);
	//Render the back quad
	glBegin(GL_QUADS);
		glTexCoord2f(1, 0); glVertex3f( min.x, min.y, min.z );
		glTexCoord2f(0, 0); glVertex3f(  max.x, min.y, min.z );
		glTexCoord2f(0, 1); glVertex3f(  max.x,  max.y, min.z );
		glTexCoord2f(1, 1); glVertex3f( min.x,  max.y, min.z );
	glEnd();
	
	AETextureBind(textures[5]);
	// Render the front quad
	glBegin(GL_QUADS);
		glTexCoord2f(1, 0); glVertex3f(  max.x, min.y,  max.z );
		glTexCoord2f(0, 0); glVertex3f( min.x, min.y,  max.z );
		glTexCoord2f(0, 1); glVertex3f( min.x,  max.y,  max.z );
		glTexCoord2f(1, 1); glVertex3f(  max.x,  max.y,  max.z );
	glEnd();
	//glEnable(GL_CULL_FACE);
}
