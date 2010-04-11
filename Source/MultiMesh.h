#pragma once
#include "Core.h"

typedef struct{
	AEVBO* vbo;
	unsigned int setCount,*keys,*lastIndexOfSets;
}AEMM;

AEMM* AEMMNew(void);
void AEMMAdd(AEMM* mm,unsigned int key,AEVBO* vbo);
AEMM* AEMMCompile(AEMM* mm);
void AEMMDraw(AEMM* mm,unsigned int set);
void AEMMDelete(AEMM* mm);