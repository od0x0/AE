/* 
This code is pretty ugly, 
I didn't really write the best code for the Mesh and VBO stuff.  
I appologize now.  
Only handles triangle meshes
*/
#include "RawMesh.h"
#include <string.h>
#include "../List.h"

void AERawMeshDraw(AERawMesh* m){
	AEVec3* v;AERawMeshFace* f;AEVec3* n;AEVec2* t;
	glBegin(GL_TRIANGLES);
		for(int cf=0; cf<m->count.f; cf++){	//For each face
			f=&m->f[cf];
			for (int cv = 0; cv <3; cv++) {	//For each face's vertices
				v=&m->v[m->f[cf].v[cv]];
				t=&m->t[m->f[cf].t[cv]];
				n=&m->n[m->f[cf].n[cv]];
				
				glNormal3f(n->x,n->y,n->z);
				glTexCoord2f(t->x,t->y);
				glVertex3f(v->x,v->y,v->z);
			}
		}
	glEnd();
}

void AERawMeshDelete(AERawMesh* m){
	free(m->f);
	free(m->v);
	free(m->t);
	free(m->n);
	free(m);
}

AERawMesh* AERawMeshLoad(const char* filename){
	AERawMesh* m;
	int fcount,vcount,ncount,tcount;
	unsigned int cv,cf,ct,cn,v[3],n[3],t[3];
	char buffer[256];
	float x,y,z;
	
	cv=cf=ct=cn=fcount=vcount=ncount=tcount=0;
	
	FILE* file=fopen(filename,"r");
	if(file==NULL){
		printf("Failed to open %s\n",filename);
	//	return NULL;
		exit(1);
	}
	while(!feof(file)){
		fgets(buffer,255,file);
		if((buffer[0]=='v')&&(buffer[1]==' ')){
			vcount++;
			
		}
		else if((buffer[0]=='f')&&(buffer[1]==' ')){
			fcount++;
			
		}
		else if((buffer[0]=='v')&&(buffer[1]=='n')){
			ncount++;
			
		}
		else if((buffer[0]=='v')&&(buffer[1]=='t')){
			tcount++;
			
		}
	}
	rewind(file);


	m=(AERawMesh*)calloc(1,sizeof(AERawMesh));
	m->v=(AEVec3*)calloc(vcount,sizeof(AEVec3));
	m->f=(AERawMeshFace*)calloc(fcount,sizeof(AERawMeshFace));
	m->n=(AEVec3*)calloc(ncount,sizeof(AEVec3));
	m->t=(AEVec2*)calloc(tcount,sizeof(AEVec2));
	
	if(m->f==NULL){
		printf("Couldn't allocate %i faces for file named %s\n",fcount,filename);
		exit(2);
	}

	if(m->v==NULL){
		printf("Couldn't allocate %i vertices for file named %s\n",vcount,filename);
		exit(2);
	}
	if(m->n==NULL){
		printf("Couldn't allocate %i normals for file named %s\n",ncount,filename);
		exit(2);
	}

	if(m->t==NULL){
		printf("Couldn't allocate %i uv's for file named %s\n",tcount,filename);
		exit(2);
	}
	
	while(!feof(file)){
		fgets(buffer,255,file);
		if(3==sscanf(buffer,"v %f %f %f",&x,&y,&z)){
			m->v[cv].x=x;
			m->v[cv].y=y;
			m->v[cv].z=z;
			cv++;
		}
		else if(2==sscanf(buffer,"vt %f %f",&x,&y)){
			m->t[ct].x=x;
			m->t[ct].y=y;
			ct++;
		}
		else if(3==sscanf(buffer,"vn %f %f %f",&x,&y,&z)){
			m->n[cn].x=x;
			m->n[cn].y=y;
			m->n[cn].z=z;
			cn++;
		}
		else if(9==sscanf(buffer,"f %i/%i/%i %i/%i/%i %i/%i/%i",&v[0],&t[0],&n[0],&v[1],&t[1],&n[1],&v[2],&t[2],&n[2])){
		
			if(v[0]<1||v[1]<1||v[2]<1){
				printf("Invalid vertex id in file %s\n",filename);
				exit(1);
			}
			
			m->f[cf].t[0]=t[0]-1;
			m->f[cf].t[1]=t[1]-1;
			m->f[cf].t[2]=t[2]-1;
			
			m->f[cf].v[0]=v[0]-1;
			m->f[cf].v[1]=v[1]-1;
			m->f[cf].v[2]=v[2]-1;
			
			m->f[cf].n[0]=n[0]-1;
			m->f[cf].n[1]=n[1]-1;
			m->f[cf].n[2]=n[2]-1;
			cf++;
		}
		else if(6==sscanf(buffer,"f %d//%d %d//%d %d//%d",&v[0],&n[0],&v[1],&n[1],&v[2],&n[2])){
		
			if(v[0]<1||v[1]<1||v[2]<1){
				printf("Invalid vertex id in file %s\n",filename);
				exit(1);    
			}
			
			m->f[cf].v[0]=v[0]-1;
			m->f[cf].v[1]=v[1]-1;
			m->f[cf].v[2]=v[2]-1;
			
			m->f[cf].n[0]=n[0]-1;
			m->f[cf].n[1]=n[1]-1;
			m->f[cf].n[2]=n[2]-1;
			cf++;
		}
		else if(6==sscanf(buffer,"f %d/%d %d/%d %d/%d",&v[0],&n[0],&v[1],&n[1],&v[2],&n[2])){
		// Because the guys that wrote the blender .obj exporter don't know the obj file standard -_-
		
			if(v[0]<1||v[1]<1||v[2]<1){
				printf("Invalid vertex id in file %s\n",filename);
				exit(1);    
			}
			
			m->f[cf].v[0]=v[0]-1;
			m->f[cf].v[1]=v[1]-1;
			m->f[cf].v[2]=v[2]-1;
			
			m->f[cf].n[0]=n[0]-1;
			m->f[cf].n[1]=n[1]-1;
			m->f[cf].n[2]=n[2]-1;
			cf++;
		}
		else if(3==sscanf(buffer,"f %d %d %d",&v[0],&v[1],&v[2])){
		
			if(v[0]<1||v[1]<1||v[2]<1){
				printf("Invalid vertex id in file %s\n",filename);
				exit(1);
			}
			m->f[cf].v[0]=v[0]-1;
			m->f[cf].v[1]=v[1]-1;
			m->f[cf].v[2]=v[2]-1;
			cf++;
		}
	}
	if(cf==fcount&&cv==vcount&&ct==tcount&&cn==ncount){
		m->count.f=fcount;
		m->count.v=vcount;
		m->count.n=ncount;
		m->count.t=tcount;
	}else{
		printf("\nError loading %s\nFound|Stated to be\n",filename);
		printf("Vertex:%d|%d\n",cv,vcount);
		printf("Face:%d|%d\n",cf,fcount);
		printf("TexCoord:%d|%d\n",ct,tcount);
		printf("Normals:%d|%d\n",cn,ncount);
		exit(1);
	}
	fclose(file);
	
	return m;
}

/*AERawMesh* AERawMeshLoad(const char* filename){
	AERawMesh* m;
	int fcount,vcount,ncount,tcount;
	unsigned int cv,cf,ct,cn,v[3],n[3],t[3];
	char buffer[256];
	float x,y,z;
	
	cv=cf=ct=cn=fcount=vcount=ncount=tcount=0;
	
	AEList *vertices,*normals,*texcoords,*faces;
	
	vertices=AEListNew(AEVec3);
	normals=AEListNew(AEVec3);
	texcoords=AEListNew(AEVec2);
	faces=AEListNew(AERawMeshFace);
	
	while(!feof(file)){
		fgets(buffer,255,file);
		if(3==sscanf(buffer,"v %f %f %f",&x,&y,&z)){
			AEVec3 v={x,y,z};
			AEListAdd(vertices,AEVec3,v);
		}
		else if(2==sscanf(buffer,"vt %f %f",&x,&y)){
			AEVec2 v={x,y};
			AEListAdd(texcoords,AEVec2,v);
		}
		else if(3==sscanf(buffer,"vn %f %f %f",&x,&y,&z)){
			AEVec3 v={x,y,z};
			AEListAdd(normals,AEVec3,v);
		}
		else if(9==sscanf(buffer,"f %i/%i/%i %i/%i/%i %i/%i/%i",&v[0],&t[0],&n[0],&v[1],&t[1],&n[1],&v[2],&t[2],&n[2])){
		
			if(v[0]<1||v[1]<1||v[2]<1){
				//printf("Invalid vertex id in file %s\n",filename);
				exit(1);
			}
			
			unsigned int i=1;
			int j=0;
			int f=0;
			
			int v,t,n,f;
			f=0;
			
			while(1){
				if(3==sscanf(buffer+i,"%i/%i/%i%n",&v,&t,&n,&j)){
					i+=j-1;
					AERawMeshFace face;
					face->v[f]=v;
					face->n[f]=n;
					face->t[f]=t;
					f++;
					AEListAdd(faces,AERawMeshFace,face);
				}else break;
				i++;
			}
			
		}
		else if(6==sscanf(buffer,"f %d//%d %d//%d %d//%d",&v[0],&n[0],&v[1],&n[1],&v[2],&n[2])){
		
			if(v[0]<1||v[1]<1||v[2]<1){
				//printf("Invalid vertex id in file %s\n",filename);
				exit(1);    
			}
			
			m->f[cf].v[0]=v[0]-1;
			m->f[cf].v[1]=v[1]-1;
			m->f[cf].v[2]=v[2]-1;
			
			m->f[cf].n[0]=n[0]-1;
			m->f[cf].n[1]=n[1]-1;
			m->f[cf].n[2]=n[2]-1;
			cf++;
		}
		else if(6==sscanf(buffer,"f %d/%d %d/%d %d/%d",&v[0],&n[0],&v[1],&n[1],&v[2],&n[2])){
		// Because the guys that wrote the blender .obj exporter don't know the obj file standard -_-
		
			if(v[0]<1||v[1]<1||v[2]<1){
				//printf("Invalid vertex id in file %s\n",filename);
				exit(1);    
			}
			
			m->f[cf].v[0]=v[0]-1;
			m->f[cf].v[1]=v[1]-1;
			m->f[cf].v[2]=v[2]-1;
			
			m->f[cf].n[0]=n[0]-1;
			m->f[cf].n[1]=n[1]-1;
			m->f[cf].n[2]=n[2]-1;
			cf++;
		}
		else if(3==sscanf(buffer,"f %d %d %d",&v[0],&v[1],&v[2])){
		
			if(v[0]<1||v[1]<1||v[2]<1){
				//printf("Invalid vertex id in file %s\n",filename);
				exit(1);
			}
			m->f[cf].v[0]=v[0]-1;
			m->f[cf].v[1]=v[1]-1;
			m->f[cf].v[2]=v[2]-1;
			cf++;
		}
	}
	
	return m;
}*/