//=============================================================================
// ブロック破壊効果 [Effect_Star.h.h]
//=============================================================================
#ifndef _STAR_H_
#define _STAR_H_

#include "main.h"
#define TEXTURE_STAR "data/TEXTURE/star_green.png"
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEffect_Star(void);
void UninitEffect_Star(void);
void UpdateEffect_Star(void);
void DrawEffect_Star(void);
LPDIRECT3DTEXTURE9 GetMemoryStar(int color);
void SetEffect_Star(D3DXVECTOR3 Pos,int Color,int nType);

#endif
