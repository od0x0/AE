#include "../AE/Core.h"

int main(int argc,char** argv){
	AEInit("Window",800,500);
	//Code here
	printf("AEVBO is of size %i\n",sizeof(AEVBO));
	printf("AEObject is of size %i\n",sizeof(AEObject));
	AEStart(NULL);
}
