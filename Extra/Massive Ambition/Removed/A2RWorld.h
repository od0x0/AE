#pragma once
#include "AE.h"
#include "Core.h"
#include "A2RVM.h"
#include "Fog.h"

struct A2RWorld;
typedef struct A2RWorld A2RWorld;

A2RWorld* A2RWorldNew(void);
void A2RWorldRender(A2RWorld* self);
void A2RWorldDelete(A2RWorld* self);