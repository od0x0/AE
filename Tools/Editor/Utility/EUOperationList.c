#include "ECOperationList.h"
#include "qsort_withuserdata.h"
#include <ctype.h>

void ECOperationListInit(ECOperationList* self){
	memset(self, 0, sizeof(ECOperationList));
	AEArrayInit(& self->operations);
}

void ECOperationListDeinit(ECOperationList* self){
	size_t length=AEArrayLength(& self->operations);
	ECOperation* ops=AEArrayAsCArray(& self->operations);
	for (size_t i=0; i<length; i++) {
		puts(ops[i].name);
		free(ops[i].name);
	}
	AEArrayDeinit(& self->operations);
	memset(self, 0, sizeof(ECOperationList));
}

static int SortOperationsAlphabetically(const void* vop, const void* vop2){
	const ECOperation* op=vop;
	const ECOperation* op2=vop2;
	return strcmp(op->name, op2->name);
}

void ECOperationListSortAlphabetically(ECOperationList* self){
	qsort(AEArrayAsCArray(& self->operations), AEArrayLength(& self->operations), sizeof(ECOperation), SortOperationsAlphabetically);
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
	const ECOperation* op=vop;
	const ECOperation* op2=vop2;
	const char* name=vname;
	return (strcasestr(op2->name, name) != NULL) - (strcasestr(op->name, name) != NULL);
}

void ECOperationListSortBySearch(ECOperationList* self, const char* name){
	qsort_withuserdata(AEArrayAsCArray(& self->operations), AEArrayLength(& self->operations), sizeof(ECOperation), SortOperationsBySearch, (void*)name);
}

void ECOperationListAdd(ECOperationList* self, const char* name, ECOperationFunc function, void* userdata){
	ECOperation op;
	memset(& op, 0, sizeof(ECOperation));
	op.name=AEStringDuplicate(name);
	op.function=function;
	op.userdata=userdata;
	AEArrayAdd(& self->operations, op);
}
