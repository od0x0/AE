#pragma once
#include "List.h"
#include "lzfx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned long long AELZFArchiveUInt64;

typedef struct{
	AELZFArchiveUInt64 size, allocated;
	AEList* isCompressed;//of char
	AEList* filenames;//of char*
	AEList* offsets;//of AELZFArchiveUInt64
	void* data;
}AELZFArchive;

AELZFArchive* AELZFArchiveNew(void);
AELZFArchiveUInt64 AELZFArchiveAdd(AELZFArchive* archive,char* filename,int shouldCompress);
void AELFZArchiveRemove(AELZFArchive* archive,AELZFArchiveUInt64 section);
AELZFArchiveUInt64 AELZFArchiveGet(AELZFArchive* archive,char* filename);
//AELZFArchiveUInt64 AELZFArchiveGetSize(AELZFArchive* archive,AELZFArchiveUInt64 section);
void* AELZFArchiveGetData(AELZFArchive* archive,AELZFArchiveUInt64 section,size_t* size);
void AELZFArchiveDelete(AELZFArchive* archive);

void AELZFArchiveSave(AELZFArchive* archive,char* filename);
AELZFArchive* AELZFArchiveLoad(char* filename);