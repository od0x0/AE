#pragma once
//#include "A2RVM.h"
#include "AE.h"

struct A2Map;
typedef struct A2Map A2Map;

A2Map* A2MapNew(void);
A2Map* A2MapRetain(A2Map* self);
void A2MapDelete(A2Map* self);

void A2MapRender(A2Map* self);
void A2MapStep(A2Map* self, float stepInSeconds);