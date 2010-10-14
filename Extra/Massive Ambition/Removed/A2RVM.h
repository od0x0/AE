#pragma once
#include "AE.h"
#include "Core.h"
#include "VBO.h"
#include "Material.h"

struct A2RVM;
typedef struct A2RVM A2RVM;
//typedef AEList A2RVM;
typedef AEList A2RVMOplist;

void A2RVMOpsAddProxyTransformBegin(A2RVMOplist* ops,AEVec3* position,AEVec3* rotation);
void A2RVMOpsAddProxyTransformEnd(A2RVMOplist* ops);
void A2RVMOpsAddVBO(A2RVMOplist* ops, AEVBO* vbo);
void A2RVMOpsAddMaterial(A2RVMOplist* ops, AEMaterial* material);
void A2RVMOpsAddPosition(A2RVMOplist* ops, float x,float y,float z);
void A2RVMOpsAddRotation(A2RVMOplist* ops, float x,float y,float z);
void A2RVMOpsAddScale(A2RVMOplist* ops, float scale);
void A2RVMOpsAddCameraBind(A2RVMOplist* ops);

A2RVMOplist* A2RVMOplistNew(void);
void A2RVMOplistDelete(A2RVMOplist* ops);

A2RVM* A2RVMOplistCompileToRVM(A2RVMOplist* ops);
void A2RVMDelete(A2RVM* rvm);
void A2RVMExecute(A2RVM* rvm);