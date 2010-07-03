#include "../AE/List.h"

int main(int argc,char** argv){
	AEList* list=AEListNew(int);
	AEListAdd(list,int,2);
	AEListAdd(list,int,3);
	AEListAdd(list,int,2);
	AEListAdd(list,int,4);
	AEListAdd(list,int,2);
	AEListAdd(list,int,5);
	
	AEList* ilist=AEListNew(size_t);
	AEListRemoveDuplicates(list,ilist);
	
	printf("Size of size_t %i\n",sizeof(size_t));
	
	for(size_t i=0;i<AEListLength(list);i++){
		printf("List[%i]: %i\n",i,AEListAsArray(list,int)[i]);
	}
	puts("");
	for(size_t i=0;i<AEListLength(ilist);i++){
		printf("Ilist[%i]: %i\n",(int)i,AEListAsArray(ilist,size_t)[i]);
	}
	
	return 0;
}
