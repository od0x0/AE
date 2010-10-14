#include "Text.h"
#include <string.h>

float AETextLineSpaceFactor=1.0f;

int AETextFitToBounds(char* string,AEFont*  font,int x,int y,int w){
	if(font==NULL) return 0;
	//int x=0;
	char* oldString=string;
	if(string==NULL) string="(null)";
	int pos=0;
	glEnable(GL_TEXTURE_RECTANGLE_ARB);
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB,font->img);
	while(*string!='\0'){
		char c=*(string++);
		//c-=32;
		pos+=font->glyph[c].w;
		if(w) if(pos > w) return (int)(string -1 -oldString);//Return the number of chars printed -1 because we didn't render this one.
		
		//putc(c,stdout);
		if(c=='\n' || c=='\r') return (int)(string-oldString);//Return the number of chars printed
	//	if(!glIsTexture(font->glyph[c].img)) puts("Not a texture");//Won't work without it oddly
		/*glBegin(GL_QUADS);
			//glColor3f(1,0,0);
			unsigned int offsetx=font->glyph[c].offsetx;
			glTexCoord2d(0+offsetx, font->glyph[c].h); glVertex3i(x, y+0, 0);
			glTexCoord2d(font->glyph[c].w+offsetx, font->glyph[c].h); glVertex3i(x+font->glyph[c].w,y+ 0, 0);
			glTexCoord2d(font->glyph[c].w+offsetx, 0); glVertex3i(x+font->glyph[c].w, y+font->glyph[c].h, 0);
			glTexCoord2d(0+offsetx, 0); glVertex3i(x, y+font->glyph[c].h, 0);
		glEnd();*/
		AEFontRenderChar(font,c,x,y,0,0);
		x+=font->glyph[c].w;
		//abort();
	}
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB, 0);
	glDisable(GL_TEXTURE_RECTANGLE_ARB);
	return 0;
}

int AEText(char* string,AEFont*  font,int x,int y){
	//int x=0;
	char* oldString=string;
	if(string==NULL) string="(null)";
	glEnable(GL_TEXTURE_RECTANGLE_ARB);
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB,font->img);
	while(*string!='\0'){
		char c=*(string++);
		//putc(c,stdout);
		if(c=='\n' || c=='\r') return (int)(string-oldString);//Return the number of chars printed
	//	if(!glIsTexture(font->glyph[c].img)) puts("Not a texture");//Won't work without it oddly
		glBegin(GL_QUADS);
			unsigned int offsetx=font->glyph[c].offsetx;
			glTexCoord2d(0+offsetx, font->glyph[c].h); glVertex3i(x, y+0, 0);
			glTexCoord2d(font->glyph[c].w+offsetx, font->glyph[c].h); glVertex3i(x+font->glyph[c].w,y+ 0, 0);
			glTexCoord2d(font->glyph[c].w+offsetx, 0); glVertex3i(x+font->glyph[c].w, y+font->glyph[c].h, 0);
			glTexCoord2d(0+offsetx, 0); glVertex3i(x, y+font->glyph[c].h, 0);
		glEnd();
		x+=font->glyph[c].w;
	}
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB, 0);
	glDisable(GL_TEXTURE_RECTANGLE_ARB);
	return 0;
}

AEVec2i AETextMultilineFitToBounds(char* string,AEFont* font,int x,int y,int w,int h){
	int numberOfCharsPrinted=0;
	int pos=0;
	while(numberOfCharsPrinted=AETextFitToBounds(string,font,x,y+pos,w)){
		pos+=((float)-(font->size)*AETextLineSpaceFactor);//Do the math as a float to get proper rounding
		if((pos + font->size) > h) break;
		string+=numberOfCharsPrinted;
	}
	AEVec2i p={x,pos+y};
	return p;
}

void AETextMultiline(char* string,AEFont* font,int x,int y){
	int numberOfCharsPrinted=0;
	while(numberOfCharsPrinted=AEText(string,font,x,y)){
		y+=((float)-(font->size)*AETextLineSpaceFactor);//Do the math as a float to get proper rounding
		string+=numberOfCharsPrinted;
	}
}

AEVec2i AETextEndFitToBounds(char* string,AEFont* font,int x,int y,int w,int h){
	//unsigned int length=strlen(string);
	unsigned int lastNewline=0;
	unsigned int wrappedlineCount=0;//A quick fix for an unknown bug
	char* oldString=string;
	int xpos=0;
	int ypos=0;
	while(*string!='\0'){
		char c=*(string++);
		
		xpos+=font->glyph[c].w;
		if(w) if(xpos > w){
			wrappedlineCount++;
			ypos+=((float)-(font->size)*AETextLineSpaceFactor);//Do the math as a float to get proper rounding
			lastNewline=(unsigned int)(string-wrappedlineCount-oldString);
			xpos=0;
		}		
		if(c=='\n' || c=='\r'){
		//	lineCount++;
			wrappedlineCount=0;
			ypos+=((float)-(font->size)*AETextLineSpaceFactor);//Do the math as a float to get proper rounding
			lastNewline=(unsigned int)(string-oldString);
			xpos=0;
		}
	}
	
	string=oldString+lastNewline;
	while(*string!='\0'){
		char c=*(string++);
		x+=font->glyph[c].w;
	}
	AEVec2i end={x,y+ypos};
	return end;
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