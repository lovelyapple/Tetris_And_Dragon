//*****************************************************************************
//関数用CPP
//*****************************************************************************
#include "game.h"
#include "input.h"
#include "Tetris_main.h"
#include "group_in_queue.h"
#include "func_mouse.h"
#include "TimeLeftCage.h"
#include "Battle_Main.h"
#include "Battle_Player.h"
#include "Battle_Enemy.h"
#include "Battle_Attack.h"
#include "Battle_Row_Charge.h"
#include "Battle_Player_HP.h"
#include "Battle_Player_MP.h"
#include "Battle_Effect_TimeLeftCount.h"
#include "Battle_Skill_Function.h"
#include "Battle_Enemy_Skill.h"
//****************************************************************************+
//現在所有関数
//
//SetMAX : 値と範囲及び範囲タイプを入れ，範囲制御を行った結果を返す
//GetRand: 確率(個数)を持つ配列とその配列のメンバー数を入れ，確率的な結果を返してくれる
//
//
//
//
//
//
//*****************************************************************************
HRESULT InitFun(void)
{
	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void UninitFun(void)
{
}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateFunction(void)
{
	//SetBattle_Attack_Single(TEAM_ENEMY,0,0,TEAM_PLAYER_MAX,pEnemy[0].fAtk,0);
	////実験用
	//if(GetKeyboardTrigger(DIK_O))
	//	SetBattle_EnemyHP(0,HP_RECOVER,-300);
	////実験用
	if(GetKeyboardPress(DIK_LCONTROL))
	{
		if(GetKeyboardTrigger(DIK_R))
		{
			BATTLE_T_MEMBER *pMember = GetTMember();
			SetBattle_PlayerMP_Recover(MP_TYPE_RECOVER,-100);
			for(int nCntMember = 0;nCntMember < TEAM_PLAYER_MAX;nCntMember++)	
				pMember[nCntMember].TurnLeft = 0;			
		}
		else if(GetKeyboardTrigger(DIK_T))
		{
			BATTLE_ENEMY *pEnemy = GetBattleEnemy();
			pEnemy[0].fRecHP = 100;
		}
		else if(GetKeyboardTrigger(DIK_SPACE))
			SetRowCharge(60,ROW_CHARGE_INCREASING);
	}
	//	for(int nCntMember = 0;nCntMember < TEAM_PLAYER_MAX;nCntMember++)	
	//	g_BattleT_Memeber[nCntMember].TurnLeft = 0;
	//}
	//if(GetKeyboardTrigger(DIK_R))
	//	MessageBoxID =  OpeBoxIDBox(D3DXVECTOR3(600.0f,400.0f,0.0f),400.0f,100.0f,MESSAGE_WORDS_HOROBIYO);
	//if(GetKeyboardTrigger(DIK_E))
	//	for(int BoxID = 0;BoxID < MESSAGE_BOX_MAX;BoxID++)
	//	CloseMessageBox(BoxID);


	//	if(GetKeyboardTrigger(DIK_SPACE))
	//	g_FieldState= FIELD_SETUP;
	//if(GetKeyboardTrigger(DIK_SPACE))
	//{
	//	InitBlockAll();//すべてのブロックを初期化する
	//	InitBlock_Q_ALL();
	//	InitGroup_Q_ALL();

	//	for(int Q_ID = 0; Q_ID < MAX_GROUP_Q;Q_ID++)
	//		SetGroup_Q(Q_ID);
	//	g_GroupLeft = 4;
	//}
	//	if(GetKeyboardTrigger(DIK_U))
	//	SetMoveTime(TIME_MOVE_LOSING,VALUE_DEFAULT_MOVE_TIME);
	//if(GetKeyboardRelease(DIK_U))
	//	SetMoveTime(TIME_MOVE_RECOVER,VALUE_DEFAULT_MOVE_TIME);
}

float SetMAX(float NOW, float MAX, float MIN,int nType)
{
	float Result;
	//タイプ１：上下限界があり，下限を下よりいくなら，反対側の大きい値になる。
	//要するに最小値と最大値を同一視して、左右リフレクトとなる無限循環通なる
	//タイプ２：上下限界があり、限界値を超えさせないようにする
	if(nType == 1)
	{
		if(NOW>MAX)
			NOW  = MIN;
		if(NOW<MIN)
			NOW  = MAX;
		Result = NOW;
	}
	if(nType ==2)
	{
		if(NOW>MAX)
			NOW = MAX;
		if(NOW<MIN)
			NOW = MIN;
		Result = NOW;
	}

	return Result;
		 
}
int GetRandColor(RAND_STRUCT Input[],int Count)
{
	int nResult = 99;
	
	for(int nIdx = 0; nIdx < Count; nIdx++)
	{
		if(Input[nIdx].bUse == false)
			continue;
		int Bonus = Input[nIdx].BonusValue;
		if(Bonus ==0)
			continue;
		if(rand()%(Count * 100) > Bonus)
			continue;
		nResult = nIdx;
		break;
	}
	if(nResult == 99)
		nResult = rand()%Count;
	return nResult;
}
D3DXVECTOR3 GetMovingPos(D3DXVECTOR3 StartPos,D3DXVECTOR3 EndPos,int MAXFrame,int FrameNow)
{
	float MoveX = (EndPos.x - StartPos.x) / MAXFrame * FrameNow;
	float MoveY = (EndPos.y - StartPos.y) / MAXFrame * FrameNow;

	D3DXVECTOR3 ResPos = D3DXVECTOR3(StartPos.x + MoveX,StartPos.y + MoveY,0.0f);
	return ResPos;
}
