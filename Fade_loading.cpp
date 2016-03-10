//====================================================
//loading.cpp
//====================================================
#include "Fade_loading.h"
#include "main.h"
//#include "input.h"
#include "Function.h"
#include "sound.h"
#define TEXTURE_LOADING_BLOCK	("data/TEXTURE/loadingblock.png")
#define TEXTURE_LOADING_BLOCK_BG	("data/TEXTURE/loadingblock.png")
#define	LOADING_BLOCK_SIZE		(100.0f)
#define LOADING_BLOCK_COUNT_X	(12)
#define LOADING_BLOCK_COUNT_Y	(7)
#define LOADING_BLOCK_MAX		(LOADING_BLOCK_COUNT_X * LOADING_BLOCK_COUNT_Y)
#define LOADING_BLOCK_POS_X		(LOADING_BLOCK_SIZE)
#define LOADING_BLOCK_POS_Y		(LOADING_BLOCK_SIZE/2)
#define LOADING_BLOCK_TEX_UNIT	(1.0f/6.0f)
#define	LOADING_BLOCK_MOVETIME	(20)
#define LOADING_BLOCK_STOPTIME	(60)
#define MAX_BLOCK_GROUP		(21)
#define LOADING_BLOCK_FADE_OUT_TIME (60)
//====================================================
//構造体宣言
//====================================================
typedef enum
{
	LOAD_BLOCK_RED = 0,
	LOAD_BLOCK_GREEN,
	LOAD_BLOCK_BLUE,
	LOAD_BLOCK_LIGHT,
	LOAD_BLOCK_PURPE,
	LOAD_BLOCK_PINK,
	LOAD_BLOCK_MAX,
}LOAD_BLOCK_COLOR;

typedef struct
{
	bool	bUse;
	int	MoveTime;			//移動可能時間
	int nColor;				//
	D3DXVECTOR3 DrawPos;
	D3DXVECTOR3 DestPos;
	D3DXVECTOR3 Move;

}LOADING_BLOCK;


//====================================================================
//ProtoType
//====================================================================
HRESULT MakeVertexFade_Loading(LPDIRECT3DDEVICE9 pDevice);
HRESULT MakeVertexFade_LoadingBG(LPDIRECT3DDEVICE9 pDevice);
void	SetVetAlpha_LoadingBG(float ValueA);
void	SetVetTexFade_Loading_Block(void);
void	InitFade_Loading_Block(void);
void	InitLoad_Group(void);
//====================================================================
//gloable
//====================================================================
LPDIRECT3DTEXTURE9		g_pTextureFade_Block = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFade_Block = NULL;		// 頂点バッファインターフェースへのポインタ
LPDIRECT3DTEXTURE9		g_pTextureFade_BlockBG = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFade_BlockBG = NULL;		// 頂点バッファインターフェースへのポインタ
LOADING_BLOCK	g_LoadBlock[LOADING_BLOCK_COUNT_Y][LOADING_BLOCK_COUNT_X];
int		g_FadeLoadPhase;										//進行段階
int		g_TimeCount;
int		g_LeftTime;
int		g_GroupIdx;
MODE	g_modeNext = MODE_TITLE;
bool	g_OpenWindow = false;
float	g_AlphaBG = 0.0f;
//int		g_nCntBlock;
//int		g_BlockOutTime;
//int		g_BlockUp;
//int		g_BlockDown;
//int		g_BlockLeft;
//int		g_BlockRight;
int		g_BlockGroupY[MAX_BLOCK_GROUP][4]={
	{5,6,6,6},
	{4,4,5,5},
	{5,5,6,6},
	{6,6,6,6},
	{5,5,6,6},
	{4,4,5,6},
	{4,4,5,5},
	{2,3,4,4},
	{3,3,4,5},
	{2,3,3,3},
	{3,3,3,4},
	{2,3,4,5},
	{2,3,3,4},
	{2,2,2,2},
	{0,0,1,1},
	{0,0,1,1},
	{0,0,1,2},
	{0,1,1,1},
	{0,1,1,2},
	{0,0,1,2},
	{0,0,1,2},
	
};
int	 g_BlockGroupX[MAX_BLOCK_GROUP][4]={
	{0,0,1,2},
	{0,1,1,2},
	{4,5,4,5},
	{6,7,8,9},
	{9,10,10,11},
	{2,3,3,3},
	{5,6,6,7},
	{9,9,9,10},
	{10,11,11,11},
	{1,0,1,2},
	{3,4,5,4},
	{8,8,8,8},
	{7,6,7,7},
	{2,3,4,5},
	{7,8,7,8},
	{2,3,1,2},
	{0,1,0,0},
	{4,3,4,5},
	{9,9,10,10},
	{10,11,11,11},
	{5,6,6,6},

};
//====================================================================
//Init
//====================================================================
HRESULT InitFade_loading(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	g_FadeLoadPhase	= LOADING_BLOCK_PHASE_MAX;
	InitFade_Loading_Block();
	// 頂点情報の設定
	MakeVertexFade_Loading(pDevice);
	MakeVertexFade_LoadingBG(pDevice);
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
								TEXTURE_LOADING_BLOCK,		// ファイルの名前
								&g_pTextureFade_Block);	// 読み込むメモリー
	D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
								TEXTURE_LOADING_BLOCK_BG,		// ファイルの名前
								&g_pTextureFade_BlockBG);	// 読み込むメモリー

	return S_OK;
}									
void InitFade_Loading_Block(void)
{
	int nColor = 0;
	g_TimeCount		= 0;
	g_GroupIdx = 0;
	g_OpenWindow = false;
	g_AlphaBG = 0.0f;
	for(int nCntBlockY = 0; nCntBlockY < LOADING_BLOCK_COUNT_Y;nCntBlockY ++)
	{	//int nColor = 5;
		for(int nCntBlockX = 0; nCntBlockX < LOADING_BLOCK_COUNT_X;nCntBlockX ++)
		{
			g_LoadBlock[nCntBlockY][nCntBlockX].MoveTime	= LOADING_BLOCK_MOVETIME;
			g_LoadBlock[nCntBlockY][nCntBlockX].bUse = false;
			g_LoadBlock[nCntBlockY][nCntBlockX].DrawPos.x = LOADING_BLOCK_POS_X + nCntBlockX*LOADING_BLOCK_SIZE;
			g_LoadBlock[nCntBlockY][nCntBlockX].DrawPos.y = 0 - LOADING_BLOCK_SIZE * 10;
			g_LoadBlock[nCntBlockY][nCntBlockX].DrawPos.z = 0.0f;
			g_LoadBlock[nCntBlockY][nCntBlockX].DestPos.x = LOADING_BLOCK_POS_X + nCntBlockX*LOADING_BLOCK_SIZE;
			g_LoadBlock[nCntBlockY][nCntBlockX].DestPos.y = LOADING_BLOCK_POS_Y + nCntBlockY*LOADING_BLOCK_SIZE;
			g_LoadBlock[nCntBlockY][nCntBlockX].DestPos.z = 0.0f;
			//LOADING_BLOCK_MOVETIME
			g_LoadBlock[nCntBlockY][nCntBlockX].Move.x	= (g_LoadBlock[nCntBlockY][nCntBlockX].DestPos.x - 
															g_LoadBlock[nCntBlockY][nCntBlockX].DrawPos.x)/LOADING_BLOCK_MOVETIME;
			g_LoadBlock[nCntBlockY][nCntBlockX].Move.y	= (g_LoadBlock[nCntBlockY][nCntBlockX].DestPos.y -
															g_LoadBlock[nCntBlockY][nCntBlockX].DrawPos.y)/LOADING_BLOCK_MOVETIME;
			g_LoadBlock[nCntBlockY][nCntBlockX].Move.z	= 0.0f;
			g_LoadBlock[nCntBlockY][nCntBlockX].nColor  = nColor;

		}

	}
	g_LoadBlock[3][3].nColor = 5;
	g_LoadBlock[3][4].nColor = 5;
	g_LoadBlock[3][5].nColor = 5;
	g_LoadBlock[3][6].nColor = 5;
	g_LoadBlock[3][7].nColor = 5;
	g_LoadBlock[3][8].nColor = 5;
		for(int Idx = 0; Idx < MAX_BLOCK_GROUP;Idx++)
		{
			int color = rand()%LOAD_BLOCK_MAX;
			for(int nBlock = 0; nBlock < 4;nBlock++)
			{
				int X = g_BlockGroupX[Idx][nBlock];
				int Y = g_BlockGroupY[Idx][nBlock];
				g_LoadBlock[Y][X].nColor = color;
			}
		}
}


void UninitFade_loading(void)
{
	if(g_pTextureFade_Block != NULL)
	{// テクスチャの開放
		g_pTextureFade_Block->Release();
		g_pTextureFade_Block = NULL;
	}

	if(g_pVtxBuffFade_Block != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffFade_Block->Release();
		g_pVtxBuffFade_Block = NULL;
	}
	if(g_pTextureFade_BlockBG != NULL)
	{// テクスチャの開放
		g_pTextureFade_BlockBG->Release();
		g_pTextureFade_BlockBG = NULL;
	}

	if(g_pVtxBuffFade_BlockBG != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffFade_BlockBG->Release();
		g_pVtxBuffFade_BlockBG = NULL;
	}
}
void SetLoadStart(MODE NextMode)
{
	InitFade_Loading_Block();
	g_modeNext = NextMode;
	g_FadeLoadPhase = LOADING_BLOCK_START;
	Set_Main_Update_Type (UPDATE_MODE_STOP);
	
}
void SetLoadEnd(void)
{
	g_OpenWindow = true;
}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateFade_loading(void)
{
	g_TimeCount ++;
	//if(GetKeyboardTrigger(DIK_L))
	//	g_FadeLoadPhase = LOADING_BLOCK_START;

	switch(g_FadeLoadPhase)
	{
	case LOADING_BLOCK_RESET:
		InitFade_Loading_Block();
		Set_Main_Update_Type (UPDATE_MODE_REAL);
		g_FadeLoadPhase = LOADING_BLOCK_NOTH;
		break;
	case LOADING_BLOCK_NOTH:
		break;
	case LOADING_BLOCK_START:

		g_AlphaBG += 0.05f ;
		if(g_AlphaBG > 1.0f)
			g_AlphaBG  =1.0f;

		if(g_TimeCount%5 == 0)
		{
			for(int nBlock = 0; nBlock < 4;nBlock++)
			{
				int X = g_BlockGroupX[g_GroupIdx][nBlock];
				int Y = g_BlockGroupY[g_GroupIdx][nBlock];
				g_LoadBlock[Y][X].bUse = true;
			}
			g_GroupIdx++;
		}
		if(g_GroupIdx >= MAX_BLOCK_GROUP)
		{
			g_LeftTime = LOADING_BLOCK_MOVETIME;
			g_FadeLoadPhase = LOADING_BLOCK_RUNING;
			
		}
		break;
	case LOADING_BLOCK_RUNING:
		g_LeftTime--;
		if(g_LeftTime <= 0)
		{
			g_FadeLoadPhase = LOADING_BLOCK_STOP;
			g_LeftTime = LOADING_BLOCK_STOPTIME;
			
			//g_LeftTime = 1000;
		}
		break;
	case LOADING_BLOCK_STOP:
		Set_Main_Update_Type (UPDATE_MODE_SHADOW);
		SetMode(g_modeNext);
		g_LeftTime = LOADING_BLOCK_MOVETIME * 5;
		g_FadeLoadPhase = LOADING_BLOCK_WAIT;
		break;
	case LOADING_BLOCK_WAIT:
		g_LeftTime -- ;
		if(g_LeftTime < 0 )
		{
				g_LeftTime = 0;
				
				if(g_OpenWindow == true)
				{
					g_FadeLoadPhase = LOADING_BLOCK_OPEN;
					g_LeftTime = LOADING_BLOCK_FADE_OUT_TIME;
				}
		}
		break;

	case LOADING_BLOCK_OPEN:
		g_LeftTime--;
		g_AlphaBG -= 1.0f / LOADING_BLOCK_FADE_OUT_TIME;
		for(int nCntBlockY = 0; nCntBlockY < LOADING_BLOCK_COUNT_Y;nCntBlockY ++)
		{
			for(int nCntBlockX = 0; nCntBlockX < LOADING_BLOCK_COUNT_X;nCntBlockX ++)
			{
				if(nCntBlockX<=5)
					g_LoadBlock[nCntBlockY][nCntBlockX].DrawPos.x -=20.0f;
				else
					g_LoadBlock[nCntBlockY][nCntBlockX].DrawPos.x +=20.0f;
			}
		}

		if(g_AlphaBG <= 0)
		{
			g_FadeLoadPhase = LOADING_BLOCK_RESET;
			
		}
		break;
	case LOADING_BLOCK_PHASE_MAX:
		break;
	}

	if(g_FadeLoadPhase == LOADING_BLOCK_START || g_FadeLoadPhase == LOADING_BLOCK_RUNING)
	for(int nCntBlockY = 0; nCntBlockY < LOADING_BLOCK_COUNT_Y;nCntBlockY ++)
	{
		for(int nCntBlockX = 0; nCntBlockX < LOADING_BLOCK_COUNT_X;nCntBlockX ++)
		{
			if(g_LoadBlock[nCntBlockY][nCntBlockX].bUse ==false)
			continue;
			g_LoadBlock[nCntBlockY][nCntBlockX].MoveTime--;
			g_LoadBlock[nCntBlockY][nCntBlockX].DrawPos += g_LoadBlock[nCntBlockY][nCntBlockX].Move;
			if(g_LoadBlock[nCntBlockY][nCntBlockX].MoveTime < 0)
			{
				g_LoadBlock[nCntBlockY][nCntBlockX].DrawPos = g_LoadBlock[nCntBlockY][nCntBlockX].DestPos;
				g_LoadBlock[nCntBlockY][nCntBlockX].MoveTime = 0;
			}

		}
	}

	g_TimeCount = SetMAX(g_TimeCount,60,0,1);
	SetVetTexFade_Loading_Block();
	SetVetAlpha_LoadingBG(g_AlphaBG);
}
//=============================================================================
//　描画処理
//=============================================================================
void DrawFade_loading(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	// 頂点バッファをデバイスのデータストリームにバインド
    pDevice->SetStreamSource(0, g_pVtxBuffFade_BlockBG, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	//pDevice->SetTexture(0, g_pTextureFade_BlockBG);
	pDevice->SetTexture(0, NULL);
	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,0, NUM_POLYGON);


	// 頂点バッファをデバイスのデータストリームにバインド
    pDevice->SetStreamSource(0, g_pVtxBuffFade_Block, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureFade_Block);

	if(g_FadeLoadPhase != LOADING_BLOCK_PHASE_MAX)
	for(int nCntBlockY = 0; nCntBlockY < LOADING_BLOCK_COUNT_Y;nCntBlockY ++)
	{		
		for(int nCntBlockX = 0; nCntBlockX < LOADING_BLOCK_COUNT_X;nCntBlockX ++)
		{
			if(g_LoadBlock[nCntBlockY][nCntBlockX].bUse == false)
				continue;
			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,(((nCntBlockY * LOADING_BLOCK_COUNT_X) + nCntBlockX) * 4), NUM_POLYGON);
		}
	}

}
//=============================================================================
//　頂点座標生成
//=============================================================================
HRESULT MakeVertexFade_Loading(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * LOADING_BLOCK_MAX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
												FVF_VERTEX_2D,				// 使用する頂点フォーマット
												D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
												&g_pVtxBuffFade_Block,			// 頂点バッファインターフェースへのポインタ
												NULL)))						// NULLに設定
	{
        return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffFade_Block->Lock(0, 0, (void**)&pVtx, 0);
	for(int nCntBlockY = 0; nCntBlockY < LOADING_BLOCK_COUNT_Y;nCntBlockY ++)
	{	
		
		for(int nCntBlockX = 0; nCntBlockX < LOADING_BLOCK_COUNT_X;nCntBlockX ++,pVtx += 4)
		{
		// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_LoadBlock[nCntBlockY][nCntBlockX].DrawPos.x - LOADING_BLOCK_SIZE/2,
										g_LoadBlock[nCntBlockY][nCntBlockX].DrawPos.y - LOADING_BLOCK_SIZE/2, 
										0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_LoadBlock[nCntBlockY][nCntBlockX].DrawPos.x +  LOADING_BLOCK_SIZE/2,
										g_LoadBlock[nCntBlockY][nCntBlockX].DrawPos.y - LOADING_BLOCK_SIZE/2, 
										0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_LoadBlock[nCntBlockY][nCntBlockX].DrawPos.x - LOADING_BLOCK_SIZE/2,
										g_LoadBlock[nCntBlockY][nCntBlockX].DrawPos.y + LOADING_BLOCK_SIZE/2, 
										0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_LoadBlock[nCntBlockY][nCntBlockX].DrawPos.x + LOADING_BLOCK_SIZE/2,
										g_LoadBlock[nCntBlockY][nCntBlockX].DrawPos.y + LOADING_BLOCK_SIZE/2,
										0.0f);

			// テクスチャのパースペクティブコレクト用
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
			pVtx[0].tex = D3DXVECTOR2(LOADING_BLOCK_TEX_UNIT * g_LoadBlock[nCntBlockY][nCntBlockX].nColor, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(LOADING_BLOCK_TEX_UNIT * g_LoadBlock[nCntBlockY][nCntBlockX].nColor + LOADING_BLOCK_TEX_UNIT, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(LOADING_BLOCK_TEX_UNIT * g_LoadBlock[nCntBlockY][nCntBlockX].nColor, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(LOADING_BLOCK_TEX_UNIT * g_LoadBlock[nCntBlockY][nCntBlockX].nColor + LOADING_BLOCK_TEX_UNIT, 1.0f);	
		}
	}
		// 頂点データをアンロックする
		g_pVtxBuffFade_Block->Unlock();
	}

	return S_OK;
}
HRESULT MakeVertexFade_LoadingBG(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * 1,	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
												FVF_VERTEX_2D,				// 使用する頂点フォーマット
												D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
												&g_pVtxBuffFade_BlockBG,			// 頂点バッファインターフェースへのポインタ
												NULL)))						// NULLに設定
	{
        return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffFade_BlockBG->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
			pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH,0.0f,0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f,SCREEN_HEIGHT,0.0f);
			pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH,SCREEN_HEIGHT,0.0f);

			// テクスチャのパースペクティブコレクト用
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
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);	

		// 頂点データをアンロックする
		g_pVtxBuffFade_BlockBG->Unlock();
	}

	return S_OK;
}
//=============================================================================
//　頂点座標設定
//=============================================================================
void	SetVetTexFade_Loading_Block(void)
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffFade_Block->Lock(0, 0, (void**)&pVtx, 0);
	for(int nCntBlockY = 0; nCntBlockY < LOADING_BLOCK_COUNT_Y;nCntBlockY ++)
	{	
		
		for(int nCntBlockX = 0; nCntBlockX < LOADING_BLOCK_COUNT_X;nCntBlockX ++,pVtx += 4)
		{
		// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_LoadBlock[nCntBlockY][nCntBlockX].DrawPos.x - LOADING_BLOCK_SIZE/2,
										g_LoadBlock[nCntBlockY][nCntBlockX].DrawPos.y - LOADING_BLOCK_SIZE/2, 
										0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_LoadBlock[nCntBlockY][nCntBlockX].DrawPos.x +  LOADING_BLOCK_SIZE/2,
										g_LoadBlock[nCntBlockY][nCntBlockX].DrawPos.y - LOADING_BLOCK_SIZE/2, 
										0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_LoadBlock[nCntBlockY][nCntBlockX].DrawPos.x - LOADING_BLOCK_SIZE/2,
										g_LoadBlock[nCntBlockY][nCntBlockX].DrawPos.y + LOADING_BLOCK_SIZE/2, 
										0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_LoadBlock[nCntBlockY][nCntBlockX].DrawPos.x + LOADING_BLOCK_SIZE/2,
										g_LoadBlock[nCntBlockY][nCntBlockX].DrawPos.y + LOADING_BLOCK_SIZE/2,
										0.0f);

			// テクスチャのパースペクティブコレクト用
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
			pVtx[0].tex = D3DXVECTOR2(LOADING_BLOCK_TEX_UNIT * g_LoadBlock[nCntBlockY][nCntBlockX].nColor, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(LOADING_BLOCK_TEX_UNIT * g_LoadBlock[nCntBlockY][nCntBlockX].nColor + LOADING_BLOCK_TEX_UNIT, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(LOADING_BLOCK_TEX_UNIT * g_LoadBlock[nCntBlockY][nCntBlockX].nColor, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(LOADING_BLOCK_TEX_UNIT * g_LoadBlock[nCntBlockY][nCntBlockX].nColor + LOADING_BLOCK_TEX_UNIT, 1.0f);	
		}
	}
		// 頂点データをアンロックする
		g_pVtxBuffFade_Block->Unlock();
	}
}
void	SetVetAlpha_LoadingBG(float ValueA)
{//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffFade_BlockBG->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, ValueA);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, ValueA);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, ValueA);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, ValueA);


	// 頂点データをアンロックする
	g_pVtxBuffFade_BlockBG->Unlock();
}

