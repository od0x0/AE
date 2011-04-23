#define STB_TRUETYPE_IMPLEMENTATION
#include "AEFont.h"
//This code is ugly, stay out until I eventually clean it!

struct AEFont{
	stbtt_bakedchar characters[96];
	AETexture texture;
	int lineheight;
};

///Some code ripped from the Recast demo
static void GetBakedQuad(stbtt_bakedchar *chardata, int pw, int ph, int char_index,
						 float *xpos, float *ypos, stbtt_aligned_quad *q)
{
	stbtt_bakedchar *b = chardata + char_index;
	int round_x = STBTT_ifloor(*xpos + b->xoff);
	int round_y = STBTT_ifloor(*ypos - b->yoff);
	
	q->x0 = (float)round_x;
	q->y0 = (float)round_y;
	q->x1 = (float)round_x + b->x1 - b->x0;
	q->y1 = (float)round_y - b->y1 + b->y0;
	
	q->s0 = b->x0 / (float)pw;
	q->t0 = b->y0 / (float)pw;
	q->s1 = b->x1 / (float)ph;
	q->t1 = b->y1 / (float)ph;
	
	*xpos += b->xadvance;
}

static float GetTextLength(stbtt_bakedchar *chardata, const char* text)
{
	float xpos = 0;
	float len = 0;
	while (*text and *text!='\r' and *text!='\n')
	{
		int c = (unsigned char)*text;
		if (c >= 32 && c < 128)
		{
			stbtt_bakedchar *b = chardata + c-32;
			int round_x = STBTT_ifloor((xpos + b->xoff) + 0.5);
			len = round_x + b->x1 - b->x0 + 0.5f;
			xpos += b->xadvance;
		}
		++text;
	}
	return len;
}

static float GetTextLengthUntil(stbtt_bakedchar *chardata, const char* text, size_t stop){
	float xpos = 0;
	float len = 0;
	size_t i=0;
	int c=0;
	while (c=text[i])
	{
		if(stop && i==(stop-1)) break;
		if(c=='\r') break;
		if(c=='\n') break;
		if (c >= 32 && c < 128)
		{
			stbtt_bakedchar *b = chardata + c-32;
			int round_x = STBTT_ifloor((xpos + b->xoff) + 0.5);
			len = round_x + b->x1 - b->x0 + 0.5f;
			xpos += b->xadvance;
		}
		i++;
	}
	return len;
}

static size_t AEFontRenderLine(AEFont* self, const char* text, int alignment, float x, float* y, float w, float h, size_t caret){
	if (alignment == AEFontAlignCenter) x -= GetTextLength(self->characters, text)/2;
	else if (alignment == AEFontAlignRight) x -= GetTextLength(self->characters, text);
	// assume orthographic projection with units = screen pixels, origin at top left
	glBindTexture(GL_TEXTURE_2D, self->texture);
	//glPointSize(10);
	glBegin(GL_TRIANGLES);
	int c=0;
	size_t i=0;
	float oldx=x;
	stbtt_aligned_quad q;
	while (c=text[i]) {
		if(caret and caret==(i+1)){
			glEnd();
			glBindTexture(GL_TEXTURE_2D, 0);
			glBegin(GL_LINES);
			//glVertex2f(q.x1, q.y0);
			//glVertex2f(q.x1, q.y1);
			glVertex2f(x+self->characters[c-32].xadvance, *y);
			glVertex2f(x+self->characters[c-32].xadvance, *y+self->lineheight);
			glEnd();
			glBindTexture(GL_TEXTURE_2D, self->texture);
			glBegin(GL_TRIANGLES);
		}
		if (c >= 32 && c < 128) {
			//stbtt_GetBakedQuad(self->characters, 512,512, c-32, &x,&y,&q,true);
			GetBakedQuad(self->characters, 512,512, c-32, &x,y,&q);
			
			glTexCoord2f(q.s0, q.t0);
			glVertex2f(q.x0, q.y0);
			glTexCoord2f(q.s1, q.t1);
			glVertex2f(q.x1, q.y1);
			glTexCoord2f(q.s1, q.t0);
			glVertex2f(q.x1, q.y0);
			
			glTexCoord2f(q.s0, q.t0);
			glVertex2f(q.x0, q.y0);
			glTexCoord2f(q.s0, q.t1);
			glVertex2f(q.x0, q.y1);
			glTexCoord2f(q.s1, q.t1);
			glVertex2f(q.x1, q.y1);
			
			if(w and (x-oldx)>w){
				x=oldx;
				*y-=self->lineheight;
			}
		}
		i++;
		if(c=='\n')
			break;//You must not use return, glEnd() is required or Bad Things (TM) will happen
	}
	glEnd();
	return i;
}

static AEFontStyle DefaultStyle;

void AEFontRenderText(AEFont* self, AEFontStyle* style, float x, float y, float w, float h, const char* text, size_t caret){
	if(text==NULL) text="";
	if(style==NULL) style = & DefaultStyle;
	glColor4f(style->textColor.r, style->textColor.g, style->textColor.b, style->textColor.a);
	size_t read=0;
	y-=self->lineheight;
	//if(w and h){glEnable(GL_SCISSOR_TEST);glScissor(x, y, w, -h);}
	while(read=AEFontRenderLine(self, text, style->alignment, x, &y, w, h, caret)){
		y-=self->lineheight;
		text+=read;
		if(caret){
			if(caret<read) caret==0;
			else caret-=read;
		}
	}
	//if(w and h){glDisable(GL_SCISSOR_TEST);}
	glColor3f(1, 1, 1);
}

void AEFontDelete(AEFont* self){
	if(self==NULL) return;
	AETextureDelete(self->texture);
	free(self);
}

AEFont* AEFontLoadFromTTF(const char* filename, float size){
	FILE* file=fopen(filename, "rb");
	if(not file){
		printf("%s(): Could not open %s\n", __func__, filename);
		abort();
	}
	
	AEFontStyleInit(& DefaultStyle);
	
	AEFont* font=calloc(1, sizeof(AEFont));
	font->lineheight=size;
	
	fseek(file, 0, SEEK_END);
	size_t length=ftell(file);
	rewind(file);
	void* ttf_buffer=malloc(length);
	fread(ttf_buffer, 1, length, file);
	fclose(file);
	//A fixed size like this isn't the best way to go, but it works for now.
	unsigned char temp_bitmap[512*512];
	stbtt_BakeFontBitmap(ttf_buffer,0, size, temp_bitmap,512,512, 32,96, font->characters);
	free(ttf_buffer);
	glGenTextures(1, & font->texture);
	glBindTexture(GL_TEXTURE_2D, font->texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 512,512, 0, GL_ALPHA, GL_UNSIGNED_BYTE, temp_bitmap);
	// can free temp_bitmap at this point
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	return font;
}

float AEFontGetLineHeight(AEFont* self){return self ? self->lineheight : 0;}
float AEFontMeasureTextLength(AEFont* self, const char* text, size_t stop){
	if(self==NULL) return 0;
	if(text==NULL) text="";
	if(stop) return GetTextLengthUntil(self->characters, text, stop);
	return GetTextLength(self->characters, text);
}
