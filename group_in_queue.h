//=============================================================================
// テトリスのメイン処理[group_in_queue.h]
//=============================================================================
#ifndef _GROUP_Q_H_
#define _GROUP_Q_H_


#include "main.h"
#include "game.h"
#include "Tetris_main.h"
#include "TimeLeftCage.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_GROUP_Q			(5)				// 最大Ｑグループ組数
#define MAX_BLOCK_Q			(64)			// 最大Ｑブロック数
#define VALUE_BLOCK_Q_SIZE	(VALUE_BLOCK_SIZE * 0.8f)			// Ｑブロックのサイズ
#define VALUE_GROUP_Q_SIZE	(VALUE_BLOCK_Q_SIZE * 4)		// Ｑグループの大きさ
#define VALUE_DEST_Q_TIME	(30.0f)
#define VALUE_Q_GROUP_POS_X	(TIMELEFT_POS_X+TIMELEFT_SIZE_X+VALUE_DEST_Q_TIME)// Ｑグループの描画開始位置X
#define VALUE_Q_GROUP_POS_Y	(VALUE_FIELD_END_Y + VALUE_BLOCK_Q_SIZE )		// Ｑグループの描画開始位置Y


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitBlock_Q_ALL(void);//すべてのＱブロックを初期化
void InitGroup_Q_ALL(void);//すべてのＱグループを初期化
void SetGroup_Q(int Q_ID);//入力ＩＤのグループを初期化
void SetGroup_Q_Type(int Q_ID,int nType);//グループの使用状態を変える
int GetGroup_Q_NumForm(int Q_ID);
int GetGroup_Q_Color(int Q_ID);
int	GetGroup_Q_D_Color(int D_ID);
int GetGroup_Q_Start(int Q_ID);
int	GetGroup_Q_D_nType(int D_ID);
void SetRandColor(void);

HRESULT InitGroup_Q(void);
void UninitGroup_Q(void);
void UpdateGroup_Q(void);
void DrawGroup_Q(void);
void SetGroupQHandleALL(int Form,int color);
#endif
