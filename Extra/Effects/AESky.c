#include "AE.h"
#include "AESky.h"
//This is very old ancient code that is really really really ugly and largely broken.
void AESkyboxRender(AESkybox* skybox){
	AEVec3 size=skybox->size;
	AETexture front=skybox->front;
	AETexture back=skybox->back;
	AETexture left=skybox->left;
	AETexture right=skybox->right;
	AETexture top=skybox->top;
	AETexture bottom=skybox->bottom;
	
	glDisable(GL_CULL_FACE);
	
	glPushMatrix();
	//glLoadIdentity();
	//AECameraBind(AECameraActiveGet());
	//AEVec3 position;
	//AECameraPositionGet(AECameraActiveGet(), &position);
	//glTranslatef(-position.x, -position.y, -position.z);
	glTranslatef(0,size.y/2,0);
	glScalef(size.x,size.y,size.z);
	// Back
	AETextureBind(back);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex3f(  0.5f, -0.5f, -0.5f );
		glTexCoord2f(1, 0); glVertex3f( -0.5f, -0.5f, -0.5f );
		glTexCoord2f(1, 1); glVertex3f( -0.5f,  0.5f, -0.5f );
		glTexCoord2f(0, 1); glVertex3f(  0.5f,  0.5f, -0.5f );
	glEnd();
	
	AETextureBind(right);
	// Render the Right quad
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex3f(  0.5f, -0.5f,  0.5f );
		glTexCoord2f(1, 0); glVertex3f(  0.5f, -0.5f, -0.5f );
		glTexCoord2f(1, 1); glVertex3f(  0.5f,  0.5f, -0.5f );
		glTexCoord2f(0, 1); glVertex3f(  0.5f,  0.5f,  0.5f );
	glEnd();
	
	AETextureBind(front);
	// Render the front quad
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex3f( -0.5f, -0.5f,  0.5f );
		glTexCoord2f(1, 0); glVertex3f(  0.5f, -0.5f,  0.5f );
		glTexCoord2f(1, 1); glVertex3f(  0.5f,  0.5f,  0.5f );
		glTexCoord2f(0, 1); glVertex3f( -0.5f,  0.5f,  0.5f );
	glEnd();
	
	AETextureBind(left);
	// Render the left quad
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex3f( -0.5f, -0.5f, -0.5f );
		glTexCoord2f(1, 0); glVertex3f( -0.5f, -0.5f,  0.5f );
		glTexCoord2f(1, 1); glVertex3f( -0.5f,  0.5f,  0.5f );
		glTexCoord2f(0, 1); glVertex3f( -0.5f,  0.5f, -0.5f );
	glEnd();
	
	AETextureBind(top);
	// Render the Top quad
	glBegin(GL_QUADS);
		glTexCoord2f(0, 1); glVertex3f( -0.5f,  0.5f, -0.5f );
		glTexCoord2f(0, 0); glVertex3f( -0.5f,  0.5f,  0.5f );
		glTexCoord2f(1, 0); glVertex3f(  0.5f,  0.5f,  0.5f );
		glTexCoord2f(1, 1); glVertex3f(  0.5f,  0.5f, -0.5f );
	glEnd();
	
	AETextureBind(bottom);
	// Render the Bottom quad
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex3f( -0.5f, -0.5f, -0.5f );
		glTexCoord2f(0, 1); glVertex3f( -0.5f, -0.5f,  0.5f );
		glTexCoord2f(1, 1); glVertex3f(  0.5f, -0.5f,  0.5f );
		glTexCoord2f(1, 0); glVertex3f(  0.5f, -0.5f, -0.5f );
	glEnd();
	
	glPopMatrix();
	
	glEnable(GL_CULL_FACE);
}