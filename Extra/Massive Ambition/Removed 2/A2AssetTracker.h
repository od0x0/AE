#pragma once
#include "VBO.h"
#include "Material.h"
#include "khash.h"

KHASH_MAP_INIT_STR(A2AssetTrackerTable, void*)

typedef struct{
	void *vboTable, *materialTable;
}A2AssetTracker;