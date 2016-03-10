//=============================================================================
//
// ゲーム画面処理 [game.h]
// HAL名古屋
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"

//*************************************
// ゲーム状態
//*************************************
typedef enum
{
	GAMESTATE_NORMAL = 0,	// 通常
	GAMESTATE_END,			// ゲーム進行終了(画面遷移開始待ち状態)
	GAMESTATE_CHANGE,		// 画面遷移開始状態
	GAMESTATE_FINISH,		// ゲーム画面終了
	GAMESTATE_MAX
} GAMESTATE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);

void SetGameState(GAMESTATE state);
GAMESTATE GetGameState(void);

void SetEnablePause(bool bPause);
bool IsEnablePause(void);


#endif
