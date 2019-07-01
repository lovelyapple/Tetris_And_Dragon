//=============================================================================
// モンスターデータ[Data_MonsterID]
//=============================================================================
#ifndef _DATA_MONSTER_H_
#define _DATA_MONSTER_H_

#include "main.h"
#include "Battle_Skill_Function.h"
#include "Battle_Enemy_Skill.h"
#include "MessageBox.h"

#define MAX_DATA_MONSTER	(MONSTER_NAME_MAX_NAME)
#define MAX_DATA_CHARACTOR	(MAX_DATA_MONSTER)

typedef enum
{
	MONSTER_NAME_ASUKA	=0,
	MONSTER_NAME_AYANAMI,
	MONSTER_NAME_MARI,
	MONSTER_NAME_MISATO,
	MONSTER_NAME_SHOUGOUKI,
	MONSTER_NAME_TEDRA_RISUS,
	MONSTER_NAME_HAKU,
	MONSTER_NAME_KENBU,
	MONSTER_NAME_FIRESOULD,
	MONSTER_NAME_GOREM,
	MONSTER_NAME_KOBORUTO,
	MONSTER_NAME_MAKENSI_LIGHT,
	MONSTER_NAME_MARK00,
	MONSTER_NAME_WITCH_GREEN,
	MONSTER_NAME_MAX_NAME,
}MONSTER_NAME;

typedef struct
{
	char	name[64];
	char	Tex[64];
	int	MAX_HP;	//HP
	int	ATK;	//攻撃
	int color;	//色
	int turn;	//攻撃ターン
	int SkillCount;	//スキルの種類
	ENEMY_SKILL_STRUCT skill[SKILL_MAX_IN_MONSTER];//スキル
	MESSAGE_WORDS	MessageWord[SKILL_MAX_IN_MONSTER]; 
}DATA_MONSTER;
typedef struct
{
	int	MAX_HP;	//HP
	int	ATK;	//攻撃
	int color;	//色
	int MPRecover;//スキルチャージ能力
	int Recover;//回復力
	SKILL_STATS Skill;//スキル
}DATA_CHARACTOR;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitData_Monster(void);
DATA_MONSTER *GetMonsterData(void);
const char *GetTexture(int ID);
DATA_CHARACTOR *GetCharactorData(void);
#endif
