//=============================================================================
// テトリスのメイン処理[Battle_Main.h]
//=============================================================================
#ifndef _MAIN_BATTELE_H
#define _MAIN_BATTELE_H


#include "main.h"
#include "game.h"
#include "Tetris_main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
//===========team===============//
typedef enum
{
	BATTLE_BG_GRASS = 0,
	BATTLE_BG_MAX,
}TEXTURE_BG_TYPE;
#define MAX_BATTLE_BG	(2)


typedef enum
{
	TEAM_PLAYER_A,
	TEAM_PLAYER_B,
	TEAM_PLAYER_C,
	TEAM_PLAYER_D,
	TEAM_PLAYER_E,
	TEAM_PLAYER_MAX,
}GAME_TEAM_PLAYER;
typedef enum
{
	TEAM_ENEMY_A = 0,
	TEAM_ENEMY_B,
	TEAM_ENEMY_C,
	TEAM_ENEMY_D,
	TEAM_ENEMY_MAX,
}GAME_TEAM_ENEMY;
typedef enum
{
	TEAM_PLAYER = 0,
	TEAM_ENEMY,
	MAX_TEAM,
}GAME_TEAM;
typedef enum
{
	FLASH_NOTH = 0,
	FLASH_FLASHING,
	FLASH_FADE_IN,
	FLASH_FADE_OUT,
	FLAHS_MAX,
}BATTLE_FLASH;
#define SYS_BATTLE_POS_START_X	 (400.0f)
#define SYS_BATTLE_POS_TETRIS_X	 (500.0f)
#define SYS_BATTLE_SIZE_X		 (SCREEN_HEIGHT*0.65f)
#define SYS_BATTLE_POS_END_X	 (SYS_BATTLE_POS_START_X + SYS_BATTLE_SIZE_X)
#define SYS_BATTLE_BATTLE_SIZE_Y	 ( SCREEN_HEIGHT - VALUE_FIELD_END_Y)
#define SYS_BATTLE_CENTER_X	(SYS_BATTLE_POS_START_X + SYS_BATTLE_BATTLE_SIZE_Y/2)
#define SYS_BATTLE_CENTER_Y (150.0f)

#define VALUE_TIME_CHANGE	(60)
#define VALUE_FIELD_START_X	(SYS_BATTLE_POS_TETRIS_X)	// フィールド描画開始位置X
#define VALUE_FIELD_START_Y	(SCREEN_HEIGHT)	// フィールド描画開始位置Y
#define VALUE_FIELD_SIZE_X	(VALUE_BLOCK_SIZE * MAX_FIELD_COLUMN)
#define VALUE_FIELD_SIZE_Y	(VALUE_BLOCK_SIZE * MAX_FIELD_ROW)
#define VALUE_FIELD_END_X	(VALUE_FIELD_START_X + VALUE_FIELD_SIZE_X)
#define VALUE_FIELD_END_Y	(VALUE_FIELD_START_Y - VALUE_FIELD_SIZE_Y)
#define VALUE_BLOCK_SIZE	(20.0f)			// ブロックの大きさ
#define CAGE_PLAYER_POS		(TEAM_ENEMY_MAX)
//===========team===============//
//==========Battle===========//
#define BATTLE_BG_POS_X			(SYS_BATTLE_POS_START_X + SYS_BATTLE_SIZE_X *0.5f)
#define BATTLE_BG_POS_Y			(0.0f + SCREEN_HEIGHT*0.5f)
#define BATTLE_BG_SIZE_X		(SYS_BATTLE_SIZE_X * 0.5f)
#define BATTLE_BG_SIZE_Y		(SCREEN_HEIGHT * 0.5f)

#define BATTLE_PLAYER_HP_L_POS_X	(50.0f)
#define BATTLE_PLAYER_HP_L_POS_Y	(500.0f)
#define BATTLE_PLAYER_HP_L_SIZE_Y	(50.0f)
#define BATTLE_PLAYER_HP_L_SIZE_X	(BATTLE_BG_SIZE_X-BATTLE_PLAYER_HP_L_POS_X)
#define BATTLE_ATTACK_TIME			(120)//演出時間
#define BATTLE_MAX_CHARACTOR		(TEAM_PLAYER_MAX + TEAM_ENEMY_MAX)
//==========Battle===========//

//==========Enemy===========//
#define		BATTLE_ENEMY_POS_Y		(150.0f)					//敵描画開始位置
#define		BATTLE_ENEMY_POS_X		(SYS_BATTLE_POS_START_X + 50.0f)			//敵描画開始位置
#define		BATTLE_ENEMY_SIZE_X		(100)						//敵のサイズ
#define		BATTLE_ENEMY_SIZE_Y		(BATTLE_ENEMY_SIZE_X)		//敵のサイズ
#define		BATTLE_ENEMY_INERVAL	(20.0f)						//敵たち間の間隔
#define		TEXTURE_ENEMY			"data/TEXTURE/charactor000.jpg"
#define		TIME_FADE_OUT			(120)						//FadeOutする時間
typedef enum
{
	ENEMY_NORMAL = 0,			//雑魚
	ENEMY_SPECIAL,				//特殊敵
	ENEMY_BOSS,					//ボス
	ENEMY_MAX_ENEMY,
}ENEMY_TYPE;
//==========Enemy===========//

//==========Player==========//
#define		BATTLE_PLAYER_HP_POS	(D3DXVECTOR3(100.0f,490,0.0f))
#define		BATTLE_PLATER_CENTER_POS (D3DXVECTOR3(BATTLE_PLAYER_HP_ENTITY_POS_X + BATTLE_PLAYER_HP_ENTITY_SIZE_X /2,BATTLE_PLAYER_HP_POS_Y,0.0f))
#define		BATTLE_TEAM_SIZE		(60.0f)
#define		BATTLE_TEAM_INTERVAL	(10.0f)
#define		BATTLE_TEAM_POS_Y	(VALUE_FIELD_END_Y - 10.0f)
#define		BATTLE_TEAM_POS_X	(SYS_BATTLE_POS_START_X + 20.0f)
//==========Player==========//

//==========HP==============//
typedef enum
{
	HP_KEEPING = 0,			//維持状態
	HP_LOSING,				//減る状態
	HP_RECOVER,				//回復状態
	HP_FADE_OUT,			//消えていく状態
	HP_DEAD,
}HP_TYPE;
//==========HP==============//

//==========SKIll==============//
typedef enum
{
	MP_TYPE_KEEPING = 0,			//維持状態
	MP_TYPE_LOSING,				//減る状態
	MP_TYPE_RECOVER,				//回復状態
	MP_TYPE_FADE_OUT,			//消えていく状態
}MP_TYPE;
//==========SKIllHP==============//
//==========Battle==============//
typedef enum
{
	BATTLE_WALKING = 0,				//フロアの移動
	BATTLE_OPENING,
	BATTLE_LOADING_FLOOR,			//フロアの設置

	BATTLE_SET_BOSS_EFFECT_START,	//ボス戦のエフェクトはじめ
	BATTLE_SET_BOSS_EFFECT_WAIT,	//ボス戦のエフェクトWait
	BATTLE_SET_BOSS_EFFECT_END,		//ボス戦のエフェクトの進行および終了

	BATTLE_SET_ENEMY,				//敵の設置
	BATTLE_FLOOR_START,				//バトル開始

	BATTLE_WAIT_TIME_CAGE,			//全快複までの時間
	BATTLE_WAIT_FIELD_MOVE_IN_PLAYER,//フィールド完全回復までの待機状態

	BATTLE_WAIT_SKILL_OR_TETRIS_PLAYER,			//プレイヤー予備ターン,スキルかテトリスか
	BATTLE_WAIT_SKILL_START,				//スキル開始待ち
	BATTLE_WAIT_SKILL_RUNNING,				//スキル演出停止待ち
	BATTLE_WAIT_SKILL_RESULT,			//スキル使った後の更新チェック

	BATTLE_TETRIS,					//テトリス更新

	BATTLE_ROW_CHAGE_CHECK,			//チャージチェック
	BATTLE_WAIT_FIELD_MOVE_OUT_PLAYER,//プレイヤーターンのフィールド移動たいk状態
	BATTLE_SET_ROW_CHARGE,			//コンボチャージ
	BATTLE_WAIT_ROW_CHARGE,			//matu
	BATTLE_WAIT_PLAYER_RECOVER,		//プレイヤーのHP回復
	
	BATTLE_PLAYER_ATTACK,			//プレイヤー攻撃チェック
	BATTLE_WAIT_PLAYER_RUNING,				//攻撃中
	
	BATTLE_NEXT,					//次のフロアー

	BATTLE_WAIT_FIELD_MOVE_OUT_ENEMY,//フィールドの停止を待つ

	BATTLE_ENEMY_SKILL_CHECK,
	BATTLE_ENEMY_SKILL_ACTIVE,
	BATTLE_WAIT_ENEMY_SKILL,

	BATTLE_ENEMY_TURN_CHECK,		//プレイヤーの攻撃チェック
	BATTLE_ENEMY_ATTACK,			//敵攻撃チェック
	BATTLE_WAIT_ENEMY,				//敵の攻撃タイム
	


	BATTLE_CLEAR,					//クリア
	BATTLE_LOSE,					//負け
}BATTLE_STATS;

//==========Battle==============//

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

HRESULT InitBattle_Main(void);
void UninitBattle_Main(void);
void UpdateBattle_Main(void);
void DrawBattle_Main(void);
void SetBattleStats(int Type);
int  GetBattleStatsWK(void);
int	 GetBattleStats(void);
void SetFlash(int nType,int Time);
void SetBattleStageID(int StageID);
#endif
//switch(g_BattleStats)
	//{
	//case BATTLE_FLOOR_START:
	//	UpdateBattle_Enemy();
	//	UpdateBattle_Player();
	//	if(GetKeyboardTrigger(DIK_SPACE))
	//		g_BattleStats = BATTLE_TETRIS;
	//	break;
	//case BATTLE_TETRIS:
	//	UpdateTetris_Main();
	//	g_BattleStatsWK = BATTLE_TETRIS;
	//	SetTetrisInput(true);
	//	break;
	//case BATTLE_PLAYER_ATTACK:
	//	g_BattleStatsWK = BATTLE_PLAYER_ATTACK;			//ワークを更新
	//	AttackcCheck = DamageCheck();				//攻撃可能のチェック
	//	if(AttackcCheck == true)
	//	{//攻撃可能であれば，攻撃処理を行い，待機モード移行
	//for(int nCntMember = 0;nCntMember < TEAM_PLAYER_MAX;nCntMember++)
	//{
	//}

	//	for(int nCntDamage = 0; nCntDamage < ATTACKE_MAX_ATTIBUTION;nCntDamage++)
	//		{if(pDamageStruct[nCntDamage].Damage == 0)
	//			continue;
	//	SetBattle_Attack_Single(PLAYER_LAST,pDamageStruct[nCntDamage].nColor,
	//							ENEMY_A,pDamageStruct[nCntDamage].Damage);
	//		}
	//	g_BattleStats = BATTLE_WAIT;
	//	}
	//	else
	//	g_BattleStats = BATTLE_ENEMY_ATTACK;//そうでない場合，敵の攻撃ターンへ移行
	//	break;
	//case BATTLE_ENEMY_ATTACK:
	//	g_BattleStatsWK = BATTLE_ENEMY_ATTACK;
	//	SetBattle_Attack_Single(ENEMY_A,0,PLAYER_LAST,10.0f);		
	//	g_BattleStats = BATTLE_WAIT;
	//	break;
	//case BATTLE_WAIT:
	//	UpdateBattle_Enemy();
	//	UpdateBattle_Player();
	//	UpdateTetris_Main();
	//	SetTetrisInput(false);
	//	g_AttackTime --;
	//	if(g_AttackTime ==0)
	//	{
	//		switch(g_BattleStatsWK)
	//		{
	//		case BATTLE_PLAYER_ATTACK:
	//			g_BattleStats = BATTLE_ENEMY_ATTACK;
	//			break;
	//		case BATTLE_ENEMY_ATTACK:
	//			g_BattleStats = BATTLE_WAIT;
	//			g_BattleStatsWK = BATTLE_WAIT;
	//			break;
	//		case BATTLE_WAIT:
	//			if(pMoveTime->nType == TIME_MOVE_KEEPING)
	//				g_BattleStats = BATTLE_TETRIS;
	//			break;
	//		}
	//		g_AttackTime = BATTLE_ATTACK_TIME;
	//		InitBattleDamage();//ダメージ計算の中身を初期化する
	//	}

	//	break;
	//}
