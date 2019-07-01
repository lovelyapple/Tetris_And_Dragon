//=============================================================================
// マウス処理 [func_mouse.cpp.h]
//=============================================================================
#ifndef _MOUSE_H_
#define _MOUSE_H_

#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitFunc_Mouse(void);
void UninitFunc_Mouse(void);
void UpdateFunctionc_Mouse(void);
void DrawFunc_Mouse(void);

D3DXVECTOR2 GetMousePos(void);

#endif
