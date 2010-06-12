#include "../Text.h"
#include <string.h>

float AETextLineSpaceFactor=1.0f;

int AEText(char* string,AEFont*  font,int x,int y){
	//int x=0;
	char* oldString=string;
	if(string==NULL) string="(null)";
	glEnable(GL_TEXTURE_RECTANGLE_ARB);
	while(*string!='\0'){
		char c=*(string++);
		//putc(c,stdout);
		if(c=='\n' || c=='\r') return (int)(string-oldString);//Return the number of chars printed
	//	if(!glIsTexture(font->glyph[c].img)) puts("Not a texture");//Won't work without it oddly
		glBindTexture(GL_TEXTURE_RECTANGLE_ARB,font->glyph[c].img);
		glBegin(GL_QUADS);
			glTexCoord2d(0, font->glyph[c].h); glVertex3i(x, y+0, 0);
			glTexCoord2d(font->glyph[c].w, font->glyph[c].h); glVertex3i(x+font->glyph[c].w,y+ 0, 0);
			glTexCoord2d(font->glyph[c].w, 0); glVertex3i(x+font->glyph[c].w, y+font->glyph[c].h, 0);
			glTexCoord2d(0, 0); glVertex3i(x, y+font->glyph[c].h, 0);
		glEnd();
		x+=font->glyph[c].w;
	}
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB, 0);
	glDisable(GL_TEXTURE_RECTANGLE_ARB);
	return 0;
}

void AETextMultiline(char* string,AEFont* font,int x,int y){
	int numberOfCharsPrinted=0;
	while(numberOfCharsPrinted=AEText(string,font,x,y)){
		y+=((float)-(font->size)*AETextLineSpaceFactor);//Do the math as a float to get proper rounding
		string+=numberOfCharsPrinted;
	}
}

AEVec2i AETextEnd(char* string,AEFont* font,int x,int y){
	//unsigned int length=strlen(string);
	unsigned int lastNewline=0;
	char* oldString=string;
	while(*string!='\0'){
		char c=*(string++);
		if(c=='\n' || c=='\r'){
			y+=((float)-(font->size)*AETextLineSpaceFactor);//Do the math as a float to get proper rounding
			lastNewline=(unsigned int)(string-oldString);
		}
	}
	
	string=oldString+lastNewline;
	while(*string!='\0'){
		char c=*(string++);
		x+=font->glyph[c].w;
	}
	AEVec2i end={x,y};
	return end;
}