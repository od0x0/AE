#include "AERTT.h"

void AERTTInit(AERTT* self, int textureSize){
	memset(self, 0, sizeof(AERTT));
	self->textureSize=textureSize;
	glGenTextures(1, & self->texture);
	glBindTexture(GL_TEXTURE_2D, self->texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureSize, textureSize, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void AERTTDeinit(AERTT* self){
	if(not self) return;
	AETextureDelete(self->texture);
	memset(self, 0, sizeof(AERTT));
}

void AERTTBeginCapture(AERTT* self){
	glGetIntegerv(GL_VIEWPORT, self->viewportBounds);
	glViewport(0, 0, self->textureSize, self->textureSize);
	//By design, we do not clear the view.
}

void AERTTEndCapture(AERTT* self){
	glBindTexture(GL_TEXTURE_2D, self->texture);
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, self->textureSize, self->textureSize);
	glViewport(self->viewportBounds[0], self->viewportBounds[1], self->viewportBounds[2], self->viewportBounds[3]);
}