//=============================================================================
//[Battle_InterFaseBG.cpp]
// HAL 名古屋　GP11A172 17 50323 リ リン
//=============================================================================

#include "Battle_Stage_End.h"
#include "Battle_Main.h"
#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_BATTLE_STAGE_END_WIN ("data/TEXTURE/stage_clear.png")
#define TEXTURE_BATTLE_STAGE_END_LOST ("data/TEXTURE/stage_lost.png")
#define BATTLE_STAGE_END_POS_X	(SYS_BATTLE_CENTER_X + 40.0f)
#define BATTLE_STAGE_END_POS_Y	(SYS_BATTLE_CENTER_Y * 0.8f)
#define BATTLE_STAGE_EN_DSIZE_X	(SYS_BATTLE_SIZE_X /2 * 0.6f)
#define BATTLE_STAGE_EN_DSIZE_Y	(40.0f)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexBattle_Stage_End(LPDIRECT3DDEVICE9 pDevice);
//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureBattle_Stage_End_win	= NULL;	//テクスチャメモリ
LPDIRECT3DTEXTURE9		g_pTextureBattle_Stage_End_lost	= NULL;	//テクスチャメモリ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBattle_Stage_End = NULL;	//頂点座標バッファ

bool	g_Stage_result;
int		g_ResultType;
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBattle_Stage_End(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	g_Stage_result = false;
	g_ResultType = STAGE_RESULT_WIN;
	D3DXCreateTextureFromFile(pDevice,
							TEXTURE_BATTLE_STAGE_END_WIN,
							&g_pTextureBattle_Stage_End_win);

	D3DXCreateTextureFromFile(pDevice,
							TEXTURE_BATTLE_STAGE_END_LOST,
							&g_pTextureBattle_Stage_End_lost);

	MakeVertexBattle_Stage_End(pDevice);
	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void UninitBattle_Stage_End(void)
{
	if(g_pTextureBattle_Stage_End_win != NULL)
	{// テクスチャの開放
		g_pTextureBattle_Stage_End_win->Release();
		g_pTextureBattle_Stage_End_win = NULL;
	}

	if(g_pTextureBattle_Stage_End_lost != NULL)
	{// テクスチャの開放
		g_pTextureBattle_Stage_End_lost->Release();
		g_pTextureBattle_Stage_End_lost = NULL;
	}

	if(g_pVtxBuffBattle_Stage_End != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffBattle_Stage_End->Release();
		g_pVtxBuffBattle_Stage_End = NULL;
	}
}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateBattle_Stage_End(void)
{
	//g_Stage_result = false;
	//g_ResultType = STAGE_RESULT_WIN;
}
void SetStageResult(int Result)
{
	g_Stage_result = true;
	g_ResultType = Result;
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBattle_Stage_End(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//フィールドの描画
	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffBattle_Stage_End,0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if(g_Stage_result == true)
	{
		if(g_ResultType == STAGE_RESULT_WIN)
		pDevice->SetTexture(0,g_pTextureBattle_Stage_End_win);
		if(g_ResultType == STAGE_RESULT_LOST)
		pDevice->SetTexture(0,g_pTextureBattle_Stage_End_lost);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}
}
//=============================================================================
// Shining頂点の作成
//=============================================================================
HRESULT MakeVertexBattle_Stage_End(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
		FVF_VERTEX_2D,						// 使用する頂点フォーマット
		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
		&g_pVtxBuffBattle_Stage_End,					// 頂点バッファインターフェースへのポインタ
		NULL)))								// NULLに設定
	{
		return E_FAIL;
	}
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffBattle_Stage_End->Lock(0, 0, (void**)&pVtx, 0);
			// 頂点座標の設定

		//=======================LeftSide
		pVtx[0].pos.x = BATTLE_STAGE_END_POS_X - BATTLE_STAGE_EN_DSIZE_X;
		pVtx[0].pos.y = BATTLE_STAGE_END_POS_Y - BATTLE_STAGE_EN_DSIZE_Y;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = BATTLE_STAGE_END_POS_X + BATTLE_STAGE_EN_DSIZE_X;
		pVtx[1].pos.y = BATTLE_STAGE_END_POS_Y - BATTLE_STAGE_EN_DSIZE_Y;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = BATTLE_STAGE_END_POS_X - BATTLE_STAGE_EN_DSIZE_X;
		pVtx[2].pos.y =	BATTLE_STAGE_END_POS_Y + BATTLE_STAGE_EN_DSIZE_Y;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = BATTLE_STAGE_END_POS_X + BATTLE_STAGE_EN_DSIZE_X;
		pVtx[3].pos.y = BATTLE_STAGE_END_POS_Y + BATTLE_STAGE_EN_DSIZE_Y;
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

		
		// 頂点データをアンロックする
		g_pVtxBuffBattle_Stage_End->Unlock();
	}
	return S_OK;

}
