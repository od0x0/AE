#include "AETable.h"
#include "khash.h"

KHASH_MAP_INIT_STR(Table, void*)

void* AETableGet(void* table, const char* name){
	khash_t(Table)* h=table;
	khiter_t k=kh_get(Table, h, name);
	if(kh_end(h)==k) return NULL;
	return kh_value(h, k);
}

void AETableSet(void* table, const char* name, void* value){
	khash_t(Table)* h=table;
	khiter_t k=kh_get(Table, h, name);
	if(kh_end(h)==k){
		int ret;
		k = kh_put(Table, h, name, &ret);
		if (!ret){
			kh_del(Table, h, k);
		}
	}
	kh_value(h, k)=value;
}

void* AETableNew(void){
	return kh_init(Table);
}

void AETableDelete(void* table){
	if(table) kh_destroy(Table, table);
}

void AETableEach(AETable* table, AETableEachFunc func, void* arg){
	khash_t(Table)* h=table;
	for (khiter_t k = kh_begin(h); k != kh_end(h); ++k)
		if (kh_exist(h, k)) func((char*)kh_key(h, k), kh_value(h, k), arg);
}

void* AETableRemove(void* table, char* name){
	khash_t(Table)* h=table;
	khiter_t k=kh_get(Table, h, name);
	if(kh_end(h)==k) return NULL;
	void* value=kh_value(h, k);
	kh_del(Table, h, k);
	return value;
}

char* AETableGetName(AETable* table, void* value){
	khash_t(Table)* h=table;
	for (khiter_t k = kh_begin(h); k != kh_end(h); ++k)
		if (kh_exist(h, k) && kh_value(h, k)==value) return (char*)kh_key(h, k);
	return NULL;
}
