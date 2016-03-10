
//=============================================================================
// バトル上のプレイヤー [Battle_Enemy.cpp]
// HAL 名古屋　GP11A172 17 50323 リ リン
//=============================================================================
#include "Battle_Enemy.h"
#include "Tetris_main.h"
#include "Data_Monster.h"
//#include "Func_ChangingCage.h"
#include "Func_NumberX.h"
#include "input.h"
#include <math.h>


#define _CRT_SECURE_NOWARNINGS
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	HP_SIZE_Y	(10.0f);
#define HP_SIDE_SIZE_X	(5.0f)
#define	TEXTURE_BATTLE_ENEMY_HP ("data/TEXTURE/EnemyHP.png")
#define TEXTURE_BATTLE_ENEMY_TURN	("data/TEXTURE/Number_Big.png")
typedef enum
{
	ENEMY_HP_TEX_RED = 0,
	ENEMY_HP_TEX_GREEN,
	ENEMY_HP_TEX_BLUE,
	ENEMY_HP_TEX_LGIHT,
	ENEMY_HP_TEX_PURPLE,
	ENEMY_HP_TEX_LEFT_SIDE,
	ENEMY_HP_TEX_RIGHT_SIDE,
	ENEMY_HP_TEX_UP_DOWN_SIDE,
	ENEMY_HP_TEX_BG,
	ENEMY_HP_TEX_BG02,
	ENEMY_HP_TEX_MAX
}TEXTURE_ENEMY_HP;
#define BATTLE_ENEMY_TURN_SIZE_X	(15)
#define BATTLE_ENEMY_TURN_SIZE_Y	(30)
#define TEXTURE_HP_UNIT	(1.0f / ENEMY_HP_TEX_MAX)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexBattle_Enemy(LPDIRECT3DDEVICE9 pDevice);
void SetVerTexBattle_Enemy(int EnemyID,float shake,float Alpha);

HRESULT MakeVertexBattle_EnemyHP(LPDIRECT3DDEVICE9 pDevice);
void ReMakeVerTexBattle_EnemyHP(void);
void SetVerTexBattle_EnemyHP(int EnemyID);

HRESULT MakeVertexBattle_EnemyTurn(LPDIRECT3DDEVICE9 pDevice);
void SetVerTexBattle_EnemyTurn(int EnemyID);

void InitBattleEnemy(int EnemyID);


//*****************************************************************************
// 構造体宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureBattle_Enemy[TEAM_ENEMY_MAX]	= {};	//テクスチャメモリ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBattle_Enemy	= NULL;	//頂点座標バッファ
LPDIRECT3DTEXTURE9		g_pTextureBattle_EnemyHP	= NULL;	//テクスチャメモリ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBattle_EnemyHP	= NULL;	//頂点座標バッファ

LPDIRECT3DTEXTURE9		g_pTextureBattle_EnemyTurn	= NULL;	//テクスチャメモリ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBattle_EnemyTurn	= NULL;	//頂点座標バッファ

BATTLE_ENEMY			g_BattleEnemy[TEAM_ENEMY_MAX];
float	fShake;	//振動の変数
int		g_nTimeEnemy;//時間制御変数（実際作りたくないよ）
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBattle_Enemy(void)
{
	const char *pMonsterFile;

	
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	for(int nEnemy = 0; nEnemy < TEAM_ENEMY_MAX;nEnemy ++)
	{
		InitBattleEnemy(nEnemy);
	}
	for(int nTex = 0; nTex < TEAM_ENEMY_MAX;nTex++)
	{
		pMonsterFile = GetTexture(nTex);
		D3DXCreateTextureFromFile(pDevice,
		pMonsterFile,
		&g_pTextureBattle_Enemy[nTex]);

	}

	D3DXCreateTextureFromFile(pDevice,
	TEXTURE_BATTLE_ENEMY_HP,
	&g_pTextureBattle_EnemyHP);

	D3DXCreateTextureFromFile(pDevice,
	TEXTURE_BATTLE_ENEMY_TURN,
	&g_pTextureBattle_EnemyTurn);

	MakeVertexBattle_Enemy(pDevice);
	MakeVertexBattle_EnemyHP(pDevice);
	MakeVertexBattle_EnemyTurn(pDevice);
	fShake = 0.0f;
	g_nTimeEnemy = 0;
	return S_OK;
}
//==============================================初期化関数デヘペロ
void InitBattleEnemy(int EnemyID)
{
		g_BattleEnemy[EnemyID].bUse		= false;
		g_BattleEnemy[EnemyID].MonsterID	= 0;
		g_BattleEnemy[EnemyID].nColor		= 0;
		g_BattleEnemy[EnemyID].nType		= ENEMY_NORMAL;
		g_BattleEnemy[EnemyID].fAtk		= 0.0f;
		g_BattleEnemy[EnemyID].HP_Type	= HP_KEEPING;
		g_BattleEnemy[EnemyID].fHP_Per_Time	= 0.0f;
		g_BattleEnemy[EnemyID].fMaxHP		= 0.0f;
		g_BattleEnemy[EnemyID].nTeamPlace	= 0.0f;
		g_BattleEnemy[EnemyID].fSize		= 1.0f;
		g_BattleEnemy[EnemyID].pos.x		= 0.0f;
		g_BattleEnemy[EnemyID].pos.y		= 0.0f;
		g_BattleEnemy[EnemyID].pos.z		= 0.0f;
		g_BattleEnemy[EnemyID].fDestHP	= g_BattleEnemy[EnemyID].fRecHP;
		g_BattleEnemy[EnemyID].fAlpha	= 1.0f;
		g_BattleEnemy[EnemyID].turn		= 0;
		g_BattleEnemy[EnemyID].turnMAX	= 0;

		g_BattleEnemy[EnemyID].HP_LeftSidePos		= D3DXVECTOR3(0.0f,0.0f,0.0f);
		g_BattleEnemy[EnemyID].HP_RightSidePos		= D3DXVECTOR3(0.0f,0.0f,0.0f);
		g_BattleEnemy[EnemyID].HP_EntityPos			= D3DXVECTOR3(0.0f,0.0f,0.0f);
		g_BattleEnemy[EnemyID].HP_SizeEntitySizeX	= 0.0f;
		g_BattleEnemy[EnemyID].HP_SizeEntitySizeY	= 0.0f;
		g_BattleEnemy[EnemyID].HP_SizeSideSizeX		= 0.0f;
		g_BattleEnemy[EnemyID].HP_SizeSideSizeY		= 0.0f;
		g_BattleEnemy[EnemyID].HP_SizePerHP			= 0.0f;
		

		for(int nSkill = 0; nSkill < SKILL_MAX_IN_MONSTER;nSkill++)
		{
			g_BattleEnemy[EnemyID].skill[nSkill].UseTime			 = 0;
			g_BattleEnemy[EnemyID].skill[nSkill].bActuate		 = false;
			g_BattleEnemy[EnemyID].skill[nSkill].SkillName		 = ENEMY_SKILL_NAME_MAX;
			g_BattleEnemy[EnemyID].skill[nSkill].SkillCondition = ENEMY_SKILL_CONDITION_MAX;
			g_BattleEnemy[EnemyID].words[nSkill]				= MESSAGE_WORDS_MAX;
		}
}
//===============================================
//敵の設定
//===============================================
void SetUpBattleEnemy(int EnemyID,int TeamPlace,int MonsterID,bool Boss,int EnemyCount)
{

	DATA_MONSTER *pMonsterData;
	pMonsterData = GetMonsterData();
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	for(int nCntEnemy = 0; nCntEnemy < TEAM_ENEMY_MAX;nCntEnemy++)
	{
		if(g_BattleEnemy[nCntEnemy].bUse == true)
			continue;
		if(Boss != true)
		{
			g_BattleEnemy[nCntEnemy].bUse = true;
			g_BattleEnemy[nCntEnemy].fAtk = pMonsterData[MonsterID].ATK;
			g_BattleEnemy[nCntEnemy].nColor	= pMonsterData[MonsterID].color;
			g_BattleEnemy[nCntEnemy].fMaxHP = pMonsterData[MonsterID].MAX_HP;
			g_BattleEnemy[nCntEnemy].fRecHP = g_BattleEnemy[nCntEnemy].fMaxHP;
			g_BattleEnemy[nCntEnemy].MonsterID = MonsterID;
			g_BattleEnemy[nCntEnemy].nTeamPlace = TeamPlace;
			g_BattleEnemy[nCntEnemy].pos.x	= BATTLE_ENEMY_POS_X + nCntEnemy * ((SYS_BATTLE_SIZE_X - 40.0f)/EnemyCount);
			g_BattleEnemy[nCntEnemy].pos.y	= BATTLE_ENEMY_POS_Y;
			g_BattleEnemy[nCntEnemy].fDestHP = g_BattleEnemy[nCntEnemy].fRecHP;
			g_BattleEnemy[nCntEnemy].turnMAX = pMonsterData[MonsterID].turn;
			g_BattleEnemy[nCntEnemy].turn	= rand()%(pMonsterData[MonsterID].turn) + 1;

			g_BattleEnemy[nCntEnemy].HP_SizeEntitySizeX = BATTLE_TEAM_SIZE * 1.3f;
			g_BattleEnemy[nCntEnemy].HP_SizeEntitySizeY = HP_SIZE_Y;
			g_BattleEnemy[nCntEnemy].HP_SizeSideSizeX	= HP_SIDE_SIZE_X;
			g_BattleEnemy[nCntEnemy].HP_SizeSideSizeY	= HP_SIZE_Y;
			g_BattleEnemy[nCntEnemy].HP_SizePerHP		= g_BattleEnemy[nCntEnemy].HP_SizeEntitySizeX / g_BattleEnemy[nCntEnemy].fMaxHP;
			g_BattleEnemy[nCntEnemy].HP_LeftSidePos.x	= g_BattleEnemy[nCntEnemy].pos.x;
			g_BattleEnemy[nCntEnemy].HP_LeftSidePos.y	= g_BattleEnemy[nCntEnemy].pos.y + 5.0f;
			g_BattleEnemy[nCntEnemy].HP_EntityPos.x		= g_BattleEnemy[nCntEnemy].HP_LeftSidePos.x + HP_SIDE_SIZE_X;
			g_BattleEnemy[nCntEnemy].HP_EntityPos.y		= g_BattleEnemy[nCntEnemy].HP_LeftSidePos.y;
			g_BattleEnemy[nCntEnemy].HP_RightSidePos.x	= 
				g_BattleEnemy[nCntEnemy].HP_LeftSidePos.x + HP_SIDE_SIZE_X + g_BattleEnemy[nCntEnemy].HP_SizeEntitySizeX;
			g_BattleEnemy[nCntEnemy].HP_RightSidePos.y	= g_BattleEnemy[nCntEnemy].HP_LeftSidePos.y;
		}
		else
		{
			g_BattleEnemy[nCntEnemy].bUse = true;
			g_BattleEnemy[nCntEnemy].fAtk = pMonsterData[MonsterID].ATK;
			g_BattleEnemy[nCntEnemy].nColor	= pMonsterData[MonsterID].color;
			g_BattleEnemy[nCntEnemy].fMaxHP = pMonsterData[MonsterID].MAX_HP;
			g_BattleEnemy[nCntEnemy].fRecHP = g_BattleEnemy[nCntEnemy].fMaxHP;
			g_BattleEnemy[nCntEnemy].MonsterID = MonsterID;
			g_BattleEnemy[nCntEnemy].nTeamPlace = TeamPlace;
			g_BattleEnemy[nCntEnemy].pos.x	= SYS_BATTLE_POS_START_X + 100.0f;
			g_BattleEnemy[nCntEnemy].pos.y	= 300.0f;
			g_BattleEnemy[nCntEnemy].fSize	= 2.8f;
			g_BattleEnemy[nCntEnemy].fDestHP = g_BattleEnemy[nCntEnemy].fRecHP;
			g_BattleEnemy[nCntEnemy].turnMAX = pMonsterData[MonsterID].turn;
			g_BattleEnemy[nCntEnemy].turn	= rand()%(pMonsterData[MonsterID].turn) + 1;


			g_BattleEnemy[nCntEnemy].HP_SizeEntitySizeX = BATTLE_TEAM_SIZE * (g_BattleEnemy[nCntEnemy].fSize + 1.0f);
			g_BattleEnemy[nCntEnemy].HP_SizeEntitySizeY = HP_SIZE_Y;
			g_BattleEnemy[nCntEnemy].HP_SizeSideSizeX	= HP_SIDE_SIZE_X;
			g_BattleEnemy[nCntEnemy].HP_SizeSideSizeY	= HP_SIZE_Y;
			g_BattleEnemy[nCntEnemy].HP_SizePerHP		= g_BattleEnemy[nCntEnemy].HP_SizeEntitySizeX / g_BattleEnemy[nCntEnemy].fMaxHP;
			g_BattleEnemy[nCntEnemy].HP_LeftSidePos.x	= g_BattleEnemy[nCntEnemy].pos.x ;
			g_BattleEnemy[nCntEnemy].HP_LeftSidePos.y	= g_BattleEnemy[nCntEnemy].pos.y - BATTLE_ENEMY_SIZE_X * g_BattleEnemy[EnemyID].fSize * 0.35f;
			g_BattleEnemy[nCntEnemy].HP_EntityPos.x		= g_BattleEnemy[nCntEnemy].HP_LeftSidePos.x + HP_SIDE_SIZE_X;
			g_BattleEnemy[nCntEnemy].HP_EntityPos.y		= g_BattleEnemy[nCntEnemy].HP_LeftSidePos.y;
			g_BattleEnemy[nCntEnemy].HP_RightSidePos.x	= 
				g_BattleEnemy[nCntEnemy].HP_LeftSidePos.x + HP_SIDE_SIZE_X + g_BattleEnemy[nCntEnemy].HP_SizeEntitySizeX;
			g_BattleEnemy[nCntEnemy].HP_RightSidePos.y	= g_BattleEnemy[nCntEnemy].HP_LeftSidePos.y;

		}

		g_BattleEnemy[nCntEnemy].nSkillCount			= pMonsterData[MonsterID].SkillCount;
		char *Tex = pMonsterData[MonsterID].Tex;
		strcpy(Tex,pMonsterData[MonsterID].Tex);
		D3DXCreateTextureFromFile(pDevice,Tex,&g_pTextureBattle_Enemy[TeamPlace]);



		for(int nSkill = 0; nSkill < SKILL_MAX_IN_MONSTER;nSkill++)
		{
			g_BattleEnemy[EnemyID].skill[nSkill].bActuate			= false;
			g_BattleEnemy[EnemyID].skill[nSkill].UseTime			= pMonsterData[MonsterID].skill[nSkill].UseTime;
			g_BattleEnemy[EnemyID].skill[nSkill].SkillName			= pMonsterData[MonsterID].skill[nSkill].SkillName;
			g_BattleEnemy[EnemyID].skill[nSkill].SkillCondition		= pMonsterData[MonsterID].skill[nSkill].SkillCondition;
			g_BattleEnemy[EnemyID].words[nSkill]					= pMonsterData[MonsterID].MessageWord[nSkill];
		}
		D3DXVECTOR3 pos = g_BattleEnemy[nCntEnemy].pos;
		pos.x -= 5.0f;
		pos.y -= 20.0f;	
		break;
	}
	ReMakeVerTexBattle_EnemyHP();

}
//=============================================================================
// 終了処理
//=============================================================================
void UninitBattle_Enemy(void)
{
	for(int nTex = 0; nTex < TEAM_ENEMY_MAX;nTex++)
	{
		if(g_pTextureBattle_Enemy[nTex] != NULL)
		{// テクスチャの開放
			g_pTextureBattle_Enemy[nTex]->Release();
			g_pTextureBattle_Enemy[nTex] = NULL;
		}
	}
	if(g_pVtxBuffBattle_Enemy != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffBattle_Enemy->Release();
		g_pVtxBuffBattle_Enemy = NULL;
	}

	if(g_pTextureBattle_EnemyHP != NULL)
	{// テクスチャの開放
		g_pTextureBattle_EnemyHP->Release();
		g_pTextureBattle_EnemyHP = NULL;
	}

	if(g_pVtxBuffBattle_EnemyHP != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffBattle_EnemyHP->Release();
		g_pVtxBuffBattle_EnemyHP = NULL;
	}
	if(g_pTextureBattle_EnemyTurn != NULL)
	{// テクスチャの開放
		g_pTextureBattle_EnemyTurn->Release();
		g_pTextureBattle_EnemyTurn = NULL;
	}

	if(g_pVtxBuffBattle_EnemyTurn != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffBattle_EnemyTurn->Release();
		g_pVtxBuffBattle_EnemyTurn = NULL;
	}
}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateBattle_Enemy(void)
{
	g_nTimeEnemy ++ ;
	if(g_nTimeEnemy > 120)
		g_nTimeEnemy -= 120;

	for(int nCntEnemy = 0; nCntEnemy < TEAM_ENEMY_MAX; nCntEnemy++)
	{
		if(g_BattleEnemy[nCntEnemy].bUse == false)
			continue;
		else//何らかの理由でelseをつけた
		{
			//ターンのデータを取得そて，描画
			NUMBER_X *pNumber = GetNumberX();
			pNumber[TURN_ENEMY_0 + nCntEnemy].nNumber = g_BattleEnemy[nCntEnemy].turn;
			g_BattleEnemy[nCntEnemy].turn = SetMAX(g_BattleEnemy[nCntEnemy].turn,g_BattleEnemy[nCntEnemy].turnMAX,0,1);


			//主にはHPの操作です
			switch(g_BattleEnemy[nCntEnemy].HP_Type)
			{
			case HP_KEEPING:
				fShake = 0.0f;
				break;
			case HP_LOSING:
				g_BattleEnemy[nCntEnemy].fRecHP -= g_BattleEnemy[nCntEnemy].fHP_Per_Time;
				fShake = ((float)(rand()%10) - 5.0f) / 3.0f;
				if(g_BattleEnemy[nCntEnemy].fRecHP <= g_BattleEnemy[nCntEnemy].fDestHP)
				{
					g_BattleEnemy[nCntEnemy].fDestHP = g_BattleEnemy[nCntEnemy].fDestHP;
					g_BattleEnemy[nCntEnemy].HP_Type = HP_KEEPING;
				}
				if(g_BattleEnemy[nCntEnemy].fRecHP <= 0.0f)//0になったらfadout に移行
					g_BattleEnemy[nCntEnemy].HP_Type = HP_FADE_OUT;
				break;
			case HP_RECOVER:
				g_BattleEnemy[nCntEnemy].fRecHP += g_BattleEnemy[nCntEnemy].fHP_Per_Time;
				if(g_BattleEnemy[nCntEnemy].fRecHP >= g_BattleEnemy[nCntEnemy].fDestHP)
				{
					g_BattleEnemy[nCntEnemy].fRecHP = g_BattleEnemy[nCntEnemy].fDestHP;
					g_BattleEnemy[nCntEnemy].HP_Type = HP_KEEPING;
				}
				break;
			case HP_FADE_OUT:
				g_BattleEnemy[nCntEnemy].fAlpha -= 1.0f / TIME_FADE_OUT;
				fShake = ((float)(rand()%20) - 10.0f) / 2.0f; 
				if(g_BattleEnemy[nCntEnemy].fAlpha <= 0.1f)
					g_BattleEnemy[nCntEnemy].HP_Type = HP_DEAD;
				break;
			case HP_DEAD:
				//pCage[nCntEnemy].bUse = false;
				InitBattleEnemy(nCntEnemy);
				Diffuse_Number(TURN_ENEMY_0 + nCntEnemy,true);

				fShake = 0;
				break;
			}
			//現在の敵のＨＰをＨＰケージに反映
			//pCage[nCntEnemy].RecValue = g_BattleEnemy[nCntEnemy].fRecHP;
			//HPの制限制御と０になったら敵キャラが死ぬ処理
			g_BattleEnemy[nCntEnemy].fRecHP = SetMAX(g_BattleEnemy[nCntEnemy].fRecHP,
				g_BattleEnemy[nCntEnemy].fMaxHP,
				0.0f,2);

			
			SetVerTexBattle_Enemy(nCntEnemy,fShake,g_BattleEnemy[nCntEnemy].fAlpha);
			SetVerTexBattle_EnemyHP(nCntEnemy);
			SetVerTexBattle_EnemyTurn(nCntEnemy);
		}
	}
	
}
void SetBattle_EnemyHP(int EnemyID,int HP_Type,float Damage)
{
	float DestBuff = 0.0f;//ＨＰ減少バッファ変数
	g_BattleEnemy[EnemyID].HP_Type = HP_Type;
	DestBuff = g_BattleEnemy[EnemyID].fDestHP;
	if(g_BattleEnemy[EnemyID].fRecHP != g_BattleEnemy[EnemyID].fDestHP)
		g_BattleEnemy[EnemyID].fDestHP = g_BattleEnemy[EnemyID].fDestHP - Damage;
	else
	{
	g_BattleEnemy[EnemyID].fDestHP = g_BattleEnemy[EnemyID].fRecHP - Damage;
	}
	g_BattleEnemy[EnemyID].fHP_Per_Time = abs(g_BattleEnemy[EnemyID].fDestHP - g_BattleEnemy[EnemyID].fRecHP)/VALUE_TIME_CHANGE;
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBattle_Enemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//フィールドの描画
	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffBattle_Enemy, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for(int nCntEnemy = 0;nCntEnemy < TEAM_ENEMY_MAX;nCntEnemy++)
	{
		if(g_BattleEnemy[nCntEnemy].bUse == false)
			continue;
		pDevice->SetTexture(0,g_pTextureBattle_Enemy[nCntEnemy]);
		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntEnemy * 4), NUM_POLYGON);	
	}

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffBattle_EnemyHP, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetTexture(0,g_pTextureBattle_EnemyHP);

	for(int nCntEnemy = 0;nCntEnemy < TEAM_ENEMY_MAX;nCntEnemy++)
	{
		if(g_BattleEnemy[nCntEnemy].bUse == false)
			continue;
		//leftside

		// ポリゴンの描画

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntEnemy * 4 * 5), NUM_POLYGON);

		//BG
		// ポリゴンの描画

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntEnemy * 4 * 5 + 4), NUM_POLYGON);

		//entity
		// ポリゴンの描画

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntEnemy * 4 * 5 + 8), NUM_POLYGON);

		//updown
		// ポリゴンの描画

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntEnemy * 4 * 5 + 12), NUM_POLYGON);

		//right
		// ポリゴンの描画

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntEnemy * 4 * 5 + 16), NUM_POLYGON);
		
	}

	//turn
	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffBattle_EnemyTurn, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for(int nCntEnemy = 0;nCntEnemy < TEAM_ENEMY_MAX;nCntEnemy++)
	{
		if(g_BattleEnemy[nCntEnemy].bUse == false)
			continue;
		pDevice->SetTexture(0,g_pTextureBattle_EnemyTurn);
		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntEnemy * 4), NUM_POLYGON);	
	}
}
//=============================================================================
// HP頂点の作成
//=============================================================================
HRESULT MakeVertexBattle_EnemyHP(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * 5 * TEAM_ENEMY_MAX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
		FVF_VERTEX_2D,						// 使用する頂点フォーマット
		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
		&g_pVtxBuffBattle_EnemyHP,			// 頂点バッファインターフェースへのポインタ
		NULL)))								// NULLに設定
	{
		return E_FAIL;
	}
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffBattle_EnemyHP->Lock(0, 0, (void**)&pVtx, 0);

		for(int nCntEnemy = 0;nCntEnemy < TEAM_ENEMY_MAX;nCntEnemy++)//,pVtx += 4)	
		{
			//Leftside
			// 頂点座標の設定
			pVtx[0].pos.x = g_BattleEnemy[nCntEnemy].HP_LeftSidePos.x;
			pVtx[0].pos.y = g_BattleEnemy[nCntEnemy].HP_LeftSidePos.y;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_BattleEnemy[nCntEnemy].HP_LeftSidePos.x + g_BattleEnemy[nCntEnemy].HP_SizeSideSizeX;
			pVtx[1].pos.y = g_BattleEnemy[nCntEnemy].HP_LeftSidePos.y ;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_BattleEnemy[nCntEnemy].HP_LeftSidePos.x;
			pVtx[2].pos.y = g_BattleEnemy[nCntEnemy].HP_LeftSidePos.y + g_BattleEnemy[nCntEnemy].HP_SizeSideSizeY;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_BattleEnemy[nCntEnemy].HP_LeftSidePos.x + g_BattleEnemy[nCntEnemy].HP_SizeSideSizeX;
			pVtx[3].pos.y = g_BattleEnemy[nCntEnemy].HP_LeftSidePos.y + g_BattleEnemy[nCntEnemy].HP_SizeSideSizeY;
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

			pVtx[0].tex = D3DXVECTOR2(TEXTURE_HP_UNIT * ENEMY_HP_TEX_LEFT_SIDE,0.0f);
			pVtx[1].tex = D3DXVECTOR2(TEXTURE_HP_UNIT * ENEMY_HP_TEX_LEFT_SIDE + TEXTURE_HP_UNIT,0.0f);
			pVtx[2].tex = D3DXVECTOR2(TEXTURE_HP_UNIT * ENEMY_HP_TEX_LEFT_SIDE,1.0f);
			pVtx[3].tex = D3DXVECTOR2(TEXTURE_HP_UNIT * ENEMY_HP_TEX_LEFT_SIDE + TEXTURE_HP_UNIT,1.0f);

			pVtx += 4;
			//BG
			// 頂点座標の設定
			pVtx[0].pos.x = g_BattleEnemy[nCntEnemy].HP_EntityPos.x;
			pVtx[0].pos.y = g_BattleEnemy[nCntEnemy].HP_EntityPos.y;
			pVtx[0].pos.z = 0.0f;
			
			pVtx[1].pos.x = g_BattleEnemy[nCntEnemy].HP_EntityPos.x + g_BattleEnemy[nCntEnemy].HP_SizeEntitySizeX;
			pVtx[1].pos.y = g_BattleEnemy[nCntEnemy].HP_EntityPos.y ;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_BattleEnemy[nCntEnemy].HP_EntityPos.x;
			pVtx[2].pos.y = g_BattleEnemy[nCntEnemy].HP_EntityPos.y + g_BattleEnemy[nCntEnemy].HP_SizeEntitySizeY;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_BattleEnemy[nCntEnemy].HP_EntityPos.x + g_BattleEnemy[nCntEnemy].HP_SizeEntitySizeX;
			pVtx[3].pos.y = g_BattleEnemy[nCntEnemy].HP_EntityPos.y + g_BattleEnemy[nCntEnemy].HP_SizeEntitySizeY;
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

			pVtx[0].tex = D3DXVECTOR2(TEXTURE_HP_UNIT * ENEMY_HP_TEX_BG,0.0f);
			pVtx[1].tex = D3DXVECTOR2(TEXTURE_HP_UNIT * ENEMY_HP_TEX_BG + TEXTURE_HP_UNIT,0.0f);
			pVtx[2].tex = D3DXVECTOR2(TEXTURE_HP_UNIT * ENEMY_HP_TEX_BG,1.0f);
			pVtx[3].tex = D3DXVECTOR2(TEXTURE_HP_UNIT * ENEMY_HP_TEX_BG + TEXTURE_HP_UNIT,1.0f);

			pVtx += 4;
			//Entity
			// 頂点座標の設定
			pVtx[0].pos.x = g_BattleEnemy[nCntEnemy].HP_EntityPos.x;
			pVtx[0].pos.y = g_BattleEnemy[nCntEnemy].HP_EntityPos.y;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_BattleEnemy[nCntEnemy].HP_EntityPos.x + g_BattleEnemy[nCntEnemy].HP_SizeEntitySizeX;
			pVtx[1].pos.y = g_BattleEnemy[nCntEnemy].HP_EntityPos.y ;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_BattleEnemy[nCntEnemy].HP_EntityPos.x;
			pVtx[2].pos.y = g_BattleEnemy[nCntEnemy].HP_EntityPos.y + g_BattleEnemy[nCntEnemy].HP_SizeEntitySizeY;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_BattleEnemy[nCntEnemy].HP_EntityPos.x + g_BattleEnemy[nCntEnemy].HP_SizeEntitySizeX;
			pVtx[3].pos.y = g_BattleEnemy[nCntEnemy].HP_EntityPos.y + g_BattleEnemy[nCntEnemy].HP_SizeEntitySizeY;
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

			pVtx[0].tex = D3DXVECTOR2(TEXTURE_HP_UNIT * g_BattleEnemy[nCntEnemy].nColor,1.0f);
			pVtx[1].tex = D3DXVECTOR2(TEXTURE_HP_UNIT * g_BattleEnemy[nCntEnemy].nColor + TEXTURE_HP_UNIT,1.0f);
			pVtx[2].tex = D3DXVECTOR2(TEXTURE_HP_UNIT * g_BattleEnemy[nCntEnemy].nColor,1.0f);
			pVtx[3].tex = D3DXVECTOR2(TEXTURE_HP_UNIT * g_BattleEnemy[nCntEnemy].nColor + TEXTURE_HP_UNIT,1.0f);

			pVtx += 4;
			//UP_DOWN SIDE
			// 頂点座標の設定
			pVtx[0].pos.x = g_BattleEnemy[nCntEnemy].HP_EntityPos.x;
			pVtx[0].pos.y = g_BattleEnemy[nCntEnemy].HP_EntityPos.y;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_BattleEnemy[nCntEnemy].HP_EntityPos.x + g_BattleEnemy[nCntEnemy].HP_SizeEntitySizeX;
			pVtx[1].pos.y = g_BattleEnemy[nCntEnemy].HP_EntityPos.y ;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_BattleEnemy[nCntEnemy].HP_EntityPos.x;
			pVtx[2].pos.y = g_BattleEnemy[nCntEnemy].HP_EntityPos.y + g_BattleEnemy[nCntEnemy].HP_SizeEntitySizeY;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_BattleEnemy[nCntEnemy].HP_EntityPos.x + g_BattleEnemy[nCntEnemy].HP_SizeEntitySizeX;
			pVtx[3].pos.y = g_BattleEnemy[nCntEnemy].HP_EntityPos.y + g_BattleEnemy[nCntEnemy].HP_SizeEntitySizeY;
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

			pVtx[0].tex = D3DXVECTOR2(TEXTURE_HP_UNIT * ENEMY_HP_TEX_UP_DOWN_SIDE,0.0f);
			pVtx[1].tex = D3DXVECTOR2(TEXTURE_HP_UNIT * ENEMY_HP_TEX_UP_DOWN_SIDE + TEXTURE_HP_UNIT,0.0f);
			pVtx[2].tex = D3DXVECTOR2(TEXTURE_HP_UNIT * ENEMY_HP_TEX_UP_DOWN_SIDE,1.0f);
			pVtx[3].tex = D3DXVECTOR2(TEXTURE_HP_UNIT * ENEMY_HP_TEX_UP_DOWN_SIDE + TEXTURE_HP_UNIT,1.0f);

			pVtx += 4;
			//RightSide
			// 頂点座標の設定
			pVtx[0].pos.x = g_BattleEnemy[nCntEnemy].HP_RightSidePos.x;
			pVtx[0].pos.y = g_BattleEnemy[nCntEnemy].HP_RightSidePos.y;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_BattleEnemy[nCntEnemy].HP_RightSidePos.x + g_BattleEnemy[nCntEnemy].HP_SizeSideSizeX;
			pVtx[1].pos.y = g_BattleEnemy[nCntEnemy].HP_RightSidePos.y ;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_BattleEnemy[nCntEnemy].HP_RightSidePos.x;
			pVtx[2].pos.y = g_BattleEnemy[nCntEnemy].HP_RightSidePos.y + g_BattleEnemy[nCntEnemy].HP_SizeSideSizeY;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_BattleEnemy[nCntEnemy].HP_RightSidePos.x + g_BattleEnemy[nCntEnemy].HP_SizeSideSizeX;
			pVtx[3].pos.y = g_BattleEnemy[nCntEnemy].HP_RightSidePos.y + g_BattleEnemy[nCntEnemy].HP_SizeSideSizeY;
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

			pVtx[0].tex = D3DXVECTOR2(TEXTURE_HP_UNIT * ENEMY_HP_TEX_RIGHT_SIDE,0.0f);
			pVtx[1].tex = D3DXVECTOR2(TEXTURE_HP_UNIT * ENEMY_HP_TEX_RIGHT_SIDE + TEXTURE_HP_UNIT,0.0f);
			pVtx[2].tex = D3DXVECTOR2(TEXTURE_HP_UNIT * ENEMY_HP_TEX_RIGHT_SIDE,1.0f);
			pVtx[3].tex = D3DXVECTOR2(TEXTURE_HP_UNIT * ENEMY_HP_TEX_RIGHT_SIDE + TEXTURE_HP_UNIT,1.0f);
			pVtx += 4;
		}
		// 頂点データをアンロックする
		g_pVtxBuffBattle_EnemyHP->Unlock();
	}
	return S_OK;

}
void ReMakeVerTexBattle_EnemyHP(void)
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffBattle_EnemyHP->Lock(0, 0, (void**)&pVtx, 0);

		for(int nCntEnemy = 0;nCntEnemy < TEAM_ENEMY_MAX;nCntEnemy++)//,pVtx+=4)	
		{
			//Leftside
			// 頂点座標の設定
			pVtx[0].pos.x = g_BattleEnemy[nCntEnemy].HP_LeftSidePos.x;
			pVtx[0].pos.y = g_BattleEnemy[nCntEnemy].HP_LeftSidePos.y;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_BattleEnemy[nCntEnemy].HP_LeftSidePos.x + g_BattleEnemy[nCntEnemy].HP_SizeSideSizeX;
			pVtx[1].pos.y = g_BattleEnemy[nCntEnemy].HP_LeftSidePos.y ;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_BattleEnemy[nCntEnemy].HP_LeftSidePos.x;
			pVtx[2].pos.y = g_BattleEnemy[nCntEnemy].HP_LeftSidePos.y + g_BattleEnemy[nCntEnemy].HP_SizeSideSizeY;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_BattleEnemy[nCntEnemy].HP_LeftSidePos.x + g_BattleEnemy[nCntEnemy].HP_SizeSideSizeX;
			pVtx[3].pos.y = g_BattleEnemy[nCntEnemy].HP_LeftSidePos.y + g_BattleEnemy[nCntEnemy].HP_SizeSideSizeY;
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

			pVtx[0].tex = D3DXVECTOR2(TEXTURE_HP_UNIT * ENEMY_HP_TEX_LEFT_SIDE,0.0f);
			pVtx[1].tex = D3DXVECTOR2(TEXTURE_HP_UNIT * ENEMY_HP_TEX_LEFT_SIDE + TEXTURE_HP_UNIT,0.0f);
			pVtx[2].tex = D3DXVECTOR2(TEXTURE_HP_UNIT * ENEMY_HP_TEX_LEFT_SIDE,1.0f);
			pVtx[3].tex = D3DXVECTOR2(TEXTURE_HP_UNIT * ENEMY_HP_TEX_LEFT_SIDE + TEXTURE_HP_UNIT,1.0f);

			pVtx += 4;
			//BG
			// 頂点座標の設定
			pVtx[0].pos.x = g_BattleEnemy[nCntEnemy].HP_EntityPos.x;
			pVtx[0].pos.y = g_BattleEnemy[nCntEnemy].HP_EntityPos.y;
			pVtx[0].pos.z = 0.0f;
			
			pVtx[1].pos.x = g_BattleEnemy[nCntEnemy].HP_EntityPos.x + g_BattleEnemy[nCntEnemy].HP_SizeEntitySizeX;
			pVtx[1].pos.y = g_BattleEnemy[nCntEnemy].HP_EntityPos.y ;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_BattleEnemy[nCntEnemy].HP_EntityPos.x;
			pVtx[2].pos.y = g_BattleEnemy[nCntEnemy].HP_EntityPos.y + g_BattleEnemy[nCntEnemy].HP_SizeEntitySizeY;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_BattleEnemy[nCntEnemy].HP_EntityPos.x + g_BattleEnemy[nCntEnemy].HP_SizeEntitySizeX;
			pVtx[3].pos.y = g_BattleEnemy[nCntEnemy].HP_EntityPos.y + g_BattleEnemy[nCntEnemy].HP_SizeEntitySizeY;
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

			pVtx[0].tex = D3DXVECTOR2(TEXTURE_HP_UNIT * ENEMY_HP_TEX_BG,0.0f);
			pVtx[1].tex = D3DXVECTOR2(TEXTURE_HP_UNIT * ENEMY_HP_TEX_BG + TEXTURE_HP_UNIT,0.0f);
			pVtx[2].tex = D3DXVECTOR2(TEXTURE_HP_UNIT * ENEMY_HP_TEX_BG,1.0f);
			pVtx[3].tex = D3DXVECTOR2(TEXTURE_HP_UNIT * ENEMY_HP_TEX_BG + TEXTURE_HP_UNIT,1.0f);

			pVtx += 4;
			//Entity
			// 頂点座標の設定
			pVtx[0].pos.x = g_BattleEnemy[nCntEnemy].HP_EntityPos.x;
			pVtx[0].pos.y = g_BattleEnemy[nCntEnemy].HP_EntityPos.y;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_BattleEnemy[nCntEnemy].HP_EntityPos.x + g_BattleEnemy[nCntEnemy].HP_SizeEntitySizeX;
			pVtx[1].pos.y = g_BattleEnemy[nCntEnemy].HP_EntityPos.y ;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_BattleEnemy[nCntEnemy].HP_EntityPos.x;
			pVtx[2].pos.y = g_BattleEnemy[nCntEnemy].HP_EntityPos.y + g_BattleEnemy[nCntEnemy].HP_SizeEntitySizeY;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_BattleEnemy[nCntEnemy].HP_EntityPos.x + g_BattleEnemy[nCntEnemy].HP_SizeEntitySizeX;
			pVtx[3].pos.y = g_BattleEnemy[nCntEnemy].HP_EntityPos.y + g_BattleEnemy[nCntEnemy].HP_SizeEntitySizeY;
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

			pVtx[0].tex = D3DXVECTOR2(TEXTURE_HP_UNIT * g_BattleEnemy[nCntEnemy].nColor,0.0f);
			pVtx[1].tex = D3DXVECTOR2(TEXTURE_HP_UNIT * g_BattleEnemy[nCntEnemy].nColor + TEXTURE_HP_UNIT,0.0f);
			pVtx[2].tex = D3DXVECTOR2(TEXTURE_HP_UNIT * g_BattleEnemy[nCntEnemy].nColor,1.0f);
			pVtx[3].tex = D3DXVECTOR2(TEXTURE_HP_UNIT * g_BattleEnemy[nCntEnemy].nColor + TEXTURE_HP_UNIT,1.0f);

			pVtx += 4;
			//UP_DOWN SIDE
			// 頂点座標の設定
			pVtx[0].pos.x = g_BattleEnemy[nCntEnemy].HP_EntityPos.x;
			pVtx[0].pos.y = g_BattleEnemy[nCntEnemy].HP_EntityPos.y;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_BattleEnemy[nCntEnemy].HP_EntityPos.x + g_BattleEnemy[nCntEnemy].HP_SizeEntitySizeX;
			pVtx[1].pos.y = g_BattleEnemy[nCntEnemy].HP_EntityPos.y ;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_BattleEnemy[nCntEnemy].HP_EntityPos.x;
			pVtx[2].pos.y = g_BattleEnemy[nCntEnemy].HP_EntityPos.y + g_BattleEnemy[nCntEnemy].HP_SizeEntitySizeY;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_BattleEnemy[nCntEnemy].HP_EntityPos.x + g_BattleEnemy[nCntEnemy].HP_SizeEntitySizeX;
			pVtx[3].pos.y = g_BattleEnemy[nCntEnemy].HP_EntityPos.y + g_BattleEnemy[nCntEnemy].HP_SizeEntitySizeY;
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

			pVtx[0].tex = D3DXVECTOR2(TEXTURE_HP_UNIT * ENEMY_HP_TEX_UP_DOWN_SIDE,0.0f);
			pVtx[1].tex = D3DXVECTOR2(TEXTURE_HP_UNIT * ENEMY_HP_TEX_UP_DOWN_SIDE + TEXTURE_HP_UNIT,0.0f);
			pVtx[2].tex = D3DXVECTOR2(TEXTURE_HP_UNIT * ENEMY_HP_TEX_UP_DOWN_SIDE,1.0f);
			pVtx[3].tex = D3DXVECTOR2(TEXTURE_HP_UNIT * ENEMY_HP_TEX_UP_DOWN_SIDE + TEXTURE_HP_UNIT,1.0f);

			pVtx += 4;
			//RightSide
			// 頂点座標の設定
			pVtx[0].pos.x = g_BattleEnemy[nCntEnemy].HP_RightSidePos.x;

			pVtx[0].pos.y = g_BattleEnemy[nCntEnemy].HP_RightSidePos.y;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_BattleEnemy[nCntEnemy].HP_RightSidePos.x + g_BattleEnemy[nCntEnemy].HP_SizeSideSizeX;
			pVtx[1].pos.y = g_BattleEnemy[nCntEnemy].HP_RightSidePos.y ;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_BattleEnemy[nCntEnemy].HP_RightSidePos.x;
			pVtx[2].pos.y = g_BattleEnemy[nCntEnemy].HP_RightSidePos.y + g_BattleEnemy[nCntEnemy].HP_SizeSideSizeY;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_BattleEnemy[nCntEnemy].HP_RightSidePos.x + g_BattleEnemy[nCntEnemy].HP_SizeSideSizeX;
			pVtx[3].pos.y = g_BattleEnemy[nCntEnemy].HP_RightSidePos.y + g_BattleEnemy[nCntEnemy].HP_SizeSideSizeY;
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

			pVtx[0].tex = D3DXVECTOR2(TEXTURE_HP_UNIT * ENEMY_HP_TEX_RIGHT_SIDE,0.0f);
			pVtx[1].tex = D3DXVECTOR2(TEXTURE_HP_UNIT * ENEMY_HP_TEX_RIGHT_SIDE + TEXTURE_HP_UNIT,0.0f);
			pVtx[2].tex = D3DXVECTOR2(TEXTURE_HP_UNIT * ENEMY_HP_TEX_RIGHT_SIDE,1.0f);
			pVtx[3].tex = D3DXVECTOR2(TEXTURE_HP_UNIT * ENEMY_HP_TEX_RIGHT_SIDE + TEXTURE_HP_UNIT,1.0f);

			pVtx+=4;

		}
		// 頂点データをアンロックする
		g_pVtxBuffBattle_EnemyHP->Unlock();
	}
}

void SetVerTexBattle_EnemyHP(int EnemyID)
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffBattle_EnemyHP->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += EnemyID * 4 * 5 + 8;

			//Entity
			// 頂点座標の設定
			pVtx[0].pos.x = g_BattleEnemy[EnemyID].HP_EntityPos.x;
			pVtx[0].pos.y = g_BattleEnemy[EnemyID].HP_EntityPos.y;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_BattleEnemy[EnemyID].HP_EntityPos.x + g_BattleEnemy[EnemyID].fRecHP * g_BattleEnemy[EnemyID].HP_SizePerHP;
			pVtx[1].pos.y = g_BattleEnemy[EnemyID].HP_EntityPos.y ;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_BattleEnemy[EnemyID].HP_EntityPos.x;
			pVtx[2].pos.y = g_BattleEnemy[EnemyID].HP_EntityPos.y + g_BattleEnemy[EnemyID].HP_SizeEntitySizeY;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_BattleEnemy[EnemyID].HP_EntityPos.x + g_BattleEnemy[EnemyID].fRecHP * g_BattleEnemy[EnemyID].HP_SizePerHP;
			pVtx[3].pos.y = g_BattleEnemy[EnemyID].HP_EntityPos.y + g_BattleEnemy[EnemyID].HP_SizeEntitySizeY;
			pVtx[3].pos.z = 0.0f;

			pVtx[0].tex = D3DXVECTOR2(TEXTURE_HP_UNIT * g_BattleEnemy[EnemyID].nColor,0.0f);
			pVtx[1].tex = D3DXVECTOR2(TEXTURE_HP_UNIT * g_BattleEnemy[EnemyID].nColor + TEXTURE_HP_UNIT,0.0f);
			pVtx[2].tex = D3DXVECTOR2(TEXTURE_HP_UNIT * g_BattleEnemy[EnemyID].nColor,1.0f);
			pVtx[3].tex = D3DXVECTOR2(TEXTURE_HP_UNIT * g_BattleEnemy[EnemyID].nColor + TEXTURE_HP_UNIT,1.0f);

		// 頂点データをアンロックする
		g_pVtxBuffBattle_EnemyHP->Unlock();
	}

}

//=============================================================================
// Light頂点の作成
//=============================================================================
HRESULT MakeVertexBattle_Enemy(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * TEAM_ENEMY_MAX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
		FVF_VERTEX_2D,						// 使用する頂点フォーマット
		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
		&g_pVtxBuffBattle_Enemy,					// 頂点バッファインターフェースへのポインタ
		NULL)))								// NULLに設定
	{
		return E_FAIL;
	}
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffBattle_Enemy->Lock(0, 0, (void**)&pVtx, 0);

		for(int nCntEnemy = 0;nCntEnemy < TEAM_ENEMY_MAX;nCntEnemy++,pVtx += 4)	
		{
			// 頂点座標の設定
			pVtx[0].pos.x = g_BattleEnemy[nCntEnemy].pos.x -BATTLE_ENEMY_SIZE_X;
			pVtx[0].pos.y = g_BattleEnemy[nCntEnemy].pos.y -BATTLE_ENEMY_SIZE_X;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_BattleEnemy[nCntEnemy].pos.x +BATTLE_ENEMY_SIZE_X;
			pVtx[1].pos.y = g_BattleEnemy[nCntEnemy].pos.y -BATTLE_ENEMY_SIZE_X;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_BattleEnemy[nCntEnemy].pos.x;
			pVtx[2].pos.y = g_BattleEnemy[nCntEnemy].pos.y;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_BattleEnemy[nCntEnemy].pos.x +BATTLE_ENEMY_SIZE_X;
			pVtx[3].pos.y = g_BattleEnemy[nCntEnemy].pos.y;
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

			pVtx[0].tex = D3DXVECTOR2(1.0f,1.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f,1.0f);
			pVtx[2].tex = D3DXVECTOR2(1.0f,1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

		}
		// 頂点データをアンロックする
		g_pVtxBuffBattle_Enemy->Unlock();
	}
	return S_OK;

}
//=============================================================================
// Lightnの設定
//=============================================================================
void SetVerTexBattle_Enemy(int EnemyID,float shake,float Alpha)
{
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffBattle_Enemy->Lock(0, 0, (void**)&pVtx, 0);
		// 頂点座標の設定
		pVtx += (EnemyID *4);
			// 頂点座標の設定
			pVtx[0].pos.x = g_BattleEnemy[EnemyID].pos.x + fShake;
			pVtx[0].pos.y = g_BattleEnemy[EnemyID].pos.y -BATTLE_ENEMY_SIZE_X * g_BattleEnemy[EnemyID].fSize + fShake;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_BattleEnemy[EnemyID].pos.x +BATTLE_ENEMY_SIZE_X * g_BattleEnemy[EnemyID].fSize + fShake;
			pVtx[1].pos.y = g_BattleEnemy[EnemyID].pos.y -BATTLE_ENEMY_SIZE_X * g_BattleEnemy[EnemyID].fSize + fShake;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_BattleEnemy[EnemyID].pos.x+ fShake;
			pVtx[2].pos.y = g_BattleEnemy[EnemyID].pos.y+ fShake;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_BattleEnemy[EnemyID].pos.x +BATTLE_ENEMY_SIZE_X * g_BattleEnemy[EnemyID].fSize + fShake;
			pVtx[3].pos.y = g_BattleEnemy[EnemyID].pos.y+ fShake;
			pVtx[3].pos.z = 0.0f;

			// rhwの設定		
			pVtx[0].rhw =
			pVtx[1].rhw =
			pVtx[2].rhw =
			pVtx[3].rhw = 1.0f;
			// 頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f,Alpha);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f,Alpha);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f,Alpha);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f,Alpha);


			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);
	
	// 頂点データをアンロックする
	g_pVtxBuffBattle_Enemy->Unlock();
	}
//=============================================================================
// Turn頂点の作成
//=============================================================================
HRESULT MakeVertexBattle_EnemyTurn(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * TEAM_ENEMY_MAX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
		FVF_VERTEX_2D,						// 使用する頂点フォーマット
		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
		&g_pVtxBuffBattle_EnemyTurn,					// 頂点バッファインターフェースへのポインタ
		NULL)))								// NULLに設定
	{
		return E_FAIL;
	}
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffBattle_EnemyTurn->Lock(0, 0, (void**)&pVtx, 0);

		for(int nCntEnemy = 0;nCntEnemy < TEAM_ENEMY_MAX;nCntEnemy++,pVtx += 4)	
		{
			// 頂点座標の設定
			pVtx[0].pos.x = g_BattleEnemy[nCntEnemy].pos.x;
			pVtx[0].pos.y = g_BattleEnemy[nCntEnemy].pos.y - BATTLE_ENEMY_TURN_SIZE_X;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_BattleEnemy[nCntEnemy].pos.x + BATTLE_ENEMY_TURN_SIZE_X;
			pVtx[1].pos.y = g_BattleEnemy[nCntEnemy].pos.y - BATTLE_ENEMY_TURN_SIZE_Y;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_BattleEnemy[nCntEnemy].pos.x;
			pVtx[2].pos.y = g_BattleEnemy[nCntEnemy].pos.y;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_BattleEnemy[nCntEnemy].pos.x + BATTLE_ENEMY_TURN_SIZE_X;
			pVtx[3].pos.y = g_BattleEnemy[nCntEnemy].pos.y;
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

			pVtx[0].tex = D3DXVECTOR2(1.0f,1.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f,1.0f);
			pVtx[2].tex = D3DXVECTOR2(1.0f,1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

		}
		// 頂点データをアンロックする
		g_pVtxBuffBattle_EnemyTurn->Unlock();
	}
	return S_OK;

}
//=============================================================================
// Lightnの設定
//=============================================================================
void SetVerTexBattle_EnemyTurn(int EnemyID)
{
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffBattle_EnemyTurn->Lock(0, 0, (void**)&pVtx, 0);
		// 頂点座標の設定
		pVtx += (EnemyID *4);
			// 頂点座標の設定
			pVtx[0].pos.x = g_BattleEnemy[EnemyID].pos.x;
			pVtx[0].pos.y = g_BattleEnemy[EnemyID].pos.y - (g_BattleEnemy[EnemyID].fSize - 1) * BATTLE_ENEMY_SIZE_X  - BATTLE_ENEMY_TURN_SIZE_Y;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_BattleEnemy[EnemyID].pos.x + BATTLE_ENEMY_TURN_SIZE_X;
			pVtx[1].pos.y = g_BattleEnemy[EnemyID].pos.y -  (g_BattleEnemy[EnemyID].fSize - 1) * BATTLE_ENEMY_SIZE_X - BATTLE_ENEMY_TURN_SIZE_Y;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_BattleEnemy[EnemyID].pos.x;
			pVtx[2].pos.y = g_BattleEnemy[EnemyID].pos.y -  (g_BattleEnemy[EnemyID].fSize - 1) * BATTLE_ENEMY_SIZE_X;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_BattleEnemy[EnemyID].pos.x + BATTLE_ENEMY_TURN_SIZE_X;
			pVtx[3].pos.y = g_BattleEnemy[EnemyID].pos.y -  (g_BattleEnemy[EnemyID].fSize - 1) * BATTLE_ENEMY_SIZE_X;
			pVtx[3].pos.z = 0.0f;

			// rhwの設定		
			pVtx[0].rhw =
			pVtx[1].rhw =
			pVtx[2].rhw =
			pVtx[3].rhw = 1.0f;
			// 頂点カラーの設定
			float Redshining = 1.0f;
			switch(g_BattleEnemy[EnemyID].turn)
			{
			case 3:
				Redshining = (sinf(g_nTimeEnemy * D3DX_PI / 60) + 1.0f)/2;
				break;
			case 2:
				Redshining = (sinf(g_nTimeEnemy * D3DX_PI / 30) + 1.0f)/2;
				break;
			case 1:
				Redshining = (sinf(g_nTimeEnemy * D3DX_PI / 10) + 1.0f)/2;
				break;
			}

			
			pVtx[0].col = D3DXCOLOR(1.0f, Redshining, Redshining,1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, Redshining, Redshining,1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, Redshining, Redshining,1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, Redshining, Redshining,1.0f);

			float fUnitSize = 1.0 / 10.0f;
			int Number = g_BattleEnemy[EnemyID].turn;
			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(fUnitSize * Number,0.0f);
			pVtx[1].tex = D3DXVECTOR2(fUnitSize * Number + fUnitSize,0.0f);
			pVtx[2].tex = D3DXVECTOR2(fUnitSize * Number,1.0f);
			pVtx[3].tex = D3DXVECTOR2(fUnitSize * Number + fUnitSize,1.0f);
	
	// 頂点データをアンロックする
	g_pVtxBuffBattle_EnemyTurn->Unlock();
	}
//=============================================================================
// 敵の取得
//=============================================================================

BATTLE_ENEMY *GetBattleEnemy(void)
{
	return &g_BattleEnemy[0];
}
