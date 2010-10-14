#include "Grass.h"

const char* AEGrassPatchVertexShader=
"vec4 AEGrassPatchWind(){\n"
"	vec4 offset=vec4(0);\n"
"	if(gl_MultiTexCoord0.y > 0.0) offset=vec4(sin(time*200.0+gl_Vertex.x)*0.5,0.0,0.0,0.0);\n"
"	return offset;\n"
"}\n"
;

const char* AEGrassPatchVertexShaderGet(AEGrassPatch* patch){
	return AEGrassPatchVertexShader;
}

AEGrassPatch* AEGrassPatchNew(bool cylindrical, size_t grassQuadCount, AEVec3* centers, AEVec3* sizes, AERGBAub* colors/*Perface color, which is translated to pervertex*/){
	AEGrassPatch* patch=calloc(1, sizeof(AEGrassPatch));
	patch->cylindrical=cylindrical;
	patch->grassQuadCount=grassQuadCount;
	
	//patch->vertices=calloc(grassQuadCount*4, sizeof(AEVec3));
	printf("%p",patch->vertices);
	
	patch->centers=calloc(grassQuadCount, sizeof(AEVec3));
	//if(centers) memcpy(patch->centers, centers, sizeof(AEVec3)*grassQuadCount);
	
	patch->sizes=calloc(grassQuadCount, sizeof(AEVec3));
	//if(sizes) memcpy(patch->sizes, sizes, sizeof(AEVec3)*grassQuadCount);
	
	patch->colors=calloc(grassQuadCount*4, sizeof(AERGBAub));
	
	patch->texcoords=calloc(grassQuadCount*4, sizeof(float));
	
	for(size_t i=0;i < grassQuadCount;i++){
		if(centers) patch->centers[i]=centers[i];
		if(sizes) patch->sizes[i]=sizes[i];
		AEVec2 tout[4]={{0,0},{1,0},{1,1},{0,1}};
		for(char j=0;j<4;j++){
			patch->texcoords[i*4+j*4+0]=tout[j].x;
			patch->texcoords[i*4+j*4+1]=tout[j].y;
			patch->colors[i*4+j*4].r= colors ? colors[i*4+j*4].r : 255;
			patch->colors[i*4+j*4].g= colors ? colors[i*4+j*4].g : 255;
			patch->colors[i*4+j*4].b= colors ? colors[i*4+j*4].b : 255;
			patch->colors[i*4+j*4].a= 255;//colors ? colors[i*4+j*4+3] : 255;
		}
	}
	return patch;
}

void AEGrassPatchRender(AEGrassPatch* patch){
	if(not patch) return;
	/*
	//What I really want
	AEVec3* v=patch->vertices;
	if(not v) v=alloca(patch->grassQuadCount*4*sizeof(AEVec3));
	*/
	
	//AEVec3* v=patch->vertices;
	//AEVec3 v[patch->grassQuadCount*4];
	AEVec3* centers=patch->centers;
	AEVec3* sizes=patch->sizes;
	AEVec3 v[patch->grassQuadCount*4];//VLAs in use here
	//float* t=patch->texcoords;
	float t[patch->grassQuadCount*4*2];
	AERGBAub* c=patch->colors;
	
	float modelview[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview);

	const AEVec3 right=AEVec3FromCoords(modelview[0], modelview[4], modelview[8]);
	const AEVec3 up =patch->cylindrical ? AEVec3FromCoords(0, 1, 0) : AEVec3FromCoords(modelview[1], modelview[5], modelview[9]);
	
	const AEVec3 rightPlusUp=AEVec3Add(right, up);
	const AEVec3 rightMinusUp=AEVec3Sub(right, up);
	//This has a desired side effect of scaling on the global axes, not so desirable for particles though
	const AEVec2 tout[4]={{0,0},{1,0},{1,1},{0,1}};
	for(int i=0; i < patch->grassQuadCount; i++){
		const AEVec3 center=centers[i];
		const AEVec3 size=sizes[i];
		v[i*4+0]=AEVec3Sub(center, AEVec3Mul(rightPlusUp, size));
		v[i*4+1]=AEVec3Add(center, AEVec3Mul(rightMinusUp, size));
		v[i*4+2]=AEVec3Add(center, AEVec3Mul(rightPlusUp, size));
		v[i*4+3]=AEVec3Sub(center, AEVec3Mul(rightMinusUp, size));
		for(char j=0;j<4;j++){
			t[i*4+j*4+0]=0.5;//tout[j].x;
			t[i*4+j*4+1]=0.5;//tout[j].y;
		}
	}
	
	//for(int i=0;i<4;i++) printf("(%f, %f, %f)",v[i].x,v[i].y,v[i].z);
	//AEError("");
	
	
	glEnable(GL_ALPHA_TEST);
	glDisable(GL_CULL_FACE);
	glAlphaFunc(GL_GREATER, 0.9);
	glBegin(GL_QUADS);
		//const AEVec2 tout[4]={{0,0},{1,0},{1,1},{0,1}};
		for(size_t i=0; i<patch->grassQuadCount; i++){
			for(char j=0; j<4; j++){
				glTexCoord2f(tout[j].x, tout[j].y);
				glColor4ub(c[i*4+j*4].r,c[i*4+j*4].g,c[i*4+j*4].b,c[i*4+j*4].a);
				//glTexCoord2f(t[i*4+j+0], t[i*4+j+1]);
			//	printf("[%i] T(%.1f, %.1f) ",(int)i,t[i*4+j+0], t[i*4+j+1]);
				glVertex3f(v[i*4+j].x, v[i*4+j].y, v[i*4+j].z);
			//	printf("V(%.1f, %.1f, %.1f)\n",v[i*4+j].x, v[i*4+j].y, v[i*4+j].z);
			}
		}
	glEnd();
	//AEError("");
	
	
	/*AEVABindIndices(NULL);
	AEVABindVertex(NULL);
	AEVABindTexcoord(NULL);
	glDisableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexPointer(3, GL_FLOAT, 0, v);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glTexCoordPointer(2, GL_FLOAT, 0, t);
	glEnableClientState(GL_COLOR_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glColorPointer(4, GL_UNSIGNED_BYTE, 0, c);
	glPointSize(100);
	glDrawArrays(GL_POINTS,0,patch->grassQuadCount);
	AEVABindVertex(NULL);
	AEVABindTexcoord(NULL);
	glDisableClientState(GL_COLOR_ARRAY);*/
	glEnable(GL_CULL_FACE);
	glDisable(GL_ALPHA_TEST);
	//AEError("");
}

void AEGrassPatchDelete(AEGrassPatch* patch){
	if(not patch) return;
	puts(__func__);
	free(patch->centers);
	puts(__func__);
	free(patch->vertices);
	puts(__func__);
	free(patch->sizes);
	puts(__func__);
	free(patch->texcoords);
	puts(__func__);
	free(patch->colors);
	puts(__func__);
	free(patch);
	puts(__func__);
}

void AEGrassPatchAlterImageToSetupForAlphaTest(AEImage* image){
	size_t pixelCount=image->w*image->h;
	if(image->channels==4){
		float max=0,min=0;
		for(size_t i=0;i<pixelCount;i++){
			max=AEMax(max,image->pixels[i*4+3]);
			min=AEMin(min,image->pixels[i*4+3]);
		}
		
		for(size_t i=0;i<pixelCount;i++){
			float alpha=image->pixels[i*4+3];
			if(alpha!=max) image->pixels[i*4+3]=0;
			else image->pixels[i*4+3]=255;
		}
	}
}

/*void DrawGrassPatch(void){
	static bool initialized=false;
	//const int particlecount=32;
	#define PARTICLECOUNT (2048*2*2*2)
	static AEVec3 centers[PARTICLECOUNT];
	static AEVec3 sizes[PARTICLECOUNT];
	static AEVec3 colors[PARTICLECOUNT];
	static unsigned char c[PARTICLECOUNT][4][4];
	static GLfloat t[PARTICLECOUNT][4][2];
	if(not initialized){
		initialized=true;
		AEVec3 minbound={-5*4,0,-5*4};
		AEVec3 maxbound={5*4,0,5*4};
		for(int i=0;i<PARTICLECOUNT;i++){
			AEVec3 minsize={1,0.2,1};
			AEVec3 maxsize={1.3,0.3,1.3};
			sizes[i]=AEVec3RandomBetween(minsize, maxsize);
			colors[i]=AEVec3FromSingle(AERandomBetween(0.7, 1.0));
			centers[i]=AEVec3RandomBetween(minbound, maxbound);
			centers[i].y+=sizes[i].y;
			float factor=1.0-AEVec3LengthInv(AEVec3Sub(centers[i], AEVec3FromCoords(0,0,0)));
			if(factor < 0) factor=0;
			if(factor > 1) factor=1;
			colors[i]=AEVec3Mul(colors[i], AEVec3FromSingle(factor));
			AEVec2 tout[4]={{0,0},{1,0},{1,1},{0,1}};
			for(char j=0;j<4;j++){
				t[i][j][0]=tout[j].x;
				t[i][j][1]=tout[j].y;
				c[i][j][0]=colors[i].x*255;
				c[i][j][1]=colors[i].y*255;
				c[i][j][2]=colors[i].z*255;
				c[i][j][3]=255;
			}
		}
	}
	AEVec3 v[PARTICLECOUNT][4];
	
	float modelview[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview);

	const AEVec3 right=AEVec3FromCoords(modelview[0], modelview[4], modelview[8]);
	const AEVec3 up = cylindrical ? AEVec3FromCoords(0, 1, 0) : AEVec3FromCoords(modelview[1], modelview[5], modelview[9]);
	
	const AEVec3 rightPlusUp=AEVec3Add(right, up);
	const AEVec3 rightMinusUp=AEVec3Sub(right, up);
	//This has a desired side effect of scaling on the global axes, not so desirable for particles though
	for(int i=0;i<PARTICLECOUNT;i++){
		v[i][0]=AEVec3Sub(centers[i],AEVec3Mul(rightPlusUp,sizes[i]));
		v[i][1]=AEVec3Add(centers[i],AEVec3Mul(rightMinusUp,sizes[i]));
		v[i][2]=AEVec3Add(centers[i],AEVec3Mul(rightPlusUp,sizes[i]));
		v[i][3]=AEVec3Sub(centers[i],AEVec3Mul(rightMinusUp,sizes[i]));
	}
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.9);
	AEVABindIndices(NULL);
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexPointer(3, GL_FLOAT, 0, v);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glTexCoordPointer(2, GL_FLOAT, 0, t);
	glEnableClientState(GL_COLOR_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glColorPointer(4, GL_UNSIGNED_BYTE, 0, c);
	glDrawArrays(GL_QUADS,0,PARTICLECOUNT);
	AEVABindVertex(NULL);
	AEVABindTexcoord(NULL);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisable(GL_ALPHA_TEST);
	#undef PARTICLECOUNT
}*/

/*
#define PARTICLECOUNT (1024*2*2)
	AEVec3 centers[PARTICLECOUNT];
	AEVec3 sizes[PARTICLECOUNT];
	AERGBAub colors[PARTICLECOUNT];
	//AEVec3 minbound={-20,0,-20};
	//AEVec3 maxbound={20,0,20};
	AEVec3 minbound={-5*4,0,-5*4};
	AEVec3 maxbound={5*4,0,5*4};
	for(int i=0;i<PARTICLECOUNT;i++){
		AEVec3 minsize={1,0.2,1};
		AEVec3 maxsize={1.3,0.3,1.3};
		//AEVec3 mincolor={0.5,0.2,0.2};
		//AEVec3 maxcolor={1,1,1};
		sizes[i]=AEVec3RandomBetween(minsize, maxsize);
		AEVec3 color=AEVec3FromSingle(AERandomBetween(0.7, 1.0));
		//AEVec3 location=AEVec3FromCoords(0.5, 0.5, 0.5);
		centers[i]=AEVec3RandomBetween(minbound, maxbound);
		//AEVec3Add(AEVec3Mul(location, AEVec3Sub(maxbound, minbound)), minbound);
		centers[i].y+=sizes[i].y;
		float factor=1.0-AEVec3LengthInv(AEVec3Sub(centers[i], AEVec3FromCoords(0,0,0)));
		if(factor < 0) factor=0;
		if(factor > 1) factor=1;
		color=AEVec3Mul(color, AEVec3FromSingle(factor));
		colors[i*4].r=color.x*255;
		colors[i*4].g=color.y*255;
		colors[i*4].b=color.z*255;
		colors[i*4].a=255;
	}
	
	GrassPatch=AEGrassPatchNew(false, PARTICLECOUNT, centers, sizes, colors);
	AEVec3 center={0,0,0};
	AEVec3 size={1,1,1};
	//GrassPatch=AEGrassPatchNew(true, 1, &center, &size, NULL);
	//AEGrassPatchDelete(GrassPatch);
	//GrassPatch=NULL;

*/