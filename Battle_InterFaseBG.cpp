//=============================================================================
//[Battle_InterFaseBG.cpp]
// HAL 名古屋　GP11A172 17 50323 リ リン
//=============================================================================

#include "Battle_InterFaseBG.h"
#include "main.h"
#include "game.h"
#include "Battle_Main.h"
#include "Function.h"
#include "group_in_queue.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_BATTLE_INTERFACE_BG ("data/TEXTURE/Battle_InterFace.png")
#define BATTLE_INTERFACE_BG_POS_X	(SYS_BATTLE_POS_START_X)
#define BATTLE_INTERFACE_BG_POS_Y	(0.0f)
#define BATTLE_INTERFACE_BG_POS_END_X	(SYS_BATTLE_POS_END_X)
#define BATTLE_INTERFACE_BG_POS_END_Y	(SCREEN_HEIGHT)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexInterFaceBG(LPDIRECT3DDEVICE9 pDevice);
void	SetVerTexInterFaceBG(void);
//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureInterFaceBG	= NULL;	//テクスチャメモリ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffInterFaceBG = NULL;	//頂点座標バッファ
float	g_FrontAlpha;
INTERFACE_BG_PHASE g_FrontPhase;
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBattle_InterFaseBG(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	g_FrontAlpha = 1.0f;
	g_FrontPhase = INTERFACE_BG_PHASE_KEEP;
	D3DXCreateTextureFromFile(pDevice,
							TEXTURE_BATTLE_INTERFACE_BG,
							&g_pTextureInterFaceBG);

	MakeVertexInterFaceBG(pDevice);
	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void UninitBattle_InterFaseBG(void)
{
	if(g_pTextureInterFaceBG != NULL)
	{// テクスチャの開放
		g_pTextureInterFaceBG->Release();
		g_pTextureInterFaceBG = NULL;
	}

	if(g_pVtxBuffInterFaceBG != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffInterFaceBG->Release();
		g_pVtxBuffInterFaceBG = NULL;
	}
}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateBattle_InterFaseBG(void)
{
	g_FrontAlpha = SetMAX(g_FrontAlpha,1.0f,0.0f,2);
	SetVerTexInterFaceBG();

	switch(g_FrontPhase)
	{
	case INTERFACE_BG_PHASE_KEEP :
		break;
	case INTERFACE_BG_PHASE_IN:
		g_FrontAlpha += 1/60.0f;
		if(g_FrontAlpha > 1.0f)
		{
			g_FrontAlpha = 1.0f;
			g_FrontPhase = INTERFACE_BG_PHASE_KEEP;
		}
		break;
	case INTERFACE_BG_PHASE_OUT:
		g_FrontAlpha -= 1/60.0f;
		if(g_FrontAlpha < 0.0f)
		{
			g_FrontAlpha = 0.0f;
			g_FrontPhase = INTERFACE_BG_PHASE_KEEP;
		}
		break;
	case INTERFACE_BG_PHASE_RESET:break;
	case INTERFACE_BG_PHASE_MAX:break;
	}

}
void SetInterPhaseChange(INTERFACE_BG_PHASE phase)
{
	g_FrontPhase = phase;
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBattle_InterFaseBG(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//フィールドの描画
	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffInterFaceBG,0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetTexture(0,g_pTextureInterFaceBG);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, NUM_POLYGON);
}
//=============================================================================
// Shining頂点の作成
//=============================================================================
HRESULT MakeVertexInterFaceBG(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * 2,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
		FVF_VERTEX_2D,						// 使用する頂点フォーマット
		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
		&g_pVtxBuffInterFaceBG,					// 頂点バッファインターフェースへのポインタ
		NULL)))								// NULLに設定
	{
		return E_FAIL;
	}
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffInterFaceBG->Lock(0, 0, (void**)&pVtx, 0);
			// 頂点座標の設定

		//=======================LeftSide
		pVtx[0].pos.x = BATTLE_INTERFACE_BG_POS_X;
		pVtx[0].pos.y = BATTLE_INTERFACE_BG_POS_Y;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = BATTLE_INTERFACE_BG_POS_END_X;
		pVtx[1].pos.y = BATTLE_INTERFACE_BG_POS_Y;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = BATTLE_INTERFACE_BG_POS_X;
		pVtx[2].pos.y =	BATTLE_INTERFACE_BG_POS_END_Y;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = BATTLE_INTERFACE_BG_POS_END_X;
		pVtx[3].pos.y = BATTLE_INTERFACE_BG_POS_END_Y;
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
		pVtx[1].tex = D3DXVECTOR2(0.5f,0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.5f,1.0f);

		pVtx += 4;
		//=======================LeftSide
		pVtx[0].pos.x = BATTLE_INTERFACE_BG_POS_X;
		pVtx[0].pos.y = BATTLE_INTERFACE_BG_POS_Y;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = BATTLE_INTERFACE_BG_POS_END_X;
		pVtx[1].pos.y = BATTLE_INTERFACE_BG_POS_Y;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = BATTLE_INTERFACE_BG_POS_X;
		pVtx[2].pos.y =	BATTLE_INTERFACE_BG_POS_END_Y;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = BATTLE_INTERFACE_BG_POS_END_X;
		pVtx[3].pos.y = BATTLE_INTERFACE_BG_POS_END_Y;
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

		pVtx[0].tex = D3DXVECTOR2(0.5f,0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.5f,1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);
		// 頂点データをアンロックする
		g_pVtxBuffInterFaceBG->Unlock();
	}
	return S_OK;

}
void SetVerTexInterFaceBG(void)
{
//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffInterFaceBG->Lock(0, 0, (void**)&pVtx, 0);
		// 頂点座標の設定

	//=======================LeftSide
	pVtx[0].pos.x = BATTLE_INTERFACE_BG_POS_X;
	pVtx[0].pos.y = BATTLE_INTERFACE_BG_POS_Y;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = BATTLE_INTERFACE_BG_POS_END_X;
	pVtx[1].pos.y = BATTLE_INTERFACE_BG_POS_Y;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = BATTLE_INTERFACE_BG_POS_X;
	pVtx[2].pos.y =	BATTLE_INTERFACE_BG_POS_END_Y;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = BATTLE_INTERFACE_BG_POS_END_X;
	pVtx[3].pos.y = BATTLE_INTERFACE_BG_POS_END_Y;
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
	pVtx[1].tex = D3DXVECTOR2(0.5f,0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.5f,1.0f);

	pVtx += 4;
	//=======================LeftSide
	pVtx[0].pos.x = BATTLE_INTERFACE_BG_POS_X;
	pVtx[0].pos.y = BATTLE_INTERFACE_BG_POS_Y;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = BATTLE_INTERFACE_BG_POS_END_X;
	pVtx[1].pos.y = BATTLE_INTERFACE_BG_POS_Y;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = BATTLE_INTERFACE_BG_POS_X;
	pVtx[2].pos.y =	BATTLE_INTERFACE_BG_POS_END_Y;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = BATTLE_INTERFACE_BG_POS_END_X;
	pVtx[3].pos.y = BATTLE_INTERFACE_BG_POS_END_Y;
	pVtx[3].pos.z = 0.0f;
	// rhwの設定
	pVtx[0].rhw =
		pVtx[1].rhw =
		pVtx[2].rhw =
		pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_FrontAlpha);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_FrontAlpha);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_FrontAlpha);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_FrontAlpha);

	pVtx[0].tex = D3DXVECTOR2(0.5f,0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.5f,1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);
	// 頂点データをアンロックする
	g_pVtxBuffInterFaceBG->Unlock();
}
