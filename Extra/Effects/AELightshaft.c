#include "AELightshaft.h"

void AELightshaftUtilityCreateVertices(AELightshaftUtilityVertex* outlineVertices, size_t outlineVertexCount, AELightshaftUtilityVertex* vertices, const AEVec3 direction, const float alphaFade){
	//Vertices is 4x the outline vertex count
	for (size_t i=0; i<outlineVertexCount; i++) {
		vertices[i*4+0].position=outlineVertices[i*2+0].position;
		vertices[i*4+0].color=outlineVertices[i*2+0].color;
		vertices[i*4+1].position=outlineVertices[i*2+1].position;
		vertices[i*4+1].color=outlineVertices[i*2+1].color;
		vertices[i*4+2].position=AEVec3Add(outlineVertices[i*2+1].position, direction);
		vertices[i*4+2].color=outlineVertices[i*2+1].color;
		vertices[i*4+2].color.a*=alphaFade;
		vertices[i*4+3].position=AEVec3Add(outlineVertices[i*2+0].position, direction);
		vertices[i*4+3].color=outlineVertices[i*2+0].color;
		vertices[i*4+3].color.a*=alphaFade;
	}
}

void AELightshaftUtilityRender(AELightshaftUtilityVertex* vertices, size_t vertexCount){
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glInterleavedArrays(GL_C4UB_V3F, 0, vertices);
	glDrawArrays(GL_QUADS, 0, vertexCount);
}