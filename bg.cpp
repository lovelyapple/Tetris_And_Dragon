//=============================================================================
//
// 背景処理 [bg.cpp]
// HAL名古屋
//
//=============================================================================
#include "bg.h"
//#include "fade.h"
#include "input.h"
#include "Fade_loading.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_BG		"data/TEXTURE/bg100.png"	// 読み込むテクスチャファイル名
#define	BG_POS_X		(0)							// 背景の表示位置(左上Ｘ)
#define	BG_POS_Y		(0)							// 背景の表示位置(左上Ｙ)
#define	BG_WIDTH		(SCREEN_WIDTH)				// 背景のサイズ(横幅)
#define	BG_HEIGHT		(SCREEN_HEIGHT)				// 背景のサイズ(縦幅)
#define MOVE_LAYER_000	(0.006f)					// 移動速度(レイヤー０)
#define MOVE_LAYER_001	(0.004f)					// 移動速度(レイヤー１)
#define MOVE_LAYER_002	(0.002f)					// 移動速度(レイヤー２)

#define	NUM_BG			(3)							// 背景数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexBG(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_apTextureBG[NUM_BG] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBG = NULL;		// 頂点バッファインターフェースへのポインタ

D3DXVECTOR3				g_posBG;					// 背景の位置
D3DXVECTOR3				g_rotBG;					// 背景の回転量
float					g_aPosTexV[NUM_BG];			// テクスチャの位置(V値)
D3DXVECTOR3				g_aPosOffsetBG[NUM_BG];		// 背景の位置
D3DXVECTOR3				g_aPosBaseBG[NUM_BG];		// 背景の位置

const char *g_pFileName[] =
{
	"data/TEXTURE/bg100.png",
	"data/TEXTURE/bg101.png",
	"data/TEXTURE/bg102.png"
};

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBG(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_posBG = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotBG = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 頂点情報の作成
	MakeVertexBG(pDevice);

	// テクスチャの読み込み
	for(int nCntTex = 0; nCntTex < NUM_BG; nCntTex++)
	{
		g_aPosTexV[nCntTex] = 1.0f;
		g_aPosOffsetBG[nCntTex] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPosBaseBG[nCntTex] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
									g_pFileName[nCntTex],		// ファイルの名前
									&g_apTextureBG[nCntTex]);	// 読み込むメモリー
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBG(void)
{
	for(int nCntTex = 0; nCntTex < NUM_BG; nCntTex++)
	{
		if(g_apTextureBG[nCntTex] != NULL)
		{// テクスチャの開放
			g_apTextureBG[nCntTex]->Release();
			g_apTextureBG[nCntTex] = NULL;
		}
	}

	if(g_pVtxBuffBG != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffBG->Release();
		g_pVtxBuffBG = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBG(void)
{
	//if(GetKeyboardTrigger(DIK_H))
		//SetFade(FADE_OUT,MODE_RESULT);
		//SetLoadStart(MODE_RESULT);
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBG(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
    pDevice->SetStreamSource(0, g_pVtxBuffBG, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for(int nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_apTextureBG[nCntBG]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntBG * 4), NUM_POLYGON);
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexBG(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * NUM_BG,	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
												FVF_VERTEX_2D,						// 使用する頂点フォーマット
												D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
												&g_pVtxBuffBG,					// 頂点バッファインターフェースへのポインタ
												NULL)))								// NULLに設定
	{
        return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);

		for(int nCntBG = 0; nCntBG < NUM_BG; nCntBG++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(BG_POS_X, BG_POS_Y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(BG_POS_X + BG_WIDTH, BG_POS_Y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(BG_POS_X, BG_POS_Y + BG_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(BG_POS_X + BG_WIDTH, BG_POS_Y + BG_HEIGHT, 0.0f);

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

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		// 頂点データをアンロックする
		g_pVtxBuffBG->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 背景の基準位置を設定
//=============================================================================
void SetPositionBaseBG(void)
{
	for(int nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		g_aPosBaseBG[nCntBG] = g_aPosOffsetBG[nCntBG];
	}
}
