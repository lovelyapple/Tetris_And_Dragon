//=============================================================================
//
// ゲーム画面処理 [pause.h]
// HAL名古屋
//
//=============================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

#include "main.h"

//*************************************
// ポーズメニュー
//*************************************
typedef enum
{
	PAUSE_MENU_CONTINUE = 0,	// コンティニュー
	PAUSE_MENU_RETRY,			// リトライ
	PAUSE_MENU_QUIT,			// クイット
	PAUSE_MENU_MAX
} PAUSE_MENU;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPause(void);
void UninitPause(void);
void UpdatePause(void);
void DrawPause(void);

void SetPauseMenu(void);
PAUSE_MENU GetPauseMenu(void);
void ResetPauseMenu(void);

#endif
