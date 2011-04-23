#pragma once
#include "AE.h"
#include <time.h>

struct AEExecItem;
typedef struct AEExecItem AEExecItem;

typedef void (*AEExecItemFunc)(AEExecItem* item);

AEExecItemFunc AEExecItemGetFunc(AEExecItem* self);
void* AEExecItemGetArg(AEExecItem* self);
uint32_t AEExecItemGetTag(AEExecItem* self);
uint32_t AEExecItemGetPriority(AEExecItem* self);

struct AEExecQueue;
typedef struct AEExecQueue AEExecQueue;

AEExecQueue* AEExecQueueNew(void);
void AEExecQueueDelete(AEExecQueue* self);
size_t AEExecQueueRun(AEExecQueue* self, size_t execLimit, bool shouldRemoveItemsAfterCompletion);
void AEExecQueuePush(AEExecQueue* self, uint64_t tag, uint32_t priority, AEExecItemFunc func, const void* arg);
void AEExecQueueSort(AEExecQueue* self);
AEExecItem* AEExecQueueLookupItem(AEExecQueue* self, uint64_t tag);
