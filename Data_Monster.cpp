//================================================================
//モンスターデータ[Data_Monster.cpp]
//
//
//================================================================
#include "Data_Monster.h"
//#include "Battle_Skill_Function.h"
#include "main.h"
#include <stdio.h>
#include <string.h>


void SetMonsterData(int Name,int nATK,int nColor,int nHP,int Turn,int SkillCount);
void SetMonsterSkill(int Name,
					bool ReAct00,int Timeleft00,ENEMY_SKILL_NAME SkillName00,ENEMY_SKILL_CONDITION Condit00,
					bool ReAct01,int Timeleft01,ENEMY_SKILL_NAME SkillName01,ENEMY_SKILL_CONDITION Condit01,
					bool ReAct02,int Timeleft02,ENEMY_SKILL_NAME SkillName02,ENEMY_SKILL_CONDITION Condit02);
void SetCharactorData(int Name,int nATK,int nColor,int nHP,int Recover);
typedef enum
{
	MONSTER_COLOR_RED = 0,
	MONSTER_COLOR_GREEN,
	MONSTER_COLOR_BLUE,
	MONSTER_COLOR_LIGHT,
	MONSTER_COLOR_DARK,
	MONSTER_COLOR_PINK,
	MONSTER_COLOR_MAX,
}MONSTER_COLOR;
const char *c_aFileNameMonsterID[MAX_DATA_MONSTER] = 
	{	
	//"data/MONSTER_BATTLE/MonsterTexMAKENSI_LIGHT.png",
	//"data/MONSTER_BATTLE/MonsterTexMARK00.png",
	//"data/MONSTER_BATTLE/MonsterTexFIRESOULD.png",
	//"data/MONSTER_BATTLE/MonsterTexGOREM.png",
	//"data/MONSTER_BATTLE/MonsterTexKOBORUTO.png",
	"data/MONSTER_BATTLE/MonsterTexASUKA.jpg",
	"data/MONSTER_BATTLE/MonsterTexAYANAMI.jpg",
	"data/MONSTER_BATTLE/MonsterTexMARI.jpg",
	"data/MONSTER_BATTLE/MonsterTexMISATO.png",
	"data/MONSTER_BATTLE/MonsterTexSYOGOUKI.png",
	"data/MONSTER_BATTLE/MonsterTexTedra_Risus.png",
	"data/MONSTER_BATTLE/MonsterTexHAKU.png",
	"data/MONSTER_BATTLE/MonsterTexKENBU.png",
	"data/MONSTER_BATTLE/MonsterTexFIRESOULD.png",
	"data/MONSTER_BATTLE/MonsterTexGOREM.png",
	"data/MONSTER_BATTLE/MonsterTexKOBORUTO.png",
	"data/MONSTER_BATTLE/MonsterTexMAKENSI_LIGHT.png",
	"data/MONSTER_BATTLE/MonsterTexMARK00.png",
	"data/MONSTER_BATTLE/MonsterTexWITCH_GREEN.png",
	};

DATA_MONSTER g_DataMonster[MAX_DATA_MONSTER];
DATA_CHARACTOR g_DataCharactor[MAX_DATA_MONSTER];
void SetMonsterData(int Name,int nATK,int nColor,int nHP,int Turn,int SkillCount)
{
	g_DataMonster[Name].ATK = nATK;
	g_DataMonster[Name].color = nColor;
	g_DataMonster[Name].MAX_HP = nHP;
	g_DataMonster[Name].turn = Turn;
	g_DataMonster[Name].SkillCount	= SkillCount;
	
	for(int nSkill = 0; nSkill < SKILL_MAX_IN_MONSTER;nSkill++)
	{
		g_DataMonster[Name].skill[nSkill].UseTime		 = 99;
		g_DataMonster[Name].skill[nSkill].bActuate		 = false;
		g_DataMonster[Name].skill[nSkill].SkillName		 = ENEMY_SKILL_NAME_MAX;
		g_DataMonster[Name].skill[nSkill].SkillCondition = ENEMY_SKILL_CONDITION_MAX;
		g_DataMonster[Name].MessageWord[nSkill] = MESSAGE_WORDS_MAX;
	}
}
void SetMonsterSkill(int Name,
					int Timeleft00,ENEMY_SKILL_NAME SkillName00,ENEMY_SKILL_CONDITION Condit00,MESSAGE_WORDS words00,
					int Timeleft01,ENEMY_SKILL_NAME SkillName01,ENEMY_SKILL_CONDITION Condit01,MESSAGE_WORDS words01,
					int Timeleft02,ENEMY_SKILL_NAME SkillName02,ENEMY_SKILL_CONDITION Condit02,MESSAGE_WORDS words02)

{
	g_DataMonster[Name].skill[0].SkillName		= SkillName00;
	g_DataMonster[Name].skill[0].UseTime		= Timeleft00;
	g_DataMonster[Name].skill[0].SkillCondition	= Condit00;
	g_DataMonster[Name].MessageWord[0]			= words00;


	g_DataMonster[Name].skill[1].SkillName		= SkillName01;
	g_DataMonster[Name].skill[1].UseTime		= Timeleft01;
	g_DataMonster[Name].skill[1].SkillCondition	= Condit01;
	g_DataMonster[Name].MessageWord[1]			= words01;

	g_DataMonster[Name].skill[2].SkillName		= SkillName02;
	g_DataMonster[Name].skill[2].UseTime		= Timeleft02;
	g_DataMonster[Name].skill[2].SkillCondition	= Condit02;
	g_DataMonster[Name].MessageWord[2]			= words02;
}

void SetCharactorData(int Name,
	int nATK,int nColor,int nHP,
	int Recover,int MPREcover,
	SKILL_NAME name)
{
	g_DataCharactor[Name].ATK = nATK;
	g_DataCharactor[Name].color = nColor;
	g_DataCharactor[Name].MAX_HP = nHP;
	g_DataCharactor[Name].Recover = Recover;
	g_DataCharactor[Name].MPRecover = MPREcover;
	g_DataCharactor[Name].Skill		= GetSkill(name);
}
HRESULT InitData_Monster(void)
{
	for(int nCntData = 0; nCntData < MAX_DATA_MONSTER;nCntData++)
	{
		g_DataMonster[nCntData].ATK = 0;
		g_DataMonster[nCntData].color = 0;
		g_DataMonster[nCntData].MAX_HP = 0;
		g_DataMonster[nCntData].turn	= 0;

		g_DataCharactor[nCntData].ATK = 0;
		g_DataCharactor[nCntData].color = 0;
		g_DataCharactor[nCntData].MAX_HP = 0;
		g_DataCharactor[nCntData].Recover = 0;
		g_DataCharactor[nCntData].MPRecover = 0;
		g_DataCharactor[nCntData].Skill.CoolTurn = 0;
		g_DataCharactor[nCntData].Skill.MPneed = 0;
		g_DataCharactor[nCntData].Skill.SkillName = SKILL_NAME_MAX_SKILL;

	}

	SetMonsterData(MONSTER_NAME_ASUKA,				1000,	MONSTER_COLOR_RED,		2500,	3,0);
	SetMonsterData(MONSTER_NAME_AYANAMI,			470,	MONSTER_COLOR_BLUE,		3000,	4,0);
	SetMonsterData(MONSTER_NAME_MARI,				880,	MONSTER_COLOR_GREEN,	2300,	4,0);
	SetMonsterData(MONSTER_NAME_MISATO,				640,	MONSTER_COLOR_LIGHT,	2700,	4,0);
	SetMonsterData(MONSTER_NAME_SHOUGOUKI,			900,	MONSTER_COLOR_DARK,		2800,	4,0);
	SetMonsterData(MONSTER_NAME_TEDRA_RISUS,		2300,	MONSTER_COLOR_LIGHT,	100000,	5,1);
	SetMonsterData(MONSTER_NAME_HAKU,				700,	MONSTER_COLOR_LIGHT,	2700,	3,0);
	SetMonsterData(MONSTER_NAME_KENBU,				600,	MONSTER_COLOR_DARK,		2960,	3,0);
	SetMonsterData(MONSTER_NAME_FIRESOULD,			900,	MONSTER_COLOR_RED,		2800,	4,0);
	SetMonsterData(MONSTER_NAME_GOREM,				800,	MONSTER_COLOR_GREEN,	4000,	3,0);
	SetMonsterData(MONSTER_NAME_KOBORUTO,			1240,	MONSTER_COLOR_BLUE,		4200,	4,0);
	SetMonsterData(MONSTER_NAME_MAKENSI_LIGHT,		800,	MONSTER_COLOR_LIGHT,	5400,	4,0);
	SetMonsterData(MONSTER_NAME_MARK00,				1000,	MONSTER_COLOR_DARK,		6400,	4,0);
	SetMonsterData(MONSTER_NAME_WITCH_GREEN,		1220,	MONSTER_COLOR_GREEN,	5000,	4,0);
	
	FILE *fc;
	if((fc = fopen("MonsterData.csv","r")) == NULL)
		printf ( "%sファイルが開けませんでした\n");

	for(int nCntMonster = 0;nCntMonster < MAX_DATA_MONSTER;nCntMonster ++)
	{
		fscanf(fc,
			"%[^,],%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%s",
			g_DataMonster[nCntMonster].name,
			&g_DataMonster[nCntMonster].ATK,
			&g_DataMonster[nCntMonster].color,
			&g_DataMonster[nCntMonster].MAX_HP,
			&g_DataMonster[nCntMonster].SkillCount,
			&g_DataMonster[nCntMonster].turn,
			&g_DataMonster[nCntMonster].skill[0].bActuate,
			&g_DataMonster[nCntMonster].skill[0].SkillCondition,
			&g_DataMonster[nCntMonster].skill[0].SkillName,
			&g_DataMonster[nCntMonster].skill[0].UseTime,
			&g_DataMonster[nCntMonster].skill[1].bActuate,
			&g_DataMonster[nCntMonster].skill[1].SkillCondition,
			&g_DataMonster[nCntMonster].skill[1].SkillName,
			&g_DataMonster[nCntMonster].skill[1].UseTime,
			&g_DataMonster[nCntMonster].skill[2].bActuate,
			&g_DataMonster[nCntMonster].skill[2].SkillCondition,
			&g_DataMonster[nCntMonster].skill[2].SkillName,
			&g_DataMonster[nCntMonster].skill[2].UseTime,
			&g_DataMonster[nCntMonster].MessageWord[0],
			&g_DataMonster[nCntMonster].MessageWord[1],
			&g_DataMonster[nCntMonster].MessageWord[2],
			g_DataMonster[nCntMonster].Tex
			);
	}
	fclose(fc);




	
	SetMonsterSkill(MONSTER_NAME_TEDRA_RISUS,
			2,	ENEMY_SKILL_NAME_SUPER_10_PUNCH_500,	ENEMY_SKILL_CONDITION_HP_UNDER_30_EVERY_TURN,	MESSAGE_WORDS_HOROBIYO,
			1,	ENEMY_SKILL_NAME_GRAVITY_99P_DAMAGE,	ENEMY_SKILL_CONDITION_HP_FULL,		MESSAGE_WORDS_NAMETERU,
			0,	ENEMY_SKILL_NAME_MAX,					ENEMY_SKILL_CONDITION_MAX,			MESSAGE_WORDS_MAX);

	SetCharactorData(MONSTER_NAME_ASUKA,	260,
		MONSTER_COLOR_RED,								1200,	50,		4,
		SKILL_NAME_CHNAGE_Q_ALL_I_RED);
	SetCharactorData(MONSTER_NAME_AYANAMI,	175,	
		MONSTER_COLOR_BLUE,								2000,	70,		10,
		SKILL_NAME_ALL_MAGIC_WATER_BALL_1000);
	SetCharactorData(MONSTER_NAME_MARI,		190,	
		MONSTER_COLOR_GREEN,	1900,	50,		4,
		SKILL_NAME_RECOVER_PIECE_CHANGE_RED_TO_RECOVER);
	SetCharactorData(MONSTER_NAME_MISATO,	250,
		MONSTER_COLOR_LIGHT,							1750,	60,		7,
		SKILL_NAME_RECOVER_HP_50);
	SetCharactorData(MONSTER_NAME_SHOUGOUKI,	300,
		MONSTER_COLOR_DARK,								2500,	40,		4,
		SKILL_NAME_ALL_MAGIC_DARK_BALL_1000);

	return S_OK;
}
DATA_MONSTER *GetMonsterData(void)
{
	return &g_DataMonster[0];
}
DATA_CHARACTOR *GetCharactorData(void)
{
	return &g_DataCharactor[0];
}
const char *GetTexture(int ID)
{
	return c_aFileNameMonsterID[ID];
}
