//===================================================================
//Battle_Enemy_Skill.h
//===================================================================
#ifndef _BATTLE_ENEMY_SKILL_FUNCTION_
#define _BATTLE_ENEMY_SKILL_FUNCTION_
#include "main.h"
#include "MessageBox.h"

#define SKILL_ACTUATE_TIME	(10)
#define SKILL_MAX_IN_MONSTER	(3)
typedef enum
{
	ENEMY_SKILL_NAME_SUPER_10_PUNCH_500 = 0,
	ENEMY_SKILL_NAME_GRAVITY_99P_DAMAGE,
	ENEMY_SKILL_NAME_MAX,
}ENEMY_SKILL_NAME;//スキルの名前

typedef enum
{
	ENEMY_SKILL_CONDITION_HP_UNDER_30_EVERY_TURN = 0,
	ENEMY_SKILL_CONDITION_HP_FULL,
	ENEMY_SKILL_CONDITION_MAX,
}ENEMY_SKILL_CONDITION;//スキル発動条件の命名

typedef struct
{
	int	 UseTime;	//実行可能回数
	bool bActuate;
	ENEMY_SKILL_NAME		SkillName;
	ENEMY_SKILL_CONDITION	SkillCondition;
}ENEMY_SKILL_STRUCT;//スキル構造体本体

typedef struct
{
	bool	bUse;
	int nTime;
	D3DXVECTOR3	pos;//メッセージを出す場所
	ENEMY_SKILL_NAME	SkillName;
	int	MessageBoxID;
	MESSAGE_WORDS wordsID;//メッセージの内容
	int	EnemyID;
}ENEMY_SKILL_RUNNING;//スキルランニング用構造体




//=====================スキルチェック

bool Enemy_Skill_Check_Multi(int EnemyID);

bool Enemy_Skill_Check_HP_Under_30(int RecHP,int MAX_HP);
bool Enemy_Skill_Check_HP_FULL(int RecHP,int MAX_HP);

//=====================スキル
void RuningEnemy_SKill(ENEMY_SKILL_NAME name,int EnemyID);
void UseEnemy_Skill(ENEMY_SKILL_NAME name,int EnemyID,D3DXVECTOR3 pos,MESSAGE_WORDS words);
void Enemy_Skill_Super_10_Punch_500(int EnemyNumber);
void Enemy_Skill_Gravity_99HP(int EnemyNumber);

HRESULT	InitBattle_Enemy_Skill(void);
void UpdateBattle_Enemy_Skill(void);


#endif
