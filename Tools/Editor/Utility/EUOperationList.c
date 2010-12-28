#include "EUOperationList.h"
#include "qsort_withuserdata.h"
#include <ctype.h>

void EUOperationListInit(EUOperationList* self){
	memset(self, 0, sizeof(EUOperationList));
	AEArrayInit(& self->operations);
}

void EUOperationListDeinit(EUOperationList* self){
	size_t length=AEArrayLength(& self->operations);
	EUOperation* ops=AEArrayAsCArray(& self->operations);
	for (size_t i=0; i<length; i++) {
		puts(ops[i].name);
		free(ops[i].name);
	}
	AEArrayDeinit(& self->operations);
	memset(self, 0, sizeof(EUOperationList));
}

static int SortOperationsAlphabetically(const void* vop, const void* vop2){
	const EUOperation* op=vop;
	const EUOperation* op2=vop2;
	return strcmp(op->name, op2->name);
}

void EUOperationListSortAlphabetically(EUOperationList* self){
	qsort(AEArrayAsCArray(& self->operations), AEArrayLength(& self->operations), sizeof(EUOperation), SortOperationsAlphabetically);
}

//From http://www.daniweb.com/code/snippet216564.html
static const char *mystrcasestr(const char *haystack, const char *needle)
{
   if ( !*needle )
   {
      return haystack;
   }
   for ( ; *haystack; ++haystack )
   {
      if ( toupper(*haystack) == toupper(*needle) )
      {
         /*
          * Matched starting char -- loop through remaining chars.
          */
         const char *h, *n;
         for ( h = haystack, n = needle; *h && *n; ++h, ++n )
         {
            if ( toupper(*h) != toupper(*n) )
            {
               break;
            }
         }
         if ( !*n ) /* matched all of 'needle' to null termination */
         {
            return haystack; /* return the start of the match */
         }
      }
   }
   return 0;
}

static int SortOperationsBySearch(const void* vop, const void* vop2, void* vname){
	const EUOperation* op=vop;
	const EUOperation* op2=vop2;
	const char* name=vname;
	return (strcasestr(op2->name, name) != NULL) - (strcasestr(op->name, name) != NULL);
}

void EUOperationListSortBySearch(EUOperationList* self, const char* name){
	qsort_withuserdata(AEArrayAsCArray(& self->operations), AEArrayLength(& self->operations), sizeof(EUOperation), SortOperationsBySearch, (void*)name);
}

void EUOperationListAdd(EUOperationList* self, const char* name, EUOperationFunc function, void* userdata){
	EUOperation op;
	memset(& op, 0, sizeof(EUOperation));
	op.name=AEStringDuplicate(name);
	op.function=function;
	op.userdata=userdata;
	AEArrayAdd(& self->operations, op);
}
