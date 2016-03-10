//===================================================================
//Battle_Enemy_Skill.cpp
//===================================================================
#include "Battle_Enemy_Skill.h"
#include "main.h"

#include "Tetris_main.h"
#include "Battle_Attack.h"
#include "Battle_Player.h"
#include "Battle_Enemy.h"
#include "Battle_Main.h"
#include "input.h"
#include "MessageBox.h"

	//BATTLE_ENEMY *pBattleEnemy = GetBattleEnemy();
	//BATTLE_T_MEMBER *pMember = GetTMember();
	//BATTLE_PLAYER *pBattlePlayer = GetBattlePlayer();
	//FIELD *pField = GetField();
ENEMY_SKILL_RUNNING	g_skillRuning;
#define SKILL_MEESAGE_TIME (90)
#define SKILL_MESSAGE_SIZE_X		(200)
#define SKILL_MESSAGE_SIZE_Y		(40)
void InitSkillRining(void);
//**************************************************************************
//初期化処理
//**************************************************************************
HRESULT	InitBattle_Enemy_Skill(void)
{

	InitSkillRining();
		return S_OK;
}
void InitSkillRining(void)
{
	g_skillRuning.bUse		= false;
	g_skillRuning.EnemyID	= SKILL_MEESAGE_TIME;
	g_skillRuning.nTime		= SKILL_MEESAGE_TIME;
	g_skillRuning.pos		= D3DXVECTOR3(0.0f,0.0f,0.0f);
	g_skillRuning.MessageBoxID	= 99;
	g_skillRuning.MessageBoxID	= MESSAGE_WORDS_MAX;
}
//**************************************************************************
//更新処理
//**************************************************************************
void UpdateBattle_Enemy_Skill(void)
{
	if(g_skillRuning.bUse == true)
	{
		if(g_skillRuning.nTime == SKILL_MEESAGE_TIME)
			g_skillRuning.MessageBoxID = OpeBoxIDBox(
			g_skillRuning.pos,
			SKILL_MESSAGE_SIZE_X,
			SKILL_MESSAGE_SIZE_Y,
			g_skillRuning.wordsID);
		g_skillRuning.nTime --;
		if(g_skillRuning.nTime < 0)
		{
			CloseMessageBox(g_skillRuning.MessageBoxID);
			RuningEnemy_SKill(g_skillRuning.SkillName,g_skillRuning.EnemyID);
			InitSkillRining();
		}
	}

	
}

//**************************************************************************
//敵スキル発動条件チェック処理
//**************************************************************************
bool Enemy_Skill_Check_Multi(int EnemyID)
{
	BATTLE_ENEMY *pBattleEnemy = GetBattleEnemy();
	bool Res = false;
	for( int nSkill = 0; nSkill < SKILL_MAX_IN_MONSTER;nSkill++)
	{
		ENEMY_SKILL_CONDITION Condition = pBattleEnemy[EnemyID].skill[nSkill].SkillCondition;
		if(pBattleEnemy[EnemyID].skill[nSkill].UseTime > 0)
		switch(Condition)
		{
			case ENEMY_SKILL_CONDITION_HP_UNDER_30_EVERY_TURN:
				if(pBattleEnemy[EnemyID].skill[nSkill].UseTime > 0)
				{
					pBattleEnemy[EnemyID].skill[nSkill].bActuate = 
						Enemy_Skill_Check_HP_Under_30( pBattleEnemy[EnemyID].fRecHP, pBattleEnemy[EnemyID].fMaxHP);
				}
				break;
			case ENEMY_SKILL_CONDITION_HP_FULL:
				if(pBattleEnemy[EnemyID].skill[nSkill].UseTime > 0)
				{
					pBattleEnemy[EnemyID].skill[nSkill].bActuate = 
						Enemy_Skill_Check_HP_FULL( pBattleEnemy[EnemyID].fRecHP, pBattleEnemy[EnemyID].fMaxHP);
				}
				break;
			case ENEMY_SKILL_CONDITION_MAX:
				pBattleEnemy[EnemyID].skill[nSkill].bActuate = false;
				break;
		}
	}//メンバーが所有しているスキルを全部走査する
	for( int nSkill = 0; nSkill < SKILL_MAX_IN_MONSTER;nSkill++)
	{
		if(pBattleEnemy[EnemyID].skill[nSkill].bActuate == true)
		{
			Res = true;
			break;
		}
	}
	return Res;
}

//**************************************************************************
//敵スキル実行関数
//**************************************************************************
void UseEnemy_Skill(ENEMY_SKILL_NAME name,int EnemyID,D3DXVECTOR3 pos,MESSAGE_WORDS words)
{
	g_skillRuning.bUse		= true;
	g_skillRuning.EnemyID	= EnemyID;
	g_skillRuning.pos		= pos;
	g_skillRuning.SkillName	= name;
	g_skillRuning.wordsID	= words;
}
//*******************************************
//敵スキル間接関数
//*******************************************
void RuningEnemy_SKill(ENEMY_SKILL_NAME name,int EnemyID)
{
	BATTLE_ENEMY *pBattleEnemy = GetBattleEnemy();
	switch(name)
	{
	case ENEMY_SKILL_NAME_SUPER_10_PUNCH_500:
		Enemy_Skill_Super_10_Punch_500(EnemyID);
		break;
	case ENEMY_SKILL_NAME_GRAVITY_99P_DAMAGE:
		Enemy_Skill_Gravity_99HP(EnemyID);
		break;

	case ENEMY_SKILL_NAME_MAX:
		break;
	}

}
//*********************************************
//敵スキルチェック本体
//*********************************************
bool Enemy_Skill_Check_HP_Under_30(int RecHP,int MAX_HP)
{
	
	if((int)RecHP <= (int)MAX_HP * 0.3f)
		return true;
	else
		return false;
}
bool Enemy_Skill_Check_HP_FULL(int RecHP,int MAX_HP)
{
	if((int)RecHP == (int)MAX_HP )
		return true;
	else
		return false;
}

//**************************************************************************
//敵スキル本体
//**************************************************************************
void Enemy_Skill_Super_10_Punch_500(int EnemyNumber)
{
	BATTLE_ENEMY *pBattleEnemy = GetBattleEnemy();
	for(int nAttack = 0; nAttack < 10; nAttack++)
	SetBattle_Attack_Single(TEAM_ENEMY,EnemyNumber,0,TEAM_PLAYER_MAX,500,20 + nAttack * 5);

}
void Enemy_Skill_Gravity_99HP(int EnemyNumber)
{
	BATTLE_T_MEMBER *pMember = GetTMember();
	BATTLE_PLAYER *pBattlePlayer = GetBattlePlayer();
	
	int Value = (int)pBattlePlayer->fRecHP * 0.01f;
	if(Value < 1)
		Value = 1;
	int Damage = (int)pBattlePlayer->fRecHP - Value;
	SetBattle_Attack_Single(TEAM_ENEMY,EnemyNumber,0,TEAM_PLAYER_MAX,pBattlePlayer->fRecHP - Value,20 + 10 * 5);
}

