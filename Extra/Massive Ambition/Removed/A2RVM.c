#include "A2RVM.h"

typedef struct{
	unsigned int type;
	union{size_t integer; void* pointer; void* pointers[2]; AEVec4 vec4;}data;
}A2RVMOp;

struct A2RVM{
	AEList* opList;
	AEList* vboList;
	AEList* materialList;
};

enum{
	A2RVMOpTypeNone,
	A2RVMOpTypeVBOBind,
	A2RVMOpTypeVBODraw,
	A2RVMOpTypeParticleSpawner,
	A2RVMOpTypeFoliagePatch,
	A2RVMOpTypeGrassPatch,
	A2RVMOpTypeMaterialBind,
	//A2RVMOpTypeJumpIfVFCFail,
	A2RVMOpTypeMove,
	A2RVMOpTypeRotate,
	//A2RVMOpTypeJump
	A2RVMOpTypeMoveThenDraw,
	A2RVMOpTypeMoveThenRotateThenDraw,
	A2RVMOpTypeLoadCameraMatrix,
	A2RVMOpTypeProxyTransformBegin,
	A2RVMOpTypeProxyTransformEnd
};

A2RVM* A2RVMNew(void){
	A2RVM* rvm=calloc(1, sizeof(A2RVM));
	rvm->opList=AEListNew(A2RVMOp);
	rvm->vboList=AEListNew(AEVBO*);
	rvm->materialList=AEListNew(AEMaterial*);
	return rvm;
}

A2RVM* A2RVMOplistCompileToRVM(A2RVMOplist* ops){
	A2RVM* rvm=A2RVMNew();
	//AEListNew(A2RVMOp);
	
	AEMaterial* activematerial=NULL;
	AEVBO* activevbo=NULL;
	AEVec4 activeposition={0,0,0,0};
	AEVec4 activerotation={0,0,0,0};
	unsigned int lastOpType=0;
	
	//printf("%i\n",(int)AEListLength(ops));
	for(size_t i=0;i<AEListLength(ops);i++){
		A2RVMOp* op=AEListAsArray(ops,A2RVMOp)+i;
		//printf("%s(): Op #%i\n",__func__,(int)i);
		switch (op->type) {
			case A2RVMOpTypeMaterialBind:
				if(activematerial not_eq op->data.pointer){
					activematerial=op->data.pointer;
					A2RVMOp newop={.type=A2RVMOpTypeMaterialBind, .data={.pointer=activematerial}};
					AEListAdd(rvm->opList, A2RVMOp, newop); 
					AEMaterial* material=activematerial;
					if(not AEListAddBytesUnique(rvm->materialList,&material)){
						AEMaterialRetain(material);
					}
					//puts("A2RVMOpTypeMaterialBind");
				}
				break;
			case A2RVMOpTypeMove:{
				AEVec4 newposition=op->data.vec4;
				if(activeposition.x not_eq newposition.x or activeposition.y not_eq newposition.y or activeposition.z not_eq newposition.z){
					AEVec4 difference={newposition.x-activeposition.x,newposition.y-activeposition.y,newposition.z-activeposition.z,0};
					activeposition=newposition;
					A2RVMOp newop={.type=A2RVMOpTypeMove, .data={.vec4=difference}};
					AEListAdd(rvm->opList, A2RVMOp, newop);
					//puts("A2RVMOpTypeMove");
				}
				}break;
			case A2RVMOpTypeRotate:{
				AEVec4 newrotation=op->data.vec4;
				if(activerotation.x not_eq newrotation.x or activerotation.y not_eq newrotation.y or activerotation.z not_eq newrotation.z or activerotation.w not_eq newrotation.w){
					AEVec4 difference={newrotation.x-activerotation.x,newrotation.y-activerotation.y,newrotation.z-activerotation.z,newrotation.w-activerotation.w};
					activerotation=newrotation;
					A2RVMOp newop={.type=A2RVMOpTypeRotate, .data={.vec4=difference}};
					AEListAdd(rvm->opList, A2RVMOp, newop);
					//puts("A2RVMOpTypeRotate");
				}
				}break;
			case A2RVMOpTypeVBODraw:{
				if(activevbo not_eq op->data.pointer){
					activevbo=op->data.pointer;
					A2RVMOp newop={.type=A2RVMOpTypeVBOBind, .data={.pointer=activevbo}};
					AEListAdd(rvm->opList, A2RVMOp, newop);
					AEVBO* vbo=activevbo;
					if(not AEListAddBytesUnique(rvm->vboList,&vbo)){
						AEVBORetain(vbo);
					}
					//puts("A2RVMOpTypeVBOBind");
				}
				A2RVMOp newop={.type=A2RVMOpTypeVBODraw, .data={.pointer=activevbo}};
				AEListAdd(rvm->opList, A2RVMOp, newop);
				//puts("A2RVMOpTypeVBODraw");
				}break;
			case A2RVMOpTypeLoadCameraMatrix:{
				A2RVMOp newop={.type=A2RVMOpTypeLoadCameraMatrix, .data={.pointer=NULL}};
				AEListAdd(rvm->opList, A2RVMOp, newop);
				//puts("A2RVMOpTypeLoadCameraMatrix");
				}break;
			case A2RVMOpTypeProxyTransformBegin:{
				A2RVMOp newop={.type=A2RVMOpTypeProxyTransformBegin, .data={.pointers={op->data.pointers[0],op->data.pointers[1]}}};
				activeposition=((AEVec4){0,0,0,0});
				activerotation=((AEVec4){0,0,0,0});
				AEListAdd(rvm->opList, A2RVMOp, newop);
				}break;
			case A2RVMOpTypeProxyTransformEnd:{
				A2RVMOp newop={.type=A2RVMOpTypeProxyTransformEnd, .data={.pointer=NULL}};
				AEListAdd(rvm->opList, A2RVMOp, newop);
				}break;
			default:
				
				break;
		}
		lastOpType=op->type;
	}
	return rvm;
}

void A2RVMDelete(A2RVM* rvm){
	if(not rvm) return;
	AEListDelete(rvm->opList);
	AEListDelete(rvm->vboList);
	AEListDelete(rvm->materialList);
	free(rvm);
}

void A2RVMExecute(A2RVM* rvm){
	for (size_t i=0; i < AEListLength(rvm->opList); i++) {
		A2RVMOp* op=AEListAsArray(rvm->opList,A2RVMOp)+i;
		switch (op->type) {
			case A2RVMOpTypeVBOBind:
				AEVBOBind(op->data.pointer);
				//puts("A2RVMOpTypeVBOBind");
				break;
			case A2RVMOpTypeVBODraw:
				AEVBODraw(op->data.pointer);
				//puts("A2RVMOpTypeVBODraw");
				break;
			case A2RVMOpTypeParticleSpawner:
				//Nothing yet
				break;
			case A2RVMOpTypeFoliagePatch:
				//Nothing yet
				break;
			case A2RVMOpTypeGrassPatch:
				
				//Nothing yet
				break;
			case A2RVMOpTypeMaterialBind:
				AEMaterialBind(op->data.pointer);
				//puts("A2RVMOpTypeMaterialBind");
				break;
			/*case A2RVMOpTypeJumpIfVFCFail:{
				AEVec4* v4=op->data.pointer;
				if(AECameraVFCheckSphere(AECameraActiveGet(),v4->x,v4->y,v4->z,v4->w)==false) i+=op->data.integer;
				}break;*/
			case A2RVMOpTypeMove:{
				AEVec4* v4=&(op->data.vec4);
				glTranslatef(v4->x, v4->y, v4->z);
				//puts("A2RVMOpTypeMove");
				}break;
			case A2RVMOpTypeRotate:{
				AEVec4* v4=&(op->data.vec4);
				if(v4->x) glRotatef(v4->x, 1, 0, 0);
				if(v4->y) glRotatef(v4->y, 0, 1, 0);
				if(v4->z) glRotatef(v4->z, 0, 0, 1);
				//puts("A2RVMOpTypeRotate");
				}break;
			case A2RVMOpTypeMoveThenDraw:{
				AEVec4* v4=&(op->data.vec4);
				glTranslatef(v4->x, v4->y, v4->z);
				op+=1;
				AEVBODraw(op->data.pointer);
				}break;
			case A2RVMOpTypeMoveThenRotateThenDraw:{
				AEVec4* v4=NULL;
				v4=&(op->data.vec4);
				glTranslatef(v4->x, v4->y, v4->z);
				op+=1;
				v4=&(op->data.vec4);
				glRotatef(v4->w, v4->x, v4->y, v4->z);
				op+=1;
				AEVBODraw(op->data.pointer);
				}break;
			case A2RVMOpTypeLoadCameraMatrix:
				AECameraBind(AECameraActiveGet());
				//puts("A2RVMOpTypeLoadCameraMatrix");
				break;
			case A2RVMOpTypeProxyTransformBegin:{
				glPushMatrix();
				AECameraBind(AECameraActiveGet());
				AEVec3* pos=op->data.pointers[0];
				AEVec3* rot=op->data.pointers[1];
				glTranslatef(pos->x, pos->y, pos->z);
				glRotatef(rot->x, 1, 0, 0);
				glRotatef(rot->y, 0, 1, 0);
				glRotatef(rot->z, 0, 0, 1);
				}break;
			case A2RVMOpTypeProxyTransformEnd:{
				glPopMatrix();
				}break;
			/*case A2RVMOpTypeJump:
				i+=op->data.integer;
				break;*/
			default:
				//Ignore
				break;
		}
	}
	//AEError("");
}

void A2RVMOpsAddVBO(A2RVMOplist* ops, AEVBO* vbo){
	//We do it differently here because we optimize later
	A2RVMOp op={.type=A2RVMOpTypeVBODraw, .data={.pointer=vbo}};
	AEListAdd(ops, A2RVMOp, op);
	//printf("%s(): List Length %i\n",__func__,(int)AEListLength(ops));
	//puts(__func__);
}
void A2RVMOpsAddMaterial(A2RVMOplist* ops, AEMaterial* material){
	A2RVMOp op={.type=A2RVMOpTypeMaterialBind, .data={.pointer=material}};
	AEListAdd(ops, A2RVMOp, op);
	//puts(__func__);
}
void A2RVMOpsAddPosition(A2RVMOplist* ops, float x,float y,float z){
	//We do it differently here because we optimize later
	A2RVMOp op={.type=A2RVMOpTypeMove, .data={.vec4={x,y,z,0}}};
	AEListAdd(ops, A2RVMOp, op);
	//puts(__func__);
}
void A2RVMOpsAddRotation(A2RVMOplist* ops, float x,float y,float z){
	//We do it differently here because we optimize later
	//AEVec4 axisangle=AEQuatToAxisAngle(AEQuatFromEuler(AEVec3FromCoords(x, y, z)));
	A2RVMOp op={.type=A2RVMOpTypeRotate, .data={.vec4={x,y,z,0}}};
	AEListAdd(ops, A2RVMOp, op);
	//puts(__func__);
}
void A2RVMOpsAddCameraBind(A2RVMOplist* ops){
	A2RVMOp op={.type=A2RVMOpTypeLoadCameraMatrix};
	AEListAdd(ops, A2RVMOp, op);
	//puts(__func__);
}

void A2RVMOpsAddProxyTransformBegin(A2RVMOplist* ops,AEVec3* position,AEVec3* rotation){
	A2RVMOp op={.type=A2RVMOpTypeProxyTransformBegin,.data={.pointers={position,rotation}}};
	AEListAdd(ops, A2RVMOp, op);
}

void A2RVMOpsAddProxyTransformEnd(A2RVMOplist* ops){
	A2RVMOp op={.type=A2RVMOpTypeProxyTransformEnd};
	AEListAdd(ops, A2RVMOp, op);
}

A2RVMOplist* A2RVMOplistNew(void){
	return AEListNew(A2RVMOp);
}
void A2RVMOplistDelete(A2RVMOplist* ops){
	if(not ops) return;
	AEListDelete(ops);
}