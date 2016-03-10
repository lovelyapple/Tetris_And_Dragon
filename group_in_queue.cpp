//=============================================================================
// テトリスのメイン処理[group_in_queue.cpp]
//=============================================================================
#include "main.h"
#include "game.h"
#include "group_in_queue.h"
#include "Effect_Tetris_Block_Shining.h"
#include "Tetris_main.h"
#include "input.h"
#include "Function.h"
#include <time.h>
#include <stdlib.h>

#define VALUE_PROBABILITY_SKILL_BLOCK		(5)
#define VALUE_PROBABILITY_GRAVITY_GROUP		(10)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexBlock_Q(LPDIRECT3DDEVICE9 pDevice);//頂点作成
void SetVerTexBlock_Q(void);//テクスチャ頂点設定

void InitBlock_Q_ALL(void);
void InitGroup_Q_ALL(void);
void InitRandColor(void);
//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureBlock_Q	= NULL;//テクスチャメモリ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBlock_Q = NULL;//頂点座標バッファ

BLOCK	g_aBlock_Q[MAX_BLOCK_Q];
GROUP	g_aGroup_Q[MAX_GROUP_Q];
RAND_STRUCT g_RandColor[BLOCK_WALL];
int g_Q_ID;
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitGroup_Q(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	InitBlock_Q_ALL();
	InitGroup_Q_ALL();
	InitRandColor();

	D3DXCreateTextureFromFile(pDevice,
		TEX_BLOCK_TEXTURE,
		&g_pTextureBlock_Q);

	MakeVertexBlock_Q(pDevice);
	g_Q_ID= -1;
	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void UninitGroup_Q(void)
{
	if(g_pTextureBlock_Q != NULL)
	{// テクスチャの開放
		g_pTextureBlock_Q->Release();
		g_pTextureBlock_Q = NULL;
	}

	if(g_pVtxBuffBlock_Q != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffBlock_Q->Release();
		g_pVtxBuffBlock_Q = NULL;
	}
}
//=============================================================================
// 描画処理
//=============================================================================
void DrawGroup_Q(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	//ロックの描画
	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffBlock_Q, 0, sizeof(VERTEX_2D));
	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	// テクスチャの設定
	pDevice->SetTexture(0,g_pTextureBlock_Q);
	for(int nCnt_Block_Q = 0; nCnt_Block_Q < MAX_BLOCK;nCnt_Block_Q ++)
	{
		if(g_aBlock_Q[nCnt_Block_Q].bUse == false)
			continue;
		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,nCnt_Block_Q*4, NUM_POLYGON);
	}

}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateGroup_Q(void)
{
	//if(GetKeyboardTrigger(DIK_U))
	//	SetGroupQHandleALL(FORM_I,BLOCK_RED);

	//==================shining Effect========================
	for(int nCntGroup_Q = 0; nCntGroup_Q < MAX_GROUP_Q; nCntGroup_Q++)
	{
		if(g_aGroup_Q[nCntGroup_Q].nType == GROUP_WAIT && rand()%1000 < 5)
			{	
				int start	= g_aGroup_Q[nCntGroup_Q].B_start;
				for(int nBlock_Q = 0; nBlock_Q < g_aGroup_Q[nCntGroup_Q].Form.nCount;nBlock_Q++)
				{
					SetBlockShining(D3DXVECTOR3(g_aBlock_Q[nBlock_Q + start].Pos.x,g_aBlock_Q[nBlock_Q + start].Pos.y,0.0f));
				}
			}
	}



	SetVerTexBlock_Q();
}
void SetGroupQHandleALL(int Form,int color)
{
	InitBlock_Q_ALL();
	InitGroup_Q_ALL();
	for(int nCntGroupQ = 0; nCntGroupQ < MAX_GROUP_Q; nCntGroupQ++)
	{
		int start = 0;
		int nCntBlock_Q = 0;
		g_aGroup_Q[nCntGroupQ].NumForm	= Form + 1;
		g_aGroup_Q[nCntGroupQ].nColor		= color;
		//g_aGroup_Q[nCntGroupQ].nColor		= GetRandColor(g_RandColor,BLOCK_WALL);
		g_aGroup_Q[nCntGroupQ].nType		= GROUP_WAIT;
		g_aGroup_Q[nCntGroupQ].Rotation	= 0;
		g_aGroup_Q[nCntGroupQ].Pos.x		= VALUE_Q_GROUP_POS_X;
		g_aGroup_Q[nCntGroupQ].Pos.y		= VALUE_Q_GROUP_POS_Y + nCntGroupQ * (VALUE_GROUP_Q_SIZE + 10.0f);
		g_aGroup_Q[nCntGroupQ].Form		= GetGroupForm(g_aGroup_Q[nCntGroupQ].NumForm);

		while(g_aBlock_Q[nCntBlock_Q].bUse == true)
			nCntBlock_Q++;
		if(nCntBlock_Q > MAX_BLOCK_Q - 10)
			InitBlock_Q_ALL();

		g_aGroup_Q[nCntGroupQ].B_start	= nCntBlock_Q;
		start	= nCntBlock_Q;
		g_aGroup_Q[nCntGroupQ].B_end	= start + g_aGroup_Q[nCntGroupQ].Form.nCount-1;

		for(int nBlock_Q = 0; nBlock_Q < g_aGroup_Q[nCntGroupQ].Form.nCount;nBlock_Q++)
		{
			g_aBlock_Q[nBlock_Q + start].bUse	 = true;
			g_aBlock_Q[nBlock_Q + start].nColor	 = g_aGroup_Q[nCntGroupQ].nColor;
			if(rand()%100 < 5)
				g_aBlock_Q[nBlock_Q + start].nColor = BLOCK_MP;
			g_aBlock_Q[nBlock_Q + start].FormPos = g_aGroup_Q[nCntGroupQ].Form.Place[nBlock_Q];
			g_aBlock_Q[nBlock_Q + start].Place.x = g_aGroup_Q[nCntGroupQ].Place.x + g_aBlock_Q[nBlock_Q + start].FormPos.x;
			g_aBlock_Q[nBlock_Q + start].Place.y = g_aGroup_Q[nCntGroupQ].Place.y - g_aBlock_Q[nBlock_Q + start].FormPos.y;
			g_aBlock_Q[nBlock_Q + start].Pos.x	 = g_aGroup_Q[nCntGroupQ].Pos.x + g_aBlock_Q[nBlock_Q + start].Place.x * VALUE_BLOCK_Q_SIZE;
			g_aBlock_Q[nBlock_Q + start].Pos.y	 = g_aGroup_Q[nCntGroupQ].Pos.y + g_aBlock_Q[nBlock_Q + start].Place.y * VALUE_BLOCK_Q_SIZE;

		}
	}



}

//=============================================================================
// Ｑグループの設定処理
//=============================================================================
void SetGroup_Q(int Q_ID)
{
	//g_aGroup_Q[Q_ID].Placeこれは設定しなくてもよい
	//srand((unsigned int) time (NULL));
	SetRandColor();
	int start = 0;
	int nCntBlock_Q = 0;
	g_aGroup_Q[Q_ID].NumForm	= rand()%(MAX_FORM_TYPE-1)+1;
	g_aGroup_Q[Q_ID].nColor		= rand()%BLOCK_WALL;
	g_aGroup_Q[Q_ID].nColor		= GetRandColor(g_RandColor,BLOCK_WALL);
	g_aGroup_Q[Q_ID].nType		= GROUP_WAIT;
	g_aGroup_Q[Q_ID].Rotation	= 0;
	g_aGroup_Q[Q_ID].Pos.x		= VALUE_Q_GROUP_POS_X;
	g_aGroup_Q[Q_ID].Pos.y		= VALUE_Q_GROUP_POS_Y + Q_ID * (VALUE_GROUP_Q_SIZE + 10.0f);
	g_aGroup_Q[Q_ID].Form		= GetGroupForm(g_aGroup_Q[Q_ID].NumForm);

	while(g_aBlock_Q[nCntBlock_Q].bUse == true)
		nCntBlock_Q++;
	if(nCntBlock_Q > MAX_BLOCK_Q - 10)
		InitBlock_Q_ALL();

	g_aGroup_Q[Q_ID].B_start	= nCntBlock_Q;
	start	= nCntBlock_Q;
	g_aGroup_Q[Q_ID].B_end	= start + g_aGroup_Q[Q_ID].Form.nCount-1;

	
	int nBlockType = 0;
	//if(rand()%100 < VALUE_PROBABILITY_GRAVITY_GROUP)
		nBlockType = BLOCK_GRAVITY;
	for(int nBlock_Q = 0; nBlock_Q < g_aGroup_Q[Q_ID].Form.nCount;nBlock_Q++)
	{
		g_aBlock_Q[nBlock_Q + start].bUse	 = true;
		g_aBlock_Q[nBlock_Q + start].Type	 = nBlockType;
		g_aBlock_Q[nBlock_Q + start].nColor	 = g_aGroup_Q[Q_ID].nColor;
		if(rand()%100 < VALUE_PROBABILITY_SKILL_BLOCK)
			g_aBlock_Q[nBlock_Q + start].nColor = BLOCK_MP;
		g_aBlock_Q[nBlock_Q + start].FormPos = g_aGroup_Q[Q_ID].Form.Place[nBlock_Q];
		g_aBlock_Q[nBlock_Q + start].Place.x = g_aGroup_Q[Q_ID].Place.x + g_aBlock_Q[nBlock_Q + start].FormPos.x;
		g_aBlock_Q[nBlock_Q + start].Place.y = g_aGroup_Q[Q_ID].Place.y - g_aBlock_Q[nBlock_Q + start].FormPos.y;
		g_aBlock_Q[nBlock_Q + start].Pos.x	 = g_aGroup_Q[Q_ID].Pos.x + g_aBlock_Q[nBlock_Q + start].Place.x * VALUE_BLOCK_Q_SIZE;
		g_aBlock_Q[nBlock_Q + start].Pos.y	 = g_aGroup_Q[Q_ID].Pos.y + g_aBlock_Q[nBlock_Q + start].Place.y * VALUE_BLOCK_Q_SIZE;

	}

}
void InitGroup_Q_ALL(void)
{
	for(int nCntGroup_Q = 0; nCntGroup_Q < MAX_GROUP_Q; nCntGroup_Q++)
	{
		g_aGroup_Q[nCntGroup_Q].B_start = 0;
		g_aGroup_Q[nCntGroup_Q].B_end	= 0;
		g_aGroup_Q[nCntGroup_Q].nColor	= BLOCK_RED;
		//g_aGroup_Q[nCntGroup_Q].nColor	= BLOCK_MP;
		g_aGroup_Q[nCntGroup_Q].nType	= GROUP_BACK;
		g_aGroup_Q[nCntGroup_Q].NumForm	= FORM__NONE;
		g_aGroup_Q[nCntGroup_Q].Place	= D3DXVECTOR2(0.0f,0.0f);
		g_aGroup_Q[nCntGroup_Q].Pos		= D3DXVECTOR2(0.0f,0.0f);
		g_aGroup_Q[nCntGroup_Q].Form.nCount = 0;
		g_aGroup_Q[nCntGroup_Q].Form.Rot	= 1;
		g_aGroup_Q[nCntGroup_Q].Rotation	= 0;
		for(int nPlace = 0; nPlace < MAX_FORM_PLACE; nPlace++)
			g_aGroup_Q[nCntGroup_Q].Form.Place[nPlace]=D3DXVECTOR2(0.0f,0.0f);

	}
}
void InitBlock_Q_ALL(void)
{
	for(int nBlock_Q = 0;nBlock_Q < MAX_BLOCK_Q;nBlock_Q++)
	{
		g_aBlock_Q[nBlock_Q].bUse	= false;
		g_aBlock_Q[nBlock_Q].nColor	= BLOCK_MP;
		g_aBlock_Q[nBlock_Q].Place	= D3DXVECTOR2(0,0);
		g_aBlock_Q[nBlock_Q].Pos		= D3DXVECTOR2(0.0f,0.0f);
		g_aBlock_Q[nBlock_Q].FormPos	= D3DXVECTOR2(0,0);
	}
}
void InitRandColor(void)
{
	for(int nColor =0;nColor<BLOCK_WALL;nColor++)
	{
		g_RandColor[nColor].bUse = false;
		g_RandColor[nColor].BonusValue	= 0.0f;
		g_RandColor[nColor].Name = nColor;
	}

}

//=============================================================================
// Block_Q頂点の作成
//=============================================================================
HRESULT MakeVertexBlock_Q(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * MAX_BLOCK_Q,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
		FVF_VERTEX_2D,						// 使用する頂点フォーマット
		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
		&g_pVtxBuffBlock_Q,					// 頂点バッファインターフェースへのポインタ
		NULL)))								// NULLに設定
	{
		return E_FAIL;
	}
	//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffBlock_Q->Lock(0, 0, (void**)&pVtx, 0);

	for(int nCnt_Block_Q = 0; nCnt_Block_Q < MAX_BLOCK;nCnt_Block_Q ++,pVtx += 4)
	{
		// 頂点座標の設定/ 
		pVtx[0].pos.x = g_aBlock_Q[nCnt_Block_Q].Pos.x;
		pVtx[0].pos.y = g_aBlock_Q[nCnt_Block_Q].Pos.y - VALUE_BLOCK_Q_SIZE;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_aBlock_Q[nCnt_Block_Q].Pos.x + VALUE_BLOCK_Q_SIZE;
		pVtx[1].pos.y = g_aBlock_Q[nCnt_Block_Q].Pos.y - VALUE_BLOCK_Q_SIZE;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_aBlock_Q[nCnt_Block_Q].Pos.y;
		pVtx[2].pos.y = g_aBlock_Q[nCnt_Block_Q].Pos.y;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_aBlock_Q[nCnt_Block_Q].Pos.y + VALUE_BLOCK_Q_SIZE;
		pVtx[3].pos.y = g_aBlock_Q[nCnt_Block_Q].Pos.y;
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
		pVtx[0].tex = D3DXVECTOR2(g_aBlock_Q[nCnt_Block_Q].nColor * (1.0f/BLOCK_MAX_COLOR), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(g_aBlock_Q[nCnt_Block_Q].nColor * (1.0f/BLOCK_MAX_COLOR)+(1.0f/BLOCK_MAX_COLOR), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(g_aBlock_Q[nCnt_Block_Q].nColor * (1.0f/BLOCK_MAX_COLOR), 0.5f);
		pVtx[3].tex = D3DXVECTOR2(g_aBlock_Q[nCnt_Block_Q].nColor * (1.0f/BLOCK_MAX_COLOR)+(1.0f/BLOCK_MAX_COLOR), 0.5f);
	}
	// 頂点データをアンロックする
	g_pVtxBuffBlock_Q->Unlock();
	return S_OK;

}
//=============================================================================
// Blockの設定
//=============================================================================
void SetVerTexBlock_Q(void)
{
	//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffBlock_Q->Lock(0, 0, (void**)&pVtx, 0);

	for(int nCnt_Block_Q = 0; nCnt_Block_Q < MAX_BLOCK; nCnt_Block_Q ++,pVtx += 4)
	{
		if(g_aBlock_Q[nCnt_Block_Q].bUse == false)
			continue;
		//頂点座標設定
		pVtx[0].pos.x = g_aBlock_Q[nCnt_Block_Q].Pos.x;
		pVtx[0].pos.y = g_aBlock_Q[nCnt_Block_Q].Pos.y - VALUE_BLOCK_Q_SIZE;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_aBlock_Q[nCnt_Block_Q].Pos.x + VALUE_BLOCK_Q_SIZE;
		pVtx[1].pos.y = g_aBlock_Q[nCnt_Block_Q].Pos.y - VALUE_BLOCK_Q_SIZE;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_aBlock_Q[nCnt_Block_Q].Pos.x;
		pVtx[2].pos.y = g_aBlock_Q[nCnt_Block_Q].Pos.y;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_aBlock_Q[nCnt_Block_Q].Pos.x + VALUE_BLOCK_Q_SIZE;
		pVtx[3].pos.y = g_aBlock_Q[nCnt_Block_Q].Pos.y;
		pVtx[3].pos.z = 0.0f;

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(g_aBlock_Q[nCnt_Block_Q].nColor * (1.0f/BLOCK_MAX_COLOR), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(g_aBlock_Q[nCnt_Block_Q].nColor * (1.0f/BLOCK_MAX_COLOR)+(1.0f/BLOCK_MAX_COLOR), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(g_aBlock_Q[nCnt_Block_Q].nColor * (1.0f/BLOCK_MAX_COLOR), 0.5f);
		pVtx[3].tex = D3DXVECTOR2(g_aBlock_Q[nCnt_Block_Q].nColor * (1.0f/BLOCK_MAX_COLOR)+(1.0f/BLOCK_MAX_COLOR), 0.5f);
	}
	// 頂点データをアンロックする
	g_pVtxBuffBlock_Q->Unlock();
}
//=============================================================================
// Groupの取得
//=============================================================================
int GetGroup_Q_NumForm(int Q_ID)
{
	return g_aGroup_Q[Q_ID].NumForm;
}
int GetGroup_Q_Color(int Q_ID)
{
	return g_aGroup_Q[Q_ID].nColor;
}
//=============================================================================
// Groupの取得
//=============================================================================
void SetGroup_Q_Type(int Q_ID,int nType)
{
	g_aGroup_Q[nType].nType = nType;
	int start = g_aGroup_Q[Q_ID].B_start;
	for(int nBlock_Q = 0; nBlock_Q < g_aGroup_Q[nType].Form.nCount;nBlock_Q++)
	{
		g_aBlock_Q[nBlock_Q + start].bUse = false;
	}

}
void SetRandColor(void)
{
	for(int nColor = 0;nColor<BLOCK_WALL;nColor++)
	{
		g_RandColor[nColor].bUse = true;
		g_RandColor[nColor].BonusValue	= 0.0f;
		g_RandColor[nColor].Name = nColor;
	}
	//g_RandColor[2].BonusValue = 80;
}
int GetGroup_Q_Start(int Q_ID)
{
	return g_aGroup_Q[Q_ID].B_start;
}

int	GetGroup_Q_D_Color(int D_ID)
{
	return g_aBlock_Q[D_ID].nColor;
}
int	GetGroup_Q_D_nType(int D_ID)
{
	return g_aBlock_Q[D_ID].Type;
}

