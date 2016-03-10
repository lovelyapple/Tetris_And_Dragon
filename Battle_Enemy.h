//=============================================================================
// バトルシーンの敵 [Battle_Enemy.h]
//=============================================================================
#ifndef _BATTLE_ENEMY_H
#define _BATTLE_ENEMY_H

#include "main.h"
#include "Battle_Main.h"
#include "Battle_Enemy_Skill.h"
#include "MessageBox.h"
#include "Function.h"
typedef struct
{
	bool	bUse;				//使用されるか
	int		MonsterID;			//使っているモンスターのＩＤ
	int		nColor;				//色
	int		nType;				//敵のランキング wick normal boss
	float	fAtk;				//攻撃力
	int		HP_Type;			//HPの状態
	float	fMaxHP;				//最大ＨＰ
	float	fRecHP;				//現在のＨＰ
	float	fDestHP;			//目標ＨＰ
	float	fHP_Per_Time;		//時間単位のＨＰ変化値
	float	nTeamPlace;			//チームにおいての位置
	float	fSize;
	D3DXVECTOR3	pos;			//描画位置
	float	fAlpha;				//透明度
	int		turn;				//ターン
	int		turnMAX;			//MAXturn

	D3DXVECTOR3	HP_LeftSidePos;
	D3DXVECTOR3	HP_RightSidePos;
	D3DXVECTOR3	HP_EntityPos;
	float		HP_SizeEntitySizeX;
	float		HP_SizeEntitySizeY;
	float		HP_SizeSideSizeX;
	float		HP_SizeSideSizeY;
	float		HP_SizePerHP;

	int			nSkillCount;	//スキルの総数
	ENEMY_SKILL_STRUCT   skill[SKILL_MAX_IN_MONSTER];//する本体
	MESSAGE_WORDS words[SKILL_MAX_IN_MONSTER];
	char		Tex[64];

}BATTLE_ENEMY;
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
//const char *GetMonsterTexture(void);//??????????????

HRESULT InitBattle_Enemy(void);
void UninitBattle_Enemy(void);
void UpdateBattle_Enemy(void);
void DrawBattle_Enemy(void);
void SetBattle_EnemyHP(int EnemyID,int HP_Type,float Damage);
BATTLE_ENEMY *GetBattleEnemy(void);
void SetUpBattleEnemy(int EnemyID,int TeamPlace,int MonsterID,bool Boss,int EnemyCount);
void SetMemberNockBack(float value);
#endif
