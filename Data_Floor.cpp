//=============================================================================
//
// フロアーデーター[Data_Floor.cpp]
//
//=============================================================================
#include "Data_Floor.h"
#include "main.h"
#include "Data_Monster.h"
void SetMonsterInFloor(int FloorID,int MonsterCount,bool Boss,
	int MonsterID_00,
	int MonsterID_01,
	int MonsterID_02
	);
FLOOR_DATA g_Floor[MAX_FLOOR];
void SetMonsterInFloor(int FloorID,int MonsterCount,bool Boss,
	int MonsterID_00,
	int MonsterID_01,
	int MonsterID_02
	)
{
	g_Floor[FloorID].Boss = Boss;
	g_Floor[FloorID].MonsterCount = MonsterCount;
	g_Floor[FloorID].MonsterID[0] = MonsterID_00;
	g_Floor[FloorID].MonsterID[1] = MonsterID_01;
	g_Floor[FloorID].MonsterID[2] = MonsterID_02;
}

HRESULT InitData_Floor(void)
{
	for(int nFloor = 0;nFloor <MAX_FLOOR;nFloor++)
	{
		g_Floor[nFloor].MonsterCount = 3;
		g_Floor[nFloor].Boss		= false;
		for(int nMonster = 0; nMonster < MAX_MONSTER_IN_FLOOR;nMonster++)
			g_Floor[nFloor].MonsterID[nMonster] = 99;
	}

SetMonsterInFloor( 0, 3, false,
	 MONSTER_NAME_ASUKA,
	 MONSTER_NAME_AYANAMI,
	 MONSTER_NAME_ASUKA
	);

SetMonsterInFloor( 1, 3, false,
	 MONSTER_NAME_ASUKA,
	 MONSTER_NAME_AYANAMI,
	 MONSTER_NAME_ASUKA
	);

SetMonsterInFloor( 2, 3, false,
	 MONSTER_NAME_MARK00,
	 MONSTER_NAME_GOREM,
	 MONSTER_NAME_MARK00
	);
SetMonsterInFloor( 3, 3, false,
	 MONSTER_NAME_MAKENSI_LIGHT,
	 MONSTER_NAME_KOBORUTO,
	 MONSTER_NAME_WITCH_GREEN
	);
SetMonsterInFloor( 4, 1, true,
	 MONSTER_NAME_TEDRA_RISUS,
	 MONSTER_NAME_MAX_NAME,
	 MONSTER_NAME_MAX_NAME
	);


	return S_OK;
}
FLOOR_DATA GetFloor_Data(int FloorID)
{
	return g_Floor[FloorID];
}
