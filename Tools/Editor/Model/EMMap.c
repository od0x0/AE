#include "EMMap.h"
size_t EMMapNodeCount(EMMap* self){
	return AEArrayLength(& self->nodes);
}
EMNode* EMMapNodeGet(EMMap* self, EMNodeID index){
	if(EMMapNodeCount(self) <= index) return NULL;
	return AEArrayAsCArray(& self->nodes)+index;
}
EMNode* EMMapNodeForVerseNodeID(EMMap* self, VNodeID verseID){
	return NULL;
}