#pragma once
#include <stdlib.h>
#include <string.h>

typedef void AETable;

AETable* AETableGet(AETable* table, char* name);
void AETableSet(AETable* table, char* name, void* value);
AETable* AETableNew(void);
void AETableDelete(AETable* table);

typedef void (*AETableEachFunc)(char* name, void* value, void* arg);

void AETableEach(AETable* table, AETableEachFunc func, void* arg);
void* AETableRemove(void* table, char* name);

char* AETableGetName(AETable* table, void* value);