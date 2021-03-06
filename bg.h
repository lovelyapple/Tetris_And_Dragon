//=============================================================================
//
// 背景処理 [bg.h]
// HAL名古屋
//
//=============================================================================
#ifndef _BG_H_
#define _BG_H_

#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBG(void);
void UninitBG(void);
void UpdateBG(void);
void DrawBG(void);

void SetPositionBaseBG(void);

#endif
