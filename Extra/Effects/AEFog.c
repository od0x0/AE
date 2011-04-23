#include "AEFog.h"

void AEFogBind(AEFog* fog){
	if(not fog){
		glDisable(GL_FOG);
	}
	
	glEnable(GL_FOG);
	
	unsigned int mode=GL_EXP;
	switch (fog->mode) {
		case AEFogModeLinear:
			mode=GL_LINEAR;
			break;
		case AEFogModeExp:
			mode=GL_EXP;
			break;
		case AEFogModeExp2:
			mode=GL_EXP2;
			break;
	}
	
	glFogi(GL_FOG_MODE, mode);
	glFogf(GL_FOG_START, fog->start);
	glFogf(GL_FOG_END, fog->end);

	glClearColor(fog->color.r, fog->color.g, fog->color.b, fog->color.a);
	glFogfv(GL_FOG_COLOR, (GLfloat*)&fog->color);
	glFogf(GL_FOG_DENSITY, fog->density);
	glHint(GL_FOG_HINT, GL_NICEST);
}

static const char* LinearFogShader=
"vec4 AEFogMixWith(vec4 colorIn){\n"
"	float z = gl_FragCoord.z / gl_FragCoord.w;\n"
"	float fogFactor = (gl_Fog.end - z) / (gl_Fog.end - gl_Fog.start);\n"
"	fogFactor = clamp(fogFactor, 0.0, 1.0);\n"
"	vec3 colorOut=mix(gl_Fog.color.rgb, colorIn.rgb, fogFactor );\n"
//"	colorOut.a=colorIn.a;\n"//Override
"	return vec4(colorOut,colorIn.a);\n"
"}\n"
;

static const char* ExpFogShader=
"vec4 AEFogMixWith(vec4 colorIn){\n"
"	float z = gl_FragCoord.z / gl_FragCoord.w;\n"
"	float fogFactor = exp( -gl_Fog.density * z );\n"
"	fogFactor = clamp(fogFactor, 0.0, 1.0);\n"
"	vec3 colorOut=mix(gl_Fog.color.rgb, colorIn.rgb, fogFactor );\n"
//"	colorOut.a=colorIn.a;\n"//Override
"	return vec4(colorOut,colorIn.a);\n"
"}\n"
;

static const char* Exp2FogShader=
"vec4 AEFogMixWith(vec4 colorIn){\n"
"	float z = gl_FragCoord.z / gl_FragCoord.w;\n"
"	float fogFactor = exp2( -gl_Fog.density * gl_Fog.density * z * z * 1.442695 );\n"
"	fogFactor = clamp(fogFactor, 0.0, 1.0);\n"
"	vec3 colorOut=mix(gl_Fog.color.rgb, colorIn.rgb, fogFactor );\n"
//"	colorOut.a=colorIn.a;\n"//Override
"	return vec4(colorOut,colorIn.a);\n"
"}\n"
;

const char* AEFogFragmentShaderGet(AEFog* fog){
	return (fog->mode==AEFogModeExp) ? ExpFogShader : ((fog->mode==AEFogModeExp2) ? Exp2FogShader : LinearFogShader);
}
