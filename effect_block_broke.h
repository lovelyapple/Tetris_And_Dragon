//=============================================================================
// ブロック破壊効果 [Effect_Broke.h.h]
//=============================================================================
#ifndef _BLOCK_BROKE_H_
#define _BLOCK_BROKE_H_

#include "main.h"
#define TEXTURE_BROKEN "data/TEXTURE/star_green.png"
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEffect_Broke(void);
void UninitEffect_Broke(void);
void UpdateEffect_Broke(void);
void DrawEffect_Broke(void);

void SetEffect_Broke(D3DXVECTOR3 Pos,D3DXVECTOR3 DestPos ,int Color);

#endif
