//=============================================================================
//
// ステート処理 [main.h]
// HAL名古屋
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

#define USE_SOUND		// サウンド使用の有無
//#undef USE_SOUND

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "windows.h"
#include "d3dx9.h"

#define DIRECTINPUT_VERSION (0x0800)
#include "dinput.h"
#ifdef USE_SOUND
#include "xaudio2.h"
#endif

//*****************************************************************************
// ライブラリファイル
//*****************************************************************************
#if 1	// [ここを"0"にした場合、"構成プロパティ" -> "リンカ" -> "入力" -> "追加の依存ファイル"に対象ライブラリを設定する]
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "winmm.lib")
#endif

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SCREEN_WIDTH	(1280)				// ウインドウの幅
#define SCREEN_HEIGHT	(720)				// ウインドウの高さ
#define SCREEN_CENTER_X	(SCREEN_WIDTH / 2)	// ウインドウの中心Ｘ座標
#define SCREEN_CENTER_Y	(SCREEN_HEIGHT / 2)	// ウインドウの中心Ｙ座標

#define	NUM_VERTEX		(4)					// 頂点数
#define	NUM_POLYGON		(2)					// ポリゴン数

// 頂点フォーマット( 頂点座標[2D] / 頂点カラー / テクスチャ座標 )
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

//*************************************
// 上記頂点フォーマットに合わせた構造体を定義
//*************************************
typedef struct
{
	D3DXVECTOR3 pos;	// 頂点座標
	float rhw;			// テクスチャのパースペクティブコレクト用
	D3DCOLOR col;		// 頂点カラー
	D3DXVECTOR2 tex;	// テクスチャ座標
} VERTEX_2D;

//*************************************
// モードの種類
//*************************************
typedef enum
{
	MODE_TITLE = 0,		// タイトル画面
	MODE_MAP,			// フィールド画面
	MODE_GAME,			// ゲーム画面
	MODE_RESULT,		// リザルト画面
	MODE_RANKING,		// ランキング画面
	MODE_MAX
} MODE;
typedef enum
{
	UPDATE_MODE_SHADOW = 0,
	UPDATE_MODE_REAL,
	UPDATE_MODE_WAIT,
	UPDATE_MODE_STOP,
	UPDATE_MODE_MAX,
}UPDATE_MODE_TYPE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LPDIRECT3DDEVICE9 GetDevice(void);
void SetMode(MODE mode);
void StopMode(MODE mode);
MODE GetMode(void);
void Set_Main_Update_Type(int Type);
LPD3DXFONT	GetGetFont(void);
#endif
