#pragma once
#include "A2RMapEntity.h"
#include "A2RMapFIE.h"
#include "A2RMapCellGroup.h"

typedef struct{
	A2RMapCellGroup* visibleCells;
	
	A2RMapFIE* instanceTemplates;
	uint32_t instanceTemplateCount;
	
	AEVec3* instancePositions;
	
	AEList* entityList;//Of A2RMapEntity*
}A2RMap;