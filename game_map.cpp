//=============================================================================
//
// ゲーム画面処理 [game.cpp]
//=============================================================================
#include "main.h"
#include "game_map.h"
#include "Map_Main.h"
#include "Map_Player.h"
#include "input.h"
//#include "fade.h"
#include "Fade_loading.h"
#include "Battle_Effect_Attack.h"
HRESULT InitGame_Map(void)
{
	InitMap_Main();
	InitMap_Player();
	//InitFade_loading();
	//InitBattle_Effect_Attack();
	return S_OK;
}
void UninitGame_Map(void)
{
	UninitMap_Main();
	UninitMap_Player();
	//UninitFade_loading();
	//UninitBattle_Effect_Attack();
}

void UpdateGame_Map(void)
{
	UpdateMap_Main();
	UpdateMap_Player();
	//UpdateFade_loading();
	//UpdateBattle_Effect_Attack();
}
void DrawGame_Map(void)
{
	DrawMap_Main();
	DrawMap_Player();
	//DrawFade_loading();
	//DrawBattle_Effect_Attack();
}
