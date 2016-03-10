//=============================================================================
//
// リザルト画面処理 [result.cpp]
// HAL名古屋
//
//=============================================================================
#include "result.h"
#include "input.h"
//#include "fade.h"
#include "Fade_loading.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_RESULT		"data/TEXTURE/bg201.jpg"				// 読み込むテクスチャファイル名
#define	TEXTURE_RESULT_LOGO	"data/TEXTURE/result_logo.png"			// 読み込むテクスチャファイル名
#define	TEXTURE_CLEAR		"data/TEXTURE/bg202.jpg"				// 読み込むテクスチャファイル名
#define	TEXTURE_CLEAR_LOGO	"data/TEXTURE/gameclear_logo.png"		// 読み込むテクスチャファイル名
#define	TEXTURE_OVER		"data/TEXTURE/bg203.jpg"				// 読み込むテクスチャファイル名
#define	TEXTURE_OVER_LOGO	"data/TEXTURE/gameover_logo.png"		// 読み込むテクスチャファイル名
#define	RESULT_WIDTH		(480)									// リザルトの幅
#define	RESULT_HEIGHT		(80)									// リザルトの高さ
#define	RESULT_POS_X		(SCREEN_WIDTH / 2 - RESULT_WIDTH / 2)	// リザルトの位置(X座標)
#define	RESULT_POS_Y		(120)									// リザルトの位置(Y座標)

#define	COUNT_WAIT_DISP		(60 * 5)								// 表示待機時間

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexResult(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureResult = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;		// 頂点バッファインターフェースへのポインタ
LPDIRECT3DTEXTURE9		g_pTextureResultLogo = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResultLogo = NULL;	// 頂点バッファインターフェースへのポインタ

int g_nCountWait = 0;									// 待機時間
bool g_bGameClear = false;								// ゲームクリア

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitResult(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_nCountWait = 0;

	// 頂点情報の作成
	MakeVertexResult(pDevice);

	if(g_bGameClear == true)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
									TEXTURE_CLEAR,			// ファイルの名前
									&g_pTextureResult);		// 読み込むメモリー

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
									TEXTURE_CLEAR_LOGO,		// ファイルの名前
									&g_pTextureResultLogo);	// 読み込むメモリー
	}
	else
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
									TEXTURE_OVER,			// ファイルの名前
									&g_pTextureResult);		// 読み込むメモリー

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
									TEXTURE_OVER_LOGO,		// ファイルの名前
									&g_pTextureResultLogo);	// 読み込むメモリー
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitResult(void)
{
	if(g_pTextureResult != NULL)
	{// テクスチャの開放
		g_pTextureResult->Release();
		g_pTextureResult = NULL;
	}

	if(g_pVtxBuffResult != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}

	if(g_pTextureResultLogo != NULL)
	{// テクスチャの開放
		g_pTextureResultLogo->Release();
		g_pTextureResultLogo = NULL;
	}

	if(g_pVtxBuffResultLogo != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffResultLogo->Release();
		g_pVtxBuffResultLogo = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateResult(void)
{
	g_nCountWait++;
	if(g_nCountWait > COUNT_WAIT_DISP)
	{
		//SetFade(FADE_OUT, MODE_MAP);
		SetLoadStart(MODE_MAP);
	}

	if(GetKeyboardTrigger(DIK_RETURN))
	{// Enter押したら、フェードアウトしてモードを切り替えいく
		//SetFade(FADE_OUT, MODE_MAP);
		SetLoadStart(MODE_MAP);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
    pDevice->SetStreamSource(0, g_pVtxBuffResult, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureResult);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

	// 頂点バッファをデバイスのデータストリームにバインド
    pDevice->SetStreamSource(0, g_pVtxBuffResultLogo, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureResultLogo);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexResult(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
												FVF_VERTEX_2D,				// 使用する頂点フォーマット
												D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
												&g_pVtxBuffResult,			// 頂点バッファインターフェースへのポインタ
												NULL)))						// NULLに設定
	{
        return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

		// テクスチャのパースペクティブコレクト用
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

		// 頂点データをアンロックする
		g_pVtxBuffResult->Unlock();
	}

	// オブジェクトの頂点バッファを生成
    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
												FVF_VERTEX_2D,				// 使用する頂点フォーマット
												D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
												&g_pVtxBuffResultLogo,		// 頂点バッファインターフェースへのポインタ
												NULL)))						// NULLに設定
	{
        return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffResultLogo->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(RESULT_POS_X, RESULT_POS_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(RESULT_POS_X + RESULT_WIDTH, RESULT_POS_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(RESULT_POS_X, RESULT_POS_Y + RESULT_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(RESULT_POS_X + RESULT_WIDTH, RESULT_POS_Y + RESULT_HEIGHT, 0.0f);

		// テクスチャのパースペクティブコレクト用
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

		// 頂点データをアンロックする
		g_pVtxBuffResultLogo->Unlock();
	}

	return S_OK;
}

//=============================================================================
// ゲームクリア設定
//=============================================================================
void SetResult(bool bGameClear)
{
	g_bGameClear = bGameClear;
}

//=============================================================================
// ゲームクリアしたかどうか
//=============================================================================
bool IsGameClear(void)
{
	return g_bGameClear;
}
