//=============================================================================
// ブロック破壊効果 [Battle_Player_Item.h]
//=============================================================================
#ifndef _BATTLE_PLAYER_ITEM_
#define _BATTLE_PLAYER_ITEM_

#include "main.h"
#define TEXTURE_STAR "data/TEXTURE/star_green.png"
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBattle_Player_Item(void);
void UninitBattle_Player_Item(void);
void UpdateBattle_Player_Item(void);
void DrawBattle_Player_Item(void);

#endif
