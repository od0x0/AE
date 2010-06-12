#include "../Node.h"
#include "../khash.h"
#include <string.h>


void* AENodeEventDefault(AENode* node,int event,void* data){
	if(event==AENodeEventInit){
		AENode* node=realloc(node,sizeof(AENode));
		memset(node,0,sizeof(AENode));
		return node;
	}
	else if(event==AENodeEventRelease) free(node);
	return NULL;
}

AENode* AENodeNew(AENodeEventFunc eventFunc,void* data){
	if(eventFunc==NULL)
		eventFunc=AENodeEventDefault;
	AENode* node=eventFunc(NULL,AENodeEventInit,NULL);
	node->event=AENodeEventDefault;
	return node;
}

/////////////////////////////////////////////////////////////////

KHASH_SET_INIT_STR(AENodeEventsType);
khash_t(AENodeEventsType) *AENodeEvents=NULL;
int AENodeEventsCurrentMax=2;

int AENodeEventsGetOrAdd(char* name){
	if(AENodeEvents==NULL){
		AENodeEvents=kh_init(AENodeEventsType);
	}
	if(name==NULL){
		printf("AENodeEventsGet(%s): Passed a null value\n",name);
		exit(1);
	}
	khiter_t iterator=kh_get(AENodeEventsType,AENodeEvents,name);
	if(kh_exist(AENodeEvents,iterator)) return kh_value(AENodeEvents,iterator);
	int success;
    iterator=kh_put(AENodeEventsType,AENodeEvents,strdup(name), &success);
    if (success==0)kh_del(AENodeEventsType,AENodeEvents,iterator);
    kh_value(AENodeEvents,iterator)=AENodeEventsCurrentMax;
	return AENodeEventsCurrentMax++;
}

int AENodeEventsGet(char* name){
	if(AENodeEvents==NULL) return -1;
	if(name==NULL) return -1;
	khiter_t iterator=kh_get(AENodeEventsType,AENodeEvents,name);
	if(kh_exist(AENodeEvents,iterator)) return kh_value(AENodeEvents,iterator);
	else return -1;
}

void AENodeEventsDelete(void){
	for (khiter_t i = kh_begin(AENodeEvents); i != kh_end(AENodeEvents); ++i)
        if (kh_exist(AENodeEvents, i)) free(kh_key(AENodeEvents, i));

	kh_destroy(AENodeEventsType,AENodeEvents);
	AENodeEvents=NULL;
}