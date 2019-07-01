//=============================================================================
//
// 背景処理 [Map_Main.cpp]
//
//=============================================================================
#include "Map_Main.h"
#include "main.h"
#include "Battle_Main.h"
#include "input.h"
//#include "fade.h"
#include "Map_Player.h"
#include "Fade_loading.h"
#include "Battle_Record_Time.h"
//#include "Sys_Record.h"
#include <time.h>
#include <stdlib.h>

#define TEXTURE_FILED_UNIT	("data/TEXTURE/field_unit.png")
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexMap_Main(LPDIRECT3DDEVICE9 pDevice);
void SetVerTexFieldUnit(void);
void InitContent(void);
void InitFieldUnit(void);
//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_apTextureUnit = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFieldUnit = NULL;// 頂点バッファインターフェースへのポインタ
int	g_FieldState;
D3DXVECTOR3		g_MapRangePos[2];
COORDINATION_INT g_MapContent[MAP_MAX_CONTENT];
FIELD_UNIT g_FiledUnit[MAP_LINE_UNIT][MAP_LINE_UNIT];
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitMap_Main(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	InitFieldUnit();
	for(int nRange = 0; nRange < 2; nRange ++)
	{
	g_MapRangePos[nRange] = D3DXVECTOR3(0.0f,0.0f,0.0f);
	}
	InitContent();


	//InitRecord_System();

	g_FieldState = FIELD_SETUP;
	D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
									TEXTURE_FILED_UNIT,		// ファイルの名前
									&g_apTextureUnit);	// 読み込むメモリー
	MakeVertexMap_Main(pDevice);
	return S_OK;
}
void InitContent(void)
{
	for(int nCntContent = 0; nCntContent < MAP_MAX_CONTENT; nCntContent ++)
	{
		g_MapContent[nCntContent].nX = 0;
		g_MapContent[nCntContent].nY = 0;

	}
}
void InitFieldUnit(void)
{
		for(int nCntUnitY = 0; nCntUnitY < MAP_LINE_UNIT ; nCntUnitY++)
	{
		for(int nCntUnitX = 0; nCntUnitX < MAP_LINE_UNIT ; nCntUnitX++)
		{
			g_FiledUnit[nCntUnitY][nCntUnitX].bWalkble = true;
			g_FiledUnit[nCntUnitY][nCntUnitX].Content.bUse	= false;
			g_FiledUnit[nCntUnitY][nCntUnitX].Content.nFloorID	= 0;
			g_FiledUnit[nCntUnitY][nCntUnitX].Content.nStageID	= 0;
			g_FiledUnit[nCntUnitY][nCntUnitX].DrawPos	= D3DXVECTOR3(0.0f,0.0f,0.0f);
			g_FiledUnit[nCntUnitY][nCntUnitX].nType		= 0;
			g_FiledUnit[nCntUnitY][nCntUnitX].Place.nX	= 0;
			g_FiledUnit[nCntUnitY][nCntUnitX].Place.nY	= 0;
		}
	}
}
//=============================================================================
// 終了処理
//=============================================================================
void UninitMap_Main(void)
{
		if(g_apTextureUnit != NULL)
	{// テクスチャの開放
		g_apTextureUnit->Release();
		g_apTextureUnit = NULL;
	}

	if(g_pVtxBuffFieldUnit != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffFieldUnit->Release();
		g_pVtxBuffFieldUnit = NULL;
	}
	//UninitRecord_System();
}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateMap_Main(void)
{
	COORDINATION_INT PlayerPos = GetMapPlayerPos();


	switch(g_FieldState)
	{
	case FIELD_SETUP:
		InitFieldUnit();
		for(int nCntUnitY = 0; nCntUnitY < MAP_LINE_UNIT ; nCntUnitY++)
		{
			for(int nCntUnitX = 0; nCntUnitX < MAP_LINE_UNIT ; nCntUnitX++)
			{
				g_FiledUnit[nCntUnitY][nCntUnitX].bWalkble = true;
				g_FiledUnit[nCntUnitY][nCntUnitX].DrawPos.x = MAP_DRAW_POS_X + nCntUnitX * MAP_UNIT_SIZE;
				g_FiledUnit[nCntUnitY][nCntUnitX].DrawPos.y = MAP_DRAW_POS_Y + nCntUnitY * MAP_UNIT_SIZE;
			}
		}
		g_MapContent[0].nX = rand()%MAP_LINE_UNIT;
		g_MapContent[0].nY = rand()%MAP_LINE_UNIT;

		for(int nCntContent = 1; nCntContent < MAP_MAX_CONTENT;nCntContent++)
		{
			g_MapContent[nCntContent].nX = rand()%MAP_LINE_UNIT;
			g_MapContent[nCntContent].nY = rand()%MAP_LINE_UNIT;
			for(int nCntIdx = 0; nCntIdx < nCntContent;nCntIdx++)
			{
				if(g_MapContent[nCntIdx].nX != g_MapContent[nCntContent].nX &&
					g_MapContent[nCntIdx].nY != g_MapContent[nCntContent].nY)
					continue;
				g_MapContent[nCntContent].nX = rand()%MAP_LINE_UNIT;
				g_MapContent[nCntContent].nY = rand()%MAP_LINE_UNIT;
				nCntIdx = -1;
			}
		}
		g_FiledUnit[g_MapContent[0].nY][g_MapContent[0].nX].nType = UNIT_STAGE;

		for(int nCntIdx = 1; nCntIdx < MAP_MAX_SINGLE_FLOOR+1;nCntIdx++)
		{
			g_FiledUnit[g_MapContent[nCntIdx].nY][g_MapContent[nCntIdx].nX].nType	= 2;
			g_FiledUnit[g_MapContent[nCntIdx].nY][g_MapContent[nCntIdx].nX].Content.bUse	= true;
			g_FiledUnit[g_MapContent[nCntIdx].nY][g_MapContent[nCntIdx].nX].Content.nFloorID = 1;
		}
		g_FieldState = FIELD_UPDATE;
		break;
	case FIELD_UPDATE:
		break;
	}
	g_MapRangePos[0] = g_FiledUnit[0][0].DrawPos;
	g_MapRangePos[1].x = g_FiledUnit[0][0].DrawPos.x + MAP_LINE_UNIT * MAP_UNIT_SIZE;
	g_MapRangePos[1].y = g_FiledUnit[0][0].DrawPos.y + MAP_LINE_UNIT * MAP_UNIT_SIZE;
	if(g_FiledUnit[PlayerPos.nY][PlayerPos.nX].nType == UNIT_STAGE)
	{
		SetBattleStageID(g_FiledUnit[PlayerPos.nY][PlayerPos.nX].Content.nStageID);
		//SetFade(FADE_OUT,MODE_GAME);
		SetLoadStart(MODE_GAME);
		g_FieldState = FIELD_SETUP;
	}
	SetVerTexFieldUnit();
	//UpdateRecord_System();
}
//=============================================================================
// マップ描画限界座標を取得
//=============================================================================
D3DXVECTOR3 *GetMapRangeLimit(void)
{
	return g_MapRangePos;
}
FIELD_UNIT *GetFieldUnit(void)
{
	return &g_FiledUnit[0][0];
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawMap_Main(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffFieldUnit, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_apTextureUnit);
	for(int nCntUnitY = 0; nCntUnitY < MAP_LINE_UNIT ; nCntUnitY++)
	{
		for(int nCntUnitX = 0; nCntUnitX < MAP_LINE_UNIT ; nCntUnitX++)
		{
		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, ((nCntUnitY * MAP_LINE_UNIT + nCntUnitX) * 4), NUM_POLYGON);
		}
	}
	//DrawRecord_System();
}
//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexMap_Main(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * MAP_MAX_UNIT,	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
												FVF_VERTEX_2D,						// 使用する頂点フォーマット
												D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
												&g_pVtxBuffFieldUnit,					// 頂点バッファインターフェースへのポインタ
												NULL)))								// NULLに設定
	{
        return E_FAIL;
	}
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffFieldUnit->Lock(0, 0, (void**)&pVtx, 0);

		for(int nCntUnitY = 0; nCntUnitY < MAP_LINE_UNIT ; nCntUnitY++)
		{
			for(int nCntUnitX = 0; nCntUnitX < MAP_LINE_UNIT ; nCntUnitX++,pVtx += 4)
			{
			
				// 頂点座標の設定
				pVtx[0].pos.x = g_FiledUnit[nCntUnitY][nCntUnitX].DrawPos.x;
				pVtx[0].pos.y = g_FiledUnit[nCntUnitY][nCntUnitX].DrawPos.y;
				pVtx[0].pos.z = 0.0f;

				pVtx[1].pos.x = g_FiledUnit[nCntUnitY][nCntUnitX].DrawPos.x + MAP_UNIT_SIZE;
				pVtx[1].pos.y = g_FiledUnit[nCntUnitY][nCntUnitX].DrawPos.y;
				pVtx[1].pos.z = 0.0f;

				pVtx[2].pos.x = g_FiledUnit[nCntUnitY][nCntUnitX].DrawPos.x ;//+ MAP_UNIT_SIZE;
				pVtx[2].pos.y = g_FiledUnit[nCntUnitY][nCntUnitX].DrawPos.y + MAP_UNIT_SIZE;
				pVtx[2].pos.z = 0.0f;
			
				pVtx[3].pos.x = g_FiledUnit[nCntUnitY][nCntUnitX].DrawPos.x + MAP_UNIT_SIZE;
				pVtx[3].pos.y = g_FiledUnit[nCntUnitY][nCntUnitX].DrawPos.y + MAP_UNIT_SIZE;
				pVtx[3].pos.z = 0.0f;
				// rhwの設定
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
				pVtx[0].tex = D3DXVECTOR2(g_FiledUnit[nCntUnitY][nCntUnitX].nType * 0.2f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(g_FiledUnit[nCntUnitY][nCntUnitX].nType * 0.2f + 0.2f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(g_FiledUnit[nCntUnitY][nCntUnitX].nType * 0.2f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(g_FiledUnit[nCntUnitY][nCntUnitX].nType * 0.2f + 0.2f, 1.0f);
			}
		}
		// 頂点データをアンロックする
		g_pVtxBuffFieldUnit->Unlock();
	}

	return S_OK;
}
//=============================================================================
// Fieldの設定
//=============================================================================
void SetVerTexFieldUnit(void)
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffFieldUnit->Lock(0, 0, (void**)&pVtx, 0);

		for(int nCntUnitY = 0; nCntUnitY < MAP_LINE_UNIT ; nCntUnitY++)
		{
			for(int nCntUnitX = 0; nCntUnitX < MAP_LINE_UNIT ; nCntUnitX++,pVtx += 4)
			{
				// 頂点座標の設定
				pVtx[0].pos.x = g_FiledUnit[nCntUnitY][nCntUnitX].DrawPos.x;
				pVtx[0].pos.y = g_FiledUnit[nCntUnitY][nCntUnitX].DrawPos.y;
				pVtx[0].pos.z = 0.0f;

				pVtx[1].pos.x = g_FiledUnit[nCntUnitY][nCntUnitX].DrawPos.x + MAP_UNIT_SIZE;
				pVtx[1].pos.y = g_FiledUnit[nCntUnitY][nCntUnitX].DrawPos.y;
				pVtx[1].pos.z = 0.0f;

				pVtx[2].pos.x = g_FiledUnit[nCntUnitY][nCntUnitX].DrawPos.x ;//+ MAP_UNIT_SIZE;
				pVtx[2].pos.y = g_FiledUnit[nCntUnitY][nCntUnitX].DrawPos.y + MAP_UNIT_SIZE;
				pVtx[2].pos.z = 0.0f;
			
				pVtx[3].pos.x = g_FiledUnit[nCntUnitY][nCntUnitX].DrawPos.x + MAP_UNIT_SIZE;
				pVtx[3].pos.y = g_FiledUnit[nCntUnitY][nCntUnitX].DrawPos.y + MAP_UNIT_SIZE;
				pVtx[3].pos.z = 0.0f;
				// rhwの設定
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
				pVtx[0].tex = D3DXVECTOR2(g_FiledUnit[nCntUnitY][nCntUnitX].nType * 0.2f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(g_FiledUnit[nCntUnitY][nCntUnitX].nType * 0.2f + 0.2f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(g_FiledUnit[nCntUnitY][nCntUnitX].nType * 0.2f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(g_FiledUnit[nCntUnitY][nCntUnitX].nType * 0.2f + 0.2f, 1.0f);
			}
		}
		// 頂点データをアンロックする
		g_pVtxBuffFieldUnit->Unlock();
	}
}
