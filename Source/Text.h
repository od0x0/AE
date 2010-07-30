#pragma once
#include "Font.h"

extern float AETextLineSpaceFactor;

int AEText(char* string,AEFont*  font,int x,int y);
int AETextFitToBounds(char* string,AEFont*  font,int x,int y,int w);
void AETextMultiline(char* string,AEFont* font,int x,int y);
AEVec2i AETextMultilineFitToBounds(char* string,AEFont* font,int x,int y,int w,int h);
AEVec2i AETextEnd(char* string,AEFont* font,int x,int y);
AEVec2i AETextEndFitToBounds(char* string,AEFont* font,int x,int y,int w,int h);