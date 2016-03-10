
//=============================================================================
// ブロック破壊効果 [BlockShining.cpp]
// HAL 名古屋　GP11A172 17 50323 リ リン
//=============================================================================
#include "Effect_Tetris_Block_Shining.h"
#include "Tetris_main.h"
#include "main.h"
#include "Function.h"
#include "game.h"
#include "input.h"
//*****************************************************************************
//使用注意
//*****************************************************************************
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_BLOCK_SHINGING		(64)
#define BLOCK_SHINING_TEX_UNIT	(1.0f / 6)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexBlockShining(LPDIRECT3DDEVICE9 pDevice);
void SetVerTexBlockShining(int Shiningn_ID);
void InitShiningSingle(int ID);
//*****************************************************************************
// 構造体宣言
//*****************************************************************************
typedef struct
{
	bool	bUse;
	int		nPhase;
	D3DXVECTOR3 pos;
}BLOCK_SHINING;

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureBlockShining	= NULL;	//テクスチャメモリ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBlockShining = NULL;	//頂点座標バッファ
BLOCK_SHINING	g_BlockShining[MAX_BLOCK_SHINGING];			//分裂ブロックの構造体
int	g_Timecount;			//時間制御変数
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBlockShining(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for(int nCntShining = 0;nCntShining < MAX_BLOCK_SHINGING;nCntShining++)
	{
		InitShiningSingle(nCntShining);
	}
		D3DXCreateTextureFromFile(pDevice,
								TEXTURE_BLOCK_SHINING,
								&g_pTextureBlockShining);

	MakeVertexBlockShining(pDevice);
	g_Timecount = 0;
	return S_OK;
}
void InitShiningSingle(int ID)
{
	g_BlockShining[ID].bUse = false;
	g_BlockShining[ID].nPhase = 0;
	g_BlockShining[ID].pos	= D3DXVECTOR3(0.0f,0.0f,0.0f);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBlockShining(void)
{
	if(g_pTextureBlockShining != NULL)
	{// テクスチャの開放
		g_pTextureBlockShining->Release();
		g_pTextureBlockShining = NULL;
	}

	if(g_pVtxBuffBlockShining != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffBlockShining->Release();
		g_pVtxBuffBlockShining = NULL;
	}
}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateBlockShining(void)
{
	g_Timecount ++;
	//if(GetKeyboardTrigger(DIK_SPACE))
	//{
	//	SetBlockShining(D3DXVECTOR3(600.0f,600.0f,0.0));
	//}


	for(int nCntShining = 0;nCntShining < MAX_BLOCK_SHINGING;nCntShining++)
	{
		if(g_BlockShining[nCntShining].bUse == false)
			continue;
		if(g_Timecount%2 ==0)
			g_BlockShining[nCntShining].nPhase ++;
		if(g_BlockShining[nCntShining].nPhase > 5)
			InitShiningSingle(nCntShining);
		SetVerTexBlockShining(nCntShining);

	}
	g_Timecount = SetMAX(g_Timecount,60,0,1);

}
//=============================================================================
// 特定のPointを初期化する
//=============================================================================
void SetBlockShining(D3DXVECTOR3 pos)
{
	for(int nCntShining = 0;nCntShining < MAX_BLOCK_SHINGING;nCntShining++)
	{
		if(g_BlockShining[nCntShining].bUse == true)
			continue;
		g_BlockShining[nCntShining].bUse = true;
		g_BlockShining[nCntShining].pos	= pos;
		break;
	}

}
//=============================================================================
// 描画処理
//=============================================================================
void DrawBlockShining(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//フィールドの描画
	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffBlockShining, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetTexture(0,g_pTextureBlockShining);
	for(int nCntShining = 0;nCntShining < MAX_BLOCK_SHINGING;nCntShining++)
	{
		if(g_BlockShining[nCntShining].bUse == false)
			continue;
		// ポリゴンの描画

		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);//set
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);//in
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntShining * 4), NUM_POLYGON);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);//set
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);// out
	}

}
//=============================================================================
// Shining頂点の作成
//=============================================================================
HRESULT MakeVertexBlockShining(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * MAX_BLOCK_SHINGING,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
		FVF_VERTEX_2D,						// 使用する頂点フォーマット
		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
		&g_pVtxBuffBlockShining,					// 頂点バッファインターフェースへのポインタ
		NULL)))								// NULLに設定
	{
		return E_FAIL;
	}
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffBlockShining->Lock(0, 0, (void**)&pVtx, 0);

		for(int nCntShining = 0;nCntShining < MAX_BLOCK_SHINGING;nCntShining++,pVtx += 4)	
		{
			// 頂点座標の設定
			pVtx[0].pos.x = g_BlockShining[nCntShining].pos.x ;
			pVtx[0].pos.y = g_BlockShining[nCntShining].pos.y - VALUE_BLOCK_SIZE;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_BlockShining[nCntShining].pos.x + VALUE_BLOCK_SIZE;
			pVtx[1].pos.y = g_BlockShining[nCntShining].pos.y - VALUE_BLOCK_SIZE;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_BlockShining[nCntShining].pos.x;
			pVtx[2].pos.y = g_BlockShining[nCntShining].pos.y;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_BlockShining[nCntShining].pos.x + VALUE_BLOCK_SIZE;
			pVtx[3].pos.y = g_BlockShining[nCntShining].pos.y;
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
		g_pVtxBuffBlockShining->Unlock();
	}
	return S_OK;

}
//=============================================================================
// Shiningnの設定
//=============================================================================
void SetVerTexBlockShining(int Shiningn_ID)
{
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffBlockShining->Lock(0, 0, (void**)&pVtx, 0);
	// 頂点座標の設定
	pVtx += (Shiningn_ID *4);
	// 頂点座標の設定
	pVtx[0].pos.x = g_BlockShining[Shiningn_ID].pos.x ;
	pVtx[0].pos.y = g_BlockShining[Shiningn_ID].pos.y - VALUE_BLOCK_SIZE;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_BlockShining[Shiningn_ID].pos.x + VALUE_BLOCK_SIZE;
	pVtx[1].pos.y = g_BlockShining[Shiningn_ID].pos.y - VALUE_BLOCK_SIZE;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_BlockShining[Shiningn_ID].pos.x;
	pVtx[2].pos.y = g_BlockShining[Shiningn_ID].pos.y;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_BlockShining[Shiningn_ID].pos.x + VALUE_BLOCK_SIZE;
	pVtx[3].pos.y = g_BlockShining[Shiningn_ID].pos.y;
	pVtx[3].pos.z = 0.0f;

	// rhwの設定
	pVtx[0].rhw =
		pVtx[1].rhw =
		pVtx[2].rhw =
		pVtx[3].rhw = 1.0f;
	// 頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);


	// テクスチャ座標の設定
	
	pVtx[0].tex = D3DXVECTOR2(BLOCK_SHINING_TEX_UNIT * g_BlockShining[Shiningn_ID].nPhase,0.0f);
	pVtx[1].tex = D3DXVECTOR2(BLOCK_SHINING_TEX_UNIT * g_BlockShining[Shiningn_ID].nPhase + BLOCK_SHINING_TEX_UNIT,0.0f);
	pVtx[2].tex = D3DXVECTOR2(BLOCK_SHINING_TEX_UNIT * g_BlockShining[Shiningn_ID].nPhase,1.0f);
	pVtx[3].tex = D3DXVECTOR2(BLOCK_SHINING_TEX_UNIT * g_BlockShining[Shiningn_ID].nPhase + BLOCK_SHINING_TEX_UNIT,1.0f);

	// 頂点データをアンロックする
	g_pVtxBuffBlockShining->Unlock();

}
