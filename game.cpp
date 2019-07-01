//=============================================================================
//
// ゲーム画面処理 [game.cpp]
// HAL名古屋
//
//=============================================================================
#include "game.h"
#include "bg.h"
#include "input.h"
//#include "fade.h"
#include "pause.h"
#include "result.h"
#include "Tetris_main.h"
#include "group_in_queue.h"
#include "effect_block_broke.h"
#include "effect_star.h"
#include "effect_curve.h"
#include "func_mouse.h"
#include "TimeLeftCage.h"
#include "Block_Light.h"
#include "Battle_Main.h"
//#include "Func_ChangingCage.h"
#include "Battle_Player.h"
#include "Battle_Enemy.h"
#include "Battle_Attack.h"
#include "Data_Monster.h"
#include "Data_Floor.h"
#include "Data_Stage.h"
#include "Damage_Number.h"
#include "Effect_Tetris_Block_Shining.h"
#include "Battle_Row_Charge.h"
#include "Battle_Member_Damage_Count.h"
#include "Battle_Effect_Attack.h"
#include "Fade_loading.h"
#include "Battle_Player_HP.h"
#include "Battle_Player_MP.h"
#include "Battle_InterFaseBG.h"
#include "Battle_Effect_TimeLeftCount.h"
#include "Battle_ComboCheers.h"
#include "Battle_Skill_Function.h"
#include "Battle_Boss_Alert_Effect.h"
#include "Battle_Enemy_Skill.h"
#include "Battle_Player_Item.h"
#include "Battle_Stage_End.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	COUNT_WAIT_FINISH_GAME	(60)	// ゲーム画面終了までの待ち時間

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
GAMESTATE g_stateGame = GAMESTATE_NORMAL;	// ゲーム状態
int g_nCountStateGame = 0;					// ゲーム状態カウンター
bool g_bPause = false;						// ポーズ中かどうか
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitGame(void)
{
	// 背景の初期化処理
	InitBG();

	// ポーズ処理の初期化処理
	InitPause();
	InitBattle_Stage_End();
	InitBattle_Skill_Function();
	InitBattle_Enemy_Skill();
	InitGroup_Q();
	InitTimeLeft();
	//InitChange_Cage();
	InitTetris_Main();
	InitBattle_Main();
	InitBattle_InterFaseBG();
	InitFunc_Mouse();
	InitBattle_Effect_TimeLeftCount();
	InitBattle_Effect_Alert();

	InitEffect_Star();
	InitEffect_Broke();//star の　後ろに書かないとメモリゲットできないよ

	InitEffect_Curve();
	InitBlock_Light();
	InitBattle_Player_HP();
	InitBattle_Player_MP();
	InitBattle_Player();
	InitBattle_Enemy();
	InitBattle_Attack();
	InitData_Monster();
	InitDamage_NumberD();
	InitBlockShining();
	InitRowCharge();
	InitCombo_Damage();
	InitBattle_Effect_Attack();
	InitBattle_Effect_ComboCheers();
	InitBattle_Player_Item();
	g_stateGame = GAMESTATE_NORMAL;
	g_nCountStateGame = 0;
	g_bPause = false;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitGame(void)
{
	// 背景の終了処理
	UninitBG();
	UninitBattle_Stage_End();
	// ポーズ処理の終了処理
	UninitPause();
	UninitBattle_Skill_Function();
	UninitTetris_Main();
	UninitGroup_Q();
	UninitEffect_Broke();
	UninitFunc_Mouse();
	UninitEffect_Star();
	UninitEffect_Curve();
	UninitBlock_Light();
	UninitTimeLeft();
	UninitBattle_Main();
//	UninitChange_Cage();
	UninitBattle_Player();
	UninitBattle_Enemy();
	UninitBattle_Attack();
	UninitDamage_NumberD();
	UninitBlockShining();
	UninitRowCharge();
	UninitCombo_Damage();
	UninitBattle_Effect_Attack();
	UninitBattle_Player_HP();
	UninitBattle_Player_MP();
	UninitBattle_InterFaseBG();
	UninitBattle_Effect_TimeLeftCount();
	UninitBattle_Effect_ComboCheers();
	UninitBattle_Effect_Alert();
	UninitBattle_Player_Item();
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateGame(void)
{
	// ポーズON/OFF
	if(GetKeyboardTrigger(DIK_P))
	{
		//if(GetFade() == FADE_NONE)
		{
			g_bPause = g_bPause ? false: true;

			if(g_bPause == true)
			{// ポーズ開始
				SetKeyboardRepeatInterval(5);
				ResetPauseMenu();
			}
			else
			{// ポーズ解除
				SetKeyboardRepeatInterval(1);
			}
		}
	}

	if(g_bPause == true)
	{// ポーズ中
		// ポーズ処理の更新処理
		UpdatePause();
	}
	else
	{
		// 背景更新
		UpdateBG();
		UpdateBattle_Skill_Function();


		//UpdateTetris_Main();
		UpdateGroup_Q();
		UpdateTimeLeft();
		UpdateFunction();
		UpdateEffect_Broke();
		UpdateEffect_Star();
		UpdateEffect_Curve();
		UpdateBlock_Light();
		UpdateBattle_Main();
		UpdateTetris_Main();
		UpdateBattle_Player();
		UpdateBattle_Enemy();
		UpdateBattle_Attack();
//		UpdateChange_Cage();
		UpdateDamage_NumberD();
		UpdateBlockShining();
		UpdateRowCharge();
		UpdateCombo_Damage();
		UpdateBattle_Player_HP();
		UpdateBattle_Player_MP();
		UpdateBattle_Effect_Attack();
		UpdateBattle_InterFaseBG();
		UpdateBattle_Effect_TimeLeftCount();
		UpdateBattle_Effect_ComboCheers();
		UpdateBattle_Effect_Alert();
		UpdateBattle_Skill_Function();
		UpdateBattle_Enemy_Skill();
		UpdateFunctionc_Mouse();
		UpdateBattle_Player_Item();
		UpdateBattle_Stage_End();

		switch(g_stateGame)
		{
		case GAMESTATE_END:
			g_nCountStateGame++;
			if(g_nCountStateGame > COUNT_WAIT_FINISH_GAME)
			{
				g_stateGame = GAMESTATE_CHANGE;
			}
			break;

		case GAMESTATE_CHANGE:
			//SetFade(FADE_OUT, MODE_RESULT);
			SetLoadStart(MODE_RESULT);
			g_stateGame = GAMESTATE_FINISH;
			break;
		}

#if 0
		if(GetKeyboardTrigger(DIK_RETURN))
		{// Enter押したら、フェードアウトしてモードを切り替えいく
			SetFade(FADE_OUT, MODE_RESULT);
		}
#endif
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawGame(void)
{
	// 背景の描画処理
	DrawBG();

	
	//時間カウントの描画
	//DrawBattle_Effect_TimeLeftCount();
	DrawBattle_Main();
	DrawBattle_Enemy();
	DrawBattle_Effect_Alert();
	DrawBattle_InterFaseBG();

	DrawEffect_Curve();
	DrawTimeLeft();
	DrawBattle_Player_Item();

	DrawTetris_Main();
	DrawBlockShining();

	//DrawChange_Cage();
	DrawBattle_Player();
	DrawBattle_Player_HP();
	DrawBattle_Player_MP();
	DrawBattle_Skill_Function();

	DrawRowCharge();
	DrawGroup_Q();
	DrawCombo_Damage();
	DrawDamage_NumberD();
	DrawBattle_Attack();
	DrawBattle_Effect_Attack();
	DrawBattle_Effect_ComboCheers();
	DrawEffect_Broke();
	DrawEffect_Star();
	DrawBattle_Stage_End();
	DrawFunc_Mouse();

	// ポーズ処理の描画処理
	if(g_bPause == true)
	{// ポーズ中
		DrawPause();
	}
}

//=============================================================================
// ゲーム状態の設定
//=============================================================================
void SetGameState(GAMESTATE state)
{
	g_stateGame = state;
}

//=============================================================================
// ゲーム状態の取得
//=============================================================================
GAMESTATE GetGameState(void)
{
	return g_stateGame;
}

//=============================================================================
// ポーズON/OFF
//=============================================================================
void SetEnablePause(bool bPause)
{
	g_bPause = bPause;
}

//=============================================================================
// ポーズ中かどうか
//=============================================================================
bool IsEnablePause(void)
{
	return g_bPause;
}
