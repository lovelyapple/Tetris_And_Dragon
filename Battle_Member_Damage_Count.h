
//=============================================================================
//combo counter damage number
//=============================================================================
#ifndef _COMBO_DAMAGE_NUMBER_
#define _COMBO_DAMAGE_NUMBER_

#include "main.h"
#define TEXTURE_COMBO_DAMAGE		"data/TEXTURE/Num_base.png"
#define COMBO_DAMAGE_MAX_DIGIT		(8)
#define COMBO_DAMAGE_MAX_GROUP		(TEAM_PLAYER_MAX)
#define COMBO_DAMAGE_TIME			(30)
#define MAX_COMBO_DAMAGE_D			(COMBO_DAMAGE_MAX_DIGIT * COMBO_DAMAGE_MAX_GROUP)
#define COMBO_DAMAGE_SIZE_Y			(40)
//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	float	BaseDamage;
	int	RecValue;
	float	DestValue;
	float	ChangeValue;
	int		nStart;
	int		nEnd;
	D3DXVECTOR3	DrawPos;
	float	fSizeX;
	float	fSizeY;
	float	fAlpha;
	int		TimeCount;
	int		nPhase;
}COMBO_DAMAGE_X;
typedef struct
{
	bool	bUse;
	int		nBox;
	float	fSizeX;
	float	fSizeY;
	float	fAlpha;
	D3DXVECTOR3 DrawPos;
}COMBO_DAMAGE_D;
typedef enum
{
	COMBO_DAMAGE_RESET = 0,
	COMBO_DAMAGE_KEEP,
	COMBO_DAMAGE_UP,
	COMBO_DAMAGE_REDUCE,
	COMBO_DAMAGE_CLOSE,
	COMBO_DAMAGE_MAX,
}COMBO_DAMAGE_PHASE;

//*****************************************************************************
// プロトタイプ宣
//*****************************************************************************
HRESULT	InitCombo_Damage(void);
void	UpdateCombo_Damage(void);
void	UninitCombo_Damage(void);
void	DrawCombo_Damage(void);
void SetComboDamageType(int MemberID,int ChangeValue,int Type,int Time);
bool GetComboDamageKeep(void);
void ResetCombo(void);
COMBO_DAMAGE_X	*GetDamageCombo(void);
#endif
