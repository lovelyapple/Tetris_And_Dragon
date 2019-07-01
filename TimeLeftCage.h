//=============================================================================
// テトリスのメイン処理[TimeLeftCage.h]
//=============================================================================
#ifndef _GTIME_LEFT_H_
#define _GTIME_LEFT_H_


#include "main.h"
#include "game.h"
#include "Tetris_main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define VALUE_DEFAULT_MOVE_TIME		(60.0f * 15)			// 初期移動可能時間
#define VALUE_RECOVER_TIME			(VALUE_TIME_DELAY)		// 回復時間

#define TIMELEFT_POS_X				(VALUE_FIELD_END_X)//描画位置左上が中心点とするX
#define TIMELEFT_POS_Y				(VALUE_FIELD_END_Y)	 //描画位置左上が中心点とするY
#define TIMELEFT_POS_ENTITY_X		(TIMELEFT_POS_X)
#define TIMELEFT_POS_ENTITY_Y		(TIMELEFT_POS_Y + 2.0f)
#define TIMELEFT_SIZE_X				(10.0f)					//描画する幅
#define TIMELEFT_SIZE_Y				(VALUE_FIELD_SIZE_Y)
#define TIMELEFT_SIZE_ENTITY_X		(TIMELEFT_SIZE_X)
#define TIMELEFT_SIZE_ENTITY_Y		(TIMELEFT_SIZE_Y * 0.99)

#define VALUE_HEIHT_PER_TIME		(TIMELEFT_SIZE_ENTITY_Y / VALUE_DEFAULT_MOVE_TIME)	//一時間単位の長さ

//#define TIMELEFT_SIZE_X	(TIMELEFT_SIZE_X * 1.2f)//移動時間ケージの幅
//#define VALUE_TIMELEFTCAGE_HEIGHT	(VALUE_FIELD_SIZE_Y+2.0f)			//移動時間ケージの長さ
#define TEXTURE_MOVETIME		"data/TEXTURE/TimeCage.png"
#define TEXTURE_MOVETIME_CAGE	"data/TEXTURE/TimeCageOutLine.png"
//*****************************************************************************
// 構造体宣言
//*****************************************************************************
typedef struct
{
	int nType;				//時間の状態
	float fMAX_Move_Time;	//最大時間
	float fMoveTimeLeft;	//現在の残り時間
	float VecTime;			//単位時間変化量
}MOVETIME;

typedef enum
{
	TIME_MOVE_KEEPING = 0,	//現在の時間を維持
	TIME_MOVE_FULL,			//時間満タンの時
	TIME_MOVE_RECOVER,		//時間回復状態
	TIME_MOVE_LOSING,		//時間減少状態
	MAX_TIME_MOVE_TYPE		//ワーク
}MOVE_TIME_TYPE;
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
MOVETIME *GetMoveTimeLeft(void);
void SetMoveTime(int Type,float MaxTimeLeft);
HRESULT InitTimeLeft(void);
void UninitTimeLeft(void);
void UpdateTimeLeft(void);
void DrawTimeLeft(void);
int GetTimeLeft(void);
#endif
