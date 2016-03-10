
//=============================================================================
// ブロック破壊効果 [Effect_Broke.cpp]
//=============================================================================
#include "effect_block_broke.h"
#include "Tetris_main.h"
#include "main.h"
#include "input.h"
#include "Function.h"
#include "Func_NumberX.h"
#include "effect_star.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define VALUE_BROKEN_BLOCK_SIZE	(VALUE_BLOCK_SIZE)// 分裂するブロックのサイズ
#define VALUE_TIME_BROKE		(30)				// 1秒
#define MAX_BROKEN				(256)				// 分裂できるブロックの最大数
#define VALUE_DEFAULT_SPEED_BROKE (3.0f)				// 初期速度


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexEffect_Broke(LPDIRECT3DDEVICE9 pDevice);
void InitEffect_BrokeSingle(int BrokeID);
void SetVerTexEffect_Broke(int Broken_ID);
//*****************************************************************************
// 構造体宣言
//*****************************************************************************
typedef enum
{
	BROKE_PHASE_DROP = 0,
	BROKE_PHASE_UP,
	BROKE_PHASE_RESET,
	BROKE_PHASE_MAX,
}BROKE_PHASE;
typedef struct
{
	bool bUse;			//使用しているかどうか
	int	life;			//消えるまでの時間
	BROKE_PHASE phase;
	D3DXVECTOR3 pos;	//描画位置(中心)
	D3DXVECTOR3	move;	//移動方向

	D3DXVECTOR3 DestMove;
	int color;			//使用している画像
	float	Alpha;		//透明度
}BROKE;

//*****************************************************************************
// グローバル変数
//*****************************************************************************
//LPDIRECT3DTEXTURE9		g_pTextureEffect_Broke	= NULL;	//テクスチャメモリ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect_Broke = NULL;	//頂点座標バッファ
BROKE g_Broken[MAX_BROKEN];			//分裂ブロックの構造体


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEffect_Broke(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for(int nCntBro = 0;nCntBro < MAX_BROKEN;nCntBro++)
	{
		InitEffect_BrokeSingle(nCntBro);
	}


	//D3DXCreateTextureFromFile(pDevice,
	//	TEXTURE_BROKEN,
	//	&g_pTextureEffect_Broke);

	MakeVertexEffect_Broke(pDevice);
	return S_OK;
}
void InitEffect_BrokeSingle(int BrokeID)
{
		g_Broken[BrokeID].bUse	= false;
		g_Broken[BrokeID].color	= BLOCK_MAX_COLOR;
		g_Broken[BrokeID].move	= D3DXVECTOR3(0.0f,VALUE_DEFAULT_SPEED_BROKE,0.0f);
		g_Broken[BrokeID].pos	= D3DXVECTOR3(0.0f,0.0f,0.0f);
		g_Broken[BrokeID].life	= VALUE_TIME_BROKE;
		g_Broken[BrokeID].Alpha	= 1.0f;
		g_Broken[BrokeID].DestMove	= D3DXVECTOR3(0.0f,0.0f,0.0f);
		g_Broken[BrokeID].phase		= BROKE_PHASE_MAX;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEffect_Broke(void)
{
	//if(g_pTextureEffect_Broke != NULL)
	//{// テクスチャの開放
	//	g_pTextureEffect_Broke->Release();
	//	g_pTextureEffect_Broke = NULL;
	//}

	if(g_pVtxBuffEffect_Broke != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffEffect_Broke->Release();
		g_pVtxBuffEffect_Broke = NULL;
	}
}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateEffect_Broke(void)
{
	for(int nCntBroken = 0; nCntBroken < MAX_BROKEN;nCntBroken++)
	{
		if(g_Broken[nCntBroken].bUse == false)
			continue;


		switch(g_Broken[nCntBroken].phase)
		{

		case 	BROKE_PHASE_DROP:
			g_Broken[nCntBroken].life --;
			g_Broken[nCntBroken].move *= 0.96f;
			g_Broken[nCntBroken].pos += g_Broken[nCntBroken].move;
			if(g_Broken[nCntBroken].life < 0)
			{
				g_Broken[nCntBroken].life = 30;
				g_Broken[nCntBroken].move = (g_Broken[nCntBroken].DestMove - g_Broken[nCntBroken].pos) / VALUE_TIME_BROKE;
				g_Broken[nCntBroken].phase = BROKE_PHASE_UP;

				if(g_Broken[nCntBroken].DestMove == D3DXVECTOR3(0.0f,0.0f,0.0f))
					g_Broken[nCntBroken].phase = BROKE_PHASE_RESET;
			}
			break;
		case 	BROKE_PHASE_UP:
			g_Broken[nCntBroken].life --;
			g_Broken[nCntBroken].pos += g_Broken[nCntBroken].move;
			if(g_Broken[nCntBroken].life < 0)
			{
				g_Broken[nCntBroken].phase = BROKE_PHASE_RESET;
			}

			break;
		case	BROKE_PHASE_RESET:
			InitEffect_BrokeSingle(nCntBroken);
			g_Broken[nCntBroken].phase = BROKE_PHASE_MAX;
			break;
		case	BROKE_PHASE_MAX:
			break;
		}

	 SetVerTexEffect_Broke(nCntBroken);

	}
}
//=============================================================================
// Brokenの設定
//=============================================================================
void SetEffect_Broke(D3DXVECTOR3 Pos,D3DXVECTOR3 DestPos ,int Color)
{
	for(int nCntBroken = 0; nCntBroken < MAX_BROKEN;nCntBroken++)
	{
		if(g_Broken[nCntBroken].bUse == true)
			continue;
		if(DestPos.x != 0.0f && DestPos.y != 0.0f)
			g_Broken[nCntBroken].life = 10;
		g_Broken[nCntBroken].bUse = true;
		g_Broken[nCntBroken].pos	= Pos;
		g_Broken[nCntBroken].move.x = rand()%7 -3;
		g_Broken[nCntBroken].move.y	= rand()%7 -3;;
		g_Broken[nCntBroken].move.z = 0;
		g_Broken[nCntBroken].phase = BROKE_PHASE_DROP;
		g_Broken[nCntBroken].DestMove	= DestPos;
		g_Broken[nCntBroken].color		= Color;

		break;
	}
		
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEffect_Broke(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//フィールドの描画
	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffEffect_Broke, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);



	for(int nCntBro = 0;nCntBro < MAX_BROKEN;nCntBro++)
	{
		if(g_Broken[nCntBro].bUse == false)
			continue;
		LPDIRECT3DTEXTURE9 pTexture = GetMemoryStar(g_Broken[nCntBro].color);

		pDevice->SetTexture(0,pTexture);
		// ポリゴンの描画

		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);//set
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);//in
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntBro * 4), NUM_POLYGON);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);//set
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);// out
	}

}
//=============================================================================
// Broke頂点の作成
//=============================================================================
HRESULT MakeVertexEffect_Broke(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * MAX_BROKEN,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
		FVF_VERTEX_2D,						// 使用する頂点フォーマット
		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
		&g_pVtxBuffEffect_Broke,					// 頂点バッファインターフェースへのポインタ
		NULL)))								// NULLに設定
	{
		return E_FAIL;
	}
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffEffect_Broke->Lock(0, 0, (void**)&pVtx, 0);

		for(int nCntBro = 0;nCntBro < MAX_BROKEN;nCntBro++,pVtx += 4)	
		{
			// 頂点座標の設定
			pVtx[0].pos.x = g_Broken[nCntBro].pos.x - VALUE_BROKEN_BLOCK_SIZE;
			pVtx[0].pos.y = g_Broken[nCntBro].pos.y - VALUE_BROKEN_BLOCK_SIZE;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_Broken[nCntBro].pos.x + VALUE_BROKEN_BLOCK_SIZE;
			pVtx[1].pos.y = g_Broken[nCntBro].pos.y - VALUE_BROKEN_BLOCK_SIZE;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_Broken[nCntBro].pos.x;
			pVtx[2].pos.y = g_Broken[nCntBro].pos.y;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_Broken[nCntBro].pos.x + VALUE_BROKEN_BLOCK_SIZE;
			pVtx[3].pos.y = g_Broken[nCntBro].pos.y;
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

			pVtx[0].tex = D3DXVECTOR2(1.0f,1.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f,1.0f);
			pVtx[2].tex = D3DXVECTOR2(1.0f,1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

		}
		// 頂点データをアンロックする
		g_pVtxBuffEffect_Broke->Unlock();
	}
	return S_OK;

}
//=============================================================================
// Brokenの設定
//=============================================================================
void SetVerTexEffect_Broke(int Broken_ID)
{
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffEffect_Broke->Lock(0, 0, (void**)&pVtx, 0);
		// 頂点座標の設定
		pVtx += (Broken_ID *4);
		float fRandSize = ((float)(rand()%10 + 1))/10 + 0.5f;
			// 頂点座標の設定
			pVtx[0].pos.x = g_Broken[Broken_ID].pos.x - VALUE_BROKEN_BLOCK_SIZE * fRandSize;
			pVtx[0].pos.y = g_Broken[Broken_ID].pos.y - VALUE_BROKEN_BLOCK_SIZE * fRandSize;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_Broken[Broken_ID].pos.x + VALUE_BROKEN_BLOCK_SIZE * fRandSize;
			pVtx[1].pos.y = g_Broken[Broken_ID].pos.y - VALUE_BROKEN_BLOCK_SIZE * fRandSize;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_Broken[Broken_ID].pos.x - VALUE_BROKEN_BLOCK_SIZE * fRandSize;
			pVtx[2].pos.y = g_Broken[Broken_ID].pos.y + VALUE_BROKEN_BLOCK_SIZE * fRandSize;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_Broken[Broken_ID].pos.x + VALUE_BROKEN_BLOCK_SIZE * fRandSize;
			pVtx[3].pos.y = g_Broken[Broken_ID].pos.y + VALUE_BROKEN_BLOCK_SIZE * fRandSize;
			pVtx[3].pos.z = 0.0f;

			// rhwの設定
			pVtx[0].rhw =
			pVtx[1].rhw =
			pVtx[2].rhw =
			pVtx[3].rhw = 1.0f;
			// 頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Broken[Broken_ID].Alpha);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Broken[Broken_ID].Alpha);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Broken[Broken_ID].Alpha);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Broken[Broken_ID].Alpha);


			// テクスチャ座標の設定

			pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);
	
	// 頂点データをアンロックする
	g_pVtxBuffEffect_Broke->Unlock();
	
}

