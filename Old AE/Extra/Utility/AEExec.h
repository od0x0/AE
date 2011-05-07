#pragma once
#include "AE.h"
#include <time.h>

struct AEExecItem;
typedef struct AEExecItem AEExecItem;

typedef bool (*AEExecItemFunc)(AEExecItem* item);//Returns true if it finished execution

AEExecItemFunc AEExecItemGetFunc(AEExecItem* self);
void* AEExecItemGetArg(AEExecItem* self);
uint32_t AEExecItemGetTag(AEExecItem* self);
uint32_t AEExecItemGetPriority(AEExecItem* self);

struct AEExecQueue;
typedef struct AEExecQueue AEExecQueue;

AEExecQueue* AEExecQueueNew(void);
void AEExecQueueDelete(AEExecQueue* self);
size_t AEExecQueueRun(AEExecQueue* self, size_t execLimit);
void AEExecQueuePush(AEExecQueue* self, uint64_t tag, uint32_t priority, AEExecItemFunc func, const void* arg);
void AEExecQueueSort(AEExecQueue* self);
AEExecItem* AEExecQueueLookupItem(AEExecQueue* self, uint64_t tag);

#ifdef AEExecQueueUnitTestEnabled
static bool AEExecQueueUnitTest_Print(AEExecItem* item){
	puts((const char*)AEExecItemGetArg(item));
	return 1;
}
void AEExecQueueUnitTest(void){
	printf("%s(): Beginning...\n", __func__);
	AEExecQueue* queue=AEExecQueueNew();
	if(not queue) AEError("Creating Queue Failed");
	AEExecQueuePush(queue, 0, 0, AEExecQueueUnitTest_Print, "Priority of 0.");
	AEExecQueuePush(queue, 0, 2, AEExecQueueUnitTest_Print, "Priority of 2.");
	AEExecQueuePush(queue, 0, 1, AEExecQueueUnitTest_Print, "Priority of 1.");
	AEExecQueueSort(queue);
	AEExecQueueRun(queue, 3);
	printf("%s(): 3 lines should have been written out to the console.\n", __func__);
	AEExecQueuePush(queue, 0, 2, AEExecQueueUnitTest_Print, "Priority of 2.");
	AEExecQueuePush(queue, 0, 1, AEExecQueueUnitTest_Print, "Priority of 1.");
	AEExecQueueSort(queue);
	AEExecQueueRun(queue, 1);
	printf("%s(): 1 line should have been written out to the console.\n", __func__);
	AEExecQueuePush(queue, 0, 2, AEExecQueueUnitTest_Print, "Priority of 2.");
	AEExecQueuePush(queue, 0, 1, AEExecQueueUnitTest_Print, "Priority of 1.");
	AEExecQueueSort(queue);
	AEExecQueueRun(queue, 0);
	printf("%s(): 3 lines should have been written out to the console.\n", __func__);
	AEExecQueueDelete(queue);
	printf("%s(): Done.\n", __func__);
}
#endif