//=============================================================================
//
// フロアーデーター[Data_Stage.h]
//
//=============================================================================
#ifndef _DATA_STAGE_
#define _DATA_STAGE_


#include "main.h"

#define MAX_STAGE	(4)
#define MAX_FLOOR_IN_STAGE	(10)
typedef struct
{
	int FloorCount;
	int FloorID[MAX_FLOOR_IN_STAGE];
}STAGE_DATA;

HRESULT InitData_Stage(void);
STAGE_DATA GetStage_Data(int StageID);
void SetStage(int FloorCount,int StageID,
	int FloorID_00,
	int FloorID_01,
	int FloorID_02,
	int FloorID_03,
	int FloorID_04,
	int FloorID_05,
	int FloorID_06,
	int FloorID_07,
	int FloorID_08,
	int FloorID_09);
#endif
