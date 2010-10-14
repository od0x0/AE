#include "A2RMap.h"

/////////////////////////////////////
////////////Map Asset////////////////
/////////////////////////////////////

typedef struct{
	size_t refcount;
	int type;
}A2RMapAsset;

typedef struct{
	A2RMapAsset super;
	AEVA va;
}A2RMapAssetVA;

typedef struct{
	A2RMapAsset super;
	AETexture texture;
}A2RMapAssetTexture;

#define A2RMapAssetTypeVA 1
#define A2RMapAssetTypeTexture 2

A2RMapAsset* A2RMapAssetRetain(A2RMapAsset* self){
	if(self) self->refcount++;
	return self;
}

AETable* A2RMapAssets=NULL;

AETexture A2RMapAssetTextureGet(A2RMapAsset* self){
	A2RMapAssetTexture* asset=(A2RMapAssetTexture*)self;
	return asset->texture;
}

AEVA* A2RMapAssetVAGet(A2RMapAsset* self){
	A2RMapAssetVA* asset=(A2RMapAssetVA*)self;
	return & asset->va;
}

A2RMapAsset* A2RMapAssetTextureLoad(char* filename){
	
	if(A2RMapAssets){
		A2RMapAssetTexture* asset = AETableGet(A2RMapAssets, filename);
		if(asset) return A2RMapAssetRetain(& asset->super);
	}

	A2RMapAssetTexture* self = calloc(1, sizeof(A2RMapAssetTexture));
	self->texture=AETextureLoad(filename);
	A2RMapAsset* super = & self->super;
	super->type = A2RMapAssetTypeTexture;
	
	if(not A2RMapAssets) A2RMapAssets=AETableNew();
	AETableSet(A2RMapAssets, filename, self);
	
	return A2RMapAssetRetain(super);
}

A2RMapAsset* A2RMapAssetVALoad(char* filename){
	
	if(A2RMapAssets){
		A2RMapAssetTexture* asset = AETableGet(A2RMapAssets, filename);
		if(asset) return A2RMapAssetRetain(& asset->super);
	}
	
	A2RMapAssetVA* self = calloc(1, sizeof(A2RMapAssetVA));
	FILE* file=fopen(filename, "rb");
		AEVAUnserializeFromFILE(& self->va, file);
	fclose(file);
	A2RMapAsset* super = & self->super;
	super->type = A2RMapAssetTypeVA;
	
	if(not A2RMapAssets) A2RMapAssets=AETableNew();
	AETableSet(A2RMapAssets, filename, self);
	
	return A2RMapAssetRetain(super);
}

void A2RMapAssetDelete(A2RMapAsset* self){
	if(not self) return;
	self->refcount--;
	if(self->refcount) return;
	
	AETableRemove(A2RMapAssets, AETableGetName(A2RMapAssets, self));
	
	switch (self->type) {
		case A2RMapAssetTypeVA:
			AEVADeinit(& ((A2RMapAssetVA*)self)->va);
			break;
		case A2RMapAssetTypeTexture:
			AETextureDelete(((A2RMapAssetTexture*)self)->texture);
			break;
		default:
			AEError("Unknown Asset Type.");
			break;
	}
	
	free(self);
}

static void A2RMapAssetsDeleteAll_each(char* key, void* value, void* arg){
	A2RMapAssetDelete(value);
}

void A2RMapAssetsDeleteAll(void){
	if(not A2RMapAssets) return;
	AETableEach(A2RMapAssets, A2RMapAssetsDeleteAll_each, NULL);
	AETableDelete(A2RMapAssets);
}

/////////////////////////////////////
////////////Map Cell/////////////////
/////////////////////////////////////

//AXA = AX-is A-ngle
//How do we switch between imposter/foi?

typedef struct A2RMapCell A2RMapCell;

struct A2RMapCell {
	size_t refcount;
	
	AEVec3 cellPosition;

	struct{
		AEVA va;//tv formatted
		AEVA ia;
		AETexture texture;
	}main;

	struct{
		AEVA* vas;
		AEVA* ias;
		AEVec3* positions;
		AEVec4* rotations;
		char* inactives;
		AETexture* textures;
		size_t* instanceCounts;
		size_t foiSetCount;
	}foi;
};

A2RMapCell* A2RMapCellRetain(A2RMapCell* self){
	if(self) self->refcount++;
	return self;
}

A2RMapCell* A2RMapCellNew(void){
	A2RMapCell* self = calloc(1, sizeof(A2RMapCell));
	self = A2RMapCellRetain(self);
	return self;
}

void A2RMapCellDelete(A2RMapCell* self){
	if(not self) return;
	self->refcount--;
	if(self->refcount) return;
	
	for (size_t i=0; i<self->foi.foiSetCount; i++) {
		AEVADeinit(self->foi.vas+i);
		AEVADeinit(self->foi.ias+i);
		AETextureDelete(self->foi.textures[i]);
	}
	
	AEVADeinit(& self->main.va);
	AEVADeinit(& self->main.ia);
	AETextureDelete(self->main.texture);
	
	free(self->foi.vas);
	free(self->foi.ias);
	free(self->foi.positions);
	free(self->foi.rotations);
	free(self->foi.inactives);
	free(self->foi.textures);
	free(self->foi.instanceCounts);
	
	free(self);
}

void A2RMapCellRender(A2RMapCell* self){
	{
	const AEVec3* positions = self->foi.positions;
	const AEVec4* rotations = self->foi.rotations;
	const AETexture* textures = self->foi.textures;
	size_t item=0;
	
	for (size_t i=0; i<self->foi.foiSetCount; i++) {
		const size_t childCount = self->foi.instanceCounts[i];
		
		AETextureBind(textures[i]);
		
		AEVABind(self->foi.vas+i);
		AEVABind(self->foi.ias+i);
		const AEVA* activeia = self->foi.ias+i;
		
		for(size_t j=0; j<childCount; j++){
			const bool isInactive = self->foi.inactives[item];
			if(isInactive) continue;
			
			const AEVec3 p = positions[item];
			const AEVec4 r = rotations[item];
			
			glTranslatef(p.x, p.y, p.z);
			if(r.w) glRotatef(r.w, r.x, r.y, r.z);
			
			AEVADraw(0, activeia->length);
			
			if(r.w) glRotatef(-r.w, r.x, r.y, r.z);
			glTranslatef(-p.x, -p.y, -p.z);
			item++;
		}
	}
	}
	
	AEVABind(& self->main.va);
	AEVABind(& self->main.ia);
	AETextureBind(self->main.texture);
}

void A2RMapCellSerializeToFILE(A2RMapCell* self,FILE* file){
	
}

//Found: http://www.siafoo.net/snippet/75
//fgetstr() - mimics behavior of fgets(), but removes new-line
//   character at end of line if it exists
static char *fgetstr(char *string, int n, FILE *stream)
{
	char *result;
	result = fgets(string, n, stream);
	if(!result)
		return(result);

	if(string[strlen(string) - 1] == '\n')
		string[strlen(string) - 1] = 0;

	return(string);
}

void A2RMapCellUnserializeFromFILE(A2RMapCell* self,FILE* file){
	
	//Clear the old data
	//This file is text based
	char line[1024];
	
	while (true) {
		fgetstr(line, 1023, file);
		A2RMapAsset* asset=NULL;
		if(line[0]=='V' and line[1]=='A' and line[3]=='@'){
			asset=A2RMapAssetVALoad(line+3);
		}
		else if(line[0]=='T' and line[1]=='X' and line[3]=='@'){
			asset=A2RMapAssetTextureLoad(line+3);
		}
		else break;
	}
}

/*void AETextureSerialize(AETexture* self, FILE* file){
	if(not *self) return;
	uint8_t buffer[16];
	//fread(buffer, 1, sizeof(uint64_t), file);
	uint64_t length=0;
	//AEUInt64From8Bytes(buffer, true);
	
	
	GLint width, height, internalFormat, isCompressed, compressedImageSize;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &internalFormat);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED, &isCompressed);
	if(isCompressed) glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED_IMAGE_SIZE, &compressedImageSize);
	
	//glGetTexImage(GL_TEXTURE_2D, 0, <#GLenum format#>, <#GLenum type#>, <#GLvoid *pixels#>);
	
	//SOIL_save_image(<#const char *filename#>, <#int image_type#>, width, height, <#int channels#>, <#const unsigned char *data#>)
	
	void* data=malloc(length);
	fwrite(data, 1, length, file);
	
	free(data);
}

void AETextureUnserialize(AETexture* self, FILE* file){
	AETextureDelete(*self);
	uint8_t buffer[16];
	fread(buffer, 1, sizeof(uint64_t), file);
	uint64_t length=AEUInt64From8Bytes(buffer, true);
	void* data=malloc(length);
	fread(data, 1, length, file);
	*self=AETextureLoadFromMemoryWithFlags(data, length, SOIL_FLAG_COMPRESS_TO_DXT|SOIL_FLAG_MIPMAPS);
	free(data);
}*/

/*void AETextureSerialize(AETexture* self, FILE* file){
	if(not *self) return;
	uint8_t buffer[16];
	//fread(buffer, 1, sizeof(uint64_t), file);
	uint64_t length=0;
	//AEUInt64From8Bytes(buffer, true);
	
	
	GLint width, height, internalFormat, isCompressed, compressedImageSize;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &internalFormat);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED, &isCompressed);
	if(isCompressed) glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED_IMAGE_SIZE, &compressedImageSize);
	
	//glGetTexImage(GL_TEXTURE_2D, 0, <#GLenum format#>, <#GLenum type#>, <#GLvoid *pixels#>);
	
	//SOIL_save_image(<#const char *filename#>, <#int image_type#>, width, height, <#int channels#>, <#const unsigned char *data#>)
	
	void* data=malloc(length);
	fwrite(data, 1, length, file);
	
	free(data);
}

void AETextureUnserialize(AETexture* self, FILE* file){
	AETextureDelete(*self);
	uint8_t buffer[16];
	fread(buffer, 1, sizeof(uint64_t), file);
	uint64_t length=AEUInt64From8Bytes(buffer, true);
	void* data=malloc(length);
	fread(data, 1, length, file);
	*self=AETextureLoadFromMemoryWithFlags(data, length, SOIL_FLAG_COMPRESS_TO_DXT|SOIL_FLAG_MIPMAPS);
	free(data);
}*/