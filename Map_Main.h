//=============================================================================
//
// 背景処理 [Map_Main.h]
//
//=============================================================================
#ifndef _Map_Main_H_
#define _Map_Main_H_

#include "main.h"
#define	MAP_LINE_UNIT	(10)		// 一列単位上のマップサイズ
#define MAP_MAX_UNIT	(MAP_LINE_UNIT*MAP_LINE_UNIT)
#define MAP_UNIT_SIZE	(50.0f)		// ユニットの大きさ
#define MAP_DRAW_POS_X	(50.0f)	// 描画開始位置X
#define MAP_DRAW_POS_Y	(50.0f)		// 描画開始位置Y
#define MAP_SIZE	(MAP_LINE_UNIT * MAP_UNIT_SIZE)　// マップの全体大きさ
#define MAP_MAX_SINGLE_FLOOR	(3)
#define MAP_MAX_CONTENT		(8)
typedef enum
{
	FIELD_SETUP = 0,
	FIELD_UPDATE,
	FIELD_MAX_STATE,
}FIELD_STATE;
typedef enum
{
	UNIT_NORMAL = 0,
	UNIT_NORMAL_ACTIVE,
	UNIT_FLOOR,
	UNIT_STAGE,
	UNIT_MAX_TYPE,
}UNIT_TYPE;

typedef struct
{
	int nX;
	int nY;
}COORDINATION_INT;		//整数座標型
typedef struct
{
	bool	bUse;		// 使われているか
	int		nFloorID;	// フロアーID
	int		nStageID;	// ステージID
}MAP_CONTENT;// コンテント構造体
typedef struct
{
	int					nType;		// タイプ
	COORDINATION_INT	Place;		// 座標位置
	D3DXVECTOR3			DrawPos;	// 描画位置
	MAP_CONTENT			Content;	// コンテント
	bool				bWalkble;	// 踏めるか
}FIELD_UNIT;			//フィールドのユニット

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitMap_Main(void);
void UninitMap_Main(void);
void UpdateMap_Main(void);
void DrawMap_Main(void);
FIELD_UNIT *GetFieldUnit(void);

#endif
