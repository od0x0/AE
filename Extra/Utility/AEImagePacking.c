#include "AEImagePacking.h"

void AEImagePack(AEImage* self, AEImage** images, size_t imageCount, float* outTC){
	AEPackNode* root=AEPackNodeNew();
	root->w=self->w;
	root->h=self->h;
	printf("#%i: %p\n",__LINE__,root);
	for (size_t i=0; i<imageCount; i++) {
		if(not images[i]) continue;
		int w=images[i]->w;
		int h=images[i]->h;
		AEPackNode* node=AEPackNodeInsert(root, w, h, images[i]);
		printf("#%i: %p\n",__LINE__,node);
		if(outTC){
			outTC[i*4+0]=0;
			outTC[i*4+1]=0;
			outTC[i*4+2]=0;
			outTC[i*4+3]=0;
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
	AEPackNodeDelete(root);
}

AEPackNode* AEPackNodeNew(void){
	return calloc(1, sizeof(AEPackNode));
}

void AEPackNodeDelete(AEPackNode* self){
	if(not self) return;
	AEPackNodeDelete(self->children[0]);
	AEPackNodeDelete(self->children[1]);
	free(self);
}

AEPackNode* AEPackNodeInsert(AEPackNode* self, int w, int h, void* tag){
	if(not self) return NULL;
	if(self->tag) return NULL;
	
	puts("Attempting Insert!");
	
	if(self->children[0] or self->children[1]){
		puts("Inserting into child!");
		AEPackNode* node=AEPackNodeInsert(self->children[0], w, h, tag);
		printf("#%i: %p\n",__LINE__,node);
		if(node){
			puts("Right!");
			return node;
		}
		node=AEPackNodeInsert(self->children[1], w, h, tag);
		printf("#%i: %p\n",__LINE__,node);
		if(node) puts("Left!");
		return node;
	}
	
	if(self->w < w or self->h < h){
		puts("Too small!");
		return NULL;
	}
	else if(self->w==w and self->h==h){
		self->tag=tag;
		puts("Just right!");
		printf("#%i: %p\n",__LINE__,self);
		return self;
	}

	self->children[0]=AEPackNodeNew();
	self->children[1]=AEPackNodeNew();
	
	int xdifference = w - self->w;
	int ydifference = h - self->h;
	
	puts("Splitting!");
	
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
	
	void* node= AEPackNodeInsert(self, w, h, tag);
	printf("#%i: %p\n",__LINE__,node);
	return node;
}