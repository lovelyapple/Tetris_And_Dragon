//=============================================================================
//[Battle_Effect_ComboCheers.cpp]
// HAL 名古屋　GP11A172 17 50323 リ リン
//=============================================================================

#include "Battle_ComboCheers.h"
#include "main.h"
#include "game.h"
#include "Function.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

#define TEXTURE_BATTLE_COMOBO_CHEERS	("data/TEXTURE/ComboCheers.png")
#define BATTLE_COMBO_CHEERS_SIZE_X		(200.0f)
#define BATTLE_COMBO_CHEERS_SIZE_Y		(45.0f)
#define BATTLE_COMBO_CHEERS_MAX			(8)
#define BATTLE_COMBO_CHEERS_LIFE		(30)
#define BATTLE_COMBO_CHEERS_UNIT_SIZE	(1.0f/4)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexBattle_ComboCheers(LPDIRECT3DDEVICE9 pDevice);
void SetVertexBattle_ComboCheers(int ID);
void InitBattle_ComboCheers(int ID);
//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureBattle_ComboCheers = NULL;	//テクスチャメモリ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBattle_ComboCheers = NULL;	//頂点座標バッファ
typedef enum
{
	CHEERS_PHASE_NORMAL = 0,
	CHEERS_PHASE_STOP,
	CHEERS_PHASE_FADE_OUT,
	CHEERS_PHASE_MAX,
}CHEERS_PHASE;
typedef struct
{
	bool	bUse;
	D3DXVECTOR3	pos;
	CHEERS_PHASE		g_phase;
	int		Type;
	int		nLife;
	float	fSizeX;
	float	fSizeY;
}COMBO_CHEERS;

COMBO_CHEERS g_ComboCheers[BATTLE_COMBO_CHEERS_MAX];
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBattle_Effect_ComboCheers(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntCheers = 0; nCntCheers < BATTLE_COMBO_CHEERS_MAX;nCntCheers++)
		InitBattle_ComboCheers(nCntCheers);

	D3DXCreateTextureFromFile(pDevice,
							TEXTURE_BATTLE_COMOBO_CHEERS,
							&g_pTextureBattle_ComboCheers);

	MakeVertexBattle_ComboCheers(pDevice);
	return S_OK;
}


void InitBattle_ComboCheers(int ID)
{

	g_ComboCheers[ID].bUse		= false;
	g_ComboCheers[ID].pos		= D3DXVECTOR3(0.0f,0.f,0.0f);
	g_ComboCheers[ID].nLife		= BATTLE_COMBO_CHEERS_LIFE;
	g_ComboCheers[ID].Type		= 0;
	g_ComboCheers[ID].g_phase	= CHEERS_PHASE_MAX;
	g_ComboCheers[ID].fSizeX	= 0.0f;
	g_ComboCheers[ID].fSizeY	= 0.0f;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBattle_Effect_ComboCheers(void)
{
	if(g_pTextureBattle_ComboCheers != NULL)
	{// テクスチャの開放
		g_pTextureBattle_ComboCheers->Release();
		g_pTextureBattle_ComboCheers = NULL;
	}

	if(g_pVtxBuffBattle_ComboCheers != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffBattle_ComboCheers->Release();
		g_pVtxBuffBattle_ComboCheers = NULL;
	}

}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateBattle_Effect_ComboCheers(void)
{
	for (int nCntCheers = 0; nCntCheers < BATTLE_COMBO_CHEERS_MAX;nCntCheers++)
	{
		if(g_ComboCheers[nCntCheers].bUse == false)
			continue;
		switch(g_ComboCheers[nCntCheers].g_phase)
		{
		case CHEERS_PHASE_NORMAL:
			g_ComboCheers[nCntCheers].nLife--;
			g_ComboCheers[nCntCheers].fSizeX += 1.0f / BATTLE_COMBO_CHEERS_LIFE * 1.2f;
			g_ComboCheers[nCntCheers].fSizeY += 1.0f / BATTLE_COMBO_CHEERS_LIFE * 1.2f;

			g_ComboCheers[nCntCheers].fSizeY = SetMAX(g_ComboCheers[nCntCheers].fSizeY,1.0f,0.0f,2);
			g_ComboCheers[nCntCheers].fSizeX = SetMAX(g_ComboCheers[nCntCheers].fSizeX,1.0f,0.0f,2);

			if(g_ComboCheers[nCntCheers].nLife < 0)
			{
				g_ComboCheers[nCntCheers].fSizeX = 1.0f;
				g_ComboCheers[nCntCheers].fSizeY = 1.0f;
				g_ComboCheers[nCntCheers].g_phase = CHEERS_PHASE_STOP;
				g_ComboCheers[nCntCheers].nLife = BATTLE_COMBO_CHEERS_LIFE;
			}
			break;
		case CHEERS_PHASE_STOP:
			g_ComboCheers[nCntCheers].nLife--;
			if(g_ComboCheers[nCntCheers].nLife < 0)
			{
				g_ComboCheers[nCntCheers].g_phase = CHEERS_PHASE_FADE_OUT;
				g_ComboCheers[nCntCheers].nLife = BATTLE_COMBO_CHEERS_LIFE;
			}
			break;
		case CHEERS_PHASE_FADE_OUT:
			g_ComboCheers[nCntCheers].nLife--;
			g_ComboCheers[nCntCheers].fSizeX *= 1.2f;
			g_ComboCheers[nCntCheers].fSizeY *= 0.8f;
			if(g_ComboCheers[nCntCheers].nLife < 0)
				InitBattle_ComboCheers(nCntCheers);
		case CHEERS_PHASE_MAX:
			break;

		}

		SetVertexBattle_ComboCheers(nCntCheers);
	}
}
void SetUpBattleBattle_Effect_ComboCheers(int nType,D3DXVECTOR3 pos)
{
	for (int nCntCheers = 0; nCntCheers < BATTLE_COMBO_CHEERS_MAX;nCntCheers++)
	{
		if(g_ComboCheers[nCntCheers].bUse == true)
			continue;

		g_ComboCheers[nCntCheers].bUse = true;
		g_ComboCheers[nCntCheers].pos = pos;
		g_ComboCheers[nCntCheers].Type = nType;
		g_ComboCheers[nCntCheers].g_phase = CHEERS_PHASE_NORMAL;
		break;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBattle_Effect_ComboCheers(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();



	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffBattle_ComboCheers,0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetTexture(0,g_pTextureBattle_ComboCheers);

	for (int nCntCheers = 0; nCntCheers < BATTLE_COMBO_CHEERS_MAX;nCntCheers++)
	{
		if(g_ComboCheers[nCntCheers].bUse == false)
			continue;
		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntCheers * 4, NUM_POLYGON);
	}
}

//=============================================================================
// Shining頂点の作成
//=============================================================================
HRESULT MakeVertexBattle_ComboCheers(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * BATTLE_COMBO_CHEERS_MAX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
		FVF_VERTEX_2D,						// 使用する頂点フォーマット
		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
		&g_pVtxBuffBattle_ComboCheers,					// 頂点バッファインターフェースへのポインタ
		NULL)))								// NULLに設定
	{
		return E_FAIL;
	}
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffBattle_ComboCheers->Lock(0, 0, (void**)&pVtx, 0);
			// 頂点座標の設定
		for (int nCntCheers = 0; nCntCheers < BATTLE_COMBO_CHEERS_MAX;nCntCheers++,pVtx += 4)
		{
			//=======================LeftSide
			pVtx[0].pos.x = g_ComboCheers[nCntCheers].pos.x - BATTLE_COMBO_CHEERS_SIZE_X * g_ComboCheers[nCntCheers].fSizeX;
			pVtx[0].pos.y = g_ComboCheers[nCntCheers].pos.y - BATTLE_COMBO_CHEERS_SIZE_Y * g_ComboCheers[nCntCheers].fSizeY;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_ComboCheers[nCntCheers].pos.x + BATTLE_COMBO_CHEERS_SIZE_X * g_ComboCheers[nCntCheers].fSizeX;
			pVtx[1].pos.y = g_ComboCheers[nCntCheers].pos.y - BATTLE_COMBO_CHEERS_SIZE_Y * g_ComboCheers[nCntCheers].fSizeY;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_ComboCheers[nCntCheers].pos.x - BATTLE_COMBO_CHEERS_SIZE_X * g_ComboCheers[nCntCheers].fSizeX;
			pVtx[2].pos.y =	g_ComboCheers[nCntCheers].pos.y + BATTLE_COMBO_CHEERS_SIZE_Y * g_ComboCheers[nCntCheers].fSizeY;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_ComboCheers[nCntCheers].pos.x + BATTLE_COMBO_CHEERS_SIZE_X * g_ComboCheers[nCntCheers].fSizeX;
			pVtx[3].pos.y = g_ComboCheers[nCntCheers].pos.y + BATTLE_COMBO_CHEERS_SIZE_Y * g_ComboCheers[nCntCheers].fSizeY;
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
		g_pVtxBuffBattle_ComboCheers->Unlock();
	}
	return S_OK;

}
void SetVertexBattle_ComboCheers(int ID)
{//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffBattle_ComboCheers->Lock(0, 0, (void**)&pVtx, 0);
		// 頂点座標の設定
	pVtx += ID * 4;
		//=======================LeftSide
			pVtx[0].pos.x = g_ComboCheers[ID].pos.x - BATTLE_COMBO_CHEERS_SIZE_X * g_ComboCheers[ID].fSizeX;
			pVtx[0].pos.y = g_ComboCheers[ID].pos.y - BATTLE_COMBO_CHEERS_SIZE_Y * g_ComboCheers[ID].fSizeY;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_ComboCheers[ID].pos.x + BATTLE_COMBO_CHEERS_SIZE_X * g_ComboCheers[ID].fSizeX;
			pVtx[1].pos.y = g_ComboCheers[ID].pos.y - BATTLE_COMBO_CHEERS_SIZE_Y * g_ComboCheers[ID].fSizeY;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_ComboCheers[ID].pos.x - BATTLE_COMBO_CHEERS_SIZE_X * g_ComboCheers[ID].fSizeX;
			pVtx[2].pos.y =	g_ComboCheers[ID].pos.y + BATTLE_COMBO_CHEERS_SIZE_Y * g_ComboCheers[ID].fSizeY;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_ComboCheers[ID].pos.x + BATTLE_COMBO_CHEERS_SIZE_X * g_ComboCheers[ID].fSizeX;
			pVtx[3].pos.y = g_ComboCheers[ID].pos.y + BATTLE_COMBO_CHEERS_SIZE_Y * g_ComboCheers[ID].fSizeY;
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

			pVtx[0].tex = D3DXVECTOR2(0.0f,BATTLE_COMBO_CHEERS_UNIT_SIZE * g_ComboCheers[ID].Type);
			pVtx[1].tex = D3DXVECTOR2(1.0f,BATTLE_COMBO_CHEERS_UNIT_SIZE * g_ComboCheers[ID].Type);
			pVtx[2].tex = D3DXVECTOR2(0.0f,BATTLE_COMBO_CHEERS_UNIT_SIZE * g_ComboCheers[ID].Type + BATTLE_COMBO_CHEERS_UNIT_SIZE);
			pVtx[3].tex = D3DXVECTOR2(1.0f,BATTLE_COMBO_CHEERS_UNIT_SIZE * g_ComboCheers[ID].Type + BATTLE_COMBO_CHEERS_UNIT_SIZE);

	g_pVtxBuffBattle_ComboCheers->Unlock();
}
