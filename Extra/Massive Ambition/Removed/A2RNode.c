#include "A2RNode.h"

/*
	A2RNodeTypeVFC,
	A2RNodeTypeObject,
	A2RNodeTypeMesh,
	A2RNodeTypeMaterial,
	A2RNodeTypeGroup,
	A2RNodeTypeLight
*/

static void A2RNodeDelete(A2RNode* node){
	//Nonrecursive
	switch (node->type) {
		case A2RNodeTypeVFC:
			//nothing
			break;
		case A2RNodeTypeObject:
			//nothing
			break;
		case A2RNodeTypeMesh:
			AEListDelete(node->data.mesh.uvs);
			AEListDelete(node->data.mesh.colors);
			AEListDelete(node->data.mesh.normals);
			AEListDelete(node->data.mesh.positions);
			break;
		case A2RNodeTypeMaterial:
			for(size_t i=0;i<AEListLength(node->data.material.images);i++)
				AEImageDelete(AEListAsArray(node->data.material.images,AEImage*)[i]);
			AEListDelete(node->data.material.images);
			AETextBufferDelete(node->data.material.vshader);
			AETextBufferDelete(node->data.material.fshader);
			break;
		case A2RNodeTypeGroup:
			AEListDelete(node->data.group.children);
			break;
		case A2RNodeTypeLight:
			//nothing
			break;
		default:
			AEError("Unknown node type.");
			break;
	}
}

void A2RNodeObjectPositionSet(A2RNode* node,float x,float y,float z){
	node->data.object.center=AEVec3FromCoords(x, y, z);
}

void A2RNodeObjectRotationSet(A2RNode* node,float x,float y,float z){
	node->data.object.rotation=AEVec3FromCoords(x, y, z);
}

void A2RNodeObjectScaleSet(A2RNode* node,float scale){
	node->data.object.scale=scale;
}

void A2RNodeObjectMeshSet(A2RNode* node,A2RNode* mesh){
	node->data.object.mesh=mesh;
}

void A2RNodeObjectMaterialSet(A2RNode* node,A2RNode* material){
	node->data.object.material=material;
}

void A2RNodeLightPositionSet(A2RNode* node,float x,float y,float z){
	node->data.light.center=AEVec3FromCoords(x, y, z);
}

void A2RNodeLightColorSet(A2RNode* node,float r,float g,float b){
	node->data.light.color.r=r*255;
	node->data.light.color.g=g*255;
	node->data.light.color.b=b*255;
}

void A2RNodeLightAttenuationSet(A2RNode* node,float attenuation){
	node->data.light.color.a=attenuation*255;
}

void A2RNodeLightChildSet(A2RNode* node,A2RNode* child){
	node->data.light.child=child;
}

void A2RNodeVFCBoundsSet(A2RNode* node,float x,float y,float z,float radius){
	node->data.vfc.center=AEVec3FromCoords(x, y, z);
	node->data.vfc.radius=radius;
}

void A2RNodeVFCChildSet(A2RNode* node,A2RNode* child){
	node->data.vfc.child=child;
}

void A2RNodeMeshVertexAdd(A2RNode* node,char* format,void* data){
	if(format and not data){
		if(node->data.mesh.format) AEError("Trying to set format of something that already has one.");
		node->data.mesh.format=strdup(format);
	}
	else if(data and not format){
		format=node->data.mesh.format;
		while(*format){
			switch (*format) {
				case 't':
					AEListAddBytes(node->data.mesh.uvs, data);
					data+=sizeof(AEVec2);
					break;
				case 'n':
					AEListAddBytes(node->data.mesh.normals, data);
					data+=sizeof(AEVec3);
					break;
				case 'c':
					AEListAddBytes(node->data.mesh.colors, data);
					data+=sizeof(AERGBAub);
					break;
				case 'v':
					AEListAddBytes(node->data.mesh.positions, data);
					data+=sizeof(AEVec3);
					break;
				//Yes, I know I should really be doing this in a better place, but I don't care, m'kay?
				case '#':{
					format++;
					char i=*format;
					if(i=='3') node->data.mesh.verticesPerPoly=3;
					else if(i=='4') node->data.mesh.verticesPerPoly=4;
					else AEError("Format has an invalid polycount following #, only 3 or 4 are accepted.");
					}break;
				default:
					break;
			}
			format++;
		}
	}
	else AEError("Given both a format AND data, baaad voodoo, man.");
}

void A2RNodeMaterialShaderAdd(A2RNode* node, char* vshader, char* fshader){
	if(vshader){
		AETextBuffer* buffer=node->data.material.vshader;
		char* text=vshader;
		AETextBufferInsert(buffer,AETextBufferLength(buffer),text);
	}
	
	if(fshader){
		AETextBuffer* buffer=node->data.material.fshader;
		char* text=fshader;
		AETextBufferInsert(buffer,AETextBufferLength(buffer),text);
	}
}

void A2RNodeMaterialImageAdd(A2RNode* node,AEImage* image){
	if(image) AEListAdd(node->data.material.images,AEImage*,AEImageRetain(image));
}

void A2RNodeGroupChildAdd(A2RNode* node,A2RNode* child){
	if(not node->data.group.children) node->data.group.children=AEListNew(A2RNode*);
	if(child) AEListAdd(node->data.group.children,A2RNode*,child);
}

A2RNode* A2RNodeListNodeNew(AEList* nodelist, unsigned int type){
	A2RNode _node;
	memset(&_node, 0, sizeof(A2RNode));
	AEListAddBytes(nodelist,&_node);
	A2RNode* node=AEListAsArray(nodelist,A2RNode)+(AEListLength(nodelist)-1);
	node->type=type;
	return node;
}

AEList* A2RNodeListNew(void){
	return AEListNew(A2RNode);
}

void A2RNodeListDelete(AEList* nodelist){
	for(size_t i=0;i<AEListLength(nodelist);i++)
		A2RNodeDelete(AEListAsArray(nodelist,A2RNode)+i);
	AEListDelete(nodelist);
}