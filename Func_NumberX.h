//=============================================================================
//
// 数列表示処理
// 
//
//=============================================================================
#ifndef _NUMBERX_
#define _NUMBERX_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_DIGIT				(8)		// 表示できる最大桁数
#define MAX_NUMBER_X	(32)			// 最大数列表示列数
#define MAX_NUMBER_COUNT	(MAX_NUMBER_X * MAX_DIGIT)	//最大数字総数
#define VALUE_FADE_OUT	(0.001f)		// fade out の変化絶対値 0.0~1.0f
#define VALUE_SIZE_CHANGE_X (1.013f)	// サイズの変化倍率X
#define VALUE_SIZE_CHANGE_Y (1.02f)		// サイズの変化倍率Y
//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{
	PLAYER_SCORE = 0,
	NUMBERX_PLAYER_HP,
	MAP_PLAYER_COORDNATION,
	TIME_LEFT,
	TURN_ENEMY_0,
	TURN_ENEMY_1,
	TURN_ENEMY_2,
	DISTENCE,
	MOUSE_X,
	MOUSE_Y,

}NAME_NUMBER;
typedef enum
{
	BASICALWB = 0,
	DIGITAL,
	FULLCOLOR,
	MAX_TEX,

}NUM_TEX;
typedef struct
{
	int		NumBox;					// 一の数字
	float		nSize_X;				// 大きさＸ
	float		nSize_Y;				// 大きさＹ
	bool	bUse;					// 使用されているかどうか

	D3DXVECTOR3	pos;				// 文字の中心座標

}NUMBER_DATA;

typedef struct
{
	int nDigit;						// 桁数
	bool bUse;						// 使用されているかどうか
	float		nSize_X;				// 大きさＸ
	float		nSize_Y;				// 大きさＹ
	int		nNumber;
	bool	bFade_Out;				// 透明化されるか
	float	fAphla;					// Aphla
	int		TexName;				// 使用するテクスチャの名前
	D3DXVECTOR3	pos;				// 描画開始位置
	int nLife;

} NUMBER_X;
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

HRESULT InitNumberX(void);
void UninitNumberX(void);
void UpdateNumberX(void);
void DrawNumberX(void);

void Set_NumberX(
				int		nName,			// 数列の名前
				bool	bUse,			// 使用の状態設定
				bool	bFade,			// Fadeの制御
				int		TexName,		// 使用するテクスチャの名前
				int		nMax_Digit,		// 入力最大桁数
				int		nInput_Data,	// 入力値
				float	fSize_X,		// 文字組の大きさX
				float	fSize_Y,		// 文字組の大きさY
				float	fAlpha,			// 透明度の設定
				D3DXVECTOR3 pos);		// 描画開始位置
void Diffuse_Number(int NumberXID,bool Fade);		// 文字列表示の解除
void Set_NumberX_Fade_Out(int NumberX_ID);
NUMBER_X *GetNumberX(void);
#endif
