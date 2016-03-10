//=============================================================================
//
// 背景処理 [Map_Player.h]
//
//=============================================================================
#ifndef _Map_Player_H_
#define _Map_Player_H_

#include "main.h"
#include "Map_Main.h"

#define	MAP_PLAYER_SIZE		(MAP_UNIT_SIZE * 0.3f)
#define MAP_PALYER_SPEED	(2.0f)
#define MAP_PLAYER_COS		(0.1f)
typedef struct
{
	COORDINATION_INT	Pos;
	D3DXVECTOR3			DrawPos;
	D3DXVECTOR3			Move;
	float				fSpeed;

}MAP_PLAYER;
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitMap_Player(void);
void UninitMap_Player(void);
void UpdateMap_Player(void);
void DrawMap_Player(void);
D3DXVECTOR3  *GetMapRangeLimit(void);
COORDINATION_INT GetMapPlayerPos(void);

#endif
