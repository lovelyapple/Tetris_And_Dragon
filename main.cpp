//=============================================================================
//
// ステート処理 [main.cpp]
// HAL名古屋
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "sound.h"
#include "Data_Floor.h"
#include "Data_Stage.h"
//#include "fade.h"
#include "Func_NumberX.h"
#include "game_map.h"
#include "Fade_loading.h"
#include "MessageBox.h"
#include "Sys_Record.h"
#include <time.h>
#include <stdlib.h>

//*****************************************************************************
// マクロ定義
//****************************************************F*************************
#define CLASS_NAME		"AppClass"		// ウインドウのクラス名
#define WINDOW_NAME		"ランキング処理"	// ウインドウのキャプション名


//*****************************************************************************
// 構造体定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);
#ifdef _DEBUG
void DrawFPS(void);
#endif

//*****************************************************************************
// グローバル変数:F
//*****************************************************************************
LPDIRECT3D9			g_pD3D = NULL;			// Direct3D オブジェクト
LPDIRECT3DDEVICE9	g_pD3DDevice = NULL;	// Deviceオブジェクト(描画に必要)
#ifdef _DEBUG
LPD3DXFONT			g_pD3DXFont = NULL;		// フォントへのポインタ


int					g_nCountFPS = 0;		// FPSカウンタ
#endif
MODE				g_mode = MODE_TITLE;	// モード

int					g_UpdateType = UPDATE_MODE_REAL;
int					g_UpdateBuffTime = 2;
//=============================================================================
// メイン関数
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW+1),
		NULL,
		CLASS_NAME,
		NULL
	};
	HWND hWnd;
	MSG msg;
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;
	DWORD dwRecordLastTime;
	
	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// ウィンドウの作成
	hWnd = CreateWindow(CLASS_NAME,
						WINDOW_NAME,
						WS_OVERLAPPEDWINDOW,
						CW_USEDEFAULT,
						CW_USEDEFAULT,
						SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2,
						SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),
						NULL,
						NULL,
						hInstance,
						NULL);

	// 初期化処理(ウィンドウを作成してから行う)
	if(FAILED(Init(hInstance, hWnd, true)))
	{
		return -1;
	}

	// フレームカウント初期化
	timeBeginPeriod(1);					// 分解能を設定
	dwExecLastTime = 
	dwFPSLastTime = timeGetTime();		// システム時刻をミリ秒単位で取得
	dwCurrentTime =
	dwFrameCount = 0;
	dwRecordLastTime = 0;	

	// ウインドウの表示(初期化処理の後に呼ばないと駄目)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	
	// メッセージループ
	while(1)
	{
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{// PostQuitMessage()が呼ばれたらループ終了
				break;
			}
			else
			{
				// メッセージの翻訳とディスパッチ
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
        }
		else
		{
			dwCurrentTime = timeGetTime();		// システム時刻を取得

			if((dwCurrentTime - dwFPSLastTime) >= 500)	// 0.5秒ごとに実行
			{
#ifdef _DEBUG
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);		// FPSを測定
#endif
				dwFPSLastTime = dwCurrentTime;		// FPSを測定した時刻を保存
				dwFrameCount = 0;					// カウントをクリア
			}

			if((dwCurrentTime - dwExecLastTime) >= (1000 / 60))		// 1/60秒ごとに実行
			{
				dwExecLastTime = dwCurrentTime;		// 処理した時刻を保存

				// 更新処理
				Update();

				// 描画処理
				Draw();

				dwFrameCount++;			// カウントを加算
			}
			 if ((dwCurrentTime - dwRecordLastTime) >= (1000/100))
			{
				dwRecordLastTime = dwCurrentTime;
				
				UpdateRecord_System();
			}

		}
	}

	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// 終了処理
	Uninit();

	// 入力処理の終了処理
	UninitInput();

	timeEndPeriod(1);				// 分解能を戻す

	return (int)msg.wParam;
}

//=============================================================================
// プロシージャ
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:					// [ESC]キーが押された
			DestroyWindow(hWnd);		// ウィンドウを破棄するよう指示する
			break;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
    D3DDISPLAYMODE d3ddm;

	// Direct3Dオブジェクトの作成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(g_pD3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
    if(FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));							// ワークをゼロクリア
	d3dpp.BackBufferWidth			= SCREEN_WIDTH;				// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight			= SCREEN_HEIGHT;			// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat			= d3ddm.Format;				// カラーモードの指定
	d3dpp.BackBufferCount			= 1;						// バックバッファの数
	d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;	// 映像信号に同期してフリップする
	d3dpp.Windowed					= bWindow;					// ウィンドウモード
	d3dpp.EnableAutoDepthStencil	= TRUE;						// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat	= D3DFMT_D16;				// デプスバッファとして16bitを使う

	if(bWindow)
	{// ウィンドウモード
		d3dpp.FullScreen_RefreshRateInHz = 0;								// リフレッシュレート
		d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;	// インターバル
	}
	else
	{// フルスクリーンモード
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			// リフレッシュレート
		d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_DEFAULT;		// インターバル
	}

	// デバイスの生成
	// ディスプレイアダプタを表すためのデバイスを作成
	// 描画と頂点処理をハードウェアで行なう
	if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
									D3DDEVTYPE_HAL, 
									hWnd, 
									D3DCREATE_HARDWARE_VERTEXPROCESSING, 
									&d3dpp, &g_pD3DDevice)))
	{
		// 上記の設定が失敗したら
		// 描画をハードウェアで行い、頂点処理はCPUで行なう
		if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
										D3DDEVTYPE_HAL, 
										hWnd, 
										D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
										&d3dpp, &g_pD3DDevice)))
		{
			// 上記の設定が失敗したら
			// 描画と頂点処理をCPUで行なう
			if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
											D3DDEVTYPE_REF,
											hWnd, 
											D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
											&d3dpp, &g_pD3DDevice)))
			{
				// 初期化失敗
				return E_FAIL;
			}
		}
	}

	// レンダリングステートパラメータの設定
    g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// カリングを行わない
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Zバッファを使用
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定

	// サンプラーステートパラメータの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャＵ値の繰り返し設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャＶ値の繰り返し設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大時の補間設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小時の補間設定

	// テクスチャステージステートパラメータの設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// アルファブレンディング処理
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// 最初のアルファ引数
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// ２番目のアルファ引数

#ifdef _DEBUG
	// 情報表示用フォントを設定
	D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
					OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "HGP創英角ﾎﾟｯﾌﾟ体", &g_pD3DXFont);
#endif
	srand((unsigned int) time (NULL));
	// 任意数字表示よりの初期化処理
	InitNumberX();

	// サウンド処理の初期化
	InitSound(hWnd);

	// 入力処理の初期化
	InitInput(hInstance, hWnd);

	// タイトル画面の初期化処理
	InitTitle();

	// フィールドの初期化処理 
	InitGame_Map();

	// ゲーム画面の初期化処理

	InitGame();

	// リザルト画面の初期化処理
	InitResult();

	// フェード処理の初期化処理
	//InitFade();
	InitFade_loading();


	// 最初はタイトル画面に
	SetMode(MODE_TITLE);
	//データを初期化
	InitData_Floor();
	InitData_Stage();
	InitRecord_System();
	InitMessageBox();

	//HRESULT res = 
	return S_OK;
	
}

//=============================================================================
// 終了処理
//=============================================================================
void Uninit(void)
{
#ifdef _DEBUG
	if(g_pD3DXFont != NULL)
	{// デバッグ情報表示用フォントを設定
		g_pD3DXFont->Release();
		g_pD3DXFont = NULL;
	}
#endif

	if(g_pD3DDevice != NULL)
	{// デバイスの開放
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	if(g_pD3D != NULL)
	{// Direct3Dオブジェクトの開放
		g_pD3D->Release();
		g_pD3D = NULL;
	}

	// タイトル画面の終了処理
	UninitTitle();

	// フィールドの終了処理
	UninitGame_Map();
	// ゲーム画面の終了処理
	UninitGame();

	// リザルト画面の終了処理
	UninitResult();


	// 入力処理の終了処理
	UninitInput();

	// 入力処理の終了処理
	UninitSound();

	// フェード処理の終了処理
	//UninitFade();
	UninitFade_loading();
	// 任意数字表示よりの終了
	UninitNumberX();
	UninitMessageBox();
	UninitRecord_System();
	
}

//=============================================================================
// 更新処理
//=============================================================================
void Update(void)
{
	// 入力更新
	UpdateInput();

	switch(g_mode)
	{
	case MODE_TITLE:		// タイトル画面の更新	
		switch(g_UpdateType)
		{
		case UPDATE_MODE_SHADOW:
			UpdateTitle();
			DrawTitle();
			g_UpdateType = UPDATE_MODE_WAIT;
			break;
		case UPDATE_MODE_WAIT:
			SetLoadEnd();
			break;
		case UPDATE_MODE_REAL:
			UpdateTitle();
			break;
		case UPDATE_MODE_STOP:
			break;
		}	
		break;
	case MODE_MAP:		// タイトル画面の更新
		switch(g_UpdateType)
		{
		case UPDATE_MODE_SHADOW:
			UpdateGame_Map();
			DrawGame_Map();
			g_UpdateType = UPDATE_MODE_WAIT;
			break;
		case UPDATE_MODE_WAIT:
			SetLoadEnd();
			break;
		case UPDATE_MODE_REAL:
			UpdateGame_Map();
			break;
		case UPDATE_MODE_STOP:
			break;
		}	
		break;
	case MODE_GAME:			// ゲーム画面の更新
		switch(g_UpdateType)
		{
		case UPDATE_MODE_SHADOW:
			UpdateGame();
			DrawGame();
			g_UpdateType = UPDATE_MODE_WAIT;
			break;
		case UPDATE_MODE_WAIT:
			g_UpdateBuffTime--;
			if(g_UpdateBuffTime >0)
				UpdateGame();
			else
			{
			SetLoadEnd();
			g_UpdateBuffTime = 3;
			}
			break;
		case UPDATE_MODE_REAL:
			UpdateGame();
			break;
		case UPDATE_MODE_STOP:
			break;
		}	
		break;

	case MODE_RESULT:		// リザルト画面の更新
		
		switch(g_UpdateType)
		{
		case UPDATE_MODE_SHADOW:
			UpdateResult();
			DrawResult();
			g_UpdateType = UPDATE_MODE_WAIT;
			break;
		case UPDATE_MODE_WAIT:
			SetLoadEnd();
			break;
		case UPDATE_MODE_REAL:
			UpdateResult();
			break;
		case UPDATE_MODE_STOP:
			break;
		}	
		break;

	}

	// フェード更新
	//UpdateFade();
	UpdateFade_loading();
	// 任意数字表示よりの更新処理
	UpdateNumberX();

	UpdateMessageBox();

}
void Set_Main_Update_Type(int Type)
{
	g_UpdateType = Type;
}

//=============================================================================
// 描画処理
//=============================================================================
void Draw(void)
{
	// バックバッファ＆Ｚバッファのクリア
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// Direct3Dによる描画の開始
	if(SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		switch(g_mode)
		{
		case MODE_TITLE:		// タイトル画面の描画
			DrawTitle();
		// 任意数字表示よりの描画処理
			break;
		case MODE_MAP:		// マップ画面の描画
			DrawGame_Map();
			break;

		case MODE_GAME:			// ゲーム画面の描画
			DrawGame();
		// 任意数字表示よりの描画処理

			break;

		case MODE_RESULT:		// リザルト画面の描画
			DrawResult();
		// 任意数字表示よりの描画処理

			break;

		}

		// フェード描画
		//DrawFade();
		DrawFade_loading();
		// 任意数字表示よりの描画処理
		//DrawNumberX();
		DrawMessageBox();

		DrawRecord_System();



//#ifdef _DEBUG
		// デバッグ表示
		DrawFPS();
//#endif

		// Direct3Dによる描画の終了
		g_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=============================================================================
// デバイスの取得
//=============================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//=============================================================================
// モードの設定
//=============================================================================
void SetMode(MODE mode)
{
	// 終了処理
	StopSound();
	switch(g_mode)
	{
	case MODE_TITLE:
		UninitResult();
		UninitTitle();
		break;
	case MODE_MAP:
		UninitTitle();
		UninitGame_Map();
		break;
	case MODE_GAME:
		UninitGame();
		UninitGame_Map();
		break;
	case MODE_RESULT:
		UninitResult();	
		break;

	}

	// 初期化処理
	switch(mode)
	{
	case MODE_TITLE:
		InitTitle();
		PlaySound(SOUND_LABEL_OPENING_BGM);
		break;
	case MODE_MAP:
		InitGame_Map();
		PlaySound(SOUND_LABEL_OPENING_BGM);
		break;
	case MODE_GAME:
		InitGame();
		PlaySound(SOUND_LABEL_STAGE_BGM);
		break;

	case MODE_RESULT:
		InitResult();
		PlaySound(SOUND_LABEL_BGM002);
		break;

	}

	g_mode = mode;
}

//=============================================================================
// モードの取得
//=============================================================================
MODE GetMode(void)
{
	return g_mode;
}

#ifdef _DEBUG
//=============================================================================
// FPS表示処理
//=============================================================================
void DrawFPS(void)
{
	RECT rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	char aStr[256];

	wsprintf(&aStr[0], "定数FPS:%d\n", g_nCountFPS);

	// テキスト描画
	g_pD3DXFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(0xff, 0xff, 0xff, 0xff));
}
#endif
LPD3DXFONT	GetGetFont(void)
{
	return g_pD3DXFont;
}
