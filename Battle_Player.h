//=============================================================================
// ブロック破壊効果 [Battle_Player.h]
//=============================================================================
#ifndef _BATTLE_PLAYER_H
#define _BATTLE_PLAYER_H

#include "main.h"
#include "Battle_Skill_Function.h"
typedef struct
{
	int		HP_Type;			//ＨＰの変化状態
	int		HP_Recover;			//HPの回復力
	int		HP_RecoverResult;	//計算後の回復力
	float	fMaxHP;				//最大ＨＰ
	float	fRecHP;				//現在のＨＰ
	float	fDestHP;			//変化目標値
	float	fHP_Per_Time;		//時間単位のＨＰ変化値

	int		MP_Type;			//
	int		MP_Recover;			//
	int		MP_Recover_Result;	//
	float	fMaxMP;				//
	float	fRecMP;				//
	float	fDestMP;			//
	float	fMP_Per_Time;		//


	D3DXVECTOR3	pos;			//描画位置
}BATTLE_PLAYER;
typedef struct
{
	bool	bUse;				//描画の許可
	int		Color;
	int		MonsterID;			//Monster番号
	float	fMAXHP;				//ＨＰ値
	float	fATK;				//攻撃力
	int		TeamPlace;			//チーム内の位置
	int		DamageBase;			//基本ダメージ計算
	int		DamageFinal;		//ダメージの最終計算
	int		nTarget;			//ターゲットの計算
	D3DXVECTOR3	pos;			//描画位置
	bool	bSkillUse;			//スキル発動できるか
	SKILL_STATS Skill;			//所持スキル
	int		TurnLeft;			//冷却ターン
	float	Light_Up;			//スキルライト
}BATTLE_T_MEMBER;
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBattle_Player(void);
void UninitBattle_Player(void);
void UpdateBattle_Player(void);
void DrawBattle_Player(void);
void SetBattle_PlayerHP_Change(int HP_Type,float Damage);
void SetBattle_PlayerMP_Recover(int MP_Type,float Recover);
int	GetBattle_playerHP(void);
int GetBattle_Player_MP_Type(void);
BATTLE_T_MEMBER *GetTMember(void);
BATTLE_PLAYER *GetBattlePlayer(void);
void ResetBattleMemberBaseDamage(void);
#endif
