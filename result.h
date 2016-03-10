//=============================================================================
//
// リザルト画面処理 [result.h]
// HAL名古屋
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitResult(void);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(void);

void SetResult(bool bGameClear);
bool IsGameClear(void);

#endif
