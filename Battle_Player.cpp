
//=============================================================================
// バトル上のプレイヤー [Battle_Player.cpp]
// HAL 名古屋　GP11A172 17 50323 リ リン
//=============================================================================
#include "Battle_Player.h"
#include "Battle_Main.h"
#include "Battle_Enemy.h"
//#include "Func_ChangingCage.h"
#include "Data_Monster.h"
#include "Func_NumberX.h"
#include "Battle_Player_HP.h"
#include "Battle_Player_MP.h"
#include "input.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
const char *c_aFileNameMember[MAX_DATA_MONSTER] =
	{
	"data/TEXTURE/charactor000.jpg",	
	"data/TEXTURE/charactor001.jpg",	
	};
#define TEXTURE_PLAYER_OUTLINE	("data/TEXTURE/CharactorOutLine.png")
#define TEXTURE_PLAYER_OUTLINE_UNIT_SIZE (1.0f / 5.0f)
#define TEXTURE_PLAYER_OUTLINE_BALL		("data/TEXTURE/Box_Color_Ball.png")
#define TEXTURE_PLAYER_SKILL_LIGHT		("data/TEXTURE/Battle_member_Light.png")

#define PLAYER_SKILL_TURN_SIZE_X		(BATTLE_TEAM_SIZE)
#define PLAYER_SKILL_TURN_SIZE_Y		(BATTLE_TEAM_SIZE * 0.4f)
#define TEXTURE_PLAYER_SKILL_BUTTON		("data/TEXTURE/skillButton.png")
#define TEXTURE_PLAYER_SKILL_NUMBER		("data/TEXTURE/Number_Big.png")
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexBattle_Member_Skill_Turn(LPDIRECT3DDEVICE9 pDevice);
void SetVerTexBattle_Member_Skill_Turn(int MemberID);

HRESULT MakeVertexBattle_T_Member(LPDIRECT3DDEVICE9 pDevice);
void SetVerTexBattle_T_Member(int MemberID);

HRESULT MakeVertexBattle_T_Member_OutLine(LPDIRECT3DDEVICE9 pDevice);
void SetVerTexBattle_T_Member_OutLine(int MemberID);

HRESULT MakeVertexBattle_T_Member_Light(LPDIRECT3DDEVICE9 pDevice);
void SetVerTexBattle_T_Member_Light(int MemberID);

void SetBattlePlayer(int MonsterID[TEAM_PLAYER_MAX]);
//*****************************************************************************
// 構造体宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureBattle_T_Member[TEAM_PLAYER_MAX] = {};	//テクスチャメモリ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBattle_T_Member	= NULL;	//頂点座標バッファ

LPDIRECT3DTEXTURE9		g_pTextureBattle_T_Member_OutLine	= NULL;	//テクスチャメモリ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBattle_T_Member_OutLine	= NULL;	//頂点座標バッファ

LPDIRECT3DTEXTURE9		g_pTextureBattle_Member_Skill_Turn	= NULL;	//テクスチャメモリ
LPDIRECT3DTEXTURE9		g_pTextureBattle_Member_Skill_Turn_Num	= NULL;	//テクスチャメモリ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBattle_Member_Skill_Turn	= NULL;	//頂点座標バッファ

LPDIRECT3DTEXTURE9		g_pTextureBattle_T_Member_Light	= NULL;	//テクスチャメモリ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBattle_T_Member_Light	= NULL;	//頂点座標バッファ

LPDIRECT3DTEXTURE9		g_pTextureBattle_T_Member_Ball	= NULL;	//テクスチャメモリ

BATTLE_PLAYER	g_Battle_Player;
BATTLE_T_MEMBER	g_BattleT_Memeber[TEAM_PLAYER_MAX];
float	g_LightUpSin;
float	g_NockBack;
float	g_Member_Red_TexAlpha;
float	g_SkillBottomLight;
D3DXVECTOR3 SkillCountPos[TEAM_PLAYER_MAX];
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBattle_Player(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	const char *pMonsterFile;
	g_LightUpSin = 0.0f;
	g_NockBack	= 0.0f;
	g_SkillBottomLight = 0;
	g_Member_Red_TexAlpha = 0.0f;
	for(int nTex = 0; nTex < MAX_DATA_MONSTER;nTex++)
	{
		pMonsterFile = GetTexture(nTex);
		D3DXCreateTextureFromFile(pDevice,
			pMonsterFile,
			&g_pTextureBattle_T_Member[nTex]);
	}
	D3DXCreateTextureFromFile(pDevice,
		TEXTURE_PLAYER_OUTLINE,
		&g_pTextureBattle_T_Member_OutLine);
	D3DXCreateTextureFromFile(pDevice,
		TEXTURE_PLAYER_OUTLINE_BALL,
		&g_pTextureBattle_T_Member_Ball);
	D3DXCreateTextureFromFile(pDevice,
		TEXTURE_PLAYER_SKILL_LIGHT,
		&g_pTextureBattle_T_Member_Light);

	//スキルのターン数
	D3DXCreateTextureFromFile(pDevice,
		TEXTURE_PLAYER_SKILL_BUTTON,
		&g_pTextureBattle_Member_Skill_Turn);
	D3DXCreateTextureFromFile(pDevice,
		TEXTURE_PLAYER_SKILL_NUMBER,
		&g_pTextureBattle_Member_Skill_Turn_Num);


	g_Battle_Player.fDestHP = 0.0f;
	g_Battle_Player.HP_Recover = 0;
	g_Battle_Player.fHP_Per_Time = 0.0f;
	g_Battle_Player.fMaxHP	= 0.0f;
	g_Battle_Player.fRecHP  = 0.0f;
	g_Battle_Player.HP_Type	= HP_KEEPING;
	g_Battle_Player.pos		= BATTLE_PLAYER_HP_POS;
	g_Battle_Player.HP_RecoverResult	= 0;
	g_Battle_Player.		MP_Type			= MP_TYPE_KEEPING;			//
	g_Battle_Player.		MP_Recover		= 0;			//
	g_Battle_Player.		MP_Recover_Result	= 0;	//
	g_Battle_Player.	fMaxMP	= 0;				//
	g_Battle_Player.	fRecMP	= 0;				//
	g_Battle_Player.	fDestMP	= 0;			//
	g_Battle_Player.	fMP_Per_Time	 = 0;		//

	for(int nCntMember = 0;nCntMember < TEAM_PLAYER_MAX;nCntMember++)
	{
		g_BattleT_Memeber[nCntMember].bUse	= false;
		g_BattleT_Memeber[nCntMember].fATK = 0.0f;
		g_BattleT_Memeber[nCntMember].Color	= 0;
		g_BattleT_Memeber[nCntMember].fMAXHP = 0.0f;
		g_BattleT_Memeber[nCntMember].MonsterID = 0;
		g_BattleT_Memeber[nCntMember].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
		g_BattleT_Memeber[nCntMember].TeamPlace	= 0;
		g_BattleT_Memeber[nCntMember].DamageBase = 0;
		g_BattleT_Memeber[nCntMember].Skill.CoolTurn = 0;
		g_BattleT_Memeber[nCntMember].Skill.MPneed	= 0;
		g_BattleT_Memeber[nCntMember].Skill.SkillName	= SKILL_NAME_MAX_SKILL;
		g_BattleT_Memeber[nCntMember].TurnLeft	= 0;
		g_BattleT_Memeber[nCntMember].Light_Up	= 0.0f;

		SkillCountPos[nCntMember] = D3DXVECTOR3(0.0f,0.0f,0.0f);


	}
	int MemberSet[TEAM_PLAYER_MAX] = {0,1,2,3,4};
		SetBattlePlayer(MemberSet);
	MakeVertexBattle_T_Member(pDevice);
	MakeVertexBattle_T_Member_OutLine(pDevice);
	MakeVertexBattle_T_Member_Light(pDevice);
	MakeVertexBattle_Member_Skill_Turn(pDevice);
	return S_OK;
}
//=====================================================================
//プレイヤーの設置
//=====================================================================
void SetBattlePlayer(int MonsterID[TEAM_PLAYER_MAX])
{
	DATA_CHARACTOR *pDataCharactor = GetCharactorData();
	g_Battle_Player.HP_Recover = 0;
	for(int nCntMember = 0;nCntMember < TEAM_PLAYER_MAX;nCntMember++)	
	{
		g_BattleT_Memeber[nCntMember].bUse		= true;
		g_BattleT_Memeber[nCntMember].MonsterID = MonsterID[nCntMember];
		g_BattleT_Memeber[nCntMember].Color		= pDataCharactor[ MonsterID[nCntMember]].color;
		g_BattleT_Memeber[nCntMember].fATK		= pDataCharactor[ MonsterID[nCntMember]].ATK;
		g_BattleT_Memeber[nCntMember].fMAXHP	= pDataCharactor[ MonsterID[nCntMember]].MAX_HP;
		g_BattleT_Memeber[nCntMember].Skill		= pDataCharactor[ MonsterID[nCntMember]].Skill;
		g_BattleT_Memeber[nCntMember].bSkillUse	= false;
		g_BattleT_Memeber[nCntMember].TurnLeft	= 0;
		g_BattleT_Memeber[nCntMember].TeamPlace	= nCntMember;
		g_BattleT_Memeber[nCntMember].pos.x		= BATTLE_TEAM_POS_X+ nCntMember * (BATTLE_TEAM_SIZE + BATTLE_TEAM_INTERVAL);
		g_BattleT_Memeber[nCntMember].pos.y		= BATTLE_TEAM_POS_Y;
		g_BattleT_Memeber[nCntMember].DamageBase  = 0;
		g_BattleT_Memeber[nCntMember].DamageFinal = 0;
		g_BattleT_Memeber[nCntMember].Skill		= pDataCharactor[ MonsterID[nCntMember]].Skill;
		g_Battle_Player.fMaxHP	+= g_BattleT_Memeber[nCntMember].fMAXHP;
		g_Battle_Player.HP_Recover += pDataCharactor[MonsterID[nCntMember]].Recover;
		g_Battle_Player.MP_Recover += pDataCharactor[MonsterID[nCntMember]].MPRecover;

		SkillCountPos[nCntMember] = D3DXVECTOR3(g_BattleT_Memeber[nCntMember].pos.x	,g_BattleT_Memeber[nCntMember].pos.y+PLAYER_SKILL_TURN_SIZE_Y*1.2f,0.0f);	
	}
		//debug
		//g_Battle_Player.fMaxHP	= 60;

		g_Battle_Player.fRecHP	= g_Battle_Player.fMaxHP;
		g_Battle_Player.fDestHP	= g_Battle_Player.fRecHP;
		g_Battle_Player.HP_Type	= HP_KEEPING;
		g_Battle_Player.fHP_Per_Time = 0.0f;
		g_Battle_Player.MP_Type	= MP_TYPE_KEEPING;
		g_Battle_Player.fMaxMP = 100;
		g_Battle_Player.fRecMP = 0;
		g_Battle_Player.fDestMP = g_Battle_Player.fRecMP;
		g_Battle_Player.fMP_Per_Time = 0.0f;
	SetBattle_Player_HP();
	SetBattle_Player_MP();
}
//=============================================================================
// 終了処理
//=============================================================================
void UninitBattle_Player(void)
{
	for(int nCntMember = 0;nCntMember < TEAM_PLAYER_MAX;nCntMember++)
	{	
		if(g_pTextureBattle_T_Member[nCntMember] != NULL)
		{// テクスチャの開放
			g_pTextureBattle_T_Member[nCntMember]->Release();
			g_pTextureBattle_T_Member[nCntMember] = NULL;
		}
	}
	if(g_pVtxBuffBattle_T_Member != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffBattle_T_Member->Release();
		g_pVtxBuffBattle_T_Member = NULL;

	if(g_pTextureBattle_T_Member_OutLine	!= NULL)
	{// テクスチャの開放
		g_pTextureBattle_T_Member_OutLine->Release();
		g_pTextureBattle_T_Member_OutLine = NULL;
	}
	}
	if(g_pVtxBuffBattle_T_Member_OutLine != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffBattle_T_Member_OutLine->Release();
		g_pVtxBuffBattle_T_Member_OutLine = NULL;
	}
	if(g_pTextureBattle_T_Member_Ball	!= NULL)
	{// テクスチャの開放
		g_pTextureBattle_T_Member_Ball->Release();
		g_pTextureBattle_T_Member_Ball = NULL;
	}

	//スキルのターン数
	if(g_pTextureBattle_Member_Skill_Turn != NULL)
	{// 頂点バッファの開放
		g_pTextureBattle_Member_Skill_Turn->Release();
		g_pTextureBattle_Member_Skill_Turn = NULL;
	}
	if(g_pTextureBattle_Member_Skill_Turn_Num != NULL)
	{// 頂点バッファの開放
		g_pTextureBattle_Member_Skill_Turn_Num->Release();
		g_pTextureBattle_Member_Skill_Turn_Num = NULL;
	}
	if(g_pVtxBuffBattle_Member_Skill_Turn	!= NULL)
	{// テクスチャの開放
		g_pVtxBuffBattle_Member_Skill_Turn->Release();
		g_pVtxBuffBattle_Member_Skill_Turn = NULL;
	}
}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateBattle_Player(void)
{
	g_LightUpSin += 0.1f;
	{
		float nockBuff = g_NockBack;
		g_NockBack -= (3.0f * nockBuff) / 50.0f;
		if(g_NockBack < 2.0f)
			g_NockBack = 0.0f;
		g_NockBack = SetMAX(g_NockBack,50.0f,0.0f,2);
	}
	if(g_Member_Red_TexAlpha > 0.0f)
	{
		g_Member_Red_TexAlpha *= 0.9f;
		if(g_Member_Red_TexAlpha < 0.1f)
			g_Member_Red_TexAlpha = 0.0f;
	}





	//skill処理
	for(int nCntMember = 0;nCntMember < TEAM_PLAYER_MAX;nCntMember++)	
	{
		if(g_BattleT_Memeber[nCntMember].TurnLeft == 0
			&&
			g_BattleT_Memeber[nCntMember].Skill.MPneed <= g_Battle_Player.fRecMP)
		{
			g_BattleT_Memeber[nCntMember].bSkillUse = true;

			g_LightUpSin = SetMAX(g_LightUpSin,D3DX_PI * 2,0.0f,1);
			g_BattleT_Memeber[nCntMember].Light_Up = sinf(g_LightUpSin);


			g_SkillBottomLight = SetMAX((sinf(g_LightUpSin) + 0.5f)*0.5f,1.0f,0.0f,1);
			//g_SkillBottomLight = SetMAX(g_SkillBottomLight,1.0f,0.0f,1);
			SetVerTexBattle_T_Member_Light(nCntMember);
		}
		else
			g_BattleT_Memeber[nCntMember].bSkillUse = false;

		g_BattleT_Memeber[nCntMember].TurnLeft = SetMAX(g_BattleT_Memeber[nCntMember].TurnLeft,99,0,2);
	}



	{//HPケージの処理
		switch(g_Battle_Player.HP_Type)
		{
		case HP_KEEPING:
			break;
		case HP_LOSING:
			SetFlash(FLASH_FLASHING,1);
			g_Battle_Player.fRecHP -= g_Battle_Player.fHP_Per_Time;
			if(g_Battle_Player.fRecHP <= g_Battle_Player.fDestHP)
			{
				g_Battle_Player.fRecHP = g_Battle_Player.fDestHP;
				g_Battle_Player.HP_Type = HP_KEEPING;
			}
			if(g_Battle_Player.fRecHP <= 0.0f)//0になったらfadout に移行
				g_Battle_Player.HP_Type = HP_DEAD;
			break;
		case HP_RECOVER:
			g_Battle_Player.fRecHP += g_Battle_Player.fHP_Per_Time;
			if(g_Battle_Player.fRecHP >= g_Battle_Player.fDestHP)
			{
				g_Battle_Player.fRecHP = g_Battle_Player.fDestHP;
				g_Battle_Player.HP_Type = HP_KEEPING;
			}
			break;
		case HP_FADE_OUT:
			break;
		case HP_DEAD:
			break;
		}
		//現在の敵のＨＰをＨＰケージに反映
		g_Battle_Player.fRecHP = SetMAX(g_Battle_Player.fRecHP,
										g_Battle_Player.fMaxHP,
										0.0f,2);
	}
	{//MPケージの処理
		switch(g_Battle_Player.MP_Type)
		{
		case MP_TYPE_KEEPING:
			break;
		case MP_TYPE_LOSING:
			SetFlash(FLASH_FLASHING,1);
			g_Battle_Player.fRecMP -= g_Battle_Player.fMP_Per_Time;
			if(g_Battle_Player.fRecMP <= g_Battle_Player.fDestMP)
			{
				g_Battle_Player.fDestMP = g_Battle_Player.fDestMP;
				g_Battle_Player.MP_Type = MP_TYPE_KEEPING;
			}
			if(g_Battle_Player.fRecMP <= 0.0f)//0になったらfadout に移行
			{
				g_Battle_Player.fRecMP = 0.0f;
				g_Battle_Player.MP_Type = MP_TYPE_KEEPING;
			}
			break;
		case MP_TYPE_RECOVER:
			g_Battle_Player.fRecMP += g_Battle_Player.fMP_Per_Time;
			if(g_Battle_Player.fRecMP >= g_Battle_Player.fDestMP)
			{
				g_Battle_Player.fRecMP = g_Battle_Player.fDestMP;
				g_Battle_Player.MP_Type = MP_TYPE_KEEPING;
			}
			break;
		case MP_TYPE_FADE_OUT:
			break;
		}
		//現在の敵のＨＰをＨＰケージに反映
		g_Battle_Player.fRecMP = SetMAX(g_Battle_Player.fRecMP,
										g_Battle_Player.fMaxMP,
										0.0f,2);
	}
	for(int nCntMember = 0;nCntMember < TEAM_PLAYER_MAX;nCntMember++)
	{
		SetVerTexBattle_T_Member(nCntMember);
		SetVerTexBattle_T_Member_OutLine(nCntMember);
		SetVerTexBattle_Member_Skill_Turn(nCntMember);
	}

}
//============================================================================
//PlayerHPの変化
//============================================================================
void SetBattle_PlayerHP_Change(int HP_Type,float Damage)
{
	float DestBuff = 0.0f;//ＨＰ減少バッファ変数
	g_Battle_Player.HP_Type = HP_Type;
	DestBuff = g_Battle_Player.fDestHP;
	if((int)g_Battle_Player.fRecHP != (int)g_Battle_Player.fDestHP)//連続攻撃を受けっていると
		g_Battle_Player.fDestHP = g_Battle_Player.fDestHP - Damage;
	else
	{
	g_Battle_Player.fDestHP = g_Battle_Player.fRecHP - Damage;
	}
	g_Battle_Player.fDestHP = SetMAX(g_Battle_Player.fDestHP,g_Battle_Player.fMaxHP,-1,2);

	g_Battle_Player.fHP_Per_Time = abs(g_Battle_Player.fDestHP - g_Battle_Player.fRecHP)/VALUE_TIME_CHANGE;
}
//============================================================================
//PlayerMPの変化
//============================================================================
void SetBattle_PlayerMP_Recover(int MP_Type,float Recover)
{
	float DestBuff = 0.0f;//MP減少バッファ変数
	g_Battle_Player.MP_Type = MP_Type;
	DestBuff = g_Battle_Player.fDestMP;
	if(g_Battle_Player.fRecMP != g_Battle_Player.fDestMP)
		g_Battle_Player.fDestMP = g_Battle_Player.fDestMP - Recover;
	else
	{
	g_Battle_Player.fDestMP = g_Battle_Player.fRecMP - Recover;
	}
	g_Battle_Player.fDestMP = SetMAX(g_Battle_Player.fDestMP,g_Battle_Player.fMaxMP,-1,2);

	g_Battle_Player.fMP_Per_Time = abs(g_Battle_Player.fDestMP - g_Battle_Player.fRecMP)/VALUE_TIME_CHANGE;
}
//=============================================================================
// 描画処理
//=============================================================================
void DrawBattle_Player(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//skillLightの描画
	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffBattle_T_Member_Light, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);


	for(int nCntMember = 0;nCntMember < TEAM_PLAYER_MAX;nCntMember++)
	{
		if(g_BattleT_Memeber[nCntMember].bUse == false)// || g_BattleT_Memeber[nCntMember].bSkillUse == false)
			continue;
		//背景
		pDevice->SetTexture(0,NULL);
		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntMember * 8), NUM_POLYGON);

		if(g_BattleT_Memeber[nCntMember].bSkillUse == true)
		pDevice->SetTexture(0,g_pTextureBattle_T_Member_Light);
		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntMember * 8 + 4), NUM_POLYGON);

	}

	//フィールドの描画
	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffBattle_T_Member, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);


	for(int nCntMember = 0;nCntMember < TEAM_PLAYER_MAX;nCntMember++)
	{
		if(g_BattleT_Memeber[nCntMember].bUse == false)
			continue;
		pDevice->SetTexture(0,g_pTextureBattle_T_Member[g_BattleT_Memeber[nCntMember].MonsterID]);
		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntMember * 8), NUM_POLYGON);

		pDevice->SetTexture(0,NULL);
		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntMember * 8 + 4), NUM_POLYGON);

	}


	//枠組みの描画
	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffBattle_T_Member_OutLine, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);


	for(int nCntMember = 0;nCntMember < TEAM_PLAYER_MAX;nCntMember++)
	{
		if(g_BattleT_Memeber[nCntMember].bUse == false)
			continue;
		pDevice->SetTexture(0,g_pTextureBattle_T_Member_OutLine);
		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntMember * 8), NUM_POLYGON);

		pDevice->SetTexture(0,g_pTextureBattle_T_Member_Ball);
		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntMember * 8 + 4), NUM_POLYGON);

	}



	//pDevice->SetStreamSource(0, g_pVtxBuffBattle_Member_Skill_Turn, 0, sizeof(VERTEX_2D));

	//// 頂点フォーマットの設定
	//pDevice->SetFVF(FVF_VERTEX_2D);


	//for(int nCntMember = 0;nCntMember < TEAM_PLAYER_MAX;nCntMember++)
	//{
	//	if(g_BattleT_Memeber[nCntMember].bUse == false)
	//		continue;
	//	pDevice->SetTexture(0,g_pTextureBattle_Member_Skill_Turn);
	//	// ポリゴンの描画
	//	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntMember * 16), NUM_POLYGON);// ボタン

	//	if(g_BattleT_Memeber[nCntMember].bSkillUse == true)
	//	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntMember * 16 + 4), NUM_POLYGON);// 光る文字

	//	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntMember * 16 + 8), NUM_POLYGON);// あと　ターン
	//	pDevice->SetTexture(0,g_pTextureBattle_Member_Skill_Turn_Num);
	//	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntMember * 16 + 12), NUM_POLYGON);// 数字

	//}
}
//=============================================================================
// スキルターン数カウントダウン
//=============================================================================
HRESULT MakeVertexBattle_Member_Skill_Turn(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * TEAM_PLAYER_MAX*4,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
		FVF_VERTEX_2D,						// 使用する頂点フォーマット
		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
		&g_pVtxBuffBattle_Member_Skill_Turn,					// 頂点バッファインターフェースへのポインタ
		NULL)))								// NULLに設定
	{
		return E_FAIL;
	}
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffBattle_Member_Skill_Turn->Lock(0, 0, (void**)&pVtx, 0);

		for(int nCntMember = 0;nCntMember < TEAM_PLAYER_MAX;nCntMember++,pVtx += 4)	
		{
			//===============================スキルボタン===========================================//
			// 頂点座標の設定
			pVtx[0].pos.x = SkillCountPos[nCntMember].x;
			pVtx[0].pos.y = SkillCountPos[nCntMember].y - PLAYER_SKILL_TURN_SIZE_Y;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = SkillCountPos[nCntMember].x + PLAYER_SKILL_TURN_SIZE_X;
			pVtx[1].pos.y = SkillCountPos[nCntMember].y - PLAYER_SKILL_TURN_SIZE_Y;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = SkillCountPos[nCntMember].x;
			pVtx[2].pos.y = SkillCountPos[nCntMember].y;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = SkillCountPos[nCntMember].x + PLAYER_SKILL_TURN_SIZE_X;
			pVtx[3].pos.y = SkillCountPos[nCntMember].y;
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
			pVtx[2].tex = D3DXVECTOR2(0.0f,0.25f);
			pVtx[3].tex = D3DXVECTOR2(1.0f,0.25f);

			//===============================スキル文字===========================================//
			pVtx += 4;
			// 頂点座標の設定
			pVtx[0].pos.x = SkillCountPos[nCntMember].x;
			pVtx[0].pos.y = SkillCountPos[nCntMember].y - PLAYER_SKILL_TURN_SIZE_Y*1.5f;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = SkillCountPos[nCntMember].x + PLAYER_SKILL_TURN_SIZE_X;
			pVtx[1].pos.y = SkillCountPos[nCntMember].y - PLAYER_SKILL_TURN_SIZE_Y*1.5f;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = SkillCountPos[nCntMember].x;
			pVtx[2].pos.y = SkillCountPos[nCntMember].y + PLAYER_SKILL_TURN_SIZE_Y*0.5f;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = SkillCountPos[nCntMember].x + PLAYER_SKILL_TURN_SIZE_X;
			pVtx[3].pos.y = SkillCountPos[nCntMember].y + PLAYER_SKILL_TURN_SIZE_Y*0.5f;
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

			pVtx[0].tex = D3DXVECTOR2(0.0f,0.25f);
			pVtx[1].tex = D3DXVECTOR2(1.0f,0.25f);
			pVtx[2].tex = D3DXVECTOR2(0.0f,0.75f);
			pVtx[3].tex = D3DXVECTOR2(1.0f,0.75f);
			//===============================スキル残りターン数===========================================//
			pVtx += 4;
			// 頂点座標の設定
			pVtx[0].pos.x = SkillCountPos[nCntMember].x;
			pVtx[0].pos.y = SkillCountPos[nCntMember].y;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = SkillCountPos[nCntMember].x + PLAYER_SKILL_TURN_SIZE_X;
			pVtx[1].pos.y = SkillCountPos[nCntMember].y;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = SkillCountPos[nCntMember].x;
			pVtx[2].pos.y = SkillCountPos[nCntMember].y + PLAYER_SKILL_TURN_SIZE_Y;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = SkillCountPos[nCntMember].x + PLAYER_SKILL_TURN_SIZE_X;
			pVtx[3].pos.y = SkillCountPos[nCntMember].y + PLAYER_SKILL_TURN_SIZE_Y;
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

			pVtx[0].tex = D3DXVECTOR2(0.0f,0.75f);
			pVtx[1].tex = D3DXVECTOR2(1.0f,0.75f);
			pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);
			//===============================残りターン数数字===========================================//
			pVtx += 4;
			// 頂点座標の設定
			pVtx[0].pos.x = SkillCountPos[nCntMember].x + PLAYER_SKILL_TURN_SIZE_X * 0.4f;
			pVtx[0].pos.y = SkillCountPos[nCntMember].y;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = SkillCountPos[nCntMember].x +  PLAYER_SKILL_TURN_SIZE_X * 0.6f;
			pVtx[1].pos.y = SkillCountPos[nCntMember].y;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = SkillCountPos[nCntMember].x	+ PLAYER_SKILL_TURN_SIZE_X * 0.4f;;
			pVtx[2].pos.y = SkillCountPos[nCntMember].y + PLAYER_SKILL_TURN_SIZE_Y;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = SkillCountPos[nCntMember].x + PLAYER_SKILL_TURN_SIZE_X * 0.6f;
			pVtx[3].pos.y = SkillCountPos[nCntMember].y + PLAYER_SKILL_TURN_SIZE_Y;
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

			int nbox = g_BattleT_Memeber[nCntMember].TurnLeft;
			pVtx[0].tex = D3DXVECTOR2(0.1f,0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f * nbox + 0.1f,0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.1f,1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f * nbox + 0.1f,1.0f);

		}
		// 頂点データをアンロックする
		g_pVtxBuffBattle_Member_Skill_Turn->Unlock();
	}
	return S_OK;

}
void SetVerTexBattle_Member_Skill_Turn(int MemberID)
{//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffBattle_Member_Skill_Turn->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += MemberID * 16;
	pVtx += 0;//ボタン
			pVtx[0].pos.x = SkillCountPos[MemberID].x;
			pVtx[0].pos.y = SkillCountPos[MemberID].y - PLAYER_SKILL_TURN_SIZE_Y;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = SkillCountPos[MemberID].x + PLAYER_SKILL_TURN_SIZE_X;
			pVtx[1].pos.y = SkillCountPos[MemberID].y - PLAYER_SKILL_TURN_SIZE_Y;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = SkillCountPos[MemberID].x;
			pVtx[2].pos.y = SkillCountPos[MemberID].y;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = SkillCountPos[MemberID].x + PLAYER_SKILL_TURN_SIZE_X;
			pVtx[3].pos.y = SkillCountPos[MemberID].y;
			pVtx[3].pos.z = 0.0f;

			pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f,0.25f);
			pVtx[3].tex = D3DXVECTOR2(1.0f,0.25f);
	pVtx += 4;//エフェクト文字
			pVtx[0].pos.x = SkillCountPos[MemberID].x;
			pVtx[0].pos.y = SkillCountPos[MemberID].y - PLAYER_SKILL_TURN_SIZE_Y*1.5f;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = SkillCountPos[MemberID].x + PLAYER_SKILL_TURN_SIZE_X;
			pVtx[1].pos.y = SkillCountPos[MemberID].y - PLAYER_SKILL_TURN_SIZE_Y*1.5f;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = SkillCountPos[MemberID].x;
			pVtx[2].pos.y = SkillCountPos[MemberID].y + PLAYER_SKILL_TURN_SIZE_Y*0.5f;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = SkillCountPos[MemberID].x + PLAYER_SKILL_TURN_SIZE_X;
			pVtx[3].pos.y = SkillCountPos[MemberID].y + PLAYER_SKILL_TURN_SIZE_Y*0.5f;
			pVtx[3].pos.z = 0.0f;


			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_SkillBottomLight);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_SkillBottomLight);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_SkillBottomLight);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_SkillBottomLight);

			pVtx[0].tex = D3DXVECTOR2(0.0f,0.25f);
			pVtx[1].tex = D3DXVECTOR2(1.0f,0.25f);
			pVtx[2].tex = D3DXVECTOR2(0.0f,0.75f);
			pVtx[3].tex = D3DXVECTOR2(1.0f,0.75f);

	pVtx += 4;//文字列
			pVtx[0].pos.x = SkillCountPos[MemberID].x;
			pVtx[0].pos.y = SkillCountPos[MemberID].y + PLAYER_SKILL_TURN_SIZE_Y * 0.2f;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = SkillCountPos[MemberID].x + PLAYER_SKILL_TURN_SIZE_X;
			pVtx[1].pos.y = SkillCountPos[MemberID].y + PLAYER_SKILL_TURN_SIZE_Y * 0.2f;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = SkillCountPos[MemberID].x;
			pVtx[2].pos.y = SkillCountPos[MemberID].y + PLAYER_SKILL_TURN_SIZE_Y + PLAYER_SKILL_TURN_SIZE_Y * 0.2f;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = SkillCountPos[MemberID].x + PLAYER_SKILL_TURN_SIZE_X;
			pVtx[3].pos.y = SkillCountPos[MemberID].y + PLAYER_SKILL_TURN_SIZE_Y + PLAYER_SKILL_TURN_SIZE_Y * 0.2f;
			pVtx[3].pos.z = 0.0f;

			pVtx[0].tex = D3DXVECTOR2(0.0f,0.75f);
			pVtx[1].tex = D3DXVECTOR2(1.0f,0.75f);
			pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

	pVtx += 4;//数字
			pVtx[0].pos.x = SkillCountPos[MemberID].x + PLAYER_SKILL_TURN_SIZE_X * 0.3f;
			pVtx[0].pos.y = SkillCountPos[MemberID].y;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = SkillCountPos[MemberID].x +  PLAYER_SKILL_TURN_SIZE_X * 0.5f;
			pVtx[1].pos.y = SkillCountPos[MemberID].y;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = SkillCountPos[MemberID].x + PLAYER_SKILL_TURN_SIZE_X * 0.3f;;
			pVtx[2].pos.y = SkillCountPos[MemberID].y + PLAYER_SKILL_TURN_SIZE_Y;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = SkillCountPos[MemberID].x + PLAYER_SKILL_TURN_SIZE_X * 0.5f;
			pVtx[3].pos.y = SkillCountPos[MemberID].y + PLAYER_SKILL_TURN_SIZE_Y;
			pVtx[3].pos.z = 0.0f;


			int nbox = g_BattleT_Memeber[MemberID].TurnLeft;
			pVtx[0].tex = D3DXVECTOR2(0.1f * nbox,0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f * nbox + 0.1f,0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.1f * nbox,1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f * nbox + 0.1f,1.0f);
	g_pVtxBuffBattle_Member_Skill_Turn->Unlock();
}
//=============================================================================
// Light頂点の作成
//=============================================================================
HRESULT MakeVertexBattle_T_Member_Light(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * TEAM_PLAYER_MAX * 2,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
		FVF_VERTEX_2D,						// 使用する頂点フォーマット
		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
		&g_pVtxBuffBattle_T_Member_Light,					// 頂点バッファインターフェースへのポインタ
		NULL)))								// NULLに設定
	{
		return E_FAIL;
	}
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffBattle_T_Member_Light->Lock(0, 0, (void**)&pVtx, 0);

		for(int nCntMember = 0;nCntMember < TEAM_PLAYER_MAX;nCntMember++,pVtx += 4)	
		{
			pVtx[0].pos.x = g_BattleT_Memeber[nCntMember].pos.x;
			pVtx[0].pos.y = g_BattleT_Memeber[nCntMember].pos.y - BATTLE_TEAM_SIZE;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_BattleT_Memeber[nCntMember].pos.x + BATTLE_TEAM_SIZE;
			pVtx[1].pos.y = g_BattleT_Memeber[nCntMember].pos.y - BATTLE_TEAM_SIZE;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_BattleT_Memeber[nCntMember].pos.x;
			pVtx[2].pos.y = g_BattleT_Memeber[nCntMember].pos.y;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_BattleT_Memeber[nCntMember].pos.x + BATTLE_TEAM_SIZE;
			pVtx[3].pos.y = g_BattleT_Memeber[nCntMember].pos.y;
			pVtx[3].pos.z = 0.0f;
			// rhwの設定
			pVtx[0].rhw =
				pVtx[1].rhw =
				pVtx[2].rhw =
				pVtx[3].rhw = 1.0f;

			// 頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);


			pVtx += 4;

			// 頂点座標の設定
			pVtx[0].pos.x = g_BattleT_Memeber[nCntMember].pos.x - BATTLE_TEAM_SIZE*0.09;
			pVtx[0].pos.y = g_BattleT_Memeber[nCntMember].pos.y - BATTLE_TEAM_SIZE;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_BattleT_Memeber[nCntMember].pos.x + BATTLE_TEAM_SIZE + BATTLE_TEAM_SIZE*0.09;
			pVtx[1].pos.y = g_BattleT_Memeber[nCntMember].pos.y - BATTLE_TEAM_SIZE;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_BattleT_Memeber[nCntMember].pos.x - BATTLE_TEAM_SIZE*0.09;
			pVtx[2].pos.y = g_BattleT_Memeber[nCntMember].pos.y + BATTLE_TEAM_SIZE*0.09;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_BattleT_Memeber[nCntMember].pos.x + BATTLE_TEAM_SIZE + BATTLE_TEAM_SIZE*0.09;
			pVtx[3].pos.y = g_BattleT_Memeber[nCntMember].pos.y + BATTLE_TEAM_SIZE*0.09;
			pVtx[3].pos.z = 0.0f;
			// rhwの設定
			pVtx[0].rhw =
				pVtx[1].rhw =
				pVtx[2].rhw =
				pVtx[3].rhw = 1.0f;

			// 頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_BattleT_Memeber[nCntMember].Light_Up);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_BattleT_Memeber[nCntMember].Light_Up);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_BattleT_Memeber[nCntMember].Light_Up);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_BattleT_Memeber[nCntMember].Light_Up);

			pVtx[0].tex = D3DXVECTOR2(0.0f,0.1f);
			pVtx[1].tex = D3DXVECTOR2(1.0f,0.1f);
			pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

		}
		// 頂点データをアンロックする
		g_pVtxBuffBattle_T_Member_Light->Unlock();
	}
	return S_OK;

}
void SetVerTexBattle_T_Member_Light(int MemberID)
{
	//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffBattle_T_Member_Light->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += MemberID*8;
	float NockD = 0.0f;
	switch(MemberID)
	{
	case 0:
		NockD = 1.0f;
		break;
	case 1:
		NockD = 1.4;
		break;
	case 2:
		NockD = 1.8;
		break;
	case 3:
		NockD = 1.4;
		break;
	case 4:
		NockD = 1.0f;
		break;
	}
	pVtx[0].pos.x = g_BattleT_Memeber[MemberID].pos.x;
	pVtx[0].pos.y = g_BattleT_Memeber[MemberID].pos.y - BATTLE_TEAM_SIZE + g_NockBack*NockD;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_BattleT_Memeber[MemberID].pos.x + BATTLE_TEAM_SIZE;
	pVtx[1].pos.y = g_BattleT_Memeber[MemberID].pos.y - BATTLE_TEAM_SIZE + g_NockBack*NockD;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_BattleT_Memeber[MemberID].pos.x;
	pVtx[2].pos.y = g_BattleT_Memeber[MemberID].pos.y + g_NockBack*NockD;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_BattleT_Memeber[MemberID].pos.x + BATTLE_TEAM_SIZE;
	pVtx[3].pos.y = g_BattleT_Memeber[MemberID].pos.y + g_NockBack*NockD;
	pVtx[3].pos.z = 0.0f;

	pVtx += 4;
	pVtx[0].pos.x = g_BattleT_Memeber[MemberID].pos.x - BATTLE_TEAM_SIZE*0.09;
	pVtx[0].pos.y = g_BattleT_Memeber[MemberID].pos.y - BATTLE_TEAM_SIZE + g_NockBack*NockD;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_BattleT_Memeber[MemberID].pos.x + BATTLE_TEAM_SIZE + BATTLE_TEAM_SIZE*0.09;
	pVtx[1].pos.y = g_BattleT_Memeber[MemberID].pos.y - BATTLE_TEAM_SIZE + g_NockBack*NockD;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_BattleT_Memeber[MemberID].pos.x - BATTLE_TEAM_SIZE*0.09;
	pVtx[2].pos.y = g_BattleT_Memeber[MemberID].pos.y + BATTLE_TEAM_SIZE*0.09 + g_NockBack*NockD;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_BattleT_Memeber[MemberID].pos.x + BATTLE_TEAM_SIZE + BATTLE_TEAM_SIZE*0.09;
	pVtx[3].pos.y = g_BattleT_Memeber[MemberID].pos.y + BATTLE_TEAM_SIZE*0.09 + g_NockBack*NockD;
	pVtx[3].pos.z = 0.0f;

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_BattleT_Memeber[MemberID].Light_Up -0.1f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_BattleT_Memeber[MemberID].Light_Up -0.1f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_BattleT_Memeber[MemberID].Light_Up);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_BattleT_Memeber[MemberID].Light_Up);

	// 頂点データをアンロックする
	g_pVtxBuffBattle_T_Member_Light->Unlock();
}

HRESULT MakeVertexBattle_T_Member(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * TEAM_PLAYER_MAX * 2,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
		FVF_VERTEX_2D,						// 使用する頂点フォーマット
		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
		&g_pVtxBuffBattle_T_Member,					// 頂点バッファインターフェースへのポインタ
		NULL)))								// NULLに設定
	{
		return E_FAIL;
	}
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffBattle_T_Member->Lock(0, 0, (void**)&pVtx, 0);

		for(int nCntMember = 0;nCntMember < TEAM_PLAYER_MAX;nCntMember++,pVtx += 4)	
		{
			// 頂点座標の設定
			pVtx[0].pos.x = g_BattleT_Memeber[nCntMember].pos.x;
			pVtx[0].pos.y = g_BattleT_Memeber[nCntMember].pos.y - BATTLE_TEAM_SIZE;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_BattleT_Memeber[nCntMember].pos.x + BATTLE_TEAM_SIZE;
			pVtx[1].pos.y = g_BattleT_Memeber[nCntMember].pos.y - BATTLE_TEAM_SIZE;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_BattleT_Memeber[nCntMember].pos.x;
			pVtx[2].pos.y = g_BattleT_Memeber[nCntMember].pos.y;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_BattleT_Memeber[nCntMember].pos.x + BATTLE_TEAM_SIZE;
			pVtx[3].pos.y = g_BattleT_Memeber[nCntMember].pos.y;
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

			pVtx += 4;

			// 頂点座標の設定
			pVtx[0].pos.x = g_BattleT_Memeber[nCntMember].pos.x;
			pVtx[0].pos.y = g_BattleT_Memeber[nCntMember].pos.y - BATTLE_TEAM_SIZE;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_BattleT_Memeber[nCntMember].pos.x + BATTLE_TEAM_SIZE;
			pVtx[1].pos.y = g_BattleT_Memeber[nCntMember].pos.y - BATTLE_TEAM_SIZE;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_BattleT_Memeber[nCntMember].pos.x;
			pVtx[2].pos.y = g_BattleT_Memeber[nCntMember].pos.y;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_BattleT_Memeber[nCntMember].pos.x + BATTLE_TEAM_SIZE;
			pVtx[3].pos.y = g_BattleT_Memeber[nCntMember].pos.y;
			pVtx[3].pos.z = 0.0f;
			// rhwの設定
			pVtx[0].rhw =
				pVtx[1].rhw =
				pVtx[2].rhw =
				pVtx[3].rhw = 1.0f;

			// 頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f);

		}
		// 頂点データをアンロックする
		g_pVtxBuffBattle_T_Member->Unlock();
	}
	return S_OK;

}
HRESULT MakeVertexBattle_T_Member_OutLine(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * TEAM_PLAYER_MAX * 2,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
		FVF_VERTEX_2D,						// 使用する頂点フォーマット
		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
		&g_pVtxBuffBattle_T_Member_OutLine,					// 頂点バッファインターフェースへのポインタ
		NULL)))								// NULLに設定
	{
		return E_FAIL;
	}
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffBattle_T_Member_OutLine->Lock(0, 0, (void**)&pVtx, 0);

		for(int nCntMember = 0;nCntMember < TEAM_PLAYER_MAX;nCntMember++,pVtx += 4)	
		{
			// 頂点座標の設定
			pVtx[0].pos.x = g_BattleT_Memeber[nCntMember].pos.x;
			pVtx[0].pos.y = g_BattleT_Memeber[nCntMember].pos.y - BATTLE_TEAM_SIZE;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_BattleT_Memeber[nCntMember].pos.x + BATTLE_TEAM_SIZE;
			pVtx[1].pos.y = g_BattleT_Memeber[nCntMember].pos.y - BATTLE_TEAM_SIZE;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_BattleT_Memeber[nCntMember].pos.x;
			pVtx[2].pos.y = g_BattleT_Memeber[nCntMember].pos.y;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_BattleT_Memeber[nCntMember].pos.x + BATTLE_TEAM_SIZE;
			pVtx[3].pos.y = g_BattleT_Memeber[nCntMember].pos.y;
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

			pVtx += 4;
			pVtx[0].pos.x = g_BattleT_Memeber[nCntMember].pos.x;
			pVtx[0].pos.y = g_BattleT_Memeber[nCntMember].pos.y - BATTLE_TEAM_SIZE;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_BattleT_Memeber[nCntMember].pos.x + BATTLE_TEAM_SIZE;
			pVtx[1].pos.y = g_BattleT_Memeber[nCntMember].pos.y - BATTLE_TEAM_SIZE;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_BattleT_Memeber[nCntMember].pos.x;
			pVtx[2].pos.y = g_BattleT_Memeber[nCntMember].pos.y;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_BattleT_Memeber[nCntMember].pos.x + BATTLE_TEAM_SIZE;
			pVtx[3].pos.y = g_BattleT_Memeber[nCntMember].pos.y;
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
		g_pVtxBuffBattle_T_Member_OutLine->Unlock();
	}
	return S_OK;

}
//=============================================================================
// Lightnの設定
//=============================================================================
void SetVerTexBattle_T_Member(int MemberID)
{
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffBattle_T_Member->Lock(0, 0, (void**)&pVtx, 0);
	// 頂点座標の設定
	pVtx += (MemberID *8);
	// 頂点座標の設定
	float NockD = 0.0f;
	switch(MemberID)
	{
	case 0:
		NockD = 1.0f;
		break;
	case 1:
		NockD = 1.4;
		break;
	case 2:
		NockD = 1.8;
		break;
	case 3:
		NockD = 1.4;
		break;
	case 4:
		NockD = 1.0f;
		break;
	}

	pVtx[0].pos.x = g_BattleT_Memeber[MemberID].pos.x;
	pVtx[0].pos.y = g_BattleT_Memeber[MemberID].pos.y - BATTLE_TEAM_SIZE + g_NockBack*NockD;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_BattleT_Memeber[MemberID].pos.x + BATTLE_TEAM_SIZE;
	pVtx[1].pos.y = g_BattleT_Memeber[MemberID].pos.y - BATTLE_TEAM_SIZE + g_NockBack*NockD;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_BattleT_Memeber[MemberID].pos.x;
	pVtx[2].pos.y = g_BattleT_Memeber[MemberID].pos.y + g_NockBack*NockD;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_BattleT_Memeber[MemberID].pos.x + BATTLE_TEAM_SIZE;
	pVtx[3].pos.y = g_BattleT_Memeber[MemberID].pos.y + g_NockBack*NockD;
	pVtx[3].pos.z = 0.0f;

	// rhwの設定		
	pVtx[0].rhw =
		pVtx[1].rhw =
		pVtx[2].rhw =
		pVtx[3].rhw = 1.0f;
	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f,1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f,1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f,1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f,1.0f);


	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

	//redFlash
	pVtx += 4;
	pVtx[0].pos.x = g_BattleT_Memeber[MemberID].pos.x;
	pVtx[0].pos.y = g_BattleT_Memeber[MemberID].pos.y - BATTLE_TEAM_SIZE + g_NockBack*NockD;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_BattleT_Memeber[MemberID].pos.x + BATTLE_TEAM_SIZE;
	pVtx[1].pos.y = g_BattleT_Memeber[MemberID].pos.y - BATTLE_TEAM_SIZE + g_NockBack*NockD;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_BattleT_Memeber[MemberID].pos.x;
	pVtx[2].pos.y = g_BattleT_Memeber[MemberID].pos.y + g_NockBack*NockD;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_BattleT_Memeber[MemberID].pos.x + BATTLE_TEAM_SIZE;
	pVtx[3].pos.y = g_BattleT_Memeber[MemberID].pos.y + g_NockBack*NockD;
	pVtx[3].pos.z = 0.0f;

	// rhwの設定		
	pVtx[0].rhw =
		pVtx[1].rhw =
		pVtx[2].rhw =
		pVtx[3].rhw = 1.0f;
	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f,g_Member_Red_TexAlpha);
	pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f,g_Member_Red_TexAlpha);
	pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f,g_Member_Red_TexAlpha);
	pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f,g_Member_Red_TexAlpha);

	// 頂点データをアンロックする
	g_pVtxBuffBattle_T_Member->Unlock();

}
void SetVerTexBattle_T_Member_OutLine(int MemberID)
{
	float NockD = 0.0f;
	switch(MemberID)
	{
	case 0:
		NockD = 1.0f;
		break;
	case 1:
		NockD = 1.4;
		break;
	case 2:
		NockD = 1.8;
		break;
	case 3:
		NockD = 1.4;
		break;
	case 4:
		NockD = 1.0f;
		break;
	}
	int color = g_BattleT_Memeber[MemberID].Color;
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffBattle_T_Member_OutLine->Lock(0, 0, (void**)&pVtx, 0);
	// 頂点座標の設定
	pVtx += (MemberID *8);
	// 頂点座標の設定
	pVtx[0].pos.x = g_BattleT_Memeber[MemberID].pos.x - BATTLE_TEAM_SIZE*0.09;
	pVtx[0].pos.y = g_BattleT_Memeber[MemberID].pos.y - BATTLE_TEAM_SIZE - BATTLE_TEAM_SIZE*0.09 + g_NockBack*NockD;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_BattleT_Memeber[MemberID].pos.x + BATTLE_TEAM_SIZE + BATTLE_TEAM_SIZE*0.09;
	pVtx[1].pos.y = g_BattleT_Memeber[MemberID].pos.y - BATTLE_TEAM_SIZE - BATTLE_TEAM_SIZE*0.09 + g_NockBack*NockD;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_BattleT_Memeber[MemberID].pos.x - BATTLE_TEAM_SIZE*0.09;
	pVtx[2].pos.y = g_BattleT_Memeber[MemberID].pos.y + BATTLE_TEAM_SIZE*0.09 + g_NockBack*NockD;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_BattleT_Memeber[MemberID].pos.x + BATTLE_TEAM_SIZE + BATTLE_TEAM_SIZE*0.09;
	pVtx[3].pos.y = g_BattleT_Memeber[MemberID].pos.y + BATTLE_TEAM_SIZE*0.09 + g_NockBack*NockD;
	pVtx[3].pos.z = 0.0f;

	// rhwの設定		
	pVtx[0].rhw =
		pVtx[1].rhw =
		pVtx[2].rhw =
		pVtx[3].rhw = 1.0f;
	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f,1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f,1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f,1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f,1.0f);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(TEXTURE_PLAYER_OUTLINE_UNIT_SIZE * color,0.0f);
	pVtx[1].tex = D3DXVECTOR2(TEXTURE_PLAYER_OUTLINE_UNIT_SIZE * color + TEXTURE_PLAYER_OUTLINE_UNIT_SIZE,0.0f);
	pVtx[2].tex = D3DXVECTOR2(TEXTURE_PLAYER_OUTLINE_UNIT_SIZE * color,1.0f);
	pVtx[3].tex = D3DXVECTOR2(TEXTURE_PLAYER_OUTLINE_UNIT_SIZE * color + TEXTURE_PLAYER_OUTLINE_UNIT_SIZE,1.0f);

	pVtx += 4;
	D3DXVECTOR3 pos = g_BattleT_Memeber[MemberID].pos;
	pos.x += BATTLE_TEAM_SIZE *0.8;
	pos.y += BATTLE_TEAM_SIZE * 0.03f;
	pVtx[0].pos.x = pos.x;
	pVtx[0].pos.y = pos.y - BATTLE_TEAM_SIZE * 0.2 + g_NockBack*NockD;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = pos.x + BATTLE_TEAM_SIZE * 0.2;
	pVtx[1].pos.y = pos.y - BATTLE_TEAM_SIZE * 0.2 + g_NockBack*NockD;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = pos.x;
	pVtx[2].pos.y = pos.y + g_NockBack*NockD;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = pos.x + BATTLE_TEAM_SIZE * 0.2;;
	pVtx[3].pos.y = pos.y + g_NockBack*NockD;
	pVtx[3].pos.z = 0.0f;

	// rhwの設定		
	pVtx[0].rhw =
		pVtx[1].rhw =
		pVtx[2].rhw =
		pVtx[3].rhw = 1.0f;
	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f,1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f,1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f,1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f,1.0f);


	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(TEXTURE_PLAYER_OUTLINE_UNIT_SIZE * color,0.0f);
	pVtx[1].tex = D3DXVECTOR2(TEXTURE_PLAYER_OUTLINE_UNIT_SIZE * color + TEXTURE_PLAYER_OUTLINE_UNIT_SIZE,0.0f);
	pVtx[2].tex = D3DXVECTOR2(TEXTURE_PLAYER_OUTLINE_UNIT_SIZE * color,1.0f);
	pVtx[3].tex = D3DXVECTOR2(TEXTURE_PLAYER_OUTLINE_UNIT_SIZE * color + TEXTURE_PLAYER_OUTLINE_UNIT_SIZE,1.0f);
	// 頂点データをアンロックする
	g_pVtxBuffBattle_T_Member_OutLine->Unlock();
}

//===================================================
//データの取得
//===================================================
BATTLE_T_MEMBER *GetTMember(void)
{
	return &g_BattleT_Memeber[0];
}
BATTLE_PLAYER *GetBattlePlayer(void)
{
	return &g_Battle_Player;
}
int	GetBattle_playerHP(void)
{
	return g_Battle_Player.fRecHP;
}
int GetBattle_Player_MP_Type(void)
{
	return g_Battle_Player.MP_Type;
}


void ResetBattleMemberBaseDamage(void)
{
	//チームメンバーの攻撃データを0にする
	for(int nCntMember = 0;nCntMember < TEAM_PLAYER_MAX;nCntMember++)
	{
		g_BattleT_Memeber[nCntMember].DamageBase = 0;
	}
}
void SetMemberNockBack(float value)
{
	g_NockBack += value;

	g_Member_Red_TexAlpha = 1.0f;
}
