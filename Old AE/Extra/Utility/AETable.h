#pragma once
#include <stdlib.h>
#include <string.h>

// A simple void pointer wrapper over khash.h

typedef void AETable;

void* AETableGet(AETable* table, const char* name);
void AETableSet(AETable* table, const char* name, void* value);
AETable* AETableNew(void);
void AETableDelete(AETable* table);

typedef void (*AETableEachFunc)(const char* name, void* value, void* arg);

void AETableEach(AETable* table, AETableEachFunc func, void* arg);
void* AETableRemove(void* table, char* name);

const char* AETableGetName(AETable* table, void* value);
