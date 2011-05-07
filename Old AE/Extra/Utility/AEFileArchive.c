#include "AEFileArchive.h"

//Compression has bugs
//#define COMPRESSSION_ENABLED

typedef enum {
	AEFileArchiveRegionFlagNone,
//	AEFileArchiveRegionFlagLZFX=1<<1
}AEFileArchiveRegionFlag;

#define HEADER_UINT64_COUNT 4

typedef struct {
	uint64_t ID, offset, size, flags;
	void* data;//This is here just because I want to reuse the structure for the archiver
}AEFileArchiveIndex;

struct AEFileArchive {
	FILE* file;
	uint64_t indexCount;
	AEFileArchiveIndex* indices;
	uint64_t mappedRegionID;
	void* mappedRegionData;
	size_t mappedRegionSize;
};

static int CompareIndices(const void* a, const void* b){
	return ((const AEFileArchiveIndex*)a)->ID - ((const AEFileArchiveIndex*)b)->ID;
}

static inline void UInt64WriteToFile(uint64_t value, FILE* file){
	//printf("write %lu\n",(unsigned long)value);
	AENetFromHost64(&value, &value, 1);
	fwrite(&value, 1, sizeof(uint64_t), file);
}

static inline uint64_t UInt64ReadFromFile(FILE* file){
	uint64_t value=0;
	fread(&value, 1, sizeof(uint64_t), file);
	AENetFromHost64(&value, &value, 1);
	//printf("read %lu\n",(unsigned long)value);
	return value;
}

AEFileArchive* AEFileArchiveNewFromFilepath(const char* filepath){
	FILE* file=fopen(filepath, "rb");
	if(not file) return NULL;
	
	AEFileArchive* self=calloc(1, sizeof(AEFileArchive));
	self->file=file;
	
	self->indexCount=UInt64ReadFromFile(file);
	
	self->indices=malloc(self->indexCount*sizeof(AEFileArchiveIndex));
	
	for (uint64_t i=0; i<self->indexCount; i++) self->indices[i].ID=UInt64ReadFromFile(file);
	for (uint64_t i=0; i<self->indexCount; i++) self->indices[i].offset=UInt64ReadFromFile(file);
	for (uint64_t i=0; i<self->indexCount; i++) self->indices[i].size=UInt64ReadFromFile(file);
	for (uint64_t i=0; i<self->indexCount; i++) self->indices[i].flags=UInt64ReadFromFile(file);
	
	//printf("Index Count = %lu\n", (unsigned long)self->indexCount);
	
	return self;
}

void AEFileArchiveDelete(AEFileArchive* self){
	if(not self) return;
	fclose(self->file);
	free(self->mappedRegionData);
	free(self->indices);
}

const void* AEFileArchiveMapRegion(AEFileArchive* self, uint64_t ID, size_t* size){
	free(self->mappedRegionData);
	self->mappedRegionData=NULL;
	self->mappedRegionSize=0;
	self->mappedRegionID=0;
	if(ID==0 and size==NULL) return NULL;
	
	AEFileArchiveIndex key;
	key.ID=ID;
	key.offset=0;
	
	AEFileArchiveIndex* index=bsearch(&key, self->indices, self->indexCount, sizeof(AEFileArchiveIndex), CompareIndices);
	
	if(index==NULL) return NULL;
	
	rewind(self->file);
	
	uint64_t offset=index->offset;
	while (offset > LONG_MAX) {
		fseek(self->file, LONG_MAX, SEEK_CUR);
		offset-=LONG_MAX;
	}
	fseek(self->file, offset, SEEK_CUR);
	
	//if(not (index->flags & AEFileArchiveRegionFlagLZFX)){
	self->mappedRegionID=index->ID;
	self->mappedRegionSize=index->size;
	self->mappedRegionData=malloc(self->mappedRegionSize);
	fread(self->mappedRegionData, 1, self->mappedRegionSize, self->file);
	//}
	/*else {
	uint64_t compressedSize=index->size;
	void* compressedData=malloc(index->size);
	
	fread(compressedData, 1, compressedSize, self->file);
	
	unsigned int decompressedSize=0;
	if(LZFX_EARGS not_eq lzfx_decompress(compressedData, compressedSize, NULL, &decompressedSize)) AEError("Decompression Stage 1 Failed.");
	
	self->mappedRegionID=index->ID;
	self->mappedRegionData=malloc(decompressedSize);
	
	int errorCode=0;
	if(errorCode=lzfx_decompress(compressedData, compressedSize, self->mappedRegionData, &decompressedSize)){
		printf("%s():  Decompression Stage 2 Failed With Error Code %i\n", __func__, errorCode);
		abort();
	}
	
	self->mappedRegionSize=decompressedSize;
	*size=decompressedSize;
	
	printf("Size: %lu\n", (unsigned long)self->mappedRegionSize);
	for (size_t i=0; i<self->mappedRegionSize; i++) {
		int c=((char*)self->mappedRegionData)[i];
		printf("%c: %i, ", (char)c, c);
	}
	putc('\n', stdout);
	
	printf("Size: %lu\n", (unsigned long)compressedSize);
	for (size_t i=0; i<compressedSize; i++) {
		int c=((char*)compressedData)[i];
		printf("%c: %i, ", (char)c, c);
	}
	putc('\n', stdout);
	
	free(compressedData);
	}*/
	
	return self->mappedRegionData;
}

struct AEFileArchiver {
	AEArray(AEFileArchiveIndex) indexArray;
};

AEFileArchiver* AEFileArchiverNew(void){
	AEFileArchiver* self=calloc(1, sizeof(AEFileArchiver));
	AEArrayInit(&self->indexArray);
	return self;
}

void AEFileArchiverDelete(AEFileArchiver* self){
	if(not self) return;
	for (size_t i=0; i<AEArrayLength(&self->indexArray); i++) {
		free(AEArrayAsCArray(&self->indexArray)[i].data);
	}
	AEArrayDeinit(&self->indexArray);
	free(self);
}

void AEFileArchiverAppendRegion(AEFileArchiver* self, uint64_t ID, const void* data, size_t size){
	if(ID==0) return;
	AEFileArchiveIndex index;
	index.offset=0;
	index.ID=ID;
	index.flags=AEFileArchiveRegionFlagNone;
	
	/*unsigned int compressedSize=size;
	void* compressedData=malloc(size);
	do {
		int errorCode=lzfx_compress(data, size, compressedData, &compressedSize);
		if(errorCode) break;
		if(errorCode==LZFX_ESIZE) break;
		else{
			printf("Error Code = %i\n", errorCode);
			AEError("Failed Compression");
		}
		if(compressedSize >= size) break;
		index.flags|=AEFileArchiveRegionFlagLZFX;
		compressedData=realloc(compressedData, compressedSize);
		index.data=compressedData;
		index.size=compressedSize;
	} while (0);
	
	if(not (index.flags & AEFileArchiveRegionFlagLZFX)){
		//puts("Writing uncompressed.");
		free(compressedData);*/
		index.data=memcpy(malloc(size), data, size);
		index.size=size;
	//}

	AEArrayAddBytes(&self->indexArray, &index);
}

void AEFileArchiverWriteToFilepath(AEFileArchiver* self, const char* filepath){
	FILE* file=fopen(filepath, "wb");
	const uint64_t indexCount=AEArrayLength(&self->indexArray);
	AEFileArchiveIndex* indices=AEArrayAsCArray(&self->indexArray);
	qsort(indices, indexCount, AEArrayTypeSize(&self->indexArray), CompareIndices);
	
	{
	uint64_t offset=sizeof(uint64_t);
	offset+=indexCount*HEADER_UINT64_COUNT*sizeof(uint64_t);
	for (size_t i=0; i<indexCount; i++) {
		indices[i].offset=offset;
		//printf("Offset #%lu = %lu\n",(unsigned long)i, (unsigned long)offset);
		offset+=indices[i].size;
	}
	}
	
	UInt64WriteToFile(indexCount, file);
	
	for (uint64_t i=0; i<indexCount; i++) UInt64WriteToFile(indices[i].ID, file);
	for (uint64_t i=0; i<indexCount; i++) UInt64WriteToFile(indices[i].offset, file);
	for (uint64_t i=0; i<indexCount; i++) UInt64WriteToFile(indices[i].size, file);
	for (uint64_t i=0; i<indexCount; i++) UInt64WriteToFile(indices[i].flags, file);
	for (uint64_t i=0; i<indexCount; i++) fwrite(indices[i].data, 1, indices[i].size, file);
	
	fclose(file);
}