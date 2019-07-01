//=============================================================================
//
// フロアーデーター[Data_Floor.h]
//
//=============================================================================
#ifndef _DATA_FLOOR_
#define _DATA_FLOOR_


#include "main.h"

#define MAX_FLOOR	(8)
#define MAX_MONSTER_IN_FLOOR	(3)
typedef struct
{
	int MonsterCount;
	int MonsterID[MAX_MONSTER_IN_FLOOR];
	bool Boss;

}FLOOR_DATA;

HRESULT InitData_Floor(void);
FLOOR_DATA GetFloor_Data(int FloorID);
#endif
