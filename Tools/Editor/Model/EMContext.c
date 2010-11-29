#include "EMContext.h"

static void OnConnectAccept(void* userdata, VNodeID avatar, const char* address, uint8_t* hostID){
	puts("Connection Accepted!");
	verse_send_node_index_subscribe(~0);
}

static void OnConnectTerminate(void* userdata, const char* address, const char* message){
	printf("You have been disconnected from %s (%s).", address, message);
}

static void OnNodeCreate(void *userdata, VNodeID nodeID, VNodeType type, VNodeOwner owner){
	printf("Creating node #%i of type %i, owned by %i\n", (int)nodeID, (int)type, (int)owner);
}

static void OnNodeNameSet(void* userdata, VNodeID nodeID, const char *name){
	printf("Setting name of node #%i to %s\n", (int)nodeID, name);
}

void EMContextInit(EMContext* self){
	memset(self, 0, sizeof(EMContext));
	verse_callback_set(verse_send_node_create, OnNodeCreate, self);
	verse_callback_set(verse_send_connect_accept, OnConnectAccept, self);
	verse_callback_set(verse_send_connect_terminate, OnConnectTerminate, self);
	verse_callback_set(verse_send_node_name_set, OnNodeNameSet, self);
}

void EMContextDeinit(EMContext* self){
	memset(self, 0, sizeof(EMContext));
}

void EMContextFixedUpdate(EMContext* self, double seconds){
	if(self->verseSession) verse_callback_update(1000*seconds);
}

void EMContextConnectToServer(EMContext* self, const char* address, const char* username, const char* password){
	if(self->verseSession) EMContextDisconnectFromServer(self);
	self->connectedServer = AEStringDuplicate(address);
	if(self->verseSession=verse_send_connect(username, password, address, NULL)) puts("Connected!");
}

void EMContextDisconnectFromServer(EMContext* self){
	verse_send_connect_terminate(self->connectedServer, "iQuit");
	self->verseSession=NULL;
	free(self->connectedServer);
}

void EMContextDoEdit(EMContext* self, EMNodeID nodeID, const char* name, EMContextTag* value){
	/*EMContextEditNode* editNode = EUMemoryAllocate(sizeof(EMContextEditNode));
	editNode->ID=++self->highestID;//This way, the values start at 1
	editNode->last=self->currentEditNode;
	editNode->target=nodeID;
	editNode->value=*value;
	editNode->name=AEStringDuplicate(name);
	size_t length=AEArrayLength(& self->editNodeLeaves);
	EMContextEditNode** leaves=AEArrayAsCArray(& self->editNodeLeaves);
	for (size_t i=0; i<length; i++) {
		if(self->currentEditNode==leaves[i]) ;
	}
	self->currentEditNode=editNode;*/
	
	
	
}


void EMContextRevertToEdit(EMContext* self, EMContextEditNode* node){
	
}
