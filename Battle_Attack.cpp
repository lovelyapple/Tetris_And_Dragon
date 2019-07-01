
//=============================================================================
// ブロック破壊効果 [Battle_Attack.cpp]
// HAL 名古屋　GP11A172 17 50323 リ リン
//=============================================================================
#include "Battle_Attack.h"
#include "Battle_Enemy.h"
#include "Battle_Player.h"
#include "input.h"
#include "Damage_Number.h"
#include "Battle_Effect_Attack.h"
#include "Battle_Player_HP.h"
#include "sound.h"
//*****************************************************************************
//使用注意
//*****************************************************************************

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define		MAX_BATTLE_ATTACK			(512)
#define		BATTLE_ATTACK_SIZE			(20.0f)
#define		ATTACK_LIFE					(20)		//停留時間
const char *c_aFileNameAttack[ATTACKE_MAX_ATTIBUTION] =
{
	"data/TEXTURE/star_red.png",	
	"data/TEXTURE/star_green.png",	
	"data/TEXTURE/star_blue.png",
	"data/TEXTURE/star_light.png",
	"data/TEXTURE/star_purple.png",	
};
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexBattle_Attack(LPDIRECT3DDEVICE9 pDevice);
void SetVerTexBattle_Attack(int Attackn_ID);
void InitBattleAtteck(int Attack_ID);
//*****************************************************************************
// 構造体宣言
//*****************************************************************************
typedef enum
{
	ATTACK_START = 0,		//開始点
	ATTACK_END,				//終点
	ATTACK_MGR_A,			//制御点Ａ
	ATTACK_MGR_B,			//制御点Ｂ
	ATTACK_MAX_POS,
	ATTACK_TARGET_PLAYER,			//プレイヤーの攻撃
	ATTACK_ENEMY,			//敵の攻撃
}BATTLE_ATTACK_POS;//4つの点の座標名称

typedef struct
{
	bool	bUse;
	int		from;
	int		team;
	D3DXVECTOR3 DrawPos;
	D3DXVECTOR3	Move;
	D3DXVECTOR3 Pos[ATTACK_MAX_POS];
	int		nColor;
	int		nLife;
	float	fLength;
	int		Target;
	float	Damage;
	float	fSpeed;
	float	fSize;
	float	rot;
}BATTLE_ATTACK;


//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureBattle_Attack[ATTACKE_MAX_ATTIBUTION]	= {};	//テクスチャメモリ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBattle_Attack = NULL;	//頂点座標バッファ

BATTLE_ATTACK	g_Attack[MAX_BATTLE_ATTACK];
BATTLE_ENEMY *pBattleEnemy = GetBattleEnemy();
DAMAGE_STRUCT g_Damage[ATTACKE_MAX_ATTIBUTION];
int SumAttackTime;//攻撃終了までの総時間
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBattle_Attack(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	SumAttackTime = 0;
	for(int nCntAttack = 0; nCntAttack < MAX_BATTLE_ATTACK; nCntAttack ++)
	{
		InitBattleAtteck(nCntAttack);
	}
	InitBattleDamage();

	for(int nCntTexAttack = 0;nCntTexAttack<ATTACKE_MAX_ATTIBUTION;nCntTexAttack++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_aFileNameAttack[nCntTexAttack],
			&g_pTextureBattle_Attack[nCntTexAttack]);
	}

	MakeVertexBattle_Attack(pDevice);
	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void UninitBattle_Attack(void)
{
	for(int nCntTexAttack = 0;nCntTexAttack<ATTACKE_MAX_ATTIBUTION;nCntTexAttack++)
		if(g_pTextureBattle_Attack[nCntTexAttack] != NULL)
		{// テクスチャの開放
			g_pTextureBattle_Attack[nCntTexAttack]->Release();		
			g_pTextureBattle_Attack[nCntTexAttack] = NULL;
		}

		if(g_pVtxBuffBattle_Attack != NULL)
		{// 頂点バッファの開放
			g_pVtxBuffBattle_Attack->Release();
			g_pVtxBuffBattle_Attack = NULL;
		}
}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateBattle_Attack(void)
{
	SumAttackTime --;
	SumAttackTime = (int)SetMAX((float)SumAttackTime,999,0.0f,2);
	BATTLE_ENEMY *pEnemy = GetBattleEnemy();


		


	for(int nCntAttack = 0; nCntAttack < MAX_BATTLE_ATTACK; nCntAttack ++)
	{
		if(g_Attack[nCntAttack].bUse == false)
			continue;
		g_Attack[nCntAttack].nLife--;
		g_Attack[nCntAttack].DrawPos += g_Attack[nCntAttack].Move;
		//単純な行動だけ
		g_Attack[nCntAttack].rot += 0.1f;
		
		//g_Attack[nCntAttack].fSize += 0.1f + (float)((rand()%11 + 1) - 5) / 100;

			if(g_Attack[nCntAttack].nLife <0)//一回のみ実行　とっても重要//ＢＢ弾でも一発で貫通したくなければ
			{
				int DamageColor	= g_Attack[nCntAttack].nColor;
				int DamageInput	= g_Attack[nCntAttack].Damage;
				int DamageTarget = g_Attack[nCntAttack].Target;
				int RecTargetHPLeft = 0;
				if(g_Attack[nCntAttack].team == TEAM_PLAYER)
				{
					SOUND_LABEL sound = SOUND_LABEL_SE_ATTACK_FIRE;
					switch(DamageColor)
					{
					case BLOCK_RED:
						sound = SOUND_LABEL_SE_ATTACK_FIRE;
						break;
					case BLOCK_GREEN:
						sound = SOUND_LABEL_SE_ATTACK_WIND;
						break;
					case BLOCK_BLUE:
						sound = SOUND_LABEL_SE_ATTACK_WATER;
						break;
					case BLOCK_LIGHT:
						sound = SOUND_LABEL_SE_ATTACK_LIGHT;
						break;
					case BLOCK_DARK:
						sound = SOUND_LABEL_SE_ATTACK_DARK;
						break;
					}

					PlaySound(sound);
					//DamageInput = DamageAnalysFinal(DamageColor,DamageInput,DamageTarget);
					SetBattle_EnemyHP(g_Attack[nCntAttack].Target,HP_LOSING,g_Attack[nCntAttack].Damage);
					//属性相性を判断し，数字の表示を入れる
					int ColorResult = DamageAnalysAttibution(DamageColor,pBattleEnemy[DamageTarget].nColor);//pBattleEnemy[g_Attack[nCntAttack].Target].
					//POSを敵の中心に修正する
					D3DXVECTOR3 Numberpos = D3DXVECTOR3(pBattleEnemy[g_Attack[nCntAttack].Target].pos.x + BATTLE_ENEMY_SIZE_X,
												  pBattleEnemy[g_Attack[nCntAttack].Target].pos.y - BATTLE_ENEMY_SIZE_X * 0.5f,
												  0.0f);
					D3DXVECTOR3 Effectpos = D3DXVECTOR3(pBattleEnemy[g_Attack[nCntAttack].Target].pos.x + BATTLE_ENEMY_SIZE_X * pBattleEnemy[g_Attack[nCntAttack].Target].fSize * 0.3f ,
												  pBattleEnemy[g_Attack[nCntAttack].Target].pos.y - BATTLE_ENEMY_SIZE_Y * pBattleEnemy[g_Attack[nCntAttack].Target].fSize * 0.3f,
												  0.0f);
					SetBattle_Effect_Attack_Final(g_Attack[nCntAttack].DrawPos,DamageColor);
					SetDamageNumberX(DamageInput,0,ColorResult,Numberpos);
					//ためーじの最終予測 
					RecTargetHPLeft = pEnemy[DamageTarget].fRecHP - DamageInput;
				}
				if(g_Attack[nCntAttack].team == TEAM_ENEMY)
				{
					PlaySound(SOUND_LABEL_SE_ATTACK_ENEMY_NORMAL);
					SetMemberNockBack(20.0f);
					SetBattle_PlayerHP_Change(HP_LOSING,(int)g_Attack[nCntAttack].Damage);
					//数字の表示
					SetDamageNumberX(g_Attack[nCntAttack].Damage,DAMAGE_NORMAL,0,BATTLE_PLATER_CENTER_POS);
					//HPの揺みを書く
					SetBattle_Player_HP_Shake();
				}
				InitBattleAtteck(nCntAttack);
			}


		
		SetVerTexBattle_Attack(nCntAttack);

	}

}
//=============================================================================
// Attackの設定
//=============================================================================
//=============================
//マルチ攻撃設定
//=============================
void SetBattleAttack_Player_Multi(int MemberID,int Target)
{
	BATTLE_T_MEMBER *pMember = GetTMember();
	if(pMember[MemberID].DamageBase != 0)
	{
		int color = pMember[MemberID].Color;
		int damage = pMember[MemberID].DamageFinal;
		SetBattle_Attack_Single(TEAM_PLAYER,MemberID,color,Target,(float)damage,0);
	}

}
//=============================
//シングル攻撃設定
//=============================
void SetBattle_Attack_Single(int team,int From,int nColor,int Target,float Damage,int Life)
{
	BATTLE_T_MEMBER *pMember;
	pMember = GetTMember();
	if(Life == 0)
		Life = ATTACK_LIFE;
	
	for(int nCntAttack = 0; nCntAttack < MAX_BATTLE_ATTACK; nCntAttack ++)
	{
		if(g_Attack[nCntAttack].bUse == true)
			continue;
		SumAttackTime = 60 + 60 + 70;//エフェクト演出時間＋ダメージ減少時間＋一次キャッシュ
		g_Attack[nCntAttack].bUse = true;
		g_Attack[nCntAttack].team = team;
		if(team == TEAM_ENEMY)
		{//敵の攻撃の場合

			float a = BATTLE_ENEMY_SIZE_X * pBattleEnemy[From].fSize/2*0.8;
			D3DXVECTOR3 pos = D3DXVECTOR3(pBattleEnemy[From].pos.x + BATTLE_ENEMY_SIZE_X * pBattleEnemy[From].fSize/2,
											pBattleEnemy[From].pos.y - BATTLE_ENEMY_SIZE_X * pBattleEnemy[From].fSize/2,
											0.0f);

			g_Attack[nCntAttack].Pos[ATTACK_START].x = pBattleEnemy[From].pos.x + BATTLE_ENEMY_SIZE_X * pBattleEnemy[Target].fSize * 0.5f;
			g_Attack[nCntAttack].Pos[ATTACK_START].y = pBattleEnemy[From].pos.y - BATTLE_ENEMY_SIZE_X * pBattleEnemy[Target].fSize * 0.5f;
			g_Attack[nCntAttack].Pos[ATTACK_END]	= BATTLE_PLATER_CENTER_POS;
			g_Attack[nCntAttack].DrawPos			= g_Attack[nCntAttack].Pos[ATTACK_START]	= pos;
		}
		else
		{//プレイヤーの攻撃の場合
			g_Attack[nCntAttack].Pos[ATTACK_START]	= pMember[From].pos;
			g_Attack[nCntAttack].Pos[ATTACK_END].x	= pBattleEnemy[Target].pos.x + BATTLE_ENEMY_SIZE_X * pBattleEnemy[Target].fSize * 0.5f;
			g_Attack[nCntAttack].Pos[ATTACK_END].y	= pBattleEnemy[Target].pos.y - BATTLE_ENEMY_SIZE_X * pBattleEnemy[Target].fSize * 0.5f;
			g_Attack[nCntAttack].DrawPos			= pMember[From].pos;
		}
		float DestX = g_Attack[nCntAttack].Pos[ATTACK_END].x - g_Attack[nCntAttack].Pos[ATTACK_START].x;
		float DestY = g_Attack[nCntAttack].Pos[ATTACK_END].y - g_Attack[nCntAttack].Pos[ATTACK_START].y;
		//g_Attack[nCntAttack].fLength	= sqrtf(DestX * DestX + DestY * DestY);

		g_Attack[nCntAttack].Move.x = DestX / Life;
		g_Attack[nCntAttack].Move.y = DestY / Life;

		g_Attack[nCntAttack].from = From;
		g_Attack[nCntAttack].nLife = Life;
		g_Attack[nCntAttack].nColor = nColor;
		g_Attack[nCntAttack].Target = Target;
		g_Attack[nCntAttack].Damage = Damage;
		break;
	}

}
//=============================================================================
// まだ初期化処理だよ　デヘ
//=============================================================================
void InitBattleAtteck(int Attack_ID)
{
	g_Attack[Attack_ID].bUse	= false;
	g_Attack[Attack_ID].from	= ATTACK_PLAYER;
	g_Attack[Attack_ID].nLife	= ATTACK_LIFE;
	g_Attack[Attack_ID].DrawPos = D3DXVECTOR3( 0.0f,0.0f,0.0f);
	g_Attack[Attack_ID].Move	= D3DXVECTOR3(0.0f,0.0f,0.0f);
	g_Attack[Attack_ID].nColor	= ATTACK_RED;
	g_Attack[Attack_ID].fLength	= 0.0f;
	g_Attack[Attack_ID].Target	= 0;
	g_Attack[Attack_ID].team	= MAX_TEAM;
	g_Attack[Attack_ID].Damage	= 0.0f;
	g_Attack[Attack_ID].fSpeed	= 0;
	g_Attack[Attack_ID].rot		= 0.0f;
	g_Attack[Attack_ID].fSize	= 1.0f;
	for(int nCntPos = 0;nCntPos < ATTACK_MAX_POS; nCntPos++)
	{
		g_Attack[Attack_ID].Pos[nCntPos] = D3DXVECTOR3( 0.0f,0.0f,0.0f);
	}
}
//=============================================================================
// 描画処理
//=============================================================================
void DrawBattle_Attack(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//フィールドの描画
	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffBattle_Attack, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for(int nCntAttack = 0;nCntAttack < MAX_BATTLE_ATTACK;nCntAttack++)
	{
		if(g_Attack[nCntAttack].bUse == false)
			continue;
		pDevice->SetTexture(0,g_pTextureBattle_Attack[g_Attack[nCntAttack].nColor]);
		//pDevice->SetTexture(0,NULL);
		// ポリゴンの描画

		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);//set
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);//in
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntAttack * 4), NUM_POLYGON);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);//set
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);// out
	}

}
//=============================================================================
// Attack頂点の作成
//=============================================================================
HRESULT MakeVertexBattle_Attack(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * MAX_BATTLE_ATTACK,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
		FVF_VERTEX_2D,						// 使用する頂点フォーマット
		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
		&g_pVtxBuffBattle_Attack,					// 頂点バッファインターフェースへのポインタ
		NULL)))								// NULLに設定
	{
		return E_FAIL;
	}
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffBattle_Attack->Lock(0, 0, (void**)&pVtx, 0);

		for(int nCntAttack = 0;nCntAttack < MAX_BATTLE_ATTACK;nCntAttack++,pVtx += 4)	
		{
			// 頂点座標の設定
			pVtx[0].pos.x = g_Attack[nCntAttack].DrawPos.x - BATTLE_ATTACK_SIZE;
			pVtx[0].pos.y = g_Attack[nCntAttack].DrawPos.y - BATTLE_ATTACK_SIZE;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_Attack[nCntAttack].DrawPos.x + BATTLE_ATTACK_SIZE;
			pVtx[1].pos.y = g_Attack[nCntAttack].DrawPos.y - BATTLE_ATTACK_SIZE;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_Attack[nCntAttack].DrawPos.x;
			pVtx[2].pos.y = g_Attack[nCntAttack].DrawPos.y;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_Attack[nCntAttack].DrawPos.x + BATTLE_ATTACK_SIZE;
			pVtx[3].pos.y = g_Attack[nCntAttack].DrawPos.y;
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
		g_pVtxBuffBattle_Attack->Unlock();
	}
	return S_OK;

}
//=============================================================================
// Attacknの設定
//=============================================================================
void SetVerTexBattle_Attack(int Attackn_ID)
{
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffBattle_Attack->Lock(0, 0, (void**)&pVtx, 0);
	// 頂点座標の設定
	pVtx += (Attackn_ID *4);
	// 頂点座標の設定
	float Rot = g_Attack[Attackn_ID].rot;
	float Size =  g_Attack[Attackn_ID].fSize;
	float SizeRnader = ((float)(rand()%10 + 1))/10 + 0.5f;
	//Rot = SetMAX(Rot,D3DX_PI,0.0f,1);
	pVtx[0].pos.x = g_Attack[Attackn_ID].DrawPos.x - cosf(D3DX_PI * 0.25f - Rot)*BATTLE_ATTACK_SIZE * Size * SizeRnader;
	pVtx[0].pos.y = g_Attack[Attackn_ID].DrawPos.y - sinf(D3DX_PI * 0.25f - Rot)*BATTLE_ATTACK_SIZE * Size * SizeRnader;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_Attack[Attackn_ID].DrawPos.x + cosf(D3DX_PI * 0.25f + Rot)*BATTLE_ATTACK_SIZE * Size * SizeRnader;
	pVtx[1].pos.y = g_Attack[Attackn_ID].DrawPos.y - sinf(D3DX_PI * 0.25f + Rot)*BATTLE_ATTACK_SIZE * Size * SizeRnader;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_Attack[Attackn_ID].DrawPos.x - cosf(D3DX_PI * 0.25f + Rot)*BATTLE_ATTACK_SIZE * Size * SizeRnader;
	pVtx[2].pos.y = g_Attack[Attackn_ID].DrawPos.y + sinf(D3DX_PI * 0.25f + Rot)*BATTLE_ATTACK_SIZE * Size * SizeRnader;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_Attack[Attackn_ID].DrawPos.x + cosf(D3DX_PI * 0.25f - Rot)*BATTLE_ATTACK_SIZE * Size * SizeRnader;
	pVtx[3].pos.y = g_Attack[Attackn_ID].DrawPos.y + sinf(D3DX_PI * 0.25f - Rot)*BATTLE_ATTACK_SIZE * Size * SizeRnader;
	pVtx[3].pos.z = 0.0f;

	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// 頂点データをアンロックする
	g_pVtxBuffBattle_Attack->Unlock();

}
//ダメージ計算の初期化処理
void InitBattleDamage(void)
{
	for(int nCntDamage = 0; nCntDamage < ATTACKE_MAX_ATTIBUTION;nCntDamage++)
	{
		g_Damage[nCntDamage].Base_Atk	= 0;
		g_Damage[nCntDamage].Damage		= 0;
		g_Damage[nCntDamage].nColor		= nCntDamage;
		g_Damage[nCntDamage].nCombo		= 0;
	}
}
//==============================================================--
//ダメージ計算を取得
//==============================================================
DAMAGE_STRUCT *GetDamageStruct(void)
{
	return &g_Damage[0];
}
//==============================================================
//ダメージの最終計算
//==============================================================
int	DamageAnalysAttibution(int BaseColor,int Target_Color)
{
	int Attibutoin = 0;
	switch(BaseColor)
	{
	case ATTACK_RED:
		if(Target_Color == ATTACK_BLUE)
			Attibutoin = DAMAGE_BAD;
		if(Target_Color == ATTACK_GREEN)
			Attibutoin = DAMAGE_GREAT;
		break;
	case ATTACK_GREEN:
		if(Target_Color == ATTACK_RED)
			Attibutoin = DAMAGE_BAD;
		if(Target_Color == ATTACK_BLUE)
			Attibutoin = DAMAGE_GREAT;
		break;
	case ATTACK_BLUE:
		if(Target_Color == ATTACK_GREEN)
			Attibutoin = DAMAGE_BAD;
		if(Target_Color == ATTACK_RED)
			Attibutoin = DAMAGE_GREAT;
		break;
	case ATTACK_LIGHT:
		if(Target_Color == ATTACK_PURPLE)
			Attibutoin = DAMAGE_GREAT;
		break;
	case ATTACK_PURPLE:
		if(Target_Color == ATTACK_LIGHT)
			Attibutoin = DAMAGE_GREAT;
		break;
	}
	return Attibutoin;
}
int	DamageAnalysFinal(int DamageColor,int BaseDamage,int Target)
{
	int FinalyDamage = BaseDamage;
	int BaseColor = DamageColor;
	int TargetColor = pBattleEnemy[Target].nColor;
	int Result = DamageAnalysAttibution(DamageColor,TargetColor);
	switch(Result)
	{
	case DAMAGE_BAD:
		FinalyDamage *= 0.5f;
		break;
	case 0:
		break;
	case DAMAGE_GREAT:
		FinalyDamage *= 2.0f;
		break;
	}
	return FinalyDamage;
}
//============================
//基本ダメージ計算及びチェック
//============================
bool RecoverCheck(void)//回復の中身をチェック
{
	bool res = false;
	BATTLE_PLAYER *pBattlePlayer = GetBattlePlayer();
	if(pBattlePlayer->HP_RecoverResult > 0)
		res = true;
	return res;
}

bool DamageCheck(void)
{
	bool Result = false;
	BATTLE_T_MEMBER *pMember = GetTMember();
	BATTLE_ENEMY *pEnemy = GetBattleEnemy();

	
	for(int nMember = 0; nMember < TEAM_PLAYER_MAX;nMember++)
	{
		if(pMember[nMember].DamageBase != 0)
			Result = true;

	}
	if(Result == true)
	{
			int	MemberIdx = 0;
		for(int nCntEnemy = 0; nCntEnemy < TEAM_ENEMY_MAX;nCntEnemy ++)
		{
			if(pEnemy[nCntEnemy].bUse == false)
				continue;
			int EnemyHP = pEnemy[nCntEnemy].fRecHP;
			int Damage = 0;
			while(MemberIdx < TEAM_PLAYER_MAX)
			{
				Damage = DamageAnalysFinal(pMember[MemberIdx].Color,		//カラー
										   pMember[MemberIdx].DamageBase,	//基本ダメージ
										   nCntEnemy);						//ターゲット
				pMember[MemberIdx].DamageFinal = Damage;
				EnemyHP -= Damage;	//敵のHPを減らす
				pMember[MemberIdx].nTarget = nCntEnemy;//現在のメンバーのターゲットを設定
				if(EnemyHP <= 0)//もし敵がここで死んだら抜け,次の敵に行く
				{
					MemberIdx ++;
					break;
				}
				MemberIdx ++;
			}
			if(MemberIdx >=  TEAM_ENEMY_MAX)
				break;
		}
	}

	return Result;
}
//============================
//攻撃演出時間の取得
//============================
int GetSUM_Attack_Time(void)
{
	return SumAttackTime;
}
