#include "A2RVM.h"

typedef struct{
	int opcode;
	size_t argOffset;
	//Offset into the bs where the argument is found
}A2RVMOp;

struct A2RVM{
	size_t refcount;
	A2RVMOp* ops;
	void* binarySegment;
	size_t opCount, opAllocated, binarySegmentUsed, binarySegmentAllocated;
	AETable* trackingTable;
	//Keep list so we can deallocate them later
	AEList* vaList;
	AEList* textureList;
};

A2RVM* A2RVMNew(void){
	return A2RVMRetain(calloc(1, sizeof(A2RVM)));
}

A2RVM* A2RVMRetain(A2RVM* self){
	if(self) self->refcount++;
	return self;
}

void A2RVMDelete(A2RVM* self){
	if(not self) return;
	self->refcount--;
	if(self->refcount) return;
	
	if(self->textureList) for (size_t i=0; i<AEListLength(self->textureList); i++){
		AETexture* pointer = self->binarySegment + (AEListAsArray(self->vaList,size_t)[i]-1);
		AETextureDelete(*pointer);
	}
	AEListDelete(self->textureList);
	
	if(self->vaList) for (size_t i=0; i<AEListLength(self->vaList); i++){
		AEVA* pointer = self->binarySegment + (AEListAsArray(self->vaList,size_t)[i]-1);
		AEVADeinit(pointer);
	}
	AEListDelete(self->vaList);
	
	AETableDelete(self->trackingTable);
	free(self->binarySegment);
	free(self);
}

//Do these when we have the RVM ops stabilized
void A2RVMSerializeToFILE(A2RVM* self, FILE* file);
void A2RVMUnserializeFromFILE(A2RVM* self, FILE* file);

void A2RVMRender(A2RVM* self){
	for (size_t i=0; i<self->opCount; i++) {
		A2RVMOp* op = self->ops + i;
		switch (op->opcode) {
			case A2RVMOpcodeNOP:
				AEError("Some idiot is putting NOPs in the stream, behead him now!");
				break;
			case A2RVMOpcodeBindVA:
				if(op->argOffset) AEVABind(self->binarySegment + op->argOffset-1);
				break;
			case A2RVMOpcodeBindVA_V:
				if(op->argOffset) AEVABindVertex(self->binarySegment + op->argOffset-1);
				else AEVABindVertex(NULL);
				break;
			case A2RVMOpcodeBindVA_N:
				if(op->argOffset) AEVABindNormal(self->binarySegment + op->argOffset-1);
				else AEVABindNormal(NULL);
				break;
			case A2RVMOpcodeBindVA_TC:
				if(op->argOffset) AEVABindTexcoord(self->binarySegment + op->argOffset-1);
				else AEVABindTexcoord(NULL);
				break;
			case A2RVMOpcodeBindVA_TC2:
				glClientActiveTexture(GL_TEXTURE1);
				if(op->argOffset) AEVABindTexcoord(self->binarySegment + op->argOffset-1);
				else AEVABindTexcoord(NULL);
				glClientActiveTexture(GL_TEXTURE0);
				break;
			case A2RVMOpcodeBindVA_TC3:
				glClientActiveTexture(GL_TEXTURE2);
				if(op->argOffset) AEVABindTexcoord(self->binarySegment + op->argOffset-1);
				else AEVABindTexcoord(NULL);
				glClientActiveTexture(GL_TEXTURE0);
				break;
			case A2RVMOpcodeBindIA:
				if(op->argOffset) AEVABindIndices(self->binarySegment + op->argOffset-1);
				break;
			case A2RVMOpcodeBindProgram:
				AEError("Some idiot is putting NOPs in the stream, behead him now!");
				break;
			case A2RVMOpcodeBindTexture:
				if(op->argOffset){
					AETexture* pointer=self->binarySegment + op->argOffset-1;
					glActiveTexture(GL_TEXTURE0);
					AETextureBind(*pointer);
				}
				break;
			case A2RVMOpcodeBindTexture2:
				glActiveTexture(GL_TEXTURE1);
				if(op->argOffset){
					AETexture* pointer=self->binarySegment + op->argOffset-1;
					glEnable(GL_TEXTURE_2D);
					AETextureBind(*pointer);
				}else glDisable(GL_TEXTURE_2D);
				glActiveTexture(GL_TEXTURE0);
				break;
			case A2RVMOpcodeBindTexture3:
				glActiveTexture(GL_TEXTURE2);
				if(op->argOffset){
					AETexture* pointer=self->binarySegment + op->argOffset-1;
					glEnable(GL_TEXTURE_2D);
					AETextureBind(*pointer);
				}else glDisable(GL_TEXTURE_2D);
				glActiveTexture(GL_TEXTURE0);
				break;
			case A2RVMOpcodeBindLight:
				AEError("Some idiot is putting NOPs in the stream, behead him now!");
				break;
			case A2RVMOpcodeBindCamera:
				if(op->argOffset){
					AECamera* camera=AECameraActiveGet();
					A2RVMCamera* pointer=self->binarySegment + op->argOffset-1;
					const AEVec3 p=pointer->position;
					AECameraPositionSet(camera, p.x, p.y, p.z);
					const AEVec3 r=pointer->rotation;
					AECameraRotationSet(camera, r.x, r.y, r.z);
					AECameraBoundsSet(camera, pointer->near, pointer->far, pointer->fov);
					AECameraVFCSet(camera, pointer->vfCullingEnabled);
					AECameraBind(camera);
					AECameraActiveSet(camera);
				}
				break;
			case A2RVMOpcodeLoadIdentity:
				glLoadIdentity();
				break;
			case A2RVMOpcodeLoadCamera:
				AECameraBind(AECameraActiveGet());
				break;
			case A2RVMOpcodeTranslate:
				if(op->argOffset){
					AEVec3* pointer=self->binarySegment + op->argOffset-1;
					glTranslatef(pointer->x, pointer->y, pointer->z);
				}
				break;
			case A2RVMOpcodeRotateAXA:
				if(op->argOffset){
					AEVec4* pointer=self->binarySegment + op->argOffset-1;
					glRotatef(pointer->w, pointer->x, pointer->y, pointer->z);
				}
				break;
			case A2RVMOpcodeRotateEUL:
				if(op->argOffset){
					AEVec3* pointer=self->binarySegment + op->argOffset-1;
					if(pointer->x) glRotatef(pointer->x, 1,0,0);
					if(pointer->y) glRotatef(pointer->y, 0,1,0);
					if(pointer->z) glRotatef(pointer->z, 0,0,1);
				}
				break;
			case A2RVMOpcodeSkipIfVFCFail:
				if(op->argOffset){
					A2RVMVFCObject* pointer=self->binarySegment + op->argOffset-1;
					const AEVec3 p=pointer->position;
					if (not AECameraVFCheckSphere(AECameraActiveGet(), p.x, p.y, p.z, pointer->radius)) {
						i += pointer->opCountToSkipIfVFTestFails;
					}
				}
				break;
			case A2RVMOpcodeDraw:
				if(op->argOffset){
					AEVA* pointer=self->binarySegment + op->argOffset-1;
					AEVADraw(0, pointer->length);
				}
				break;
			default:
				AEError("Some idiot is putting NOPs in the stream, behead him now!");
				break;
		}
	}
}

A2RVMTrackedData* A2RVMTrackedDataLookup(A2RVM* self, char* name){
	if(not self->trackingTable) return NULL;
	void* value = AETableGet(self->trackingTable, name);
	if(not value) return NULL;
	size_t offset = -1 + (size_t)value;
	return offset + self->binarySegment;
}

AEVec3* A2RVMTrackedDataAsVec3(A2RVMTrackedData* self){return (AEVec3*)self;}
AEVec4* A2RVMTrackedDataAsVec4(A2RVMTrackedData* self){return (AEVec4*)self;}
float* A2RVMTrackedDataAs4x4(A2RVMTrackedData* self){return (float*)self;}
AEVA* A2RVMTrackedDataAsVA(A2RVMTrackedData* self){return (AEVA*)self;}
AEVA* A2RVMTrackedDataAsIA(A2RVMTrackedData* self){return (AEVA*)self;}
AETexture* A2RVMTrackedDataAsTexture(A2RVMTrackedData* self){return (AETexture*)self;}
A2RVMVFCObject* A2RVMTrackedDataAsVFCObject(A2RVMTrackedData* self){return (A2RVMVFCObject*)self;}
A2RVMCamera* A2RVMTrackedDataAsCamera(A2RVMTrackedData* self){return (A2RVMCamera*)self;}
void A2RVMTrackedDataShouldUpdate(A2RVMTrackedData* self){}

static size_t A2RVMBSAllocate(A2RVM* self, const void* from, size_t size){
	self->binarySegmentUsed+=size;
	if(self->binarySegmentUsed > self->binarySegmentAllocated){
		self->binarySegmentAllocated = self->binarySegmentUsed * 1.1;
		self->binarySegment = realloc(self->binarySegment, self->binarySegmentAllocated);
	}
	void* to = self->binarySegment + (self->binarySegmentUsed - size);
	memcpy(to, from, size);
	return (self->binarySegmentUsed-size)+1;
};

size_t A2RVMVAAdd(A2RVM* self, AEVA* va){
	AEVA vacloned;
	AEVAInitCopy(&vacloned,va);
	size_t offset=A2RVMBSAllocate(self, &vacloned, sizeof(AEVA));
	if(not self->vaList) self->vaList=AEListNew(size_t);
	AEListAdd(self->vaList, size_t, offset);
	return offset;
}

size_t A2RVMIAAdd(A2RVM* self, AEVA* ia){
	AEVA iacloned;
	AEVAInitCopy(&iacloned,ia);
	size_t offset=A2RVMBSAllocate(self, &iacloned, sizeof(AEVA));
	if(not self->vaList) self->vaList=AEListNew(size_t);
	AEListAdd(self->vaList, size_t, offset);
	return offset;
}

size_t A2RVMTextureAdd(A2RVM* self, AETexture texture){
	size_t offset=A2RVMBSAllocate(self, &texture, sizeof(AETexture));
	if(not self->textureList) self->textureList=AEListNew(size_t);
	
	AEListAdd(self->textureList, size_t, offset);
	return offset;
}

size_t A2RVMCameraAdd(A2RVM* self, A2RVMCamera* camera){
	return A2RVMBSAllocate(self, camera, sizeof(A2RVMCamera));
}

size_t A2RVMVec3Add(A2RVM* self, float x, float y, float z){
	AEVec3 v3={x,y,z};
	return A2RVMBSAllocate(self, &v3, sizeof(AEVec3));
}

size_t A2RVMVec4Add(A2RVM* self, float x, float y, float z, float w){
	AEVec4 v4={x,y,z,w};
	return A2RVMBSAllocate(self, &v4, sizeof(AEVec4));
}

size_t A2RVMVFCObjectAdd(A2RVM* self, A2RVMVFCObject* object){
	return A2RVMBSAllocate(self, object, sizeof(A2RVMVFCObject));
}

void A2RVMTrackIndex(A2RVM* self, char* name, size_t index){
	if(not index) return;
	if(not self->trackingTable) self->trackingTable = AETableNew();
	AETableSet(self->trackingTable, name, (void*)index);
}

void A2RVMOpAdd(A2RVM* self, A2RVMOpcode opcode, size_t argOffset){
	self->opCount++;
	if(self->opCount > self->opAllocated){
		self->opAllocated = self->opCount * 1.1;
		self->ops = realloc(self->ops, self->opAllocated * sizeof(A2RVMOp));
	}
	self->ops[self->opCount-1].opcode = opcode;
	self->ops[self->opCount-1].argOffset = argOffset;
}
/*
void A2RVMBindVAAdd(A2RVM* self, size_t vaindex){  }
void A2RVMBindIAAdd(A2RVM* self, size_t iaindex);
void A2RVMBindProgramAdd(A2RVM* self, size_t textureindex);
void A2RVMBindTextureAdd(A2RVM* self, size_t textureindex);
void A2RVMBindTexture2Add(A2RVM* self, size_t textureindex);
void A2RVMBindLightmapAdd(A2RVM* self, size_t textureindex);
void A2RVMBindLightAdd(A2RVM* self, size_t lightindex);

void A2RVMLoadIdentityAdd(A2RVM* self, size_t unused);
void A2RVMLoadCameraAdd(A2RVM* self, size_t cameraindex);

void A2RVMTranslateAdd(A2RVM* self, size_t vec3index);
void A2RVMRotateAXAAdd(A2RVM* self, size_t vec4index);
void A2RVMRotateEULAdd(A2RVM* self, size_t vec3index);

void A2RVMSkipIfVFCFailAdd(A2RVM* self, size_t vfcobjindex);
void A2RVMDrawAdd(A2RVM* self, size_t unused);
*/