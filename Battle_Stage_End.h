//=============================================================================
// ブロック破壊効果 [Battle_Stage_End.h]
//=============================================================================
#ifndef _BATTLE_STAGE_H_
#define _BATTLE_STAGE_H_

#include "main.h"


typedef enum
{
	STAGE_RESULT_WIN = 0,
	STAGE_RESULT_LOST,
	STAGE_RESULT_MAX,
}STAGE_RESULT;
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBattle_Stage_End(void);
void UninitBattle_Stage_End(void);
void UpdateBattle_Stage_End(void);
void DrawBattle_Stage_End(void);
void SetStageResult(int Result);

#endif
