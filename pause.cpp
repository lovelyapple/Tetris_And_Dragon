//=============================================================================
//
// ゲーム画面処理 [game.cpp]
// HAL名古屋
//
//=============================================================================
#include "pause.h"
#include "input.h"
#include "game.h"
//#include "fade.h"
#include "Fade_loading.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	NUM_PAUSE_MENU		(3)											// ポーズメニュー数
#define	PLATE_WIDTH			(360.0f)									// プレートの幅
#define	PLATE_HEIGHT		(340.0f)									// プレートの幅
#define	PLATE_POS_X			(SCREEN_WIDTH / 2 - PLATE_WIDTH / 2)		// プレートの位置(X座標)
#define	PLATE_POS_Y			(180.0f)									// プレートの位置(Y座標)
#define	PAUSE_MENU_WIDTH	(320.0f)									// ポーズメニュー幅
#define	PAUSE_MENU_HEIGHT	(60.0f)										// ポーズメニュー高さ
#define	PAUSE_MENU_POS_X	(SCREEN_WIDTH / 2 - PAUSE_MENU_WIDTH / 2)	// ポーズメニュー位置(X座標)
#define	PAUSE_MENU_POS_Y	(PLATE_POS_Y + 40.0f)						// ポーズメニュー位置(Y座標)
#define	PAUSE_MENU_INTERVAL	(100.0f)									// ポーズメニュー間隔

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexPause(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_apTexturePause[3] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;		// 頂点バッファインターフェースへのポインタ

PAUSE_MENU g_nSelectMenu = PAUSE_MENU_CONTINUE;		// 選択中のメニューNo.
float g_fValueBlink = 0.0f;							// 点滅係数
bool g_bDispMenu = true;							// メニュー表示ON/OFF

const char *c_aFileNamePauseMenu[NUM_PAUSE_MENU] =
{
	"data/TEXTURE/pause000.png",	// コンティニュー
	"data/TEXTURE/pause001.png",	// リトライ
	"data/TEXTURE/pause002.png"		// クイット
};

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPause(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for(int nCntPauseMenu = 0; nCntPauseMenu < NUM_PAUSE_MENU; nCntPauseMenu++)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,									// デバイスへのポインタ
									c_aFileNamePauseMenu[nCntPauseMenu],	// ファイルの名前
									&g_apTexturePause[nCntPauseMenu]);		// 読み込むメモリー
	}

	// 頂点情報の作成
	MakeVertexPause(pDevice);

	g_nSelectMenu = PAUSE_MENU_CONTINUE;
	g_fValueBlink = 0.0f;
	g_bDispMenu = true;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPause(void)
{
	for(int nCntPauseMenu = 0; nCntPauseMenu < NUM_PAUSE_MENU; nCntPauseMenu++)
	{
		if(g_apTexturePause[nCntPauseMenu] != NULL)
		{// テクスチャの開放
			g_apTexturePause[nCntPauseMenu]->Release();
			g_apTexturePause[nCntPauseMenu] = NULL;
		}
	}

	if(g_pVtxBuffPause != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePause(void)
{

		// メニュー選択
		if(GetKeyboardRepeat(DIK_W))
		{
			g_nSelectMenu = (PAUSE_MENU)((g_nSelectMenu + NUM_PAUSE_MENU - 1) % NUM_PAUSE_MENU);
			SetPauseMenu();
		}
		else if(GetKeyboardRepeat(DIK_S))
		{
			g_nSelectMenu = (PAUSE_MENU)((g_nSelectMenu + 1) % NUM_PAUSE_MENU);
			SetPauseMenu();
		}

		// メニュー決定
		if(GetKeyboardTrigger(DIK_RETURN))
		{
			switch(g_nSelectMenu)
			{
			case PAUSE_MENU_CONTINUE:
				SetEnablePause(false);
				break;

			case PAUSE_MENU_RETRY:
				//SetFade(FADE_OUT, MODE_GAME);
				SetLoadStart(MODE_GAME);
				break;

			case PAUSE_MENU_QUIT:
				//SetFade(FADE_OUT, MODE_TITLE);
				SetLoadStart(MODE_TITLE);
				break;
			}

			SetKeyboardRepeatInterval(1);
		}


	// 選択メニュー点滅
	g_fValueBlink += D3DX_PI * 0.01f;
	if(g_fValueBlink > D3DX_PI)
	{
		g_fValueBlink -= D3DX_PI * 2.0f;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += ((g_nSelectMenu + 2) * 4);

		// 頂点カラーの設定
		float fCol = cosf(g_fValueBlink) * 0.2f + 0.8f;
		pVtx[0].col = D3DXCOLOR(fCol, fCol, fCol, 1.0f);
		pVtx[1].col = D3DXCOLOR(fCol, fCol, fCol, 1.0f);
		pVtx[2].col = D3DXCOLOR(fCol, fCol, fCol, 1.0f);
		pVtx[3].col = D3DXCOLOR(fCol, fCol, fCol, 1.0f);

		// 頂点データをアンロックする
		g_pVtxBuffPause->Unlock();
	}

	if(GetKeyboardTrigger(DIK_F4))
	{// メニュー表示ON/OFF
		g_bDispMenu = g_bDispMenu ? false: true;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPause(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if(g_bDispMenu == true)
	{// メニュー表示ON
		// 頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, NULL);

		// ポリゴンの描画(背景)
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

		// ポリゴンの描画(プレート)
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, NUM_POLYGON);

		for(int nCntPauseMenu = 0; nCntPauseMenu < NUM_PAUSE_MENU; nCntPauseMenu++)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_apTexturePause[nCntPauseMenu]);

			// ポリゴンの描画(メニュー)
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 8 + nCntPauseMenu * 4, NUM_POLYGON);
		}
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexPause(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * 5,	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,				// 頂点バッファの使用法　
												FVF_VERTEX_2D,					// 使用する頂点フォーマット
												D3DPOOL_MANAGED,				// リソースのバッファを保持するメモリクラスを指定
												&g_pVtxBuffPause,				// 頂点バッファインターフェースへのポインタ
												NULL)))							// NULLに設定
	{
        return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

		// 背景
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
		pVtx[0].col = D3DXCOLOR(0.1f, 0.1f, 0.1f, 0.75f);
		pVtx[1].col = D3DXCOLOR(0.1f, 0.1f, 0.1f, 0.75f);
		pVtx[2].col = D3DXCOLOR(0.1f, 0.1f, 0.1f, 0.75f);
		pVtx[3].col = D3DXCOLOR(0.1f, 0.1f, 0.1f, 0.75f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);	

		pVtx += 4;

		// プレート
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(PLATE_POS_X, PLATE_POS_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(PLATE_POS_X + PLATE_WIDTH, PLATE_POS_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(PLATE_POS_X, PLATE_POS_Y + PLATE_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(PLATE_POS_X + PLATE_WIDTH, PLATE_POS_Y + PLATE_HEIGHT, 0.0f);

		// テクスチャのパースペクティブコレクト用
		pVtx[0].rhw =
		pVtx[1].rhw =
		pVtx[2].rhw =
		pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 0.5f, 0.1f, 0.50f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.5f, 0.1f, 0.50f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.5f, 0.1f, 0.50f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.5f, 0.1f, 0.50f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);	

		pVtx += 4;

		// メニュー
		for(int nCntPauseMenu = 0; nCntPauseMenu < NUM_PAUSE_MENU; nCntPauseMenu++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(PAUSE_MENU_POS_X, PAUSE_MENU_POS_Y + nCntPauseMenu * PAUSE_MENU_INTERVAL, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(PAUSE_MENU_POS_X + PAUSE_MENU_WIDTH, PAUSE_MENU_POS_Y + nCntPauseMenu * PAUSE_MENU_INTERVAL, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(PAUSE_MENU_POS_X, PAUSE_MENU_POS_Y + PAUSE_MENU_HEIGHT + nCntPauseMenu * PAUSE_MENU_INTERVAL, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(PAUSE_MENU_POS_X + PAUSE_MENU_WIDTH, PAUSE_MENU_POS_Y + PAUSE_MENU_HEIGHT + nCntPauseMenu * PAUSE_MENU_INTERVAL, 0.0f);

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
		}
		
		// 頂点データをアンロックする
		g_pVtxBuffPause->Unlock();
	}

	return S_OK;
}

//=============================================================================
// ポーズメニューの設定
//=============================================================================
void SetPauseMenu(void)
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += 8;		// 背景・プレートの分を進める

		for(int nCntPauseMenu = 0; nCntPauseMenu < NUM_PAUSE_MENU; nCntPauseMenu++, pVtx += 4)
		{
			// 頂点カラーの設定
			if(nCntPauseMenu == g_nSelectMenu)
			{
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}
			else
			{
				pVtx[0].col = D3DXCOLOR(0.30f, 0.30f, 0.30f, 1.0f);
				pVtx[1].col = D3DXCOLOR(0.30f, 0.30f, 0.30f, 1.0f);
				pVtx[2].col = D3DXCOLOR(0.30f, 0.30f, 0.30f, 1.0f);
				pVtx[3].col = D3DXCOLOR(0.30f, 0.30f, 0.30f, 1.0f);
			}
		}

		// 頂点データをアンロックする
		g_pVtxBuffPause->Unlock();
	}

	g_fValueBlink = 0.0f;
}

//=============================================================================
// ポーズメニューの取得
//=============================================================================
PAUSE_MENU GetPauseMenu(void)
{
	return g_nSelectMenu;
}

//=============================================================================
// ポーズメニューのリセット
//=============================================================================
void ResetPauseMenu(void)
{
	g_nSelectMenu = PAUSE_MENU_CONTINUE;
	SetPauseMenu();
}
