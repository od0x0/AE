#include "../Ent.h"
//#include "../khash.h"
#include <string.h>

void* AEEntEventDefault(AEEnt* ent,int event,void* data){
	AEEntSignalChildren(ent,event,data);
	
	if(event==AEEntEventRender){
		if(ent->vbo==NULL || ent->texture==0) return NULL;
		glPushMatrix();
			glTranslatef(ent->x,ent->y,ent->z);
			glRotatef(ent->rotation.x,	1,0,0);
			glRotatef(ent->rotation.y,	0,1,0);
			glRotatef(ent->rotation.z,	0,0,1);
			
			AETextureBind(ent->texture);
			AEVBOBind(ent->vbo);
			AEVBODraw(ent->vbo);
		glPopMatrix();
	}
	else if(event==AEEntEventInit);
	else if(event==AEEntEventRelease){
		if(ent->subents) AEListDelete(ent->subents);
		if(ent->texture) AETextureDelete(ent->texture);
		if(ent->vbo) AEVBODelete(ent->vbo);
	}
	
	return NULL;
}

AEEnt AEEntNew(AEEntEventFunc eventFunc,void* data){
	if(eventFunc==NULL)
		eventFunc=AEEntEventDefault;
	AEEnt ent;
	memset(&ent,0,sizeof(AEEnt));
	eventFunc(&ent,AEEntEventInit,NULL);
	ent.event=AEEntEventDefault;
	return ent;
}

void AEEntAdd(AEEnt* ent,AEEnt subent){
	if(ent->subents==NULL) ent->subents=AEListNew(AEEnt);
	unsigned int length=AEListLength(ent->subents);
	AEEnt* subents=AEListAsArray(ent->subents,AEEnt);
	AEEnt blankEnt;
	memset(&blankEnt,0,sizeof(AEEnt));
	for(unsigned int i=0;i<length;i++){
		if(memcmp(subents+i,&blankEnt,sizeof(AEEnt))==0){
			subents[i]=subent;
			return;
		}
	}
	//If we get here, we did not find an empty slot
	AEListAdd(ent->subents,AEEnt,subent);
}

void AEEntRemove(AEEnt* ent,AEEnt subent){
	unsigned int length=AEListLength(ent->subents);
	AEEnt* subents=AEListAsArray(ent->subents,AEEnt);
	for(unsigned int i=0;i<length;i++){
		if(memcmp(subents+i,&subent,sizeof(AEEnt))==0){
			memset(&subent,0,sizeof(AEEnt));//Only remove the first occurence
			return;
		}
	}
	//If we get here, it was never here to begin with
}
////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////


/// Ugly temp code
//KHASH_SET_INIT_STR(AEEntEventsType);
//khash_t(AEEntEventsType) *AEEntEvents=NULL;
int AEEntEventsCurrentMax=0;

AEList* AEEntEvents=NULL;
typedef struct{char* key;int value;} AEEntEventTag;

int AEEntEventsGetOrAdd(char* name){
	if(AEEntEvents==NULL){
		//AEEntEvents=kh_init(AEEntEventsType);
		AEEntEvents=AEListNew(AEEntEventTag);
	}
	if(name==NULL){
		printf("AEEntEventsGet(%s): Was passed a null value\n",name);
		exit(1);
	}
	unsigned int length=AEListLength(AEEntEvents);
	//puts(name);
	for(unsigned int i=0;i<length;i++){
		char* key=AEListAsArray(AEEntEvents,AEEntEventTag)[i].key;
		if(strcmp(name,key)==0) return AEListAsArray(AEEntEvents,AEEntEventTag)[i].value;
	}
	AEEntEventTag tag={strdup(name),AEEntEventsCurrentMax++};
	AEListAdd(AEEntEvents,AEEntEventTag,tag);
	return tag.value;
	
	/*khiter_t iterator=kh_get(AEEntEventsType,AEEntEvents,name);
	if(kh_exist(AEEntEvents,iterator)) return kh_value(AEEntEvents,iterator);
	int success;
    iterator=kh_put(AEEntEventsType,AEEntEvents,strdup(name), &success);
    if (success==0)kh_del(AEEntEventsType,AEEntEvents,iterator);
    kh_value(AEEntEvents,iterator)=AEEntEventsCurrentMax;
	return AEEntEventsCurrentMax++;*/
}

int AEEntEventsGet(char* name){
	if(AEEntEvents==NULL) return -1;
	if(name==NULL) return -1;
	unsigned int length=AEListLength(AEEntEvents);
	for(unsigned int i=0;i<length;i++){
		char* key=AEListAsArray(AEEntEvents,AEEntEventTag)[i].key;
		if(strcmp(name,key)==0) return AEListAsArray(AEEntEvents,AEEntEventTag)[i].value;
	}
	return -1;
	/*khiter_t iterator=kh_get(AEEntEventsType,AEEntEvents,name);
	if(kh_exist(AEEntEvents,iterator)) return kh_value(AEEntEvents,iterator);
	else return -1;*/
}

void AEEntEventsDelete(void){
	if(AEEntEvents==NULL) return;
	
	unsigned int length=AEListLength(AEEntEvents);
	for(unsigned int i=0;i<length;i++){
		char* key=AEListAsArray(AEEntEvents,AEEntEventTag)[i].key;
		free(key);
	}
	AEListDelete(AEEntEvents);
	/*for (khiter_t i = kh_begin(AEEntEvents); i != kh_end(AEEntEvents); ++i)
        if (kh_exist(AEEntEvents, i)) free((void*)kh_key(AEEntEvents, i));

	kh_destroy(AEEntEventsType,AEEntEvents);
	AEEntEvents=NULL;*/
}