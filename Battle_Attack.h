//=============================================================================
// 攻撃処理[Attack.h]
//=============================================================================
#ifndef _ATTACK_H
#define _ATTACK_H

#include "main.h"
#include "game.h"
#define		ATTACK_MAGNIFCATION_PER_PIECE		(0.25f)
typedef enum
{
	ATTACK_RED = 0,
	ATTACK_GREEN,
	ATTACK_BLUE,
	ATTACK_LIGHT,
	ATTACK_PURPLE,
	ATTACK_PINK,
	ATTACKE_MAX_ATTIBUTION,
	ATTACK_PLAYER,
}ATTACK_ATTIBUTOIN;

typedef struct
{
	int nColor;
	int Base_Atk;
	int nCombo;
	int Damage;
}DAMAGE_STRUCT;//ダメージの計算構造体
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBattle_Attack(void);
void UninitBattle_Attack(void);
void UpdateBattle_Attack(void);
void DrawBattle_Attack(void);
void SetBattle_Attack_Single(int team,int From,int nColor,int Target,float Damage,int Life);//シングル攻撃の設定
DAMAGE_STRUCT *GetDamageStruct(void);
bool DamageCheck(void);//ダメージの中身のチェック処理
bool RecoverCheck(void);//回復の中身をチェック
int	DamageAnalysFinal(int DamageColor,int BaseDamage,int Target);
int	DamageAnalysAttibution(int BaseColor,int Target_Color);
void InitBattleDamage(void);//ダメージ保存構造体の初期化
void SetBattleAttack_Player_Multi(int MemberID,int Target);
int GetSUM_Attack_Time(void);
#endif
