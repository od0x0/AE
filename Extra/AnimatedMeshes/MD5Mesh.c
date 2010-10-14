#include "AniMesh.h"

int AEAniMeshFlipVertexWinding=true;

static AEAniModel* AEAniModelLoadMD5FromFILE(FILE* file){
	char linebuffer[1024];
	char* line=linebuffer;
	int integers[5];
	float numbers[6];
	int currentMesh=0;
	AEAniModel* model=calloc(1,sizeof(AEAniModel));
	while (!feof (file)){
		line=linebuffer;
		fgets (line, sizeof (linebuffer), file);
		
		while(line[0]==' ' || line[0]=='\t') line++;
		
		if(line[0]=='\n' || line[0]=='\r'){
			//Nothing to do here
		}
		else if (sscanf (line, " MD5Version %i", integers+0) == 1){
			if (integers[0] != 10) printf("Invalid md5mesh version! (should be 10, but recieved %i)...  Still going to try loading though...\n",integers[0]);
		}
		else if (sscanf (line, " numJoints %i", integers+0) == 1){
			model->jointCount=integers[0];
			if (model->jointCount > 0) model->baseSkeleton=calloc(model->jointCount, sizeof(AEAniJoint));
		}
		else if (sscanf (line, " numMeshes %i", integers+0) == 1){
			model->meshCount=integers[0];
			if (model->meshCount > 0) model->meshes = calloc(model->meshCount, sizeof (AEAniMesh));
		}
		else if (strncmp (line, "joints {", 8) == 0){
			for (int i = 0; i < model->jointCount; ++i){
				AEAniJoint* joint = model->baseSkeleton+i;
				
				line=linebuffer;
				fgets (line, sizeof (linebuffer), file);
				while(line[0]==' ' || line[0]=='\t') line++;
				char namebuffer[512]={0};
				
				if (sscanf (line, " %511s %i ( %f %f %f ) ( %f %f %f )", namebuffer, integers+0, numbers+0, numbers+1, numbers+2, numbers+3, numbers+4, numbers+5) == 8){
					snprintf(joint->name,sizeof(joint->name)-1,"%s",namebuffer);
					joint->name[sizeof(joint->name)-1]='\0';
					joint->parentID=integers[0];
					
					joint->position.x=numbers[0];
					joint->position.y=numbers[1];
					joint->position.z=numbers[2];
					
					joint->rotation.x=numbers[3];
					joint->rotation.y=numbers[4];
					joint->rotation.z=numbers[5];
					AEQuatComputeW(&joint->rotation);
					//joint->name
				}
			}
		}
		else if (strncmp (line, "mesh {", 6) == 0){
			AEAniMesh* mesh = model->meshes + currentMesh;

			while ((line[0] != '}') && !feof (file)){
				line=linebuffer;
				fgets (line, sizeof (linebuffer), file);
				while(line[0]==' ' || line[0]=='\t') line++;
				
				/*
				if (strstr (line, "shader ")){
					int quote = 0, j = 0;
					for (i = 0; i < sizeof (line) && (quote < 2); ++i){
						if (line[i] == '\"') quote++;
						if ((quote == 1) && (line[i] != '\"')){
							mesh->shader[j] = line[i];
							j++;
						}
					}
				}
				else */
				if (sscanf (line, " numverts %i", integers+0) == 1){
					mesh->vertexCount=integers[0];
					if (mesh->vertexCount > 0) mesh->vertices = malloc(sizeof (AEAniVertex) * mesh->vertexCount);

					//if (mesh->vertexCount > max_verts) max_verts = mesh->vertexCount;
				}
				else if (sscanf (line, " numtris %i", integers+0) == 1){
					mesh->indexCount=integers[0];
					mesh->indexCount*=3;//3 indices for every triangle
					if (mesh->indexCount > 0){
						mesh->indices = malloc(sizeof (unsigned int) * mesh->indexCount);
					}
					//if (mesh->num_tris > max_tris) max_tris = mesh->num_tris;
				}
				else if (sscanf (line, " numweights %i", integers+0) == 1){
					mesh->weightCount=integers[0];
					if (mesh->weightCount > 0) mesh->weights = malloc(sizeof (AEAniWeight) * mesh->weightCount);
				}
				else if (sscanf (line, " vert %i ( %f %f ) %i %i", integers+0, numbers+0, numbers+1, integers+1, integers+2) == 5){
					if(AEAniMeshFlipVertexWinding) integers[0]=(mesh->vertexCount-1)-integers[0];
					mesh->vertices[integers[0]].u = numbers[0];
					mesh->vertices[integers[0]].v = 1.0 - numbers[1];//Because md5 stores UVs flipped over the y
					mesh->vertices[integers[0]].startingWeight = integers[1];
					mesh->vertices[integers[0]].weightCount = integers[2];
				}
				else if (sscanf (line, " tri %i %i %i %i", integers+0, integers+1, integers+2, integers+3) == 4){
					mesh->indices[integers[0]*3+0] = integers[1];
					mesh->indices[integers[0]*3+1] = integers[2];
					mesh->indices[integers[0]*3+2] = integers[3];
				}
				else if (sscanf (line, " weight %i %i %f ( %f %f %f )", integers+0, integers+1, numbers+0, numbers+1, numbers+2, numbers+3) == 6){
					mesh->weights[integers[0]].jointID  = integers[1];
					mesh->weights[integers[0]].bias   = numbers[0];
					mesh->weights[integers[0]].offsetFromJoint.x = numbers[1];
					mesh->weights[integers[0]].offsetFromJoint.y = numbers[2];
					mesh->weights[integers[0]].offsetFromJoint.z = numbers[3];
				}
			}
			currentMesh++;
		}
	}
	return model;
}

AEAniModel* AEAniModelLoadMD5(char* filename){
	FILE* file=fopen(filename,"r");
	if(file==NULL){
		printf("%s(\"%s\"): Could not open.\n",__func__,filename);
		exit(1);
	}
	AEAniModel* model = AEAniModelLoadMD5FromFILE(file);
	fclose(file);
	return model;
}

void AEAniModelDelete(AEAniModel* model){
	if(model==NULL) return;
	free(model->baseSkeleton);
	for(int i=0;i < model->meshCount;i++){
		free(model->meshes[i].weights);
		free(model->meshes[i].vertices);
	}
	free(model);
}