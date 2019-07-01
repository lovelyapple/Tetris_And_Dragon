//===================================================================
//Skill_Function.h
//===================================================================
#ifndef _BATTLE_SKILL_FUNCTION_
#define _BATTLE_SKILL_FUNCTION_
#include "main.h"


typedef enum
{
	SKILL_NAME_RECOVER_HP_50 = 0,
	SKILL_NAME_ALL_MAGIC_FIRE_BALL_1000,
	SKILL_NAME_ALL_MAGIC_WATER_BALL_1000,
	SKILL_NAME_ALL_MAGIC_WIND_BALL_1000,
	SKILL_NAME_ALL_MAGIC_DARK_BALL_1000,
	SKILL_NAME_RECOVER_PIECE_CHANGE_RED_TO_RECOVER,
	SKILL_NAME_CHNAGE_Q_ALL_I_RED,
	SKILL_NAME_MAX_SKILL,
}SKILL_NAME;
typedef struct
{
	SKILL_NAME	SkillName;
	int	MPneed;
	int	CoolTurn;
}SKILL_STATS;


//===================================================================
//プロトタイプ宣言
//===================================================================
HRESULT InitBattle_Skill_Function(void);
void UninitBattle_Skill_Function(void);
void UpdateBattle_Skill_Function(void);
void DrawBattle_Skill_Function(void);
void UseSkill(SKILL_NAME name);
SKILL_STATS	GetSkill(SKILL_NAME name);
void SetSkillShining(D3DXVECTOR3 pos);
//===================================================================
//SKILLプロトタイプ
//===================================================================
void Skill_Recover_Weak(void);
void Skill_Fire_Magic_Attack_ALL_Weak_1000(void);
void Skill_Water_Magic_Attack_ALL_Weak_1000(void);
void Skill_Wind_Magic_Attack_ALL_Weak_1000(void);
void Skill_Dark_Magic_Attack_ALL_Weak_1000(void);
void Skill_Recover_Piece_Change_Fire_To_Recover(void);
void Skill_Change_Q_ALL_to_I_RED(void);
#endif
