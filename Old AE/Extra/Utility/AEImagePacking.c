#include "AEImagePacking.h"

void AEImagePack(AEImage* self, AEImage** images, size_t imageCount, float* outTC){
	AEImagePackNode* root=AEImagePackNodeNew();
	root->w=self->w;
	root->h=self->h;
	//printf("#%i: %p\n",__LINE__,root);
	for (size_t i=0; i<imageCount; i++) {
		if(not images[i]) continue;
		int w=images[i]->w;
		int h=images[i]->h;
		AEImagePackNode* node=AEImagePackNodeInsert(root, w, h, images[i]);
		//printf("#%i: %p\n",__LINE__,node);
		if(outTC){
			outTC[i*4+0]=INFINITY;
			outTC[i*4+1]=INFINITY;
			outTC[i*4+2]=INFINITY;
			outTC[i*4+3]=INFINITY;
		}
		if(not node) continue;
		AEImageBlit(self, node->x, node->y, images[i]);
		if(outTC){
			/*outTC[i*4+0]=(float)node->x/root->w;
			outTC[i*4+1]=(float)node->y/root->h;
			outTC[i*4+2]=outTC[i*4+0]+(float)node->w/root->w;
			outTC[i*4+3]=outTC[i*4+1]+(float)node->h/root->h;*/
			outTC[i*4+0]=(float)node->x;///root->w;
			outTC[i*4+1]=(float)node->y;///root->h;
			outTC[i*4+2]=outTC[i*4+0]+(float)node->w;///root->w;
			outTC[i*4+3]=outTC[i*4+1]+(float)node->h;///root->h;
		}
	}
	AEImagePackNodeDelete(root);
}

AEImagePackNode* AEImagePackNodeNew(void){
	return calloc(1, sizeof(AEImagePackNode));
}

void AEImagePackNodeDelete(AEImagePackNode* self){
	if(not self) return;
	AEImagePackNodeDelete(self->children[0]);
	AEImagePackNodeDelete(self->children[1]);
	free(self);
}

//Ported to C from public domain code by clb of gamedev.net
AEImagePackNode* AEImagePackNodeInsert(AEImagePackNode* self, int w, int h, void* tag)
{
	if(not self) return NULL;
	//if(self->tag) return NULL;
	if(self->w==0 or self->h==0) return NULL;
	
	// If this self is an internal self, try both leaves for possible space.
	// (The rectangle in an internal self stores used space, the leaves store free space)
	if (self->children[0] || self->children[1])
	{
		if (self->children[0])
		{
			AEImagePackNode *newNode = AEImagePackNodeInsert(self->children[0], w, h, tag);
			if (newNode)
				return newNode;
		}
		if (self->children[1])
		{
			AEImagePackNode *newNode = AEImagePackNodeInsert(self->children[1], w, h, tag);
			if (newNode)
				return newNode;
		}
		return NULL; // Didn't fit into either subtree!
	}

	// This self is a leaf, but can we fit the new rectangle here?
	if (w > self->w || h > self->h)
		return NULL; // Too bad, no space.

	// The new cell will fit, split the remaining space along the shorter axis,
	// that is probably more optimal.
	int xdifference = self->w - w;
	int ydifference = self->h - h;
	self->children[0] = AEImagePackNodeNew();
	self->children[1] = AEImagePackNodeNew();
	if (xdifference <= ydifference) // Split the remaining space in horizontal direction.
	{
		self->children[0]->x = self->x + w;
		self->children[0]->y = self->y;
		self->children[0]->w = xdifference;
		self->children[0]->h = h;

		self->children[1]->x = self->x;
		self->children[1]->y = self->y + h;
		self->children[1]->w = self->w;
		self->children[1]->h = ydifference;
	}
	else // Split the remaining space in vertical direction.
	{
		self->children[0]->x = self->x;
		self->children[0]->y = self->y + h;
		self->children[0]->w = w;
		self->children[0]->h = ydifference;

		self->children[1]->x = self->x + w;
		self->children[1]->y = self->y;
		self->children[1]->w = xdifference;
		self->children[1]->h = self->h;
	}
	// Note that as a result of the above, it can happen that self->children[0] or self->children[1]
	// is now a degenerate (zero area) rectangle. No need to do anything about it,
	// like remove the nodes as "unnecessary" since they need to exist as children of
	// this self (this self can't be a leaf anymore).

	// This self is now a non-leaf, so shrink its area - it now denotes
	// *occupied* space instead of free space. Its children spawn the resulting
	// area of free space.
	self->w = w;
	self->h = h;
	self->tag=tag;
	return self;
}
/*{
	if(not self) return NULL;
	if(self->tag) return NULL;
	
	if(self->w==0 or self->h==0){
		abort();
		return NULL;
	}
	
	printf("Trying to put an image of size %i, %i in a node of size %i, %i\n", w, h, self->w, self->h);
	
	//puts("Attempting Insert!");
	
	if(self->children[0] or self->children[1]){
		//puts("Inserting into child!");
		AEImagePackNode* node=AEImagePackNodeInsert(self->children[0], w, h, tag);
		//printf("#%i: %p\n",__LINE__,node);
		if(node){
			puts("Child 1!");
			return node;
		}
		printf("self->children[1] = {w=%i, h=%i}\n", self->children[1]->w, self->children[1]->h);
		node=AEImagePackNodeInsert(self->children[1], w, h, tag);
		//printf("#%i: %p\n",__LINE__,node);
		if(node) puts("Child 2!");
		return node;
	}
	
	int xdifference = self->w - w;
	int ydifference = self->h - h;
	
	if(xdifference < 0 or ydifference < 0){
		puts("Too small!");
		return NULL;
	}
	
	
	
	if(xdifference==0 and ydifference==0){
		self->tag=tag;
		puts("Just children[1]!");
		//printf("#%i: %p\n",__LINE__,self);
		return self;
	}
	
	self->children[0]=AEImagePackNodeNew();
	self->children[1]=AEImagePackNodeNew();
	
	
	if (xdifference <= ydifference) // Split the remaining space in horizontal direction.
	{
		self->children[0]->x = self->x + h;
		self->children[0]->y = self->y;
		self->children[0]->w = xdifference;
		self->children[0]->h = h;

		self->children[1]->x = self->x;
		self->children[1]->y = self->y + h;
		self->children[1]->w = self->w;
		self->children[1]->h = ydifference;
	}
	else // Split the remaining space in vertical direction.
	{
		self->children[0]->x = self->x;
		self->children[0]->y = self->y + h;
		self->children[0]->w = w;
		self->children[0]->h = ydifference;

		self->children[1]->x = self->x + w;
		self->children[1]->y = self->y;
		self->children[1]->w = xdifference;
		self->children[1]->h = self->h;
	}
	if(xdifference > ydifference) {
		self->children[0]->x=self->x;
		self->children[0]->y=self->y;
		self->children[0]->w=w;
		self->children[0]->h=h;
		self->children[1]->x=self->x+w;
		self->children[1]->y=self->y;
		self->children[1]->w=self->w-w;
		self->children[1]->h=self->h;
	}
	else {
		self->children[0]->x=self->x;
		self->children[0]->y=self->y;
		self->children[0]->w=w;
		self->children[0]->h=h;
		self->children[1]->x=self->x;
		self->children[1]->y=self->y+h;
		self->children[1]->w=self->w;
		self->children[1]->h=self->h-h;
	}
	
	//if(xdifference <= 0 or ydifference <= 0) abort();
	
	AEImagePackNode*  node=AEImagePackNodeInsert(self, w, h, tag);
	//printf("#%i: %p\n",__LINE__,node);
	return node;
	
	int xdifference = self->w - w;
	int ydifference = self->h - h;
	
	if(self->w==w and self->h==h){
		self->tag=tag;
		puts("Just children[1]!");
		//printf("#%i: %p\n",__LINE__,self);
		return self;
	}
	
	self->children[0]=AEImagePackNodeNew();
	self->children[1]=AEImagePackNodeNew();
	
	//int xdifference = w - self->w;
	//int ydifference = h - self->h;

	
	
	
	//puts("Splitting!");
	printf("%i > %i ? : %s\n", xdifference, ydifference, xdifference > ydifference ? "true" : "false");
	
	self->children[0]->x=self->x;
	self->children[0]->y=self->y;
	self->children[0]->w=w;
	self->children[0]->h=h;
	
	if(xdifference > ydifference) {
		self->children[1]->x=self->x+w;
		self->children[1]->y=self->y;
		self->children[1]->w=self->w-w;
		self->children[1]->h=self->h;
	}
	else {
		self->children[1]->x=self->x;
		self->children[1]->y=self->y+h;
		self->children[1]->w=self->w;
		self->children[1]->h=self->h-h;
	}
	
	//if(xdifference <= 0 or ydifference <= 0) abort();
	
	AEImagePackNode*  node=AEImagePackNodeInsert(self, w, h, tag);
	//printf("#%i: %p\n",__LINE__,node);
	return node;
}*/
