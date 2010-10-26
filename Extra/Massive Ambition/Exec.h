#pragma once
#include "AECore.h"
#include <time.h>

struct AEExecItem;
typedef struct AEExecItem AEExecItem;

typedef void (*AEExecItemFunc)(AEExecItem* item);

AEExecItemFunc AEExecItemFuncGet(AEExecItem* self);
void* AEExecItemArgGet(AEExecItem* self);

struct AEExecQueue;
typedef struct AEExecQueue AEExecQueue;

AEExecQueue* AEExecQueueNew(void);
void AEExecQueueDelete(AEExecQueue* self);
size_t AEExecQueueRun(AEExecQueue* self,size_t execLimit);
void AEExecQueuePush(AEExecQueue* self,AEExecItemFunc func,void* arg,size_t argsize);