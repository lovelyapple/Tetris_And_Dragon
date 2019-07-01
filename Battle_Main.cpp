//=============================================================================
// テトリスのメイン処理[group_in_queue.cpp]
//
//
//=============================================================================
#include "Battle_Main.h"
#include "Data_Floor.h"
#include "Data_Stage.h"
#include "main.h"
#include "game.h"
#include "Battle_Attack.h"
#include "Battle_Enemy.h"
#include "Battle_Player.h"
#include "input.h"
#include "TimeLeftCage.h"
//#include "fade.h"
#include "Func_NumberX.h"
#include "Battle_Row_Charge.h"
#include "Battle_Member_Damage_Count.h"
#include "Fade_loading.h"
#include "sound.h"
#include "Battle_Boss_Alert_Effect.h"
#include "Battle_Skill_Function.h"
#include "Battle_Enemy_Skill.h"
#include "Battle_Stage_End.h"
#include "Sys_Record.h"
#include "Battle_Record_Time.h"
#include <math.h>


#define ATTACK_INVERV (10) //攻撃時間間隔
#define VALUE_JUMP_LIMIT	(5);
#define BATTLE_MARK_SPEED	(10.0f)
#define TEXTRUE_MARK_STAGE	("data/TEXTURE/MarkStage.png")
#define TEXTURE_MARK_SIZE_X		(150.0f)
#define TEXTURE_MARK_SIZE_Y		(50.0f)
#define TIME_FLOOR_CHANGE		(180)
#define TEXTURE_MARK_UNIT		(1.0f / 5)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexBattle_Main_BG(LPDIRECT3DDEVICE9 pDevice);//頂点作成
HRESULT MakeVertexBattle_Effect_Flash(LPDIRECT3DDEVICE9 pDevice);//頂点作成
HRESULT MakeVertexBattle_MarkStage(LPDIRECT3DDEVICE9 pDevice);//頂点作成
void SetVerTexBattle_MarkStage(void);
void SetVerTexBattle_BG(void);
void InitBattleBG(void);
void MemeberDamageAnalys(void);
void SetTextureBattle_Effect_Flash(void);
void InitRecFloor(void);
void InitRecStage(void);
void ResetBG01(void);
void ResetMarkStage(void);
typedef struct
{
	bool	bUse;		// 使われているか
	int		nType;		// タイプ
	float	falpha;		// 透明度
	float	SizeX;		// 大きさX
	float	SizeY;		// 大きさY
	D3DXVECTOR3	DrawPos;	// 描画位置
	float	fSinValue;	//sin value
}BATTLE_BG;
typedef struct
{
	bool bUse;
	D3DXVECTOR3	pos;
	float fmove;
	int	Life;
}MARK_STAGE;

const char *c_aFileNameBattleBG[BATTLE_BG_MAX]=
{
	"data/TEXTURE/BattleBG_Grass.jpg",
};

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureBattle_Main_BG[BATTLE_BG_MAX]	= {};//テクスチャメモリ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBattle_Main_BG		= NULL;//頂点座標バッファ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBattle_Effect_Flash	= NULL;//頂点座標バッファ
LPDIRECT3DTEXTURE9		g_pTextureBattle_MarkStage		= NULL;//テクスチャメモリ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBattle_MarkStage		= NULL;//頂点座標バッファ
BATTLE_BG	g_BattleBG[MAX_BATTLE_BG];
int		g_AttackTime;
BATTLE_STATS	g_BattleStats;		//バトルの進行状況を保存するワーク
BATTLE_STATS	g_BattleStatsBuff;	//バトルの進行状況を保存するワークbefore
bool	g_bBattleATK;		//攻撃可能か
int		g_AttackQ;			//攻撃順番
int		g_BatteTarget;		//攻撃のターゲット
bool	g_EnemyTurn;		//敵のターンが0になったかどうか

FLOOR_DATA g_RecFloor;		//現在のフロアーを保存するワーク
int		g_EnemyLeft;		//敵の残機
int		g_RecFloorID;		//現在のフロアーＩＤ
STAGE_DATA g_RecStage;		//現在のステージを保存するワーク
int		g_FloorLeft;		//フロアの残り
int		g_RecStageID;		//現在のステージＩＤ
int		g_FloorIdx;			//現在ステージフロア

int		g_EnemyInFloor;

int		nFlash_Type;		//フラッシュ状態
float	fFlash_Aphla;		//フラッシュのα値
int		nFlashTime;			//フラッシュする時間

MARK_STAGE g_Mark;			//マークの変数

int		Player_SkillTime;		//プレイヤースキルの所要時間
int		SkillActiveMemberID;	//どのメンバーがスキルを使うのか

int		Enemy_SkillTime;		//敵スキルの所要時間
bool	Enmey_SkillCheck;		//敵のスキルが発動するのか?


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBattle_Main(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	InitBattleBG();
	for(int nCntBG = 0; nCntBG < MAX_BATTLE_BG; nCntBG++)
	D3DXCreateTextureFromFile(pDevice,
		c_aFileNameBattleBG[nCntBG],
		&g_pTextureBattle_Main_BG[nCntBG]);

	D3DXCreateTextureFromFile(pDevice,
		TEXTRUE_MARK_STAGE,
		&g_pTextureBattle_MarkStage);

	MakeVertexBattle_Main_BG(pDevice);
	MakeVertexBattle_Effect_Flash(pDevice);
	MakeVertexBattle_MarkStage(pDevice);

	g_AttackTime = 0;
	g_BattleStats = BATTLE_OPENING;
	g_BattleStatsBuff = g_BattleStats;
	g_bBattleATK	= false;
	g_AttackQ	= 0;
	g_BatteTarget = 0;
	g_EnemyTurn = false;
	g_EnemyInFloor = 3;
	g_EnemyLeft = g_EnemyInFloor;
	g_FloorIdx  = 0;
	InitRecFloor();
	InitRecStage();
	ResetMarkStage();
	Enemy_SkillTime = 0;
	nFlash_Type = FLASH_NOTH;
	fFlash_Aphla  = 0.0f;
	nFlashTime = 0;

	g_RecFloorID = 0;
	g_RecStageID = 0;
	SkillActiveMemberID	=0;
	Enmey_SkillCheck = false;
	return S_OK;
}
void InitBattleBG(void)
{
	for(int nCntBG = 0; nCntBG < MAX_BATTLE_BG; nCntBG++)
	{
		g_BattleBG[nCntBG].bUse = false;
		g_BattleBG[nCntBG].DrawPos = D3DXVECTOR3(BATTLE_BG_POS_X,0.0f,0.0f);
		g_BattleBG[nCntBG].falpha  = 0.0f;
		g_BattleBG[nCntBG].nType   = BATTLE_BG_GRASS;
		g_BattleBG[nCntBG].SizeX	= 0.0f;
		g_BattleBG[nCntBG].SizeX	= 0.0f;
		g_BattleBG[nCntBG].fSinValue = 0.0f;
	}
}

void InitRecFloor(void)
{
	g_RecFloor.MonsterCount = 3;
	for(int nMonster = 0; nMonster<MAX_MONSTER_IN_FLOOR;nMonster++)
	g_RecFloor.MonsterID[nMonster]= 0;
}
void InitRecStage(void)
{
	g_RecStage.FloorCount = 3;
	for(int nFloor = 0; nFloor<MAX_MONSTER_IN_FLOOR;nFloor++)
	g_RecStage.FloorID[nFloor]= 0;

}
void ResetBG01(void)
{
	g_BattleBG[1].bUse = true;
	g_BattleBG[1].DrawPos.x = SYS_BATTLE_POS_START_X + SYS_BATTLE_SIZE_X /2;
	g_BattleBG[1].DrawPos.y = SYS_BATTLE_BATTLE_SIZE_Y / 2;
	g_BattleBG[1].falpha	= 1.0f;
	g_BattleBG[1].SizeX		= SYS_BATTLE_SIZE_X/2;
	g_BattleBG[1].SizeY		= SYS_BATTLE_BATTLE_SIZE_Y / 4;
	g_BattleBG[1].fSinValue	= 1.0f;
}
void ResetMarkStage(void)
{
	g_Mark.bUse = false;
	g_Mark.pos	= D3DXVECTOR3(SYS_BATTLE_POS_START_X,SYS_BATTLE_BATTLE_SIZE_Y*0.4f,0.0f);
	g_Mark.fmove	= SYS_BATTLE_SIZE_X/2;
	g_Mark.Life		= 0;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBattle_Main(void)
{

	//いろいろ変数追加するよ
	int pTetrisStats = GetGameStats();
	int SumAttackTime = GetSUM_Attack_Time();
	int PlayerHP	= GetBattle_playerHP();
	bool AttackcCheck = false;
	DAMAGE_STRUCT *pDamageStruct;
	pDamageStruct = GetDamageStruct();
	BATTLE_T_MEMBER *pMember = GetTMember();
	BATTLE_PLAYER	*pBattlePlayer = GetBattlePlayer();
	BATTLE_ENEMY	*pEnemy = GetBattleEnemy();
	MOVETIME *pMoveTime = GetMoveTimeLeft();
	ALERT_PHASE AlertPhase = GetBOSS_Alert_Phase();
	g_AttackTime++;
	g_AttackTime = (int)SetMAX((float)g_AttackTime,1000.0f,-9.0f,1);
	
	if(pEnemy[g_BatteTarget].bUse == 0)
		g_BatteTarget++;
	if(g_BatteTarget > 2)
		g_BatteTarget =0;

	//ここからは本番だよ
	switch ( g_BattleStats)
	{
	case BATTLE_WALKING:
		SetTetrisInput(false);
			g_BattleBG[1].falpha	-= 1.0f/TIME_FLOOR_CHANGE;
			g_BattleBG[1].SizeX		+= (BATTLE_BG_SIZE_X / 2 * 1.2f)/TIME_FLOOR_CHANGE;
			g_BattleBG[1].SizeY		+= (BATTLE_BG_SIZE_Y / 2 * 1.2f)/TIME_FLOOR_CHANGE;
			g_BattleBG[1].fSinValue	+= (D3DX_PI * 0.5f)/ (TIME_FLOOR_CHANGE / 8);//0.5PI 90sec 4times
			g_BattleBG[1].DrawPos.y -= sinf(g_BattleBG[1].fSinValue)*VALUE_JUMP_LIMIT;
			if(g_BattleBG[1].SizeX >= SYS_BATTLE_SIZE_X/2)
			{
				g_BattleBG[1].SizeX = SYS_BATTLE_SIZE_X/2;
				g_BattleBG[1].SizeY = SYS_BATTLE_SIZE_X/2;
			}


			g_Mark.bUse = true;
			g_Mark.Life ++;
			if(g_Mark.Life < TIME_FLOOR_CHANGE/2)
			{
				g_Mark.fmove *= 0.9f;
				g_Mark.pos.x = SYS_BATTLE_POS_START_X + SYS_BATTLE_SIZE_X/2 + g_Mark.fmove;
			}
			if(g_Mark.Life >= TIME_FLOOR_CHANGE/2)
			{
				g_Mark.fmove *= 1.2f;
				g_Mark.pos.x = SYS_BATTLE_POS_START_X + SYS_BATTLE_SIZE_X/2 - g_Mark.fmove;
			}
			

			g_RecFloor = GetFloor_Data(g_RecFloorID);
			
			if(g_BattleBG[1].falpha <= 0.1f)
			{
				switch(g_BattleStatsBuff)
				{
				case BATTLE_OPENING:
					 g_BattleStats = BATTLE_LOADING_FLOOR;
					 SetSysRecordMode(SYS_RECORD_START);
					break;
				case BATTLE_NEXT:
					g_BattleStats = BATTLE_LOADING_FLOOR;
					//if(g_RecFloor.Boss == true)
					//{
					//	g_BattleStats = BATTLE_SET_BOSS_EFFECT_START;
					//	StopSound();
					//}
					//else
					// g_BattleStats = BATTLE_SET_ENEMY;
					break;
				}
				g_BattleBG[1].bUse = true;
				g_BattleBG[1].DrawPos.x = SYS_BATTLE_POS_START_X + SYS_BATTLE_SIZE_X /2;
				g_BattleBG[1].DrawPos.y = SYS_BATTLE_BATTLE_SIZE_Y / 2;
				g_BattleBG[1].falpha	= 0.0f;
				g_BattleBG[1].SizeX		= SYS_BATTLE_SIZE_X / 2;
				g_BattleBG[1].SizeY		= SYS_BATTLE_BATTLE_SIZE_Y / 4;
				ResetMarkStage();
			}

		break;
	case BATTLE_OPENING:
		SetTetrisInput(false);
		//BGの初期設定
			g_BattleBG[0].bUse = true;
			g_BattleBG[0].DrawPos.x = BATTLE_BG_POS_X;
			g_BattleBG[0].DrawPos.y = BATTLE_BG_POS_Y;
			g_BattleBG[0].falpha	= 1.0f;
			g_BattleBG[0].SizeX		= BATTLE_BG_SIZE_X;
			g_BattleBG[0].SizeY		= BATTLE_BG_SIZE_Y;
			ResetBG01();


		g_FloorIdx = 0;
		g_RecStage = GetStage_Data(g_RecStageID);
		g_BattleStatsBuff = g_BattleStats;
		g_BattleStats = BATTLE_WALKING;

		break;
	case BATTLE_LOADING_FLOOR:
		SetTetrisInput(false);
		g_RecFloorID = g_RecStage.FloorID[g_FloorIdx];
		g_RecFloor = GetFloor_Data(g_RecFloorID);
		if(g_RecFloor.Boss == true)
		{
			g_BattleStats = BATTLE_SET_BOSS_EFFECT_START;
			StopSound();
		}
		else
		 g_BattleStats = BATTLE_SET_ENEMY;
		//g_BattleStats = BATTLE_SET_ENEMY;
		break;

	case BATTLE_SET_BOSS_EFFECT_START:
		SetTetrisInput(false);
		PlaySound(SOUND_LABEL_SE_BOSS_ALERT);
		SetBossAlertStart();
		g_BattleStats = BATTLE_SET_BOSS_EFFECT_WAIT;
		break;
	case BATTLE_SET_BOSS_EFFECT_WAIT:
		SetTetrisInput(false);
		if(AlertPhase == ALERT_PHASE_MAX)
		g_BattleStats = BATTLE_SET_BOSS_EFFECT_END;
			break;
	case BATTLE_SET_BOSS_EFFECT_END:
		SetTetrisInput(false);
		PlaySound(SOUND_LABEL_BGM_BOSS);
		g_BattleStats = BATTLE_SET_ENEMY;
		break;

	case BATTLE_SET_ENEMY:
		SetTetrisInput(false);
		g_RecFloor = GetFloor_Data(g_RecFloorID);
		g_EnemyInFloor = g_RecFloor.MonsterCount;
		g_EnemyLeft = g_EnemyInFloor;
		for(int nEnemy = 0; nEnemy < g_EnemyInFloor;nEnemy++)
		{
			SetUpBattleEnemy(nEnemy,nEnemy,g_RecFloor.MonsterID[nEnemy],g_RecFloor.Boss,g_EnemyInFloor);
		}
		g_BattleStats = BATTLE_FLOOR_START;
		break;
	case BATTLE_FLOOR_START:
		SetTetrisInput(false);
		SetTerisMainStats(TETRIS_START);//STARTから自動WAIT,STOPまで移行できる
		g_BattleStats = BATTLE_WAIT_TIME_CAGE;
		break;
	case BATTLE_WAIT_TIME_CAGE:
		SetTetrisInput(false);
		if(pMoveTime->nType == TIME_MOVE_FULL)
		{
			SetTetrisFieldMove(FIELD_MOVE_IN);
			g_BattleStats = BATTLE_WAIT_FIELD_MOVE_IN_PLAYER;//もし準備オッケならまだ移行
			SetTerisMainStats(TETRIS_STOP);
		}
		break;
	case BATTLE_WAIT_FIELD_MOVE_IN_PLAYER:
		SetTetrisInput(false);
		if(GetFieldMoveType() == FIELD_MOVE_KEEP)
			g_BattleStats = BATTLE_WAIT_SKILL_OR_TETRIS_PLAYER;
		break;
	case BATTLE_WAIT_SKILL_OR_TETRIS_PLAYER:
		SetTetrisInput(false);
		if(GetKeyboardTrigger(DIK_1))
		{
			if(pMember[0].bSkillUse == true)
			{
				SetSkillShining(pMember[0].pos);
			//	UseSkill(pMember[0].Skill.SkillName);
				g_BattleStats = BATTLE_WAIT_SKILL_START;
				Player_SkillTime	= 40;
				SkillActiveMemberID = 0;
				SetBattle_PlayerMP_Recover(MP_TYPE_LOSING,pMember[0].Skill.MPneed);
				pMember[0].TurnLeft = pMember[0].Skill.MPneed;
				PlaySound(SOUND_LABEL_SE_BATTLE_PLAYER_USE_SKILL);
			}
		}
		else if(GetKeyboardTrigger(DIK_2))
		{
			if(pMember[1].bSkillUse == true)
			{
			//	UseSkill(pMember[1].Skill.SkillName);
				SetSkillShining(pMember[1].pos);
				g_BattleStats = BATTLE_WAIT_SKILL_START;
				Player_SkillTime	= 40;
				SkillActiveMemberID = 1;
				SetBattle_PlayerMP_Recover(MP_TYPE_LOSING,pMember[1].Skill.MPneed);
				pMember[1].TurnLeft = pMember[1].Skill.MPneed;
				PlaySound(SOUND_LABEL_SE_BATTLE_PLAYER_USE_SKILL);
			}
		}
		else if(GetKeyboardTrigger(DIK_3))
		{
			if(pMember[2].bSkillUse == true)
			{
			//	UseSkill(pMember[2].Skill.SkillName);
				SetSkillShining(pMember[2].pos);
				g_BattleStats = BATTLE_WAIT_SKILL_START;
				Player_SkillTime	= 40;
				SkillActiveMemberID = 2;
				SetBattle_PlayerMP_Recover(MP_TYPE_LOSING,pMember[2].Skill.MPneed);
				pMember[2].TurnLeft = pMember[2].Skill.MPneed;
				PlaySound(SOUND_LABEL_SE_BATTLE_PLAYER_USE_SKILL);
			}
		}
		else if(GetKeyboardTrigger(DIK_4))
		{
			if(pMember[3].bSkillUse == true)
			{
			//	UseSkill(pMember[3].Skill.SkillName);
				SetSkillShining(pMember[3].pos);
				g_BattleStats = BATTLE_WAIT_SKILL_START;
				Player_SkillTime	= 40;
				SkillActiveMemberID = 3;
				SetBattle_PlayerMP_Recover(MP_TYPE_LOSING,pMember[3].Skill.MPneed);
				pMember[3].TurnLeft = pMember[3].Skill.MPneed;
				PlaySound(SOUND_LABEL_SE_BATTLE_PLAYER_USE_SKILL);
			}
		}
		else if(GetKeyboardTrigger(DIK_5))
		{
			if(pMember[4].bSkillUse == true)
			{
				//UseSkill(pMember[4].Skill.SkillName);
				SetSkillShining(pMember[4].pos);
				g_BattleStats = BATTLE_WAIT_SKILL_START;
				Player_SkillTime	= 40;
				SkillActiveMemberID = 4;
				SetBattle_PlayerMP_Recover(MP_TYPE_LOSING,pMember[4].Skill.MPneed);
				pMember[4].TurnLeft = pMember[4].Skill.MPneed;
				PlaySound(SOUND_LABEL_SE_BATTLE_PLAYER_USE_SKILL);
			}
		}

		else if(GetKeyboardTrigger(KEY_START))
		{
			g_BattleStats = BATTLE_TETRIS;
			SetTetrisInput(true);
		}

		break;
	case BATTLE_WAIT_SKILL_START:
		Player_SkillTime--;
		if(Player_SkillTime < 0)
		{
			UseSkill(pMember[SkillActiveMemberID].Skill.SkillName);
			Player_SkillTime	= 200;
			g_BattleStats = BATTLE_WAIT_SKILL_RUNNING;
		}
		break;
	case BATTLE_WAIT_SKILL_RUNNING:
		SetTetrisInput(false);
		Player_SkillTime --;
		if(Player_SkillTime < 0)
			g_BattleStats = BATTLE_WAIT_SKILL_RESULT;
		break;
	case BATTLE_WAIT_SKILL_RESULT:
		SetTetrisInput(false);
		g_EnemyLeft = g_EnemyInFloor;
		for(int nEnemy = 0;nEnemy < g_EnemyInFloor;nEnemy++)
		{
			if(pEnemy[nEnemy].bUse == false)
			{
				g_EnemyLeft --;
				if(g_EnemyLeft == 0)
				{
					g_BattleStats = BATTLE_NEXT;
					break;
				}
			}
			else
			{
				g_BattleStats = BATTLE_WAIT_SKILL_OR_TETRIS_PLAYER;
				break;
			}
		}
		break;
	case BATTLE_TETRIS:
		if(pTetrisStats == TETRIS_WAIT_TIME)//このターンが終了した時
		{

			g_BattleStats = BATTLE_ROW_CHAGE_CHECK;

			//bool bRecover = RecoverCheck();
			//if(bRecover == true)
			//{
			//	SetBattle_PlayerHP_Change(HP_RECOVER,
			//		pBattlePlayer->HP_RecoverResult * -1);
			//	g_BattleStats = BATTLE_WAIT_PLAYER_RECOVER;
			//}
			//else
			//{
			//	g_bBattleATK = DamageCheck();
			//	if(g_bBattleATK == true)
			//	{
			//		g_BattleStats = BATTLE_PLAYER_ATTACK;
			//		g_AttackQ = 0;
			//	}
			//	else
			//	{
			//		g_BattleStats = BATTLE_ENEMY_TURN_CHECK;
			//		g_AttackQ = 0;
			//	}
			//}
		}//pBattlePlayer
		break;
	case BATTLE_ROW_CHAGE_CHECK:
		SetTetrisInput(false);
			if(GetRowChargeValue() > 0.0f)//1行以上消した場合,コンボボーナスに入る
			{

				g_BattleStats = BATTLE_WAIT_FIELD_MOVE_OUT_PLAYER;
				SetTetrisFieldMove(FIELD_MOVE_OUT);
			}
			else//そうでなければ,コンボケージが減り,ふつうの攻撃シーンに移行
			{
				SetRowCharge(0,ROW_CHARGE_RESET);
				g_BattleStats = BATTLE_WAIT_ROW_CHARGE;
			}
		break;
	case BATTLE_WAIT_FIELD_MOVE_OUT_PLAYER:
		SetTetrisInput(false);
		if(GetFieldMoveType() == FIELD_MOVE_KEEP)
			g_BattleStats = BATTLE_SET_ROW_CHARGE;

		break;
	case BATTLE_SET_ROW_CHARGE:
		SetTetrisInput(false);
		//SetComboDamageTypeROW_CHARGE_MAX_VALUE
		for(int nMember = 0; nMember < TEAM_PLAYER_MAX; nMember ++)
		{
			if(pMember[nMember].DamageBase == 0)
				continue;

				float nCombo = GetRowChargeValue() / ROW_MAX_ROW;
				int Damage = pMember[nMember].DamageBase;
				Damage *= nCombo;
				int Value = Damage; //- pMember[nMember].DamageBase;
				int Time = Value * ROW_VALUE_RESET + 60;//現在ケージにある量が0まで減るの時間と,数字変化時間バッファ
				if(Time > 60 * 5)
					Time = 60 * 5;
				SetComboDamageType(nMember,Value,COMBO_DAMAGE_UP,Time);
				pMember[nMember].DamageBase += Damage;
	
		}
		SetRowCharge(0,ROW_CHARGE_RESET);
		g_BattleStats = BATTLE_WAIT_ROW_CHARGE;
		break;
	case BATTLE_WAIT_ROW_CHARGE:
		SetTetrisInput(false);
		if(GetRowChargeType() == ROW_CHARGE_MAX_TYPE)
		{
			bool bRecover = RecoverCheck();
			if(bRecover == true)
			{
				SetBattle_PlayerHP_Change(HP_RECOVER,
					pBattlePlayer->HP_RecoverResult * -1);
				g_BattleStats = BATTLE_WAIT_PLAYER_RECOVER;
			}
			else
			{
				g_bBattleATK = DamageCheck();
				if(g_bBattleATK == true)
				{
					g_BattleStats = BATTLE_PLAYER_ATTACK;
					g_AttackQ = 0;
				}
				else
				{
					g_BattleStats = BATTLE_ENEMY_TURN_CHECK;
					g_AttackQ = 0;
				}
			}
		}
		break;
	case BATTLE_WAIT_PLAYER_RECOVER:
		SetTetrisInput(false);
		if(pBattlePlayer->HP_Type == HP_KEEPING && GetComboDamageKeep() == true && GetBattle_Player_MP_Type() == MP_TYPE_KEEPING)//オーバー切る機能がついていないからこれを作るべき
		{
			g_bBattleATK = DamageCheck();
			if(g_bBattleATK == true)
			{
				g_BattleStats = BATTLE_PLAYER_ATTACK;
				SetTetrisFieldMove(FIELD_MOVE_OUT);
				g_AttackQ = 0;
			}
			else
			{
				g_BattleStats = BATTLE_PLAYER_ATTACK;
				g_AttackQ = 0;
			}
		}

		break;

	case BATTLE_PLAYER_ATTACK://シーンの移行チェック
		SetTetrisInput(false);
		if(g_AttackTime % 10 == 0)
		{
			if(g_AttackQ > 4)
			{
				g_AttackQ = 0;
				ResetBattleMemberBaseDamage();//メンバーごとの基本ダメージを0にする
				pBattlePlayer->HP_RecoverResult = 0;//回復の最終値を空にする
				InitBattleDamage();
				g_BattleStats = BATTLE_WAIT_PLAYER_RUNING;
			}
			else
			{
				if(pMember[g_AttackQ].DamageBase != 0 )
				{
					SetBattleAttack_Player_Multi(g_AttackQ,pMember[g_AttackQ].nTarget);
					SetComboDamageType(g_AttackQ,0,COMBO_DAMAGE_REDUCE,COMBO_DAMAGE_TIME);//メンバーごとのダメージ表示を消す
				}
			}
			g_AttackQ ++;//次のメンバーに進む
		}
		break;
	case BATTLE_WAIT_PLAYER_RUNING://プレイヤーの攻撃ターン
		SetTetrisInput(false);
		if(SumAttackTime == 0)
		{
			for(int nMember = 0; nMember < TEAM_PLAYER_MAX; nMember ++)
			{
				pMember[nMember].TurnLeft --;
			}//冷却ターンが減る

		
			g_EnemyLeft = g_EnemyInFloor;
			for(int nEnemy = 0;nEnemy < g_EnemyInFloor;nEnemy++)
			{
				if(pEnemy[nEnemy].bUse == false)
				{
					g_EnemyLeft --;
					if(g_EnemyLeft == 0)
					{
						g_BattleStats = BATTLE_NEXT;
						break;
					}
				}
				else
				{
					g_BattleStats = BATTLE_ENEMY_TURN_CHECK;
					break;
				}
			}
			
		}
		break;
	case BATTLE_NEXT:
		SetTetrisInput(false);
		g_FloorIdx ++;

		ResetBG01();

		if(g_FloorIdx > g_RecStage.FloorCount-1)
			g_BattleStats = BATTLE_CLEAR;
		else
		{
			g_BattleStatsBuff = g_BattleStats;
			g_BattleStats = BATTLE_WALKING;
		}
			break;


	case BATTLE_ENEMY_TURN_CHECK://敵の攻撃チェック
		SetTetrisInput(false);
		if(SumAttackTime == 0)
		g_EnemyTurn = false;
		for(int nEnemy = 0; nEnemy < g_EnemyInFloor;nEnemy++)
		{
			pEnemy[nEnemy].turn --;
			if(pEnemy[nEnemy].turn <= 0)
			{
				g_EnemyTurn = true;
				g_AttackQ = 0;
			}
				
		}
		if(g_EnemyTurn == true)
		{
			g_BattleStats = BATTLE_WAIT_FIELD_MOVE_OUT_ENEMY;
			SetTetrisFieldMove(FIELD_MOVE_OUT);
		}
		else
			g_BattleStats = BATTLE_WAIT_TIME_CAGE;
		break;

	case BATTLE_WAIT_FIELD_MOVE_OUT_ENEMY:
		SetTetrisInput(false);
		if(GetFieldMoveType() == FIELD_MOVE_KEEP)
			g_BattleStats = BATTLE_ENEMY_SKILL_CHECK;
		break;

	case BATTLE_ENEMY_SKILL_CHECK:
		Enmey_SkillCheck = false;
		for(int nEnemy = 0;nEnemy < g_EnemyInFloor;nEnemy++)
		{
			if(pEnemy[nEnemy].bUse == false && pEnemy[g_AttackQ].turn > 0)
				continue;
			if(Enemy_Skill_Check_Multi(nEnemy)== true)
			{
				Enmey_SkillCheck = true;
			}
		}

		if(Enmey_SkillCheck == true)
			g_BattleStats = BATTLE_ENEMY_SKILL_ACTIVE;
		else
			g_BattleStats = BATTLE_ENEMY_ATTACK;
		
		break;

	case BATTLE_ENEMY_SKILL_ACTIVE:
		for(int nEnemy = 0;nEnemy < g_EnemyInFloor;nEnemy++)
		{
			bool SkillCheckSingle = false;
			for( int nSkill = 0; nSkill < SKILL_MAX_IN_MONSTER;nSkill++)
				if(pEnemy[nEnemy].skill[nSkill].bActuate == true)
				{
					D3DXVECTOR3 pos = D3DXVECTOR3(SYS_BATTLE_POS_START_X + SYS_BATTLE_SIZE_X/2,100,0);
					PlaySound(SOUND_LABEL_SE_ENEMY_SKILL_USE);
					UseEnemy_Skill( pEnemy[nEnemy].skill[nSkill].SkillName,nEnemy,pos,pEnemy[nEnemy].words[nSkill]);
					//RuningEnemy_SKill( pEnemy[nEnemy].skill[nSkill].SkillName,nEnemy);
					Enemy_SkillTime = 70;
					pEnemy[nEnemy].skill[nSkill].bActuate = false;
					pEnemy[g_AttackQ].turn = pEnemy[g_AttackQ].turnMAX;
					pEnemy[nEnemy].skill[nSkill].UseTime--;
					SkillCheckSingle = true;
					break;//スキルループから抜く
				}
				if(SkillCheckSingle == true)
				{
					g_BattleStats = BATTLE_WAIT_ENEMY_SKILL;
					break;//敵のループから抜く
				}
			//もし最後まで走査したら
			if(nEnemy == g_EnemyInFloor -1)
				g_BattleStats = BATTLE_ENEMY_SKILL_ACTIVE;
		}
		break;

	case BATTLE_WAIT_ENEMY_SKILL:
		Enemy_SkillTime -- ;
		if(Enemy_SkillTime < 0)
		{
			if(pBattlePlayer->fRecHP <= 0)
				g_BattleStats = BATTLE_LOSE;
			else
			g_BattleStats = BATTLE_WAIT_TIME_CAGE;
		}

		break;

	case BATTLE_ENEMY_ATTACK://敵の攻撃ターン
		SetTetrisInput(false);
		if(g_AttackTime % 10 == 0)
		{
			if(g_AttackQ > 3)
			{
				g_AttackQ = 0;
				SetTerisMainStats(TETRIS_WAIT_TIME);
				g_BattleStats = BATTLE_WAIT_ENEMY;
			}
			else
			{
				if(pEnemy[g_AttackQ].bUse == true&&pEnemy[g_AttackQ].turn == 0)
				{
					SetBattle_Attack_Single(TEAM_ENEMY,g_AttackQ,0,TEAM_PLAYER_MAX,pEnemy[g_AttackQ].fAtk,0);
					pEnemy[g_AttackQ].turn = pEnemy[g_AttackQ].turnMAX;
				}
			}
			g_AttackQ ++;
		}
		break;
	case BATTLE_WAIT_ENEMY:
		SetTetrisInput(false);
		if(SumAttackTime == 0)
		{
			if(PlayerHP == 0)
			g_BattleStats = BATTLE_LOSE;
			else
			g_BattleStats = BATTLE_WAIT_TIME_CAGE;	
		}
		break;
	case BATTLE_LOSE:
		SetSysRecordMode(SYS_RECORD_STOP);
		SetTetrisInput(false);
		SetStageResult(STAGE_RESULT_LOST);
		if(GetKeyboardTrigger(DIK_RETURN))
		SetLoadStart(MODE_RESULT);
		break;
	case BATTLE_CLEAR:
		SetSysRecordMode(SYS_RECORD_STOP);
		SaveRecord();
		SetTetrisInput(false);
		SetStageResult(STAGE_RESULT_WIN);
		if(GetKeyboardTrigger(DIK_RETURN))
		SetLoadStart(MODE_RESULT);
		break;
	}
	 SetTextureBattle_Effect_Flash();
	 SetVerTexBattle_BG();
	 SetVerTexBattle_MarkStage();
}
//=============================================================================
// 
//=============================================================================
void SetBattleStats(BATTLE_STATS Type)
{
	g_BattleStats = Type;
}
int	 GetBattleStats(void)
{
	return g_BattleStats;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBattle_Main(void)
{
	for(int nCntBG = 0; nCntBG < MAX_BATTLE_BG; nCntBG++)
	if(g_pTextureBattle_Main_BG[nCntBG] != NULL)
	{// テクスチャの開放
		g_pTextureBattle_Main_BG[nCntBG]->Release();
		g_pTextureBattle_Main_BG[nCntBG] = NULL;
	}
	if(g_pVtxBuffBattle_Main_BG != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffBattle_Main_BG->Release();
		g_pVtxBuffBattle_Main_BG = NULL;
	}
	if(g_pVtxBuffBattle_Effect_Flash != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffBattle_Effect_Flash->Release();
		g_pVtxBuffBattle_Effect_Flash = NULL;
	}

	if(g_pTextureBattle_MarkStage != NULL)
	{// テクスチャの開放
		g_pTextureBattle_MarkStage->Release();
		g_pTextureBattle_MarkStage = NULL;
	}
	if(g_pVtxBuffBattle_MarkStage != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffBattle_MarkStage->Release();
		g_pVtxBuffBattle_MarkStage = NULL;
	}
}
//=============================================================================
// 描画処理
//=============================================================================
void DrawBattle_Main(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

//================BGの描画====================================================//
	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffBattle_Main_BG, 0, sizeof(VERTEX_2D));
	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	// テクスチャの設定
	for(int nCntBG = 0;nCntBG < MAX_BATTLE_BG;nCntBG++)
	{
		pDevice->SetTexture(0,g_pTextureBattle_Main_BG[g_BattleBG[nCntBG].nType]);
		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,nCntBG * 4, NUM_POLYGON);
	}


//================Effectの描画====================================================//
	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffBattle_Effect_Flash, 0, sizeof(VERTEX_2D));
	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	// テクスチャの設定
	pDevice->SetTexture(0,NULL);
		// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,0, NUM_POLYGON);

//================MARKの描画====================================================//
	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffBattle_MarkStage, 0, sizeof(VERTEX_2D));
	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	// テクスチャの設定
	pDevice->SetTexture(0,g_pTextureBattle_MarkStage);
	if(g_Mark.bUse)
	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);


}
//=============================================================================
// MARK頂点の作成
//=============================================================================
HRESULT MakeVertexBattle_MarkStage(LPDIRECT3DDEVICE9 pDevice)//頂点作成
{
	// オブジェクトの頂点バッファを生成
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * 1,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
		FVF_VERTEX_2D,						// 使用する頂点フォーマット
		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
		&g_pVtxBuffBattle_MarkStage,					// 頂点バッファインターフェースへのポインタ
		NULL)))								// NULLに設定
	{
		return E_FAIL;
	}
	//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffBattle_MarkStage->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定/ 
		pVtx[0].pos.x = g_Mark.pos.x - TEXTURE_MARK_SIZE_X;
		pVtx[0].pos.y = g_Mark.pos.y - TEXTURE_MARK_SIZE_Y;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_Mark.pos.x + TEXTURE_MARK_SIZE_X;
		pVtx[1].pos.y = g_Mark.pos.y - TEXTURE_MARK_SIZE_Y;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_Mark.pos.x - TEXTURE_MARK_SIZE_X;
		pVtx[2].pos.y = g_Mark.pos.y + TEXTURE_MARK_SIZE_Y;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_Mark.pos.x + TEXTURE_MARK_SIZE_X;
		pVtx[3].pos.y = g_Mark.pos.y + TEXTURE_MARK_SIZE_Y;
		pVtx[3].pos.z = 0.0f;
		// rhwの設定
		pVtx[0].rhw =
			pVtx[1].rhw =
			pVtx[2].rhw =
			pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点データをアンロックする
	g_pVtxBuffBattle_MarkStage->Unlock();
	return S_OK;

}
void SetVerTexBattle_MarkStage(void)
{
//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffBattle_MarkStage->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定/ 
		pVtx[0].pos.x = g_Mark.pos.x - TEXTURE_MARK_SIZE_X;
		pVtx[0].pos.y = g_Mark.pos.y - TEXTURE_MARK_SIZE_Y;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_Mark.pos.x + TEXTURE_MARK_SIZE_X;
		pVtx[1].pos.y = g_Mark.pos.y - TEXTURE_MARK_SIZE_Y;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_Mark.pos.x - TEXTURE_MARK_SIZE_X;
		pVtx[2].pos.y = g_Mark.pos.y + TEXTURE_MARK_SIZE_Y;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_Mark.pos.x + TEXTURE_MARK_SIZE_X;
		pVtx[3].pos.y = g_Mark.pos.y + TEXTURE_MARK_SIZE_Y;
		pVtx[3].pos.z = 0.0f;

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f,TEXTURE_MARK_UNIT * g_FloorIdx);
		pVtx[1].tex = D3DXVECTOR2(1.0f,TEXTURE_MARK_UNIT * g_FloorIdx);
		pVtx[2].tex = D3DXVECTOR2(0.0f,TEXTURE_MARK_UNIT * g_FloorIdx + TEXTURE_MARK_UNIT);
		pVtx[3].tex = D3DXVECTOR2(1.0f,TEXTURE_MARK_UNIT * g_FloorIdx + TEXTURE_MARK_UNIT);
	// 頂点データをアンロックする
	g_pVtxBuffBattle_MarkStage->Unlock();
}
//=============================================================================
// BG頂点の作成
//=============================================================================
HRESULT MakeVertexBattle_Main_BG(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * MAX_BATTLE_BG,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
		FVF_VERTEX_2D,						// 使用する頂点フォーマット
		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
		&g_pVtxBuffBattle_Main_BG,					// 頂点バッファインターフェースへのポインタ
		NULL)))								// NULLに設定
	{
		return E_FAIL;
	}
	//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffBattle_Main_BG->Lock(0, 0, (void**)&pVtx, 0);

	for(int nCntBG = 0;nCntBG < MAX_BATTLE_BG;nCntBG++,pVtx += 4)
	{
		// 頂点座標の設定/ 
		pVtx[0].pos.x = BATTLE_BG_POS_X;
		pVtx[0].pos.y = 0.0f;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = BATTLE_BG_POS_X;
		pVtx[1].pos.y = 0.0f;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = SYS_BATTLE_POS_END_X;
		pVtx[2].pos.y = SCREEN_HEIGHT;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = SYS_BATTLE_POS_END_X;
		pVtx[3].pos.y = g_BattleBG[nCntBG].DrawPos.y + g_BattleBG[nCntBG].SizeY;
		pVtx[3].pos.z = 0.0f;
		// rhwの設定
		pVtx[0].rhw =
			pVtx[1].rhw =
			pVtx[2].rhw =
			pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	// 頂点データをアンロックする
	g_pVtxBuffBattle_Main_BG->Unlock();
	return S_OK;

}
void SetVerTexBattle_BG(void)
{
//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffBattle_Main_BG->Lock(0, 0, (void**)&pVtx, 0);

	for(int nCntBG = 0;nCntBG < MAX_BATTLE_BG;nCntBG++,pVtx += 4)
	{
		// 頂点座標の設定/ 
		pVtx[0].pos.x = g_BattleBG[nCntBG].DrawPos.x - g_BattleBG[nCntBG].SizeX;
		pVtx[0].pos.y = g_BattleBG[nCntBG].DrawPos.y - g_BattleBG[nCntBG].SizeY;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_BattleBG[nCntBG].DrawPos.x + g_BattleBG[nCntBG].SizeX;
		pVtx[1].pos.y = g_BattleBG[nCntBG].DrawPos.y - g_BattleBG[nCntBG].SizeY;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_BattleBG[nCntBG].DrawPos.x - g_BattleBG[nCntBG].SizeX;
		pVtx[2].pos.y = g_BattleBG[nCntBG].DrawPos.y + g_BattleBG[nCntBG].SizeY;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_BattleBG[nCntBG].DrawPos.x + g_BattleBG[nCntBG].SizeX;
		pVtx[3].pos.y = g_BattleBG[nCntBG].DrawPos.y + g_BattleBG[nCntBG].SizeY;
		pVtx[3].pos.z = 0.0f;
		// rhwの設定
		pVtx[0].rhw =
			pVtx[1].rhw =
			pVtx[2].rhw =
			pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_BattleBG[nCntBG].falpha);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_BattleBG[nCntBG].falpha);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_BattleBG[nCntBG].falpha);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_BattleBG[nCntBG].falpha);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	// 頂点データをアンロックする
	g_pVtxBuffBattle_Main_BG->Unlock();
}


//=============================================================================
// Flash頂点の作成
//=============================================================================
HRESULT MakeVertexBattle_Effect_Flash(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * 1,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
		FVF_VERTEX_2D,						// 使用する頂点フォーマット
		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
		&g_pVtxBuffBattle_Effect_Flash,		// 頂点バッファインターフェースへのポインタ
		NULL)))								// NULLに設定
	{
		return E_FAIL;
	}
	//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffBattle_Effect_Flash->Lock(0, 0, (void**)&pVtx, 0);


		// 頂点座標の設定/ 
		pVtx[0].pos.x = SYS_BATTLE_POS_START_X;
		pVtx[0].pos.y = 0.0f;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = SYS_BATTLE_POS_START_X + SYS_BATTLE_SIZE_X;
		pVtx[1].pos.y = 0.0f;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = SYS_BATTLE_POS_START_X;
		pVtx[2].pos.y = SCREEN_HEIGHT;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = SYS_BATTLE_POS_START_X + SYS_BATTLE_SIZE_X;
		pVtx[3].pos.y = SCREEN_HEIGHT;
		pVtx[3].pos.z = 0.0f;
		// rhwの設定
		pVtx[0].rhw =
			pVtx[1].rhw =
			pVtx[2].rhw =
			pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	// 頂点データをアンロックする
	g_pVtxBuffBattle_Effect_Flash->Unlock();
	return S_OK;

}
//=============================================================================
// Flash
//=============================================================================
void SetTextureBattle_Effect_Flash(void)
{
	nFlashTime --;
	nFlashTime = (int)SetMAX(nFlashTime,999,0,2);
	//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;

	switch(nFlash_Type)
	{
	case FLASH_NOTH:
		fFlash_Aphla = 0.0f;
		break;
	case FLASH_FLASHING:
		if(rand()%2==0)
		fFlash_Aphla = 0.5f;
		else
		fFlash_Aphla = 0.0f;

		if(nFlashTime == 0)
			nFlash_Type = FLASH_NOTH;
		break;
	}

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffBattle_Effect_Flash->Lock(0, 0, (void**)&pVtx, 0);
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fFlash_Aphla);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fFlash_Aphla);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fFlash_Aphla);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fFlash_Aphla);
	// 頂点データをアンロックする
	g_pVtxBuffBattle_Effect_Flash->Unlock();
}
//=============================================================================
// Flashタイプの設定
//=============================================================================
void SetFlash(int nType,int Time)
{
	nFlash_Type = nType;
	nFlashTime += Time;
}
void SetBattleStageID(int StageID)
{
	g_RecStageID = StageID;
}

