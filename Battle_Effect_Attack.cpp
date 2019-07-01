
//=============================================================================
// ブロック破壊効果 [Battle_Effect_Attack.cpp]
// HAL 名古屋　GP11A172 17 50323 リ リン
//=============================================================================
#include "Battle_Effect_Attack.h"
#include "main.h"
#include "game.h"
#include "Battle_Enemy.h"
#include "input.h"
//*****************************************************************************
//使用注意
//*****************************************************************************
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_BATTLE_ATTACK_EFFECT	(32)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexBattle_Effect_Attack(LPDIRECT3DDEVICE9 pDevice);
void SetVerTexBattle_Effect_Attack(int Attack_Effectn_ID);
void InitAttack_EffectSingle(int ID);
//*****************************************************************************
// 構造体宣言
//*****************************************************************************
const char *c_aFileNameBattle_Effect_Attack[BATTLE_EFFECT_ATTACK_MAX_TEX] = 
	{	
	"data/EFFECT/Effect_Fire_explo_10.png",
	"data/EFFECT/Effect_Green_Tornado_12.png",
	"data/EFFECT/Effect_Water_Ball_12.png",
	"data/EFFECT/Effect_Lighting_8.png",
	"data/EFFECT/Effect_Dark_Explo_13.png",
	};
//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureBattle_Effect_Attack[BATTLE_EFFECT_ATTACK_MAX_TEX]	= {};	//テクスチャメモリ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBattle_Effect_Attack	= NULL;	//頂点座標バッファ

BATTELE_EFFECT_ATTACK	g_Battle_Effect_Attack[MAX_BATTLE_ATTACK_EFFECT];			//分裂ブロックの構造体
int	g_Timecount_Attack_Effect;			//時間制御変数
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBattle_Effect_Attack(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for(int nCntAttack_Effect = 0;nCntAttack_Effect < MAX_BATTLE_ATTACK_EFFECT;nCntAttack_Effect++)
	{
		InitAttack_EffectSingle(nCntAttack_Effect);
	}
	for(int nTex = 0; nTex < BATTLE_EFFECT_ATTACK_MAX_TEX;nTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
								c_aFileNameBattle_Effect_Attack[nTex],
								&g_pTextureBattle_Effect_Attack[nTex]);
	}
	g_Timecount_Attack_Effect = 0;
	MakeVertexBattle_Effect_Attack(pDevice);
	return S_OK;
}
void InitAttack_EffectSingle(int ID)
{
	g_Battle_Effect_Attack[ID].bUse		= false;				// 使用中か
	g_Battle_Effect_Attack[ID].DrawPos	= D3DXVECTOR3(0.0f,0.0f,0.0f);	// 描画位置
	g_Battle_Effect_Attack[ID].nType	= BATTLE_EFFECT_ATTACK_MAX_TEX;				// どの画像（属性）
	g_Battle_Effect_Attack[ID].nFrame	= -1;				// 何フレーム目か	
	g_Battle_Effect_Attack[ID].nMaxFrame = 0;
	g_Battle_Effect_Attack[ID].fVerTex_Per_Unit = 0.0f;	// 毎フレームのテクスチャ頂点距離
	g_Battle_Effect_Attack[ID].fSizeX	= 0.0f;			// 横幅
	g_Battle_Effect_Attack[ID].fSizeY	= 0.0f;			// 縦幅
	g_Battle_Effect_Attack[ID].fSizeUp	= 1.0f;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBattle_Effect_Attack(void)
{
	for(int nTex = 0; nTex < BATTLE_EFFECT_ATTACK_MAX_TEX;nTex++)
	{
		if(g_pTextureBattle_Effect_Attack[nTex] != NULL)
		{// テクスチャの開放
			g_pTextureBattle_Effect_Attack[nTex]->Release();
			g_pTextureBattle_Effect_Attack[nTex] = NULL;
		}
	}
	if(g_pVtxBuffBattle_Effect_Attack != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffBattle_Effect_Attack->Release();
		g_pVtxBuffBattle_Effect_Attack = NULL;
	}
}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateBattle_Effect_Attack(void)
{
	g_Timecount_Attack_Effect ++;
	if(g_Timecount_Attack_Effect > 100)
		g_Timecount_Attack_Effect = 0;


	for(int nCntAttack_Effect = 0;nCntAttack_Effect < MAX_BATTLE_ATTACK_EFFECT;nCntAttack_Effect++)
	{
		if(g_Battle_Effect_Attack[nCntAttack_Effect].bUse == false)
			continue;
		if(g_Timecount_Attack_Effect%7 == 0)
		{
		g_Battle_Effect_Attack[nCntAttack_Effect].nFrame ++;
		g_Battle_Effect_Attack[nCntAttack_Effect].fSizeUp += 0.1f;
		}

		SetVerTexBattle_Effect_Attack(nCntAttack_Effect);

		if(g_Battle_Effect_Attack[nCntAttack_Effect].nFrame >= g_Battle_Effect_Attack[nCntAttack_Effect].nMaxFrame)
			InitAttack_EffectSingle(nCntAttack_Effect);

	}
}
//=============================================================================
// 特定のPointを初期化する
//=============================================================================
void SetBattle_Effect_Attack_Final(D3DXVECTOR3 pos,int nType)
{
	for(int nCntAttack_Effect = 0;nCntAttack_Effect < MAX_BATTLE_ATTACK_EFFECT;nCntAttack_Effect++)
	{
		if(g_Battle_Effect_Attack[nCntAttack_Effect].bUse == true)
			continue;
		g_Battle_Effect_Attack[nCntAttack_Effect].bUse = true;
		g_Battle_Effect_Attack[nCntAttack_Effect].DrawPos = pos;
		g_Battle_Effect_Attack[nCntAttack_Effect].nType = nType;
		int Frame=0;
		float fSizeX = 0.0f;
		float fSizeY = 0.0f;
		switch(nType)
		{
		case BATTLE_EFFECT_ATTACK_RED:		Frame = 10;	fSizeX = 75.0f;fSizeY = 75.0f;break;
		case BATTLE_EFFECT_ATTACK_GREEN:	Frame = 12;	fSizeX = 75.0f;fSizeY = 75.0f;break;
		case BATTLE_EFFECT_ATTACK_BLUE:		Frame = 12;	fSizeX = 75.0f;fSizeY = 75.0f;break;
		case BATTLE_EFFECT_ATTACK_LIGHT:	Frame = 8;	fSizeX =  40.0f;fSizeY = 150.0f;break;
		case BATTLE_EFFECT_ATTACK_PURPLE:	Frame = 13;	fSizeX = 75.0f;fSizeY = 75.0f;break;
		}
		g_Battle_Effect_Attack[nCntAttack_Effect].nMaxFrame = Frame;
		g_Battle_Effect_Attack[nCntAttack_Effect].fVerTex_Per_Unit = 1.0 / Frame;
		g_Battle_Effect_Attack[nCntAttack_Effect].fSizeX = fSizeX;
		g_Battle_Effect_Attack[nCntAttack_Effect].fSizeY = fSizeY;
		break;
	}

}
//=============================================================================
// 描画処理
//=============================================================================
void DrawBattle_Effect_Attack(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//フィールドの描画
	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffBattle_Effect_Attack, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for(int nCntAttack_Effect = 0;nCntAttack_Effect < MAX_BATTLE_ATTACK_EFFECT;nCntAttack_Effect++)
	{
		if(g_Battle_Effect_Attack[nCntAttack_Effect].bUse == false)
			continue;
		pDevice->SetTexture(0,g_pTextureBattle_Effect_Attack[g_Battle_Effect_Attack[nCntAttack_Effect].nType]);
		// ポリゴンの描画

		//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);//set
		//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);//in
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntAttack_Effect * 4), NUM_POLYGON);
		//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);//set
		//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);// out
	}

}
//=============================================================================
// Attack_Effect頂点の作成
//=============================================================================
HRESULT MakeVertexBattle_Effect_Attack(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * MAX_BATTLE_ATTACK_EFFECT,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
		FVF_VERTEX_2D,						// 使用する頂点フォーマット
		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
		&g_pVtxBuffBattle_Effect_Attack,					// 頂点バッファインターフェースへのポインタ
		NULL)))								// NULLに設定
	{
		return E_FAIL;
	}
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffBattle_Effect_Attack->Lock(0, 0, (void**)&pVtx, 0);

		for(int nCntAttack_Effect = 0;nCntAttack_Effect < MAX_BATTLE_ATTACK_EFFECT;nCntAttack_Effect++,pVtx += 4)	
		{
			float fSizeX = g_Battle_Effect_Attack[nCntAttack_Effect].fSizeX;
			float fSizeY = g_Battle_Effect_Attack[nCntAttack_Effect].fSizeY;
			float fUnitSize = g_Battle_Effect_Attack[nCntAttack_Effect].fVerTex_Per_Unit;
			int	  nFrame	= g_Battle_Effect_Attack[nCntAttack_Effect].nFrame;
			// 頂点座標の設定
			pVtx[0].pos.x = g_Battle_Effect_Attack[nCntAttack_Effect].DrawPos.x - fSizeX;
			pVtx[0].pos.y = g_Battle_Effect_Attack[nCntAttack_Effect].DrawPos.y - fSizeY;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_Battle_Effect_Attack[nCntAttack_Effect].DrawPos.x + fSizeX;
			pVtx[1].pos.y = g_Battle_Effect_Attack[nCntAttack_Effect].DrawPos.y - fSizeY;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_Battle_Effect_Attack[nCntAttack_Effect].DrawPos.x	- fSizeX;
			pVtx[2].pos.y = g_Battle_Effect_Attack[nCntAttack_Effect].DrawPos.y;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_Battle_Effect_Attack[nCntAttack_Effect].DrawPos.x + fSizeX;
			pVtx[3].pos.y = g_Battle_Effect_Attack[nCntAttack_Effect].DrawPos.y;
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

			pVtx[0].tex = D3DXVECTOR2(fUnitSize * nFrame,0.0f);
			pVtx[1].tex = D3DXVECTOR2(fUnitSize * nFrame + fUnitSize,0.0f);
			pVtx[2].tex = D3DXVECTOR2(fUnitSize * nFrame,1.0f);
			pVtx[3].tex = D3DXVECTOR2(fUnitSize * nFrame + fUnitSize,1.0f);

		}
		// 頂点データをアンロックする
		g_pVtxBuffBattle_Effect_Attack->Unlock();
	}
	return S_OK;

}
//=============================================================================
// Attack_Effectnの設定
//=============================================================================
void SetVerTexBattle_Effect_Attack(int Attack_Effectn_ID)
{
//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffBattle_Effect_Attack->Lock(0, 0, (void**)&pVtx, 0);
	pVtx += Attack_Effectn_ID*4;

		float fSizeX = g_Battle_Effect_Attack[Attack_Effectn_ID].fSizeX;
		float fSizeY = g_Battle_Effect_Attack[Attack_Effectn_ID].fSizeY;
		float fUnitSize = g_Battle_Effect_Attack[Attack_Effectn_ID].fVerTex_Per_Unit;
		float fSizeUp = g_Battle_Effect_Attack[Attack_Effectn_ID].fSizeUp;
		int	  nFrame	= g_Battle_Effect_Attack[Attack_Effectn_ID].nFrame;
		// 頂点座標の設定
		pVtx[0].pos.x = g_Battle_Effect_Attack[Attack_Effectn_ID].DrawPos.x - fSizeX * fSizeUp;
		pVtx[0].pos.y = g_Battle_Effect_Attack[Attack_Effectn_ID].DrawPos.y - fSizeY * fSizeUp;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_Battle_Effect_Attack[Attack_Effectn_ID].DrawPos.x + fSizeX * fSizeUp;
		pVtx[1].pos.y = g_Battle_Effect_Attack[Attack_Effectn_ID].DrawPos.y - fSizeY * fSizeUp;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_Battle_Effect_Attack[Attack_Effectn_ID].DrawPos.x	- fSizeX * fSizeUp;
		pVtx[2].pos.y = g_Battle_Effect_Attack[Attack_Effectn_ID].DrawPos.y + fSizeY * fSizeUp;;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_Battle_Effect_Attack[Attack_Effectn_ID].DrawPos.x + fSizeX * fSizeUp;
		pVtx[3].pos.y = g_Battle_Effect_Attack[Attack_Effectn_ID].DrawPos.y + fSizeY * fSizeUp;;
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

		pVtx[0].tex = D3DXVECTOR2(fUnitSize * nFrame,0.0f);
		pVtx[1].tex = D3DXVECTOR2(fUnitSize * nFrame + fUnitSize,0.0f);
		pVtx[2].tex = D3DXVECTOR2(fUnitSize * nFrame,1.0f);
		pVtx[3].tex = D3DXVECTOR2(fUnitSize * nFrame + fUnitSize,1.0f);

	// 頂点データをアンロックする
	g_pVtxBuffBattle_Effect_Attack->Unlock();

}
