//=============================================================================
// ブロック破壊効果 [BlockShining.h]
//=============================================================================
#ifndef _BLOCK_SHINING_
#define _BLOCK_SHINING_

#include "main.h"
#define  TEXTURE_BLOCK_SHINING "data/TEXTURE/blockShining.png"
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBlockShining(void);
void UninitBlockShining(void);
void UpdateBlockShining(void);
void DrawBlockShining(void);
void SetBlockShining(D3DXVECTOR3 pos);

#endif
