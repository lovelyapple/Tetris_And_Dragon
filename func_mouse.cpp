
//=============================================================================
// ブロック破壊効果 [func_mouse.cpp]
//=============================================================================
#include "func_mouse.h"
#include "effect_star.h"
#include "Tetris_main.h"
#include "main.h"
#include "input.h"
#include "Function.h"
#include <d3dx9.h>

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_MOUSE	"data/TEXTURE/mouse_01.png"	// ブロックの画像データ
#define VALUE_MOUSE_SIZE_X	(60.0f)					// 描画サイズＸ
#define VALUE_MOUSE_SIZE_Y	(60.0f)					// 描画サイズＹ

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexMouse(LPDIRECT3DDEVICE9 pDevice);
void SetVerTexMouse(void);

//*****************************************************************************
// 構造体宣言
//*****************************************************************************

typedef struct
{
	D3DXVECTOR3	pos;		//現在位置
	D3DXVECTOR3	move;		//移動量
}MOUSE;
//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureMouse	= NULL;	//テクスチャメモリ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMouse = NULL;	//頂点座標バッファ
int g_Count =0;
MOUSE g_Mouse;
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitFunc_Mouse(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_Mouse.move = D3DXVECTOR3(0.0f,0.0f,0.0f);
	g_Mouse.pos = D3DXVECTOR3(0.0f,0.0f,0.0f);

	D3DXCreateTextureFromFile(pDevice,
			TEXTURE_MOUSE,
			&g_pTextureMouse);

	MakeVertexMouse(pDevice);

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void UninitFunc_Mouse(void)
{
	if(g_pTextureMouse != NULL)
	{// テクスチャの開放
		g_pTextureMouse->Release();
		g_pTextureMouse = NULL;
	}

	if(g_pVtxBuffMouse != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffMouse->Release();
		g_pVtxBuffMouse = NULL;
	}
}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateFunctionc_Mouse(void)
{
	g_Count++;
	g_Mouse.move.x	= GetMouseAxisX();
	g_Mouse.move.y	= GetMouseAxisY();

	g_Mouse.pos += g_Mouse.move*1.1f;;
	g_Mouse.pos.x = SetMAX(g_Mouse.pos.x,SCREEN_WIDTH,0.0f,2);
	g_Mouse.pos.y = SetMAX(g_Mouse.pos.y,SCREEN_HEIGHT,0.0f,2);
	g_Count	= SetMAX(g_Count,100.0f,0.0f,1);	
		//if(GetMouseLeftTrigger()||g_Count%10==0)
			//	SetEffect_Star(g_Mouse.pos,BLOCK_RAND_COLOR,0);



	SetVerTexMouse();
}
//=============================================================================
// 描画処理
//=============================================================================
void DrawFunc_Mouse(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//フィールドの描画
	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffMouse, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetTexture(0,g_pTextureMouse);
	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

}
//=============================================================================
// Broke頂点の作成
//=============================================================================
HRESULT MakeVertexMouse(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * 1,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
		FVF_VERTEX_2D,						// 使用する頂点フォーマット
		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
		&g_pVtxBuffMouse,					// 頂点バッファインターフェースへのポインタ
		NULL)))								// NULLに設定
	{
		return E_FAIL;
	}
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffMouse->Lock(0, 0, (void**)&pVtx, 0);
		// 頂点座標の設定
		pVtx[0].pos.x = g_Mouse.pos.x;
		pVtx[0].pos.y = g_Mouse.pos.y;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_Mouse.pos.x + VALUE_MOUSE_SIZE_X;
		pVtx[1].pos.y = g_Mouse.pos.y;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_Mouse.pos.x;
		pVtx[2].pos.y = g_Mouse.pos.y + VALUE_MOUSE_SIZE_Y;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_Mouse.pos.x + VALUE_MOUSE_SIZE_X;
		pVtx[3].pos.y = g_Mouse.pos.y + VALUE_MOUSE_SIZE_Y;
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

		pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

	}
	// 頂点データをアンロックする
	g_pVtxBuffMouse->Unlock();
	return S_OK;
}
//=============================================================================
// Broke頂点の設定
//=============================================================================
void SetVerTexMouse(void)
{
	{//頂点バッファの中身を埋[める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffMouse->Lock(0, 0, (void**)&pVtx, 0);
		//float a = VALUE_MOUSE_SIZE_X * (float)(40.0f/120.0f);
		pVtx[0].pos.x = g_Mouse.pos.x - (VALUE_MOUSE_SIZE_X * (float)(1.0f/3.0f));
		pVtx[0].pos.y = g_Mouse.pos.y - (VALUE_MOUSE_SIZE_X * (float)(1.0f/3.0f));
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_Mouse.pos.x + (VALUE_MOUSE_SIZE_X * (float)(2.0f/3.0f));
		pVtx[1].pos.y = g_Mouse.pos.y - (VALUE_MOUSE_SIZE_X * (float)(1.0f/3.0f));
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_Mouse.pos.x - (VALUE_MOUSE_SIZE_X * (float)(1.0f/3.0f));
		pVtx[2].pos.y = g_Mouse.pos.y + (VALUE_MOUSE_SIZE_X * (float)(2.0f/3.0f));
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_Mouse.pos.x + (VALUE_MOUSE_SIZE_X * (float)(2.0f/3.0f));
		pVtx[3].pos.y = g_Mouse.pos.y + (VALUE_MOUSE_SIZE_X * (float)(2.0f/3.0f));
		pVtx[3].pos.z = 0.0f;
		// 頂点データをアンロックする
		g_pVtxBuffMouse->Unlock();
	}
}
