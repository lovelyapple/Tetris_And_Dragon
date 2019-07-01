//=============================================================================
//
// フロアーデーター[Data_Stage.cpp]
//
//=============================================================================
#include "Data_Stage.h"
#include "main.h"
#include "game.h"



STAGE_DATA g_Stage[MAX_STAGE];
HRESULT InitData_Stage(void)
{
	for(int nStage = 0;nStage <MAX_STAGE;nStage++)
	{
		g_Stage[nStage].FloorCount = MAX_FLOOR_IN_STAGE;
		for(int nFloor = 0; nFloor < MAX_FLOOR_IN_STAGE;nFloor ++)
			g_Stage[nStage].FloorID[nFloor] = 99;
	}
	SetStage(0,2,3,4,99,99,99,99,99,99,99,99);

	//g_Stage[0].FloorCount = 3;
	//g_Stage[0].FloorID[0] = 3;
	//g_Stage[0].FloorID[1] = 1;
	//g_Stage[0].FloorID[2] = 4;

	//g_Stage[1].FloorID[0] = 1;
	//g_Stage[1].FloorID[1] = 1;
	//g_Stage[1].FloorID[2] = 1;

	//g_Stage[2].FloorID[0] = 2;
	//g_Stage[2].FloorID[1] = 1;
	//g_Stage[2].FloorID[2] = 2;

	//g_Stage[3].FloorID[0] = 0;
	//g_Stage[3].FloorID[1] = 1;
	//g_Stage[3].FloorID[2] = 2;

	return S_OK;
}
STAGE_DATA GetStage_Data(int StageID)
{
	return g_Stage[StageID];
}

void SetStage(int StageID,int FloorCount,
	int FloorID_00,
	int FloorID_01,
	int FloorID_02,
	int FloorID_03,
	int FloorID_04,
	int FloorID_05,
	int FloorID_06,
	int FloorID_07,
	int FloorID_08,
	int FloorID_09
	)
{
	g_Stage[StageID].FloorCount = FloorCount;
	g_Stage[StageID].FloorID[0] = FloorID_00;
	g_Stage[StageID].FloorID[1] = FloorID_01; 
	g_Stage[StageID].FloorID[2] = FloorID_02; 
	g_Stage[StageID].FloorID[3] = FloorID_03; 
	g_Stage[StageID].FloorID[4] = FloorID_04; 
	g_Stage[StageID].FloorID[5] = FloorID_05; 
	g_Stage[StageID].FloorID[6] = FloorID_06; 
	g_Stage[StageID].FloorID[7] = FloorID_07; 
	g_Stage[StageID].FloorID[8] = FloorID_08; 
	g_Stage[StageID].FloorID[9] = FloorID_09;
}
