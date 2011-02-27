#include "AEVAUtility.h"
#include "AEInternalRawMesh.h"

void AEVASerializeToMBuffer(AEVA* va,AEMBuffer* mbuffer){
	
	uint64_t version=2;
	version=AENetU64FromHost(version);
	//fwrite(&version, 1, sizeof(uint64_t), file);
	AEMBufferWrite(mbuffer, &version, sizeof(uint64_t));
	
	uint64_t length=va->length;
	length=AENetU64FromHost(length);
	//fwrite(&length, 1, sizeof(uint64_t), file);
	AEMBufferWrite(mbuffer, &length, sizeof(uint64_t));
	
	uint64_t formatbits;
	formatbits=AEVAFormatTo64Bits(& va->format);
	AEMBufferWrite(mbuffer, & formatbits, sizeof(formatbits));
	
	
	void* memory=AEVAMap(va, va->length, GL_READ_ONLY);
	
	if(not va->format.isAnIndexArray)
		//fwrite(memory, sizeof(float), va->length, file);
		AEMBufferWrite(mbuffer, memory, va->length*sizeof(float));
	else{
		uint32_t* ints=memory;
		for (size_t i=0; i<va->length; i++) {
			ints[i]=AENetU32FromHost(ints[i]);
			//fwrite(ints+i, sizeof(uint32_t), 1, file);
			AEMBufferWrite(mbuffer, ints+i, sizeof(uint32_t));
		}
	}
	
	AEVAUnmap(va);
}

void AEVAUnserializeFromMBuffer(AEVA* va,AEMBuffer* mbuffer){
	uint64_t version;
	//fread(&version, 1, sizeof(uint64_t), file);
	AEMBufferRead(mbuffer, &version, sizeof(uint64_t));
	version=AEHostU64FromNet(version);
	if(version not_eq 2)
		AEError("Invalid version, only 2 is accepted.");
	AEVADeinit(va);
	memset(va, 0, sizeof(AEVA));
	
	uint64_t length;
	//fread(&length, 1, sizeof(uint64_t), file);
	AEMBufferRead(mbuffer, &length, sizeof(uint64_t));
	length=AEHostU64FromNet(length);
	
	uint64_t formatbits;
	AEMBufferRead(mbuffer, & formatbits, sizeof(formatbits));
	AEVAFormatFrom64Bits(& va->format, formatbits);
	//fread(bytes, 1, 4, file);
	
	void* memory=AEVAMap(va, length, GL_WRITE_ONLY);
	
	if(not va->format.isAnIndexArray)
		//fread(memory, sizeof(float), va->length, file);
		AEMBufferRead(mbuffer, memory, va->length*sizeof(float));
	else{
		uint32_t* ints=memory;
		for (size_t i=0; i<va->length; i++) {
			//fread(ints+i, sizeof(uint32_t), 1, file);
			AEMBufferRead(mbuffer, ints+i, sizeof(uint32_t));
			ints[i]=AEHostU32FromNet(ints[i]);
		}
	}
	
	AEVAUnmap(va);
}

void AEVALoadFromObj(AEVA* va, AEVA* ia, const char* objfilename){
	
	bool hasColors=va->format.hasColors;
	bool hasNormals=va->format.hasNormals;
	int floatcount=hasColors+va->format.textureCoordsPerVertex*2+hasNormals*3+3;
	AEArray(float) vertexList;
	AEArrayInitWithTypeOfSize(&vertexList, sizeof(float)*floatcount);
	AEArray(unsigned int) indexList;
	AEArrayInitWithTypeOfSize(&indexList, sizeof(unsigned int));
	//AEList* vertexList=AEListNewWithTypeSize(floatcount*sizeof(float));
	//AEList* indexList=AEListNew(unsigned int);
	
	AERawMesh* m=AERawMeshLoad(objfilename);
	for(unsigned int i=0;i < m->count.f;i++){
		AERawMeshFace* face=m->f+i;
		for(int j=0;j<3;j++){
			float v[floatcount];
			int size=0;
			if(hasColors){
				unsigned char rgba[4]={255,255,255,255};
				memcpy(v+(size++), rgba, 4);
			}
			for(int k=0;k<(va->format.textureCoordsPerVertex*2);k+=2){
				v[size++]=m->t[face->t[j]].x;
				v[size++]=m->t[face->t[j]].y;
			}
			if(hasNormals){
				v[size++]=m->n[face->n[j]].x;
				v[size++]=m->n[face->n[j]].y;
				v[size++]=m->n[face->n[j]].z;
			}
			v[size++]=m->v[face->v[j]].x;
			v[size++]=m->v[face->v[j]].y;
			v[size++]=m->v[face->v[j]].z;
			if(ia){
				//unsigned int index=AEListAddBytesUnique(vertexList, v);
				//AEListAdd(indexList, unsigned int, index);
				unsigned int index=AEArrayAddBytesUnique(&vertexList, v);
				AEArrayAdd(&indexList, index);
			}
			else AEArrayAddBytes(&vertexList, v);
			//AEListAddBytes(vertexList,v);
		}
	}
	AERawMeshDelete(m);

	void* data=NULL;
	
	if(ia){
		//data=AEVAMap(ia,AEListLength(indexList),GL_WRITE_ONLY);
		//memcpy(data, AEListAsArray(indexList,unsigned int), AEListLengthInSizeofType(indexList,char));
		//AEVAUnmap(ia);
		data=AEVAMap(ia,AEArrayLength(&indexList),GL_WRITE_ONLY);
		memcpy(data, AEArrayAsCArray(&indexList), AEArrayLengthInBytes(&indexList));
		AEVAUnmap(ia);
	}
	
	//data=AEVAMap(va,AEListLengthInSizeofType(vertexList,float),GL_WRITE_ONLY);
	//memcpy(data,AEListAsArray(vertexList,void),AEListLengthInSizeofType(vertexList,char));
	//AEVAUnmap(va);
	data=AEVAMap(va,AEArrayLengthInSizeofType(&vertexList,float),GL_WRITE_ONLY);
	memcpy(data,AEArrayAsCArray(&vertexList),AEArrayLengthInBytes(&vertexList));
	AEVAUnmap(va);
	
	AEArrayDeinit(&indexList);
	AEArrayDeinit(&vertexList);
	
	//AEListDelete(indexList);
	//AEListDelete(vertexList);
}
