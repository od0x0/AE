#pragma once
#include "AE.h"

//Packs multiple images into one, stores the texture coordinates in outTC (x,y,x2,y2) (in 0..1 range)
void AEImagePack(AEImage* self, AEImage** images, size_t imageCount, float* outTC);

//Utility
typedef struct AEImagePackNode AEImagePackNode;
struct AEImagePackNode{
	AEImagePackNode* children[2];
	int x, y, w, h;
	void* tag;
};

AEImagePackNode* AEImagePackNodeNew(void);
void AEImagePackNodeDelete(AEImagePackNode* self);
AEImagePackNode* AEImagePackNodeInsert(AEImagePackNode* self, int w, int h, void* tag);
