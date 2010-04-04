#pragma once
#include "Font.h"

extern float AETextLineSpaceFactor;

int AEText(char* string,AEFont*  font,int x,int y);
void AETextMultiline(char* string,AEFont* font,int x,int y);
AEVec2i AETextEnd(char* string,AEFont* font,int x,int y);