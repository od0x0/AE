#include "../UI.h"
#include "../Core.h"

void AEUIRenderMode(int on){
	static int lastState=0;
	if(lastState==on){
		printf("AEUIRenderMode was called while mode was %s to change to %s\n",lastState?"3d":"2d",on?"3d":"2d");
			exit(1);
	}
	lastState=on;
	if(on){
		glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glOrtho(0,AEActiveState.w,0,AEActiveState.h,0,1000);
		glMatrixMode(GL_MODELVIEW);
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
	}
	else{
		glMatrixMode(GL_PROJECTION);
			glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
	}
}

