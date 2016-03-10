//=============================================================================
//  [Battle_Effect_Attack.h]
//=============================================================================
#ifndef _BATTELE_EFFECT_ATTACK_
#define _BATTELE_EFFECT_ATTACK_

#include "main.h"
typedef enum
{
	BATTLE_EFFECT_ATTACK_RED	= 0,
	BATTLE_EFFECT_ATTACK_GREEN,
	BATTLE_EFFECT_ATTACK_BLUE,
	BATTLE_EFFECT_ATTACK_LIGHT,
	BATTLE_EFFECT_ATTACK_PURPLE,
	BATTLE_EFFECT_ATTACK_MAX_TEX,
}BATTLE_EFFECT_EVEN;

typedef struct
{
	bool bUse;				// 使用中か
	D3DXVECTOR3 DrawPos;	// 描画位置(真ん中下)
	int	nType;				// どの画像（属性）
	int	nFrame;				// 何フレーム目か
	int	nMaxFrame;			// 最大フレーム数
	float fVerTex_Per_Unit;	// 毎フレームのテクスチャ頂点距離
	float fSizeX;			// 横幅
	float fSizeY;			// 縦幅
	float fSizeUp;			// 大きさの変化
}BATTELE_EFFECT_ATTACK;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBattle_Effect_Attack(void);
void UninitBattle_Effect_Attack(void);
void UpdateBattle_Effect_Attack(void);
void DrawBattle_Effect_Attack(void);
void SetBattle_Effect_Attack_Final(D3DXVECTOR3 pos,int nType);

#endif
