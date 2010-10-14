#include "LZFArchiver.h"

AELZFArchive* AELZFArchiveNew(void){
	return calloc(1, sizeof(AELZFArchive));
}

static size_t AEFileLength(FILE* file){
	if(!file) return 0;
	fseek (file,0,SEEK_END);
	size_t length=ftell(file);
	rewind(file);
	return length;
}

/*char* AEFileBytes(const char* filename){
	FILE* file=fopen(filename,"r");
	if(!file) return NULL;
	fseek (file,0,SEEK_END);
	int length=ftell(file);
	rewind(file);
	char* string=calloc(length+1,1);
	fread(string,1,length,file);
	fclose(file);
	return string;
}*/

AELZFArchiveUInt64 AELZFArchiveAdd(AELZFArchive* archive,char* filename,int shouldCompress){
	if(archive==NULL) return 0;
	if(archive->filenames==NULL) archive->filenames=AEListNew(char*);
	if(archive->offsets==NULL) archive->offsets=AEListNew(AELZFArchiveUInt64);
	if(archive->isCompressed==NULL) archive->isCompressed=AEListNew(char);
	
	FILE* file=fopen(filename,"rb");
	if(!file) return 0;
	
	AELZFArchiveUInt64 offset=0;
	size_t length=AEFileLength(file);
	void* indata=malloc(length);
	fread(indata,1,length,file);
	fclose(file);
	//int lzfx_compress(const void* ibuf, unsigned int ilen,void* obuf, unsigned int *olen);
	
	if(shouldCompress){
		void* oldindata=indata;
		size_t oldlength=length;
		indata=malloc(length);
		unsigned int intlength;
		lzfx_compress(oldindata, oldlength, indata, &intlength);
		length=intlength;
		free(oldindata);
	}
	
	offset=archive->size;
	archive->size+=length;
	if(archive->allocated < archive->size){
		archive->allocated=archive->size;
		archive->data=realloc(archive->data, archive->allocated);
	}
	
	memcpy(archive->data+offset, indata, length);

	
	AEListAdd(archive->filenames,char*,strdup(filename));
	AEListAdd(archive->offsets,AELZFArchiveUInt64,offset);
	AEListAdd(archive->isCompressed,char,shouldCompress);
	
	return AEListLength(archive->filenames);
}

void AELFZArchiveRemove(AELZFArchive* archive,AELZFArchiveUInt64 section){
	if(archive==NULL) return;
	if(archive->filenames==NULL) return;
	if(archive->offsets==NULL) return;
	if(archive->isCompressed==NULL) return;
	if(archive->data==NULL) return;
}

AELZFArchiveUInt64 AELZFArchiveGet(AELZFArchive* archive,char* filename){
	if(archive==NULL) return 0;
	if(archive->filenames==NULL) return 0;
	if(archive->offsets==NULL) return 0;
	if(archive->isCompressed==NULL) return 0;
	if(archive->data==NULL) return 0;
	
	for(size_t i=0;i<AEListLength(archive->filenames);i++)
		if(strcmp(filename,AEListAsArray(archive->filenames,char*)[i])==0)
			return i+1;
			
	return 0;
}

static AELZFArchiveUInt64 AELZFArchiveGetCompressedSize(AELZFArchive* archive,AELZFArchiveUInt64 section){
	if(archive==NULL) return 0;
	if(archive->filenames==NULL) return 0;
	if(archive->offsets==NULL) return 0;
	if(archive->isCompressed==NULL) return 0;
	if(archive->data==NULL) return 0;
	
	size_t i=section-1;
	
	if(i==AEListLength(archive->offsets)) return archive->size-AEListAsArray(archive->offsets,AELZFArchiveUInt64)[i];
	
	return AEListAsArray(archive->offsets,AELZFArchiveUInt64)[i+1]-AEListAsArray(archive->offsets,AELZFArchiveUInt64)[i];
}

void* AELZFArchiveGetData(AELZFArchive* archive,AELZFArchiveUInt64 section,size_t* size){
	if(archive==NULL) return 0;
	if(archive->filenames==NULL) return 0;
	if(archive->offsets==NULL) return 0;
	if(archive->isCompressed==NULL) return 0;
	if(archive->data==NULL) return 0;
	
	void* compresseddata=archive->data+AEListAsArray(archive->offsets,AELZFArchiveUInt64)[section-1];
	size_t compresseddatalength=AELZFArchiveGetCompressedSize(archive,section);
	void* decompresseddata=NULL;
	if(AEListAsArray(archive->isCompressed,char)[section-1]){
		unsigned int olen=0;
		lzfx_decompress(compresseddata,compresseddatalength,NULL,&olen);
		decompresseddata=malloc(olen);
		lzfx_decompress(compresseddata, compresseddatalength, decompresseddata, &olen);
	}
	else{
		decompresseddata=malloc(compresseddatalength);
		memcpy(decompresseddata, compresseddata, compresseddatalength);
	}
	return decompresseddata;
}

void AELZFArchiveDelete(AELZFArchive* archive){
	if(archive==NULL) return;
	free(archive->data);
	AEListDelete(archive->filenames);
	AEListDelete(archive->offsets);
	AEListDelete(archive->isCompressed);
	free(archive);
}

void AELZFArchiveSave(AELZFArchive* archive,char* filename){
	
}

AELZFArchive* AELZFArchiveLoad(char* filename){
	return NULL;
}