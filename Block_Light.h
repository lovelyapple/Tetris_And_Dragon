//=============================================================================
// ブロック破壊効果 [Block_Light.h]
//=============================================================================
#ifndef _BLOCK_LIGHT_H
#define _BLOCK_LIGHT_H

#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBlock_Light(void);
void UninitBlock_Light(void);
void UpdateBlock_Light(void);
void DrawBlock_Light(void);
void SetLight(D3DXVECTOR3 Pos);

#endif
