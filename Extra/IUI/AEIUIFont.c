#include "AEIUIFont.h"
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

struct AEIUIFont{
	stbtt_bakedchar characters[96];
	AETexture texture;
	int lineheight;
};

AEIUIFont* AEIUIFontLoad(char* filename, int size){
	
	FILE* file=fopen(filename, "rb");
	if(not file){
		printf("Could not open %s\n", filename);
		abort();
	}
	AEIUIFont* font=calloc(1,sizeof(AEIUIFont));
	font->lineheight=size;
	
	fseek(file, 0, SEEK_END);
	size_t length=ftell(file);
	rewind(file);
	void* ttf_buffer=malloc(length);
	fread(ttf_buffer, 1, length, file);
	fclose(file);
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
	while (*text)
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

static size_t AEIUIFontDrawLine(AEIUIFont* self, char* text, int alignment, float x, float* y, float w, float h, size_t caret){
	if (alignment == AEIUIFontAlignCenter) x -= GetTextLength(self->characters, text)/2;
	else if (alignment == AEIUIFontAlignRight) x -= GetTextLength(self->characters, text);
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
			glVertex2f(q.x1, q.y0);
			glVertex2f(q.x1, q.y1);
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

void AEIUIFontDrawParametersInit(AEIUIFontDrawParameters* self){
	memset(self, 0, sizeof(AEIUIFontDrawParameters));
	self->text="";
	self->alignment=-1;
	self->textColor.a=1;
}

void AEIUIFontDraw(AEIUIFont* self, AEIUIFontDrawParameters* params){
	float x=params->x;
	float y=params->y;
	float w=params->w;
	float h=params->h;
	char* text=params->text;
	int alignment=params->alignment;
	size_t caret=params->caret;
	
	glColor4f(params->textColor.r, params->textColor.g, params->textColor.b, params->textColor.a);
	size_t read=0;
	y-=self->lineheight;
	if(w and h){
		glEnable(GL_SCISSOR_TEST);
		glScissor(x, y, w, -h);
	}
	while(read=AEIUIFontDrawLine(self, text, alignment, x, &y, w, h, caret)){
		y-=self->lineheight;
		text+=read;
		if(caret){
			if(caret<read) caret==0;
			else caret-=read;
		}
	}
	if(w and h){
		glDisable(GL_SCISSOR_TEST);
	}
	glColor3f(1, 1, 1);
}

void AEIUIFontDelete(AEIUIFont* self){
	if(not self) return;
	AETextureDelete(self->texture);
	free(self);
}