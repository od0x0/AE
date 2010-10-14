#pragma once
#include "AE.h"
#include "ode/ode.h"

typedef struct A2DMap;

A2DMap* A2DMapNew(void);
void A2DMapStep(A2DMap* self, float stepInSeconds);
void A2DMapDelete(A2DMap* self);