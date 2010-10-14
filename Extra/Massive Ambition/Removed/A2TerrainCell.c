#include "A2TerrainCell.h"

A2TerrainCell* A2TerrainCellRetain(A2TerrainCell* self){
	if(not self) return self;
	self->info.refcount++;
	return self;
}

A2TerrainCell* A2TerrainCellNew(void){
	A2TerrainCell* self=calloc(1, sizeof(A2TerrainCell));
	self->texture=0;
	self->normalmap=0;
	self->textureImage=NULL;
	self->normalmapImage=NULL;
	self->va=AEVANew(false, AEVAVBOTypeDynamic);
	self->ia=AEVANew(false, AEVAVBOTypeDynamic);
	return A2TerrainCellRetain(self);
}

void A2TerrainCellDelete(A2TerrainCell* self){
	if(not self) return;
	if(self->info.refcount--) return;
	AEVAClear(&(self->va));
	AEVAClear(&(self->ia));
	AEImageDelete(self->textureImage);
	AEImageDelete(self->normalmapImage);
	AETextureDelete(self->texture);
	AETextureDelete(self->normalmap);
	for(uint64_t i=0;i<self->info.cellCount;i++){
		A2TerrainCell* cell=self->info.cells[i];
		A2TerrainCellDelete(cell);
	}
	free(self);
}

void A2TerrainCellRender(A2TerrainCell* self){
	//We only bind the normalmap in the presence of a shader, there's no real way to handle this in fixed function anyway, this does serve to keep the code here cleaner as well
	AETextureBind(self->texture);
	AEVABindIndices(&(self->ia));
	glTranslatef(self->info.min.x, self->info.min.y, self->info.min.z);
	if(self->info.hasNormals)
		AEVABindInterleavedTNV(&(self->va));
	else
		AEVABindInterleavedTV(&(self->va));
	AEVADraw(0,self->ia.length);
	glTranslatef(-self->info.min.x, -self->info.min.y, -self->info.min.z);
	for(uint64_t i=0;i<self->info.cellCount;i++){
		A2TerrainCell* cell=self->info.cells[i];
		A2TerrainCellRender(cell);
	}
}

A2TerrainCell* A2TerrainCellChildGet(A2TerrainCell* self,uint64_t i){
	if(not self) return NULL;
	if(i < self->info.cellCount) return self->info.cells[i];
	return NULL;
}

A2TerrainCell* A2TerrainCellLoad(char* filename,bool shouldLoadChildren){
	A2TerrainCell* self=A2TerrainCellNew();
	if(not filename) return self;
	FILE* file=fopen(filename, "rb");
	if(not file){
		printf("%s(%s,%s): Could not open.\n",__func__,filename,shouldLoadChildren ? "true" : "false");
		abort();
	}
	
	uint8_t buffer[256];
	uint64_t fileformatversion=0;
	
	fread(buffer, sizeof(uint64_t), 1, file);
	fileformatversion=AEUInt64From8Bytes(buffer, true);
	
	switch (fileformatversion) {
		case 1:{
			fread(&(self->info.min), sizeof(AEVec3), 1, file);
			fread(&(self->info.max), sizeof(AEVec3), 1, file);
			fseek(file, 64-sizeof(AEVec3[2]), SEEK_CUR);//Skip the padding
			fread(buffer, sizeof(uint64_t), 1, file);
			if(shouldLoadChildren) break;
			uint64_t cellCount=AEUInt64From8Bytes(buffer, true);
			self->info.cells=calloc(cellCount,sizeof(A2TerrainCell*));
			for (size_t i=0; i<cellCount; i++) {
				char childfilename[128];
				snprintf(childfilename, 128-1, "%i.a2tc",(int)cellCount);
				self->info.cells[i]=A2TerrainCellLoad(childfilename,false);
			}
			self->info.cellCount=cellCount;
			}break;
		default:
			AEError("The info version # must be 1.");
			break;
	}
	
	fread(buffer, sizeof(uint64_t), 1, file);
	fileformatversion=AEUInt64From8Bytes(buffer, true);
	
	AEVAClear(&(self->va));
	
	switch (fileformatversion) {
		case 1:{
			fread(buffer, sizeof(uint64_t), 1, file);
			uint64_t floatCount=AEUInt64From8Bytes(buffer, true);
			//float2 t float3 v
			float* floats=AEVAMap(&(self->va), floatCount, GL_WRITE_ONLY);
			fread(floats, sizeof(float), floatCount, file);
			AEVAUnmap(&(self->va));
			self->info.hasNormals=false;
			}break;
		default:
			AEError("The va version # must be 1.");
			break;
	}
	
	fread(buffer, sizeof(uint64_t), 1, file);
	fileformatversion=AEUInt64From8Bytes(buffer, true);
	
	AEVAClear(&(self->ia));
	
	switch (fileformatversion) {
		case 1:{
			fread(buffer, sizeof(uint64_t), 1, file);
			uint64_t indexCount=AEUInt64From8Bytes(buffer, true);
			//uint32_t
			unsigned int* indices=AEVAMap(&(self->ia), indexCount, GL_WRITE_ONLY);
			for (uint64_t i=0; i<indexCount; i++) {
				fread(buffer, sizeof(uint32_t), 1, file);
				indices[i]=AEUInt32From4Bytes(buffer, true);
			}
			AEVAUnmap(&(self->ia));
			}break;
		default:
			AEError("The ia version # must be 1.");
			break;
	}
	
	fread(buffer, sizeof(uint64_t), 1, file);
	fileformatversion=AEUInt64From8Bytes(buffer, true);
	
	switch (fileformatversion) {
		case 1:{
			fread(buffer, sizeof(uint64_t), 1, file);
			const size_t imageBlockSize=AEUInt64From8Bytes(buffer, true);
			void* memory=malloc(imageBlockSize);
			fread(memory, imageBlockSize, 1, file);
			AEImageDelete(self->textureImage);
			self->textureImage=AEImageFromMemory(memory, imageBlockSize);
			free(memory);
			///if(self->texture==0) printf("Texture loading failed because %s",SOIL_last_result());
			
			}break;
		default:
			AEError("The texture version # must be 1.");
			break;
	}
	
	//Nope, I'm not loading the normalmap yet
	
	fclose(file);
	return self;
}

void A2TerrainCellSave(A2TerrainCell* self,char* filename){
	if(not self) return;
	if(not filename) return;
	FILE* file=fopen(filename, "wb");
	if(not file){
		printf("%s(%p,%s): Could not open.\n",__func__,(void*)self,filename);
		abort();
	}
	
	uint8_t buffer[256];
	uint64_t fileformatversion=1;
	
	AEUInt64To8Bytes(fileformatversion, buffer, true);
	fwrite(buffer, sizeof(uint64_t), 1, file);
	
	fwrite(&(self->info.min), sizeof(AEVec3), 1, file);
	fwrite(&(self->info.max), sizeof(AEVec3), 1, file);
	fseek(file, 64-sizeof(AEVec3[2]), SEEK_CUR);//Skip the padding
	fwrite(buffer, sizeof(uint64_t), 1, file);
	AEUInt64To8Bytes(self->info.cellCount, buffer, true);
	self->info.cells=calloc(cellCount,sizeof(A2TerrainCell*));
	for (size_t i=0; i<cellCount; i++) {
		char childfilename[128];
		uint64_t cellID=;
		snprintf(childfilename, 128-1, "%i.a2tc",(int)cellID);
		self->info.cells[i]=A2TerrainCellLoad(childfilename,false);
	}
	self->info.cellCount=cellCount;
	
	fread(buffer, sizeof(uint64_t), 1, file);
	fileformatversion=AEUInt64From8Bytes(buffer, true);
	
	fread(buffer, sizeof(uint64_t), 1, file);
	uint64_t floatCount=AEUInt64From8Bytes(buffer, true);
			//float2 t float3 v
	float* floats=AEVAMap(&(self->va), floatCount, GL_WRITE_ONLY);
	fread(floats, sizeof(float), floatCount, file);
	AEVAUnmap(&(self->va));
			self->info.hasNormals=false;
	
	fread(buffer, sizeof(uint64_t), 1, file);
	fileformatversion=AEUInt64From8Bytes(buffer, true);
	
	fread(buffer, sizeof(uint64_t), 1, file);
	uint64_t indexCount=AEUInt64From8Bytes(buffer, true);
			//uint32_t
	unsigned int* indices=AEVAMap(&(self->ia), indexCount, GL_WRITE_ONLY);
	for (uint64_t i=0; i<indexCount; i++) {
		fread(buffer, sizeof(uint32_t), 1, file);
		indices[i]=AEUInt32From4Bytes(buffer, true);
	}
	AEVAUnmap(&(self->ia));
	
	fread(buffer, sizeof(uint64_t), 1, file);
	fileformatversion=AEUInt64From8Bytes(buffer, true);
	
	fread(buffer, sizeof(uint64_t), 1, file);
	const size_t imageBlockSize=AEUInt64From8Bytes(buffer, true);
	void* memory=malloc(imageBlockSize);
	fread(memory, imageBlockSize, 1, file);
	AEImageDelete(self->textureImage);
	self->textureImage=AEImageFromMemory(memory, imageBlockSize);
	free(memory);
	
	//Nope, I'm not loading the normalmap yet
	
	fclose(file);
}

void A2TerrainCellLoadVAFromMemory(A2TerrainCell* self, void* _memory, size_t length){
	uint8_t* memory=_memory;
	uint64_t fileformatversion=AEUInt64From8Bytes(memory, true);
	memory+=sizeof(uint64_t);
	
	AEVAClear(&(self->va));
	
	switch (fileformatversion) {
		case 1:{
			uint64_t floatCount=AEUInt64From8Bytes(memory, true);
			memory+=sizeof(uint64_t);
			//float2 t float3 v
			float* floats=AEVAMap(&(self->va), floatCount, GL_WRITE_ONLY);
			memcpy(floats, memory, floatCount*sizeof(float));
			AEVAUnmap(&(self->va));
			self->info.hasNormals=false;
			memory+=floatCount*sizeof(float);
			}break;
		default:
			AEError("The version # must be 1.");
			break;
	}
}

void A2TerrainCellLoadIAFromMemory(A2TerrainCell* self, void* _memory, size_t length){
	uint8_t* memory=_memory;
	uint64_t fileformatversion=AEUInt64From8Bytes(memory, true);
	memory+=sizeof(uint64_t);
	
	AEVAClear(&(self->ia));
	
	switch (fileformatversion) {
		case 1:{
			uint64_t indexCount=AEUInt64From8Bytes(memory, true);
			memory+=sizeof(uint64_t);
			//uint32_t
			unsigned int* indices=AEVAMap(&(self->ia), indexCount, GL_WRITE_ONLY);
			for (uint64_t i=0; i<indexCount; i++) {
				indices[i]=AEUInt32From4Bytes(memory, true);
				memory+=sizeof(uint32_t);
			}
			AEVAUnmap(&(self->ia));
			}break;
		default:
			AEError("The version # must be 1.");
			break;
	}
}

void A2TerrainCellLoadTextureFromMemory(A2TerrainCell* self, void* _memory, size_t length){
	uint8_t* memory=_memory;
	uint64_t fileformatversion=AEUInt64From8Bytes(memory, true);
	memory+=sizeof(uint64_t);
	
	switch (fileformatversion) {
		case 1:{
			const size_t imageBlockSize=length-((size_t)memory-(size_t)_memory);
			self->texture=SOIL_load_OGL_texture_from_memory(memory, imageBlockSize, SOIL_LOAD_AUTO, self->texture, SOIL_FLAG_COMPRESS_TO_DXT |SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
			if(self->texture==0) printf("Texture loading failed because %s",SOIL_last_result());
			}break;
		default:
			AEError("The version # must be 1.");
			break;
	}
}
/*
void A2TerrainCellLoadInfoFromMemory(A2TerrainCell* self, void* _memory, size_t length){
	uint8_t* memory=_memory;
	uint64_t fileformatversion=AEUInt64From8Bytes(memory, true);
	memory+=sizeof(uint64_t);
	
	switch (fileformatversion) {
		case 1:{
			memcpy(&(self->info.min), memory, sizeof(AEVec3));
			memory+=sizeof(AEVec3);
			
			memcpy(&(self->info.max), memory, sizeof(AEVec3));
			memory+=sizeof(AEVec3);
			
			self->info.visibleCount=AEUInt64From8Bytes(memory, true);
			memory+=sizeof(uint64_t);
			
			self->info.visibles=malloc(self->info.visibleCount*sizeof(unsigned int));
			for (size_t i=0; i<self->info.visibleCount; i++) {
				self->info.visibles[i]=AEUInt32From4Bytes(memory, true);
				memory+=sizeof(uint32_t);
			}
			}break;
		default:
			AEError("The version # must be 1.");
			break;
	}
}
*/
/*
	A2TC fileformat
	
	byte[8] padding
	float32[3] position
	byte[8] padding
	byte[8] sizeOfTexture
	byte[] data
	byte[8] padding
	byte[8] sizeofNormalMap
	byte[] data
	byte[8] padding
	byte[8] rendermeshvcount
	byte[] data
	byte[8] rendermeshicount
	byte[] data
	byte[8] padding
	byte[8] physicmeshvcount
	byte[] data
	byte[8] physicmeshicount
	byte[] data
*/

/*void A2TerrainCellLoadFromMemory(A2TerrainCell* self,void* memory,size_t memorySize){
	//Future issue to handle:  Currently, we do not check the memory size to see if we went out of bounds
	//Common variables that get reused
	#define Uint64Size (sizeof(uint64_t))
	#define PaddingSize sizeof(uint8_t[8])
	uint64_t integers[3]={0,0,0};
	//Load the position
	memory+=PaddingSize;
	{
		size_t length=sizeof(AEVec3);
		void* to=&(self->position);
		memcpy(to, memory, length);
		memory+=length;
	}
	//Now load the texture
	memory+=PaddingSize;
	{
		//Clear what we had before
		AEImageDelete(self->textureImage);
		self->textureImage=NULL;
		
		uint64_t sectionSize=0;
		//Get the section size so that we can skip past the embedded image data
		{
			size_t length=Uint64Size;
			void* to=&sectionSize;
			memcpy(to, memory, length);
			sectionSize=AEUInt64From8Bytes(to, true);
			memory+=Uint64Size;
		}
		
		if(sectionSize) {
			//Load the image data from the file, it should be in a format that SOIL understands
			self->textureImage=AEImageFromMemory(memory, sectionSize);
			memory+=sectionSize;
		}
		else {
			//We get here if sectionSize is zero, meaning that we have no image to load, in that case we can just sit here and do nothing, for now...
			
		}
	}
	//Then load the normalmap
	memory+=PaddingSize;
	memory+=Uint64Size;
	//Load the rendermesh
	memory+=PaddingSize;
	{
		AEVAClear(&(self->va));
		//Vertex count
		uint64_t vcount=0;
		{
			void* to=&vcount;
			memcpy(to, memory, Uint64Size);
			vcount=AEUInt64From8Bytes(to, true);
			memory+=Uint64Size;
		}
		
		//Read the vdata
		{
			const int floatsPerVertex=2+3+3;
			float* data=AEVAMap(&(self->va), floatsPerVertex*vcount, GL_WRITE_ONLY);
			memcpy(data, memory, floatsPerVertex*vcount*sizeof(float));
			
		}
	}
	//And now the physicalmesh
	
	
	#undef Uint64Size
	#undef PaddingSize
}*/
//////////////////