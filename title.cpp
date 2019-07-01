//=============================================================================
//
// タイトル画面処理 [title.cpp]
// HAL名古屋
//
//=============================================================================
#include "title.h"
#include "input.h"
//#include "fade.h"
#include "sound.h"
#include "Fade_loading.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_TITLE			"data/TEXTURE/titleBG.png"					// 読み込むテクスチャファイル名
#define	TEXTURE_TITLE_LOGO		"data/TEXTURE/Title.png"				// 読み込むテクスチャファイル名
#define	TEXTURE_LOGO_START		"data/TEXTURE/press_enter.png"				// 読み込むテクスチャファイル名

#define	TITLE_LOGO_POS_Y_START	(-400)										// タイトルロゴのスタート位置(Y座標)

#define	TITLE_LOGO_WIDTH		(960)										// タイトルの幅
#define	TITLE_LOGO_HEIGHT		(450)										// タイトルの高さ
#define	TITLE_LOGO_POS_X		(SCREEN_WIDTH / 2 - TITLE_LOGO_WIDTH / 2)	// タイトルの位置(X座標)
#define	TITLE_LOGO_POS_Y		(120)										// タイトルの位置(Y座標)

#define	START_WIDTH				(320)										// スタートボタンの幅
#define	START_HEIGHT			(80)										// スタートボタンの高さ
#define	START_POS_X				(SCREEN_WIDTH / 2 - START_WIDTH / 2)		// スタートボタンの位置(X座標)
#define	START_POS_Y				(540)										// スタートボタンの位置(Y座標)

#define	COUNT_APPERA_START		(60)										// スタートボタン出現までの時間
#define	INTERVAL_DISP_START		(60)										// スタートボタン点滅の時間

#define	COUNT_WAIT_DEMO			(60 * 5)									// デモまでの待ち時間

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexTitle(LPDIRECT3DDEVICE9 pDevice);
void SetVertexTitleLogo(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureTitle = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;		// 頂点バッファインターフェースへのポインタ
LPDIRECT3DTEXTURE9		g_pTextureTitleLogo = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitleLogo = NULL;	// 頂点バッファインターフェースへのポインタ
LPDIRECT3DTEXTURE9		g_pTextureStart = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffStart = NULL;		// 頂点バッファインターフェースへのポインタ
int g_nCountAppearStart = 0;						// "PRESS ENTER"が出るまでのカウンタ
float g_fPosYLogo = 0.0f;							// タイトルロゴの位置(Y座標)
int g_nCountDispStart = 0;							// "PRESS ENTER"の表示カウンタ
int g_nIntervalDisp = 0;							// "PRESS ENTER"の表示インターバル
bool g_bDispStart = false;							// "PRESS ENTER"の表示ON/OFF
bool g_bDecideStart = false;						// 決定したかどうか
int g_nConutDemo = 0;								// デモ画面に移行するまでのカウンタ

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_nCountAppearStart = 0;
	g_fPosYLogo = TITLE_LOGO_POS_Y_START;
	g_nCountDispStart = 0;
	g_nIntervalDisp = INTERVAL_DISP_START;
	g_bDispStart = false;
	g_bDecideStart = false;
	g_nConutDemo = 0;

	// 頂点情報の作成
	MakeVertexTitle(pDevice);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
								TEXTURE_TITLE,			// ファイルの名前
								&g_pTextureTitle);		// 読み込むメモリー

	D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
								TEXTURE_TITLE_LOGO,		// ファイルの名前
								&g_pTextureTitleLogo);	// 読み込むメモリー

	D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
								TEXTURE_LOGO_START,		// ファイルの名前
								&g_pTextureStart);		// 読み込むメモリー

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTitle(void)
{
	if(g_pTextureTitle != NULL)
	{// テクスチャの開放
		g_pTextureTitle->Release();
		g_pTextureTitle = NULL;
	}

	if(g_pVtxBuffTitle != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}

	if(g_pTextureTitleLogo != NULL)
	{// テクスチャの開放
		g_pTextureTitleLogo->Release();
		g_pTextureTitleLogo = NULL;
	}

	if(g_pVtxBuffTitleLogo != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffTitleLogo->Release();
		g_pVtxBuffTitleLogo = NULL;
	}

	if(g_pTextureStart != NULL)
	{// テクスチャの開放
		g_pTextureStart->Release();
		g_pTextureStart = NULL;
	}

	if(g_pVtxBuffStart != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffStart->Release();
		g_pVtxBuffStart = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTitle(void)
{
	if(g_fPosYLogo < TITLE_LOGO_POS_Y)
	{// タイトルロゴ下降中
		g_fPosYLogo += 2.0f;
		SetVertexTitleLogo();
	}
	else
	{// タイトルロゴ固定済
		g_nCountAppearStart++;
		if(g_nCountAppearStart > COUNT_APPERA_START)
		{// 点滅開始
			if(g_bDecideStart == false)
			{// 決定前
				g_nCountDispStart = (g_nCountDispStart + 1) % (int)(g_nIntervalDisp * 1.4f);
				if(g_nCountDispStart > g_nIntervalDisp)
				{
					g_bDispStart = false;
				}
				else
				{
					g_bDispStart = true;
				}
			}
			else
			{// 決定後
				g_nCountDispStart = (g_nCountDispStart + 1) % 4;
				if(g_nCountDispStart > 1)
				{
					g_bDispStart = false;
				}
				else
				{
					g_bDispStart = true;
				}
			}
		}
	}

	if(GetKeyboardTrigger(DIK_RETURN))
	{// Enter押したら、フェードアウトしてモードを切り替えいく
		if(g_nCountAppearStart == 0)
		{// タイトルロゴ登場スキップ
			g_fPosYLogo = TITLE_LOGO_POS_Y;
			SetVertexTitleLogo();

			g_nCountAppearStart = COUNT_APPERA_START;
		}
		else
		{// ゲーム画面に移行
			if(g_bDecideStart == false)
			{
				g_bDecideStart = true;

				// 決定音再生
				PlaySound(SOUND_LABEL_SE_DECIDE);

				//SetFade(FADE_OUT, MODE_GAME);
				//SetFade(FADE_OUT, MODE_MAP);
				SetLoadStart(MODE_MAP);
			}
		}
	}

	//if(g_nCountAppearStart >= COUNT_APPERA_START)
	//{
	//	g_nConutDemo++;
	//	if(g_nConutDemo > COUNT_WAIT_DEMO)
	//	{// ランキング画面に移行
	//		//SetFade(FADE_OUT, MODE_GAME);
	//		//SetFade(FADE_OUT, MODE_MAP);
	//		SetLoadStart(MODE_MAP);
	//	}
	//}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
    pDevice->SetStreamSource(0, g_pVtxBuffTitle, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureTitle);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);


	// 頂点バッファをデバイスのデータストリームにバインド
    pDevice->SetStreamSource(0, g_pVtxBuffTitleLogo, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureTitleLogo);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

	if(g_bDispStart == true)
	{
		// 頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, g_pVtxBuffStart, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureStart);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexTitle(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
												FVF_VERTEX_2D,				// 使用する頂点フォーマット
												D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
												&g_pVtxBuffTitle,			// 頂点バッファインターフェースへのポインタ
												NULL)))						// NULLに設定
	{
        return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

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
		g_pVtxBuffTitle->Unlock();
	}

	// オブジェクトの頂点バッファを生成
    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
												FVF_VERTEX_2D,				// 使用する頂点フォーマット
												D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
												&g_pVtxBuffTitleLogo,		// 頂点バッファインターフェースへのポインタ
												NULL)))						// NULLに設定
	{
        return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffTitleLogo->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(TITLE_LOGO_POS_X, g_fPosYLogo, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(TITLE_LOGO_POS_X + TITLE_LOGO_WIDTH, g_fPosYLogo, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(TITLE_LOGO_POS_X, g_fPosYLogo + TITLE_LOGO_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(TITLE_LOGO_POS_X + TITLE_LOGO_WIDTH, g_fPosYLogo + TITLE_LOGO_HEIGHT, 0.0f);

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
		g_pVtxBuffTitleLogo->Unlock();
	}

	// オブジェクトの頂点バッファを生成
    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
												FVF_VERTEX_2D,				// 使用する頂点フォーマット
												D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
												&g_pVtxBuffStart,			// 頂点バッファインターフェースへのポインタ
												NULL)))						// NULLに設定
	{
        return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffStart->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(START_POS_X, START_POS_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(START_POS_X + START_WIDTH, START_POS_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(START_POS_X, START_POS_Y + START_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(START_POS_X + START_WIDTH, START_POS_Y + START_HEIGHT, 0.0f);

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
		g_pVtxBuffStart->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 頂点の作成
//=============================================================================
void SetVertexTitleLogo(void)
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffTitleLogo->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(TITLE_LOGO_POS_X, g_fPosYLogo, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(TITLE_LOGO_POS_X + TITLE_LOGO_WIDTH, g_fPosYLogo, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(TITLE_LOGO_POS_X, g_fPosYLogo + TITLE_LOGO_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(TITLE_LOGO_POS_X + TITLE_LOGO_WIDTH, g_fPosYLogo + TITLE_LOGO_HEIGHT, 0.0f);

		// 頂点データをアンロックする
		g_pVtxBuffTitleLogo->Unlock();
	}

}

