#pragma once
#include "AE.h"
#include "lzfx.h"
#include "AEEndian.h"
typedef struct AEFileArchive AEFileArchive;

AEFileArchive* AEFileArchiveNewFromFilepath(const char* filepath);
void AEFileArchiveDelete(AEFileArchive* self);
const void* AEFileArchiveMapRegion(AEFileArchive* self, uint64_t ID, size_t* size);

typedef struct AEFileArchiver AEFileArchiver;

AEFileArchiver* AEFileArchiverNew(void);

void AEFileArchiverDelete(AEFileArchiver* self);

void AEFileArchiverAppendRegion(AEFileArchiver* self, uint64_t ID, const void* data, size_t size);

void AEFileArchiverWriteToFilepath(AEFileArchiver* self, const char* filepath);
