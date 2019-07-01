//===================================================================
//Skill_Function.cpp
//===================================================================
#include "Battle_Skill_Function.h"
#include "effect_block_broke.h"
#include "main.h"
#include "game.h"

#include "Tetris_main.h"
#include "Battle_Attack.h"
#include "Battle_Player.h"
#include "Battle_Enemy.h"
#include "Battle_Main.h"
#include "Tetris_main.h"
#include "group_in_queue.h"
#include "input.h"
#include "sound.h"
#define SKILL_SHINING_MAX	(8)
#define SKILL_SHINING_SIZE_X	(100)
#define SKILL_SHINING_SIZE_Y	(100)
#define SKILL_SHINING_FRAME		(6)
#define SKILL_SHINING_LIFE		(60)
#define SKILL_SHINING_UNIT_SIZE	(1.0f / SKILL_SHINING_FRAME)
#define TEXTURE_SKILL_SHINING	("data/TEXTURE/skillshining.png")
typedef struct
{
	bool bUse;
	int	nLife;
	int Frame;
	D3DXVECTOR3 pos;
}SKIL_SHINING;
//===================================================================
//プロトタイプ宣言
//===================================================================
HRESULT MakeVertexSkillShining(LPDIRECT3DDEVICE9 pDevice);
void SetVerTexSkillShining(int SkillShiningID);
void SetUpSkillStats(SKILL_NAME name,int Turn,int Mp); 
void InitSjikkShiningSingle(int ID);
//===================================================================
//グローバル変数
//===================================================================
LPDIRECT3DTEXTURE9		g_pTextureSkillShining	= NULL;	//テクスチャメモリ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSkillShining	= NULL;	//頂点座標バッファ
SKILL_STATS		g_SkillStats[SKILL_NAME_MAX_SKILL];
SKIL_SHINING	g_SkillShining[SKILL_SHINING_MAX];


//===================================================================
//Skillの初期化
//===================================================================
HRESULT InitBattle_Skill_Function(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	for(int nCntSkill = 0; nCntSkill < SKILL_NAME_MAX_SKILL;nCntSkill++)
	{
		g_SkillStats[nCntSkill].CoolTurn = 0;
		g_SkillStats[nCntSkill].MPneed = 0;
		g_SkillStats[nCntSkill].SkillName = SKILL_NAME_MAX_SKILL;

	}
	SetUpSkillStats(SKILL_NAME_RECOVER_HP_50,3,40);
	SetUpSkillStats(SKILL_NAME_ALL_MAGIC_FIRE_BALL_1000,2,15);
	SetUpSkillStats(SKILL_NAME_ALL_MAGIC_WATER_BALL_1000,2,15);
	SetUpSkillStats(SKILL_NAME_ALL_MAGIC_WIND_BALL_1000,2,15);
	SetUpSkillStats(SKILL_NAME_ALL_MAGIC_DARK_BALL_1000,2,15);
	SetUpSkillStats(SKILL_NAME_RECOVER_PIECE_CHANGE_RED_TO_RECOVER,4,40);
	SetUpSkillStats(SKILL_NAME_CHNAGE_Q_ALL_I_RED,4,60);


	for(int nCntShining = 0;nCntShining < SKILL_SHINING_MAX;nCntShining++)
	{
		InitSjikkShiningSingle(nCntShining);
	}

	D3DXCreateTextureFromFile(pDevice,
		TEXTURE_SKILL_SHINING,
		&g_pTextureSkillShining);

	MakeVertexSkillShining(pDevice);
	return S_OK;
}
void InitSjikkShiningSingle(int ID)
{
		g_SkillShining[ID].bUse		= false;
		g_SkillShining[ID].nLife		= SKILL_SHINING_LIFE;
		g_SkillShining[ID].pos		= D3DXVECTOR3(0.0f,0.0f,0.0f);
		g_SkillShining[ID].Frame		= 0;
}
//===================================================================
//終了処理
//===================================================================
void UninitBattle_Skill_Function(void)
{
	if(g_pTextureSkillShining != NULL)
	{// テクスチャの開放
		g_pTextureSkillShining->Release();
		g_pTextureSkillShining = NULL;
	}

	if(g_pVtxBuffSkillShining != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffSkillShining->Release();
		g_pVtxBuffSkillShining = NULL;
	}
}
//===================================================================
//更新処理
//===================================================================
void UpdateBattle_Skill_Function(void)
{
	//BATTLE_ENEMY *pBattleEnemy = GetBattleEnemy();
	//BATTLE_T_MEMBER *pMember = GetTMember();
	//BATTLE_PLAYER *pBattlePlayer = GetBattlePlayer();
	//FIELD *pField = GetField();

	for(int nCntShining = 0;nCntShining < SKILL_SHINING_MAX;nCntShining++)
	{
		if(g_SkillShining[nCntShining].bUse == false)
			continue;
		if(g_SkillShining[nCntShining].nLife % (int)(SKILL_SHINING_LIFE / SKILL_SHINING_FRAME) == 0)
			g_SkillShining[nCntShining].Frame ++;
		g_SkillShining[nCntShining].nLife --;
		if(g_SkillShining[nCntShining].nLife <= 0)
			InitSjikkShiningSingle(nCntShining);

		SetVerTexSkillShining(nCntShining);

	}


}
//===================================================================
//描画処理
//===================================================================
void DrawBattle_Skill_Function(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//フィールドの描画
	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffSkillShining, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetTexture(0,g_pTextureSkillShining);
	for(int nCntShining = 0;nCntShining < SKILL_SHINING_MAX;nCntShining++)
	{
		if(g_SkillShining[nCntShining].bUse == false)
			continue;
		// ポリゴンの描画
		//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);//set
		//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);//in
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntShining * 4), NUM_POLYGON);
		//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);//set
		//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);// out
	}

}
//===================================================================
//頂点作成
//===================================================================
HRESULT MakeVertexSkillShining(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * SKILL_SHINING_MAX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
		FVF_VERTEX_2D,						// 使用する頂点フォーマット
		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
		&g_pVtxBuffSkillShining,					// 頂点バッファインターフェースへのポインタ
		NULL)))								// NULLに設定
	{
		return E_FAIL;
	}
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffSkillShining->Lock(0, 0, (void**)&pVtx, 0);

		for(int nCntShining = 0;nCntShining < SKILL_SHINING_MAX;nCntShining++,pVtx += 4)	
		{
			// 頂点座標の設定
			pVtx[0].pos.x = g_SkillShining[nCntShining].pos.x + BATTLE_TEAM_SIZE - SKILL_SHINING_SIZE_X;
			pVtx[0].pos.y = g_SkillShining[nCntShining].pos.y - BATTLE_TEAM_SIZE - SKILL_SHINING_SIZE_Y;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_SkillShining[nCntShining].pos.x + BATTLE_TEAM_SIZE + SKILL_SHINING_SIZE_X;
			pVtx[1].pos.y = g_SkillShining[nCntShining].pos.y - BATTLE_TEAM_SIZE - SKILL_SHINING_SIZE_Y;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_SkillShining[nCntShining].pos.x + BATTLE_TEAM_SIZE - SKILL_SHINING_SIZE_X;
			pVtx[2].pos.y = g_SkillShining[nCntShining].pos.y - BATTLE_TEAM_SIZE + SKILL_SHINING_SIZE_Y;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_SkillShining[nCntShining].pos.x + BATTLE_TEAM_SIZE + SKILL_SHINING_SIZE_X;
			pVtx[3].pos.y = g_SkillShining[nCntShining].pos.y - BATTLE_TEAM_SIZE + SKILL_SHINING_SIZE_Y;
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

			pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

		}
		// 頂点データをアンロックする
		g_pVtxBuffSkillShining->Unlock();
	}
	return S_OK;

}
void SetVerTexSkillShining(int SkillShiningID)
{//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffSkillShining->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += SkillShiningID*4;
	// 頂点座標の設定
	pVtx[0].pos.x = g_SkillShining[SkillShiningID].pos.x + BATTLE_TEAM_SIZE/2 - SKILL_SHINING_SIZE_X;
	pVtx[0].pos.y = g_SkillShining[SkillShiningID].pos.y - BATTLE_TEAM_SIZE/2 - SKILL_SHINING_SIZE_Y;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_SkillShining[SkillShiningID].pos.x + BATTLE_TEAM_SIZE/2 + SKILL_SHINING_SIZE_X;
	pVtx[1].pos.y = g_SkillShining[SkillShiningID].pos.y - BATTLE_TEAM_SIZE/2 - SKILL_SHINING_SIZE_Y;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_SkillShining[SkillShiningID].pos.x + BATTLE_TEAM_SIZE/2 - SKILL_SHINING_SIZE_X;
	pVtx[2].pos.y = g_SkillShining[SkillShiningID].pos.y - BATTLE_TEAM_SIZE/2 + SKILL_SHINING_SIZE_Y;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_SkillShining[SkillShiningID].pos.x + BATTLE_TEAM_SIZE/2 + SKILL_SHINING_SIZE_X;
	pVtx[3].pos.y = g_SkillShining[SkillShiningID].pos.y - BATTLE_TEAM_SIZE/2 + SKILL_SHINING_SIZE_Y;
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

	int Frame = g_SkillShining[SkillShiningID].Frame;
	pVtx[0].tex = D3DXVECTOR2(0.0f,SKILL_SHINING_UNIT_SIZE * Frame);
	pVtx[1].tex = D3DXVECTOR2(1.0f,SKILL_SHINING_UNIT_SIZE * Frame);
	pVtx[2].tex = D3DXVECTOR2(0.0f,SKILL_SHINING_UNIT_SIZE * Frame + SKILL_SHINING_UNIT_SIZE);
	pVtx[3].tex = D3DXVECTOR2(1.0f,SKILL_SHINING_UNIT_SIZE * Frame + SKILL_SHINING_UNIT_SIZE);

	// 頂点データをアンロックする
	g_pVtxBuffSkillShining->Unlock();
}
//===================================================================
//設定用関数
//===================================================================
void SetUpSkillStats(SKILL_NAME name,int Turn,int Mp)
{
	g_SkillStats[name].CoolTurn = Turn;
	g_SkillStats[name].SkillName = name;
	g_SkillStats[name].MPneed = Mp;
}
SKILL_STATS	GetSkill(SKILL_NAME name)
{
	return g_SkillStats[name];
}

void UseSkill(SKILL_NAME name)
{
	switch (name)
	{
	case SKILL_NAME_RECOVER_HP_50:
		Skill_Recover_Weak();
		break;
	case SKILL_NAME_ALL_MAGIC_FIRE_BALL_1000:
		Skill_Fire_Magic_Attack_ALL_Weak_1000();
		break;
	case SKILL_NAME_ALL_MAGIC_WATER_BALL_1000:
		Skill_Water_Magic_Attack_ALL_Weak_1000();
		break;
	case SKILL_NAME_ALL_MAGIC_WIND_BALL_1000:
		Skill_Wind_Magic_Attack_ALL_Weak_1000();
		break;
	case SKILL_NAME_ALL_MAGIC_DARK_BALL_1000:
		Skill_Dark_Magic_Attack_ALL_Weak_1000();
		break;
	case SKILL_NAME_RECOVER_PIECE_CHANGE_RED_TO_RECOVER:
		Skill_Recover_Piece_Change_Fire_To_Recover();
		break;
	case SKILL_NAME_CHNAGE_Q_ALL_I_RED:
		Skill_Change_Q_ALL_to_I_RED();
		break;
	}

}
void SetSkillShining(D3DXVECTOR3 pos)
{
	for(int nCntShining = 0;nCntShining < SKILL_SHINING_MAX;nCntShining++)
	{
		if(g_SkillShining[nCntShining].bUse == true)
			continue;
		InitSjikkShiningSingle(nCntShining);
		g_SkillShining[nCntShining].bUse = true;
		g_SkillShining[nCntShining].pos = pos;

		break;
	}
}

//===================================================================
//スキルの実体
//===================================================================
void Skill_Recover_Weak(void)
{
	BATTLE_PLAYER *pBattlePlayer = GetBattlePlayer();
	int Recover = pBattlePlayer->fMaxHP * 0.50f;
	SetBattle_PlayerHP_Change(HP_RECOVER,Recover * -1);
}
void Skill_Fire_Magic_Attack_ALL_Weak_1000(void)
{
	BATTLE_ENEMY *pBattleEnemy = GetBattleEnemy();
	//SetBattle_Attack_Single
	for(int nEnemy = 0; nEnemy < TEAM_ENEMY_MAX;nEnemy ++)
	{
		if(pBattleEnemy[nEnemy].bUse == false)
			continue;
		int Damage = DamageAnalysFinal(BLOCK_RED,1000,nEnemy);
		SetBattle_Attack_Single(TEAM_PLAYER,2,BLOCK_RED,nEnemy,(float)Damage,0);
	}
}
void Skill_Water_Magic_Attack_ALL_Weak_1000(void)
{
	BATTLE_ENEMY *pBattleEnemy = GetBattleEnemy();
	//SetBattle_Attack_Single
	for(int nEnemy = 0; nEnemy < TEAM_ENEMY_MAX;nEnemy ++)
	{
		if(pBattleEnemy[nEnemy].bUse == false)
			continue;
		int Damage = DamageAnalysFinal(BLOCK_BLUE,1000,nEnemy);
		SetBattle_Attack_Single(TEAM_PLAYER,2,BLOCK_BLUE,nEnemy,(float)Damage,0);
	}
}
void Skill_Wind_Magic_Attack_ALL_Weak_1000(void)
{
	BATTLE_ENEMY *pBattleEnemy = GetBattleEnemy();
	//SetBattle_Attack_Single
	for(int nEnemy = 0; nEnemy < TEAM_ENEMY_MAX;nEnemy ++)
	{
		if(pBattleEnemy[nEnemy].bUse == false)
			continue;
		int Damage = DamageAnalysFinal(BLOCK_GREEN,1000,nEnemy);
		SetBattle_Attack_Single(TEAM_PLAYER,2,BLOCK_GREEN,nEnemy,(float)Damage,0);
	}
}
void Skill_Dark_Magic_Attack_ALL_Weak_1000(void)
{
	BATTLE_ENEMY *pBattleEnemy = GetBattleEnemy();
	//SetBattle_Attack_Single
	for(int nEnemy = 0; nEnemy < TEAM_ENEMY_MAX;nEnemy ++)
	{
		if(pBattleEnemy[nEnemy].bUse == false)
			continue;
		int Damage = DamageAnalysFinal(BLOCK_DARK,10000,nEnemy);
		SetBattle_Attack_Single(TEAM_PLAYER,2,BLOCK_DARK,nEnemy,(float)Damage,0);
	}
}
void Skill_Recover_Piece_Change_Fire_To_Recover(void)
{
	FIELD *pField = GetField();
	ChangeFieldColor(BLOCK_RED,BLOCK_PINK);
	SetVerTexField();
	PlaySound(SOUND_LABEL_SE_TETRIS_PIECE_CHANGE);

}
void Skill_Change_Q_ALL_to_I_RED(void)
{
	SetGroupQHandleALL(FORM_I,BLOCK_RED);
}



