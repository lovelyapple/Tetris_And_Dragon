//=============================================================================
//[Battle_Effect_TimeLeftCount.cpp]
// HAL 名古屋　GP11A172 17 50323 リ リン
//=============================================================================

#include "Battle_Effect_TimeLeftCount.h"
#include "main.h"
#include "game.h"
#include "Battle_Main.h"
#include "Tetris_main.h"
#include "TimeLeftCage.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************

#define TEXTURE_BATTLE_EFFECT_TIME_LEFT_COUNT_NUMBER	("data/TEXTURE/Time321count.png")
#define BATTLE_EFFECT_TIME_LEFT_COUNT_BG_POS_X	(VALUE_FIELD_START_X)
#define BATTLE_EFFECT_TIME_LEFT_COUNT_BG_POS_Y	(VALUE_FIELD_START_Y)
#define BATTLE_EFFECT_TIME_LEFT_COUNT_BG_SIZE_X	(VALUE_FIELD_SIZE_X)
#define BATTLE_EFFECT_TIME_LEFT_COUNT_BG_SIZE_Y	(VALUE_FIELD_SIZE_Y)
#define BATTLE_EFFECT_TIME_LEFT_COUNT_NUMBER_POS_X	(BATTLE_EFFECT_TIME_LEFT_COUNT_BG_POS_X + BATTLE_EFFECT_TIME_LEFT_COUNT_BG_SIZE_X/2)
#define BATTLE_EFFECT_TIME_LEFT_COUNT_NUMBER_POS_Y	(BATTLE_EFFECT_TIME_LEFT_COUNT_BG_POS_Y - BATTLE_EFFECT_TIME_LEFT_COUNT_BG_SIZE_Y/2 - 100.0f)
#define BATTLE_EFFECT_TIME_LEFT_COUNT_NUMBER_SIZE_X	(50.0f)
#define BATTLE_EFFECT_TIME_LEFT_COUNT_NUMBER_UNIT_SIZE	(1.0f/3.0f)
#define BATTLE_EFFECT_TIME_LEFT_COUNT_NUMBER_SIZE_Y	(70.0f)
#define BATTLE_EFFECT_TIME_LEFT_COUNT_NUMBER_LIFE	(5)
#define BATTLE_EFFECT_TIME_LEFT_NUMBER_LIFE			(30)
#define  BATTLE_EFFECT_TIME_LEFT_COUNT_MAX_NUMBER	(16)
#define BATTLE_EFFECT_TIME_LEFT_MAX_ACTIVE	(3)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexEffect_Time_count_BG(LPDIRECT3DDEVICE9 pDevice);
HRESULT MakeVertexEffect_Time_count_Number(LPDIRECT3DDEVICE9 pDevice);
void SetVertexEffect_Time_count_Number(int ID);
void InitEffect_Time_Count_Number(int ID);
//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureEffect_Time_count_Number = NULL;	//テクスチャメモリ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect_Time_count_Number = NULL;	//頂点座標バッファ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect_Time_count_BG = NULL;	//頂点座標バッファ

bool g_TimeCountDraw = false;
int	 g_TimeLeft;
int	 g_TimeLeftWK;
typedef struct
{
	bool	bUse;
	int		nNumber;
	int		Life;
	float	fSize;
	float	fAlpha;
}TIME_LEFT_COUNT;
TIME_LEFT_COUNT g_TimeLeftCount[BATTLE_EFFECT_TIME_LEFT_COUNT_MAX_NUMBER];
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBattle_Effect_TimeLeftCount(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_TimeLeft = 0;
	g_TimeCountDraw = false;
	g_TimeLeftWK = 0;
	for (int nCntNumber = 0; nCntNumber < BATTLE_EFFECT_TIME_LEFT_COUNT_MAX_NUMBER;nCntNumber++)
		InitEffect_Time_Count_Number(nCntNumber);

	D3DXCreateTextureFromFile(pDevice,
							TEXTURE_BATTLE_EFFECT_TIME_LEFT_COUNT_NUMBER,
							&g_pTextureEffect_Time_count_Number);

	MakeVertexEffect_Time_count_BG(pDevice);
	MakeVertexEffect_Time_count_Number(pDevice);
	return S_OK;
}
void InitEffect_Time_Count_Number(int ID)
{
	g_TimeLeftCount[ID].bUse = false;
	g_TimeLeftCount[ID].fAlpha = 1.0f;
	g_TimeLeftCount[ID].Life  = BATTLE_EFFECT_TIME_LEFT_COUNT_NUMBER_LIFE;
	g_TimeLeftCount[ID].fSize = 1.0f;
	g_TimeLeftCount[ID].nNumber = 0;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBattle_Effect_TimeLeftCount(void)
{
	if(g_pTextureEffect_Time_count_Number != NULL)
	{// テクスチャの開放
		g_pTextureEffect_Time_count_Number->Release();
		g_pTextureEffect_Time_count_Number = NULL;
	}

	if(g_pVtxBuffEffect_Time_count_Number != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffEffect_Time_count_Number->Release();
		g_pVtxBuffEffect_Time_count_Number = NULL;
	}

	if(g_pVtxBuffEffect_Time_count_BG != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffEffect_Time_count_BG->Release();
		g_pVtxBuffEffect_Time_count_BG = NULL;
	}
}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateBattle_Effect_TimeLeftCount(void)
{
	int Battle_Stats = GetBattleStats();
	int time = GetTimeLeft();
	int TetrisTime = (int)(GetTimeLeft()/60); 
	if(Battle_Stats == BATTLE_TETRIS)
	{
		if(TetrisTime < 3 && g_TimeLeftWK != TetrisTime)
		{
			g_TimeLeftCount[0].bUse = true;
			g_TimeLeftCount[0].fAlpha = 1.0f;
			g_TimeLeftWK = TetrisTime;
			g_TimeLeftCount[0].nNumber = TetrisTime;
			int Count = 0;
			while ( Count < BATTLE_EFFECT_TIME_LEFT_MAX_ACTIVE)
			{
				for(int nTime = 1; nTime < BATTLE_EFFECT_TIME_LEFT_COUNT_MAX_NUMBER;nTime++)
				{
					if(g_TimeLeftCount[nTime].bUse == true)
						continue;
					g_TimeLeftCount[nTime].bUse = true;
					g_TimeLeftCount[nTime].Life		+= Count * 10;
					g_TimeLeftCount[nTime].fAlpha	= 1.0f;
					g_TimeLeftCount[nTime].fSize	= 1.0f;
					g_TimeLeftCount[nTime].nNumber	= TetrisTime;
					Count ++ ;
					break;
				}
			}
		}

	}

	for(int nTime = 1; nTime < BATTLE_EFFECT_TIME_LEFT_COUNT_MAX_NUMBER;nTime++)
	{
		if(g_TimeLeftCount[nTime].bUse == false)
			continue;
		g_TimeLeftCount[nTime].Life -- ;
		if(g_TimeLeftCount[nTime].Life	< 0 )
		{
		g_TimeLeftCount[nTime].fAlpha -= 1.0f / BATTLE_EFFECT_TIME_LEFT_NUMBER_LIFE;
		g_TimeLeftCount[nTime].fSize += 1.0f / BATTLE_EFFECT_TIME_LEFT_NUMBER_LIFE;
		}
		if(g_TimeLeftCount[nTime].fAlpha <= 0)
			InitEffect_Time_Count_Number(nTime);
		SetVertexEffect_Time_count_Number(nTime);
	}
	g_TimeLeftCount[0].fAlpha -= 0.01f;
	if(g_TimeLeftCount[0].fAlpha < 0.0f)
		InitEffect_Time_Count_Number(0);
	SetVertexEffect_Time_count_Number(0);

}
//=============================================================================
// 描画処理
//=============================================================================
void DrawBattle_Effect_TimeLeftCount(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//フィールドの描画
	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffEffect_Time_count_BG,0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetTexture(0,NULL);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);



	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffEffect_Time_count_Number,0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetTexture(0,g_pTextureEffect_Time_count_Number);

	for (int nCntNumber = 0; nCntNumber < BATTLE_EFFECT_TIME_LEFT_COUNT_MAX_NUMBER;nCntNumber++)
	{
		if(g_TimeLeftCount[nCntNumber].bUse == false)
			continue;
		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntNumber * 4, NUM_POLYGON);
	}
}
//=============================================================================
// BG頂点の作成
//=============================================================================
HRESULT MakeVertexEffect_Time_count_BG(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * 1,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
		FVF_VERTEX_2D,						// 使用する頂点フォーマット
		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
		&g_pVtxBuffEffect_Time_count_BG,					// 頂点バッファインターフェースへのポインタ
		NULL)))								// NULLに設定
	{
		return E_FAIL;
	}
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffEffect_Time_count_BG->Lock(0, 0, (void**)&pVtx, 0);
			// 頂点座標の設定

		//=======================LeftSide
		pVtx[0].pos.x = BATTLE_EFFECT_TIME_LEFT_COUNT_BG_POS_X;
		pVtx[0].pos.y = BATTLE_EFFECT_TIME_LEFT_COUNT_BG_POS_Y;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = BATTLE_EFFECT_TIME_LEFT_COUNT_BG_POS_X + BATTLE_EFFECT_TIME_LEFT_COUNT_BG_SIZE_X;
		pVtx[1].pos.y = BATTLE_EFFECT_TIME_LEFT_COUNT_BG_POS_Y;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = BATTLE_EFFECT_TIME_LEFT_COUNT_BG_POS_X;
		pVtx[2].pos.y =	BATTLE_EFFECT_TIME_LEFT_COUNT_BG_POS_Y + BATTLE_EFFECT_TIME_LEFT_COUNT_BG_SIZE_Y;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = BATTLE_EFFECT_TIME_LEFT_COUNT_BG_POS_X + BATTLE_EFFECT_TIME_LEFT_COUNT_BG_SIZE_X;
		pVtx[3].pos.y = BATTLE_EFFECT_TIME_LEFT_COUNT_BG_POS_Y + BATTLE_EFFECT_TIME_LEFT_COUNT_BG_SIZE_Y;
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
		g_pVtxBuffEffect_Time_count_BG->Unlock();
	}
	return S_OK;

}
//=============================================================================
// Shining頂点の作成
//=============================================================================
HRESULT MakeVertexEffect_Time_count_Number(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * BATTLE_EFFECT_TIME_LEFT_COUNT_MAX_NUMBER,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
		FVF_VERTEX_2D,						// 使用する頂点フォーマット
		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
		&g_pVtxBuffEffect_Time_count_Number,					// 頂点バッファインターフェースへのポインタ
		NULL)))								// NULLに設定
	{
		return E_FAIL;
	}
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffEffect_Time_count_Number->Lock(0, 0, (void**)&pVtx, 0);
			// 頂点座標の設定
		for (int nCntNumber = 0; nCntNumber < BATTLE_EFFECT_TIME_LEFT_COUNT_MAX_NUMBER;nCntNumber++,pVtx += 4)
		{
			//=======================LeftSide
			pVtx[0].pos.x = BATTLE_EFFECT_TIME_LEFT_COUNT_NUMBER_POS_X - BATTLE_EFFECT_TIME_LEFT_COUNT_NUMBER_SIZE_X;
			pVtx[0].pos.y = BATTLE_EFFECT_TIME_LEFT_COUNT_NUMBER_POS_Y - BATTLE_EFFECT_TIME_LEFT_COUNT_NUMBER_SIZE_Y;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = BATTLE_EFFECT_TIME_LEFT_COUNT_NUMBER_POS_X + BATTLE_EFFECT_TIME_LEFT_COUNT_NUMBER_SIZE_X;
			pVtx[1].pos.y = BATTLE_EFFECT_TIME_LEFT_COUNT_NUMBER_POS_Y - BATTLE_EFFECT_TIME_LEFT_COUNT_NUMBER_SIZE_Y;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = BATTLE_EFFECT_TIME_LEFT_COUNT_NUMBER_POS_X - BATTLE_EFFECT_TIME_LEFT_COUNT_NUMBER_SIZE_X;
			pVtx[2].pos.y =	BATTLE_EFFECT_TIME_LEFT_COUNT_NUMBER_POS_Y + BATTLE_EFFECT_TIME_LEFT_COUNT_NUMBER_SIZE_Y;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = BATTLE_EFFECT_TIME_LEFT_COUNT_NUMBER_POS_X + BATTLE_EFFECT_TIME_LEFT_COUNT_NUMBER_SIZE_X;
			pVtx[3].pos.y = BATTLE_EFFECT_TIME_LEFT_COUNT_NUMBER_POS_Y + BATTLE_EFFECT_TIME_LEFT_COUNT_NUMBER_SIZE_Y;
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
		g_pVtxBuffEffect_Time_count_Number->Unlock();
	}
	return S_OK;

}
void SetVertexEffect_Time_count_Number(int ID)
{//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffEffect_Time_count_Number->Lock(0, 0, (void**)&pVtx, 0);
		// 頂点座標の設定
	pVtx += ID * 4;
		//=======================LeftSide
		pVtx[0].pos.x = BATTLE_EFFECT_TIME_LEFT_COUNT_NUMBER_POS_X - BATTLE_EFFECT_TIME_LEFT_COUNT_NUMBER_SIZE_X * g_TimeLeftCount[ID].fSize;
		pVtx[0].pos.y = BATTLE_EFFECT_TIME_LEFT_COUNT_NUMBER_POS_Y - BATTLE_EFFECT_TIME_LEFT_COUNT_NUMBER_SIZE_Y * g_TimeLeftCount[ID].fSize;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = BATTLE_EFFECT_TIME_LEFT_COUNT_NUMBER_POS_X + BATTLE_EFFECT_TIME_LEFT_COUNT_NUMBER_SIZE_X * g_TimeLeftCount[ID].fSize;
		pVtx[1].pos.y = BATTLE_EFFECT_TIME_LEFT_COUNT_NUMBER_POS_Y - BATTLE_EFFECT_TIME_LEFT_COUNT_NUMBER_SIZE_Y * g_TimeLeftCount[ID].fSize;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = BATTLE_EFFECT_TIME_LEFT_COUNT_NUMBER_POS_X - BATTLE_EFFECT_TIME_LEFT_COUNT_NUMBER_SIZE_X * g_TimeLeftCount[ID].fSize;
		pVtx[2].pos.y =	BATTLE_EFFECT_TIME_LEFT_COUNT_NUMBER_POS_Y + BATTLE_EFFECT_TIME_LEFT_COUNT_NUMBER_SIZE_Y * g_TimeLeftCount[ID].fSize;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = BATTLE_EFFECT_TIME_LEFT_COUNT_NUMBER_POS_X + BATTLE_EFFECT_TIME_LEFT_COUNT_NUMBER_SIZE_X * g_TimeLeftCount[ID].fSize;
		pVtx[3].pos.y = BATTLE_EFFECT_TIME_LEFT_COUNT_NUMBER_POS_Y + BATTLE_EFFECT_TIME_LEFT_COUNT_NUMBER_SIZE_Y * g_TimeLeftCount[ID].fSize;
			pVtx[3].pos.z = 0.0f;
		pVtx[3].pos.z = 0.0f;
		// rhwの設定
		pVtx[0].rhw =
			pVtx[1].rhw =
			pVtx[2].rhw =
			pVtx[3].rhw = 1.0f;
		int nBox = g_TimeLeftCount[ID].nNumber;
		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_TimeLeftCount[ID].fAlpha);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_TimeLeftCount[ID].fAlpha);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_TimeLeftCount[ID].fAlpha);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_TimeLeftCount[ID].fAlpha);

		pVtx[0].tex = D3DXVECTOR2(BATTLE_EFFECT_TIME_LEFT_COUNT_NUMBER_UNIT_SIZE * nBox,0.0f);
		pVtx[1].tex = D3DXVECTOR2(BATTLE_EFFECT_TIME_LEFT_COUNT_NUMBER_UNIT_SIZE * nBox + BATTLE_EFFECT_TIME_LEFT_COUNT_NUMBER_UNIT_SIZE,0.0f);
		pVtx[2].tex = D3DXVECTOR2(BATTLE_EFFECT_TIME_LEFT_COUNT_NUMBER_UNIT_SIZE * nBox,1.0f);
		pVtx[3].tex = D3DXVECTOR2(BATTLE_EFFECT_TIME_LEFT_COUNT_NUMBER_UNIT_SIZE * nBox + BATTLE_EFFECT_TIME_LEFT_COUNT_NUMBER_UNIT_SIZE,1.0f);

	g_pVtxBuffEffect_Time_count_Number->Unlock();
}
