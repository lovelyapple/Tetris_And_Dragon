//=============================================================================
//[Battle_Effect_Alert.cpp]
// HAL 名古屋　GP11A172 17 50323 リ リン
//=============================================================================

#include "Battle_Boss_Alert_Effect.h"
#include "main.h"
#include "game.h"
#include "Function.h"
#include "Battle_Main.h"
#include "input.h"
#include "Function.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

#define TEXTURE_BATTLE_ALERT_EYE	("data/TEXTURE/BossAlertEye.png")
#define TEXTURE_BATTLE_ALERT_OUTLINE	("data/TEXTURE/BossAlertOutLine.png")


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexBattle_Alert_Eye(LPDIRECT3DDEVICE9 pDevice);
HRESULT MakeVertexBattle_Alert_Line(LPDIRECT3DDEVICE9 pDevice);
void SetVertexBattle_Alert_Eye(int ID);
void SetVertexBattle_Alert_Line(int ID);
void InitBattle_Alert_Eye(int ID);
void InitBattle_Alert_Line(int ID);

HRESULT MakeVertexBattle_Alert_Fade(LPDIRECT3DDEVICE9 pDevice);
void	SetVertexBattle_Alert_Fade(void);
//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureBattle_Alert_Eye = NULL;	//テクスチャメモリ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBattle_Alert_Eye = NULL;	//頂点座標バッファ
LPDIRECT3DTEXTURE9		g_pTextureBattle_Alert_Line = NULL;	//テクスチャメモリ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBattle_Alert_Line = NULL;	//頂点座標バッファ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBattle_Alert_Fade = NULL;	//頂点座標バッファ

typedef struct
{
	bool	bUse;
	D3DXVECTOR3	pos;
	//CHEERS_PHASE		g_phase;
	int		Type;
	int		nLife;
	float	fSizeX;
	float	fSizeY;
	float	fAlpha;
}BOSS_ALERT_EYE;
typedef struct
{
	bool	bUse;
	D3DXVECTOR3	pos;
	float VerTexPos;
	float Alpha;
}BOSS_ALERT_LINE;
BOSS_ALERT_EYE g_Alert_Eye[BATTLE_BOSS_ALERT_EYE_MAX];
BOSS_ALERT_LINE	g_Alert_Line[BATTLE_BOSS_ALERT_LINE_MAX];
ALERT_PHASE	g_AlertPhase = ALERT_PHASE_MAX;
int				g_AlertTime;
float			g_BG_Alpha;
float			g_BG_Sin;
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBattle_Effect_Alert(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	g_AlertPhase = ALERT_PHASE_MAX;
	g_AlertTime	= ATTLE_BOSS_ALERT_START_TIME;
	g_BG_Alpha	= 0.0f;
	g_BG_Sin	= 0.0f;
	for (int nCntAlert_Eye = 0; nCntAlert_Eye < BATTLE_BOSS_ALERT_EYE_MAX;nCntAlert_Eye++)
		InitBattle_Alert_Eye(nCntAlert_Eye);
	for (int nCntAlert_Line = 0; nCntAlert_Line < BATTLE_BOSS_ALERT_LINE_MAX;nCntAlert_Line++)
		InitBattle_Alert_Line(nCntAlert_Line);

	D3DXCreateTextureFromFile(pDevice,
							TEXTURE_BATTLE_ALERT_EYE,
							&g_pTextureBattle_Alert_Eye);
	D3DXCreateTextureFromFile(pDevice,
							TEXTURE_BATTLE_ALERT_OUTLINE,
							&g_pTextureBattle_Alert_Line);

	MakeVertexBattle_Alert_Eye(pDevice);
	MakeVertexBattle_Alert_Line(pDevice);
	MakeVertexBattle_Alert_Fade(pDevice);
	return S_OK;
}

void InitBattle_Alert_Line(int ID)
{
	g_Alert_Line[ID].Alpha = 1.0f;
	g_Alert_Line[ID].bUse = false;
	g_Alert_Line[ID].VerTexPos = 0.0f;
	g_Alert_Line[0].pos	= D3DXVECTOR3(BATTLE_BOSS_ALERT_LINE_POS_X,BATTLE_BOSS_ALERT_LINE_POS_Y,0.0f);
	g_Alert_Line[1].pos	= D3DXVECTOR3(BATTLE_BOSS_ALERT_LINE_POS_X,BATTLE_TEAM_POS_Y - BATTLE_TEAM_SIZE*2,0.0f);
}

void InitBattle_Alert_Eye(int ID)
{

	g_Alert_Eye[ID].bUse	= false;
	g_Alert_Eye[ID].pos		= D3DXVECTOR3(BATTLE_BOSS_ALERT_EYE_POS_X,BATTLE_BOSS_ALERT_EYE_POS_Y,0.0f);
	g_Alert_Eye[ID].nLife	= BATTLE_BOSS_ALERT_EYE_LIFE + ID * 5;
	g_Alert_Eye[ID].Type	= 0;
	g_Alert_Eye[ID].fAlpha	= 0.0f;
	//g_Alert_Eye[ID].g_phase	= CHEERS_PHASE_MAX;
	g_Alert_Eye[ID].fSizeX	= 1.0f;
	g_Alert_Eye[ID].fSizeY	= 1.0f;
	g_Alert_Line[0].Alpha	= 0.0f;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBattle_Effect_Alert(void)
{
	if(g_pTextureBattle_Alert_Eye != NULL)
	{// テクスチャの開放
		g_pTextureBattle_Alert_Eye->Release();
		g_pTextureBattle_Alert_Eye = NULL;
	}

	if(g_pVtxBuffBattle_Alert_Eye != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffBattle_Alert_Eye->Release();
		g_pVtxBuffBattle_Alert_Eye = NULL;
	}

	if(g_pTextureBattle_Alert_Line != NULL)
	{// テクスチャの開放
		g_pTextureBattle_Alert_Line->Release();
		g_pTextureBattle_Alert_Line = NULL;
	}

	if(g_pVtxBuffBattle_Alert_Line != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffBattle_Alert_Line->Release();
		g_pVtxBuffBattle_Alert_Line = NULL;
	}

	if(g_pVtxBuffBattle_Alert_Fade != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffBattle_Alert_Fade->Release();
		g_pVtxBuffBattle_Alert_Fade = NULL;
	}
}
	

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBattle_Effect_Alert(void)
{
	//ALERT_PHASE_START = 0,
	//ALERT_PHASE_EYE_SHINE,
	//ALERT_PHASE_OUT,
	//ALERT_PHASE_RESET,
	//ALERT_PHASE_MAX,
	g_Alert_Line[0].VerTexPos += 0.01f;
	g_Alert_Line[1].VerTexPos -= 0.01f;

	g_Alert_Line[1].VerTexPos = SetMAX(g_Alert_Line[1].VerTexPos,10.0f,-10.0f,1);
	g_Alert_Line[1].VerTexPos = SetMAX(g_Alert_Line[0].VerTexPos,10.0f,-10.0f,1);
	switch(g_AlertPhase)
	{
	case ALERT_PHASE_START:
		g_AlertTime --;
		g_Alert_Line[0].bUse = true;
		g_Alert_Line[1].bUse = true;
		g_Alert_Eye[0].bUse = true;
		g_Alert_Eye[0].fAlpha += 1.0f / (float)ATTLE_BOSS_ALERT_START_TIME;
		if(g_AlertTime < 0)
		{	
			g_Alert_Eye[0].fAlpha = 1.0f;
			g_AlertTime = 60;
			g_AlertPhase = ALERT_PHASE_EYE_SHINE;
		}
		break;
	case ALERT_PHASE_EYE_SHINE:
		g_AlertTime --;
		g_Alert_Line[0].Alpha -= 1.0f / (float)ATTLE_BOSS_ALERT_START_TIME;

		if(g_AlertTime < 0)
		{
			g_AlertTime = 120;
			g_AlertPhase = ALERT_PHASE_OUT;
			for (int nCntAlert_Eye = 0; nCntAlert_Eye < BATTLE_BOSS_ALERT_EYE_MAX;nCntAlert_Eye++)
				g_Alert_Eye[nCntAlert_Eye].bUse = true;
		}
		break;
	case ALERT_PHASE_OUT:
		g_AlertTime --;
		if(g_AlertTime < 0)
		{
			g_AlertTime = 60;
			g_AlertPhase = ALERT_PHASE_RESET;
		}
		break;
	case ALERT_PHASE_RESET:
		for (int nCntAlert_Eye = 0; nCntAlert_Eye < BATTLE_BOSS_ALERT_EYE_MAX;nCntAlert_Eye++)
			InitBattle_Alert_Eye(nCntAlert_Eye);
		for (int nCntAlert_Line = 0; nCntAlert_Line < BATTLE_BOSS_ALERT_LINE_MAX;nCntAlert_Line++)
			InitBattle_Alert_Line(nCntAlert_Line);
		g_BG_Sin = 0.0f;
		g_BG_Alpha = 1.0f;
		g_AlertPhase = ALERT_PHASE_MAX;
		break;
	case ALERT_PHASE_MAX:
		break;
	}

	for (int nCntAlert_Eye = 1; nCntAlert_Eye < BATTLE_BOSS_ALERT_EYE_MAX;nCntAlert_Eye++)
	{
		if(g_Alert_Eye[nCntAlert_Eye].bUse == false)
			continue;
		g_Alert_Eye[nCntAlert_Eye].nLife -- ;
		if(g_Alert_Eye[nCntAlert_Eye].nLife < 0)
		{
			g_Alert_Eye[nCntAlert_Eye].fAlpha += 1.0f / (60.0f*2) ;
			g_Alert_Eye[nCntAlert_Eye].fSizeX += 1.0f / 60.0f ;
			g_Alert_Eye[nCntAlert_Eye].nLife = 0;
		}
		SetVertexBattle_Alert_Eye(nCntAlert_Eye);
	}
	SetVertexBattle_Alert_Eye(0);
	for (int nCntAlert_Line = 0; nCntAlert_Line < BATTLE_BOSS_ALERT_LINE_MAX;nCntAlert_Line++)
		SetVertexBattle_Alert_Line(nCntAlert_Line);

	if(g_AlertPhase != ALERT_PHASE_MAX)
	{
		g_BG_Sin += 0.045f;
		g_BG_Alpha = (sinf(g_BG_Sin) + 1.0f)* 0.5f;
	}
	else
	 g_BG_Alpha = 0.0f;
	SetVertexBattle_Alert_Fade();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBattle_Effect_Alert(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffBattle_Alert_Fade,0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetTexture(0,NULL);
	if(g_BG_Alpha !=0)
		// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);


	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffBattle_Alert_Eye,0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetTexture(0,g_pTextureBattle_Alert_Eye);

	for (int nCntAlert_Eye = 0; nCntAlert_Eye < BATTLE_BOSS_ALERT_EYE_MAX;nCntAlert_Eye++)
	{
		if(g_Alert_Eye[nCntAlert_Eye].bUse == false)
			continue;
		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntAlert_Eye * 4, NUM_POLYGON);
	}



	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffBattle_Alert_Line,0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetTexture(0,g_pTextureBattle_Alert_Line);

	for (int nCntAlert_Line = 0; nCntAlert_Line < BATTLE_BOSS_ALERT_LINE_MAX;nCntAlert_Line++)
	{
		if(g_Alert_Line[nCntAlert_Line].bUse == false)
			continue;
		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntAlert_Line * 4, NUM_POLYGON);
	}
}
HRESULT MakeVertexBattle_Alert_Fade(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * 1,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
		FVF_VERTEX_2D,						// 使用する頂点フォーマット
		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
		&g_pVtxBuffBattle_Alert_Fade,					// 頂点バッファインターフェースへのポインタ
		NULL)))								// NULLに設定
	{
		return E_FAIL;
	}
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffBattle_Alert_Fade->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定/ 
		pVtx[0].pos.x = SYS_BATTLE_POS_START_X;
		pVtx[0].pos.y = 0.0f;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = SYS_BATTLE_POS_START_X + SYS_BATTLE_SIZE_X;
		pVtx[1].pos.y = 0.0f;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = SYS_BATTLE_POS_START_X;
		pVtx[2].pos.y = SCREEN_HEIGHT;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = SYS_BATTLE_POS_START_X + SYS_BATTLE_SIZE_X;
		pVtx[3].pos.y = SCREEN_HEIGHT;
		pVtx[3].pos.z = 0.0f;
		// rhwの設定
		pVtx[0].rhw =
			pVtx[1].rhw =
			pVtx[2].rhw =
			pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

		pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

		g_pVtxBuffBattle_Alert_Fade->Unlock();
	}
	return S_OK;

}

void	SetVertexBattle_Alert_Fade(void)
{
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffBattle_Alert_Fade->Lock(0, 0, (void**)&pVtx, 0);
	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, g_BG_Alpha);
	pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, g_BG_Alpha);
	pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, g_BG_Alpha);
	pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, g_BG_Alpha);

	g_pVtxBuffBattle_Alert_Fade->Unlock();
}

//=============================================================================
// Shining頂点の作成
//=============================================================================
HRESULT MakeVertexBattle_Alert_Eye(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * BATTLE_BOSS_ALERT_EYE_MAX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
		FVF_VERTEX_2D,						// 使用する頂点フォーマット
		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
		&g_pVtxBuffBattle_Alert_Eye,					// 頂点バッファインターフェースへのポインタ
		NULL)))								// NULLに設定
	{
		return E_FAIL;
	}
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffBattle_Alert_Eye->Lock(0, 0, (void**)&pVtx, 0);
			// 頂点座標の設定
		for (int nCntAlert_Eye = 0; nCntAlert_Eye < BATTLE_BOSS_ALERT_EYE_MAX;nCntAlert_Eye++,pVtx += 4)
		{
			//=======================LeftSide
			pVtx[0].pos.x = g_Alert_Eye[nCntAlert_Eye].pos.x - BATTLE_BOSS_ALERT_EYE_SIZE_X * g_Alert_Eye[nCntAlert_Eye].fSizeX;
			pVtx[0].pos.y = g_Alert_Eye[nCntAlert_Eye].pos.y - BATTLE_BOSS_ALERT_EYE_SIZE_Y * g_Alert_Eye[nCntAlert_Eye].fSizeY;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_Alert_Eye[nCntAlert_Eye].pos.x + BATTLE_BOSS_ALERT_EYE_SIZE_X * g_Alert_Eye[nCntAlert_Eye].fSizeX;
			pVtx[1].pos.y = g_Alert_Eye[nCntAlert_Eye].pos.y - BATTLE_BOSS_ALERT_EYE_SIZE_Y * g_Alert_Eye[nCntAlert_Eye].fSizeY;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_Alert_Eye[nCntAlert_Eye].pos.x - BATTLE_BOSS_ALERT_EYE_SIZE_X * g_Alert_Eye[nCntAlert_Eye].fSizeX;
			pVtx[2].pos.y =	g_Alert_Eye[nCntAlert_Eye].pos.y + BATTLE_BOSS_ALERT_EYE_SIZE_Y * g_Alert_Eye[nCntAlert_Eye].fSizeY;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_Alert_Eye[nCntAlert_Eye].pos.x + BATTLE_BOSS_ALERT_EYE_SIZE_X * g_Alert_Eye[nCntAlert_Eye].fSizeX;
			pVtx[3].pos.y = g_Alert_Eye[nCntAlert_Eye].pos.y + BATTLE_BOSS_ALERT_EYE_SIZE_Y * g_Alert_Eye[nCntAlert_Eye].fSizeY;
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
		g_pVtxBuffBattle_Alert_Eye->Unlock();
	}
	return S_OK;

}
void SetVertexBattle_Alert_Eye(int ID)
{//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffBattle_Alert_Eye->Lock(0, 0, (void**)&pVtx, 0);
		// 頂点座標の設定
	pVtx += ID * 4;
		//=======================LeftSide
			pVtx[0].pos.x = BATTLE_BOSS_ALERT_EYE_POS_X - BATTLE_BOSS_ALERT_EYE_SIZE_X * g_Alert_Eye[ID].fSizeX;
			pVtx[0].pos.y = BATTLE_BOSS_ALERT_EYE_POS_Y - BATTLE_BOSS_ALERT_EYE_SIZE_Y * g_Alert_Eye[ID].fSizeX;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = BATTLE_BOSS_ALERT_EYE_POS_X + BATTLE_BOSS_ALERT_EYE_SIZE_X * g_Alert_Eye[ID].fSizeX;
			pVtx[1].pos.y = BATTLE_BOSS_ALERT_EYE_POS_Y - BATTLE_BOSS_ALERT_EYE_SIZE_Y * g_Alert_Eye[ID].fSizeX;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = BATTLE_BOSS_ALERT_EYE_POS_X - BATTLE_BOSS_ALERT_EYE_SIZE_X * g_Alert_Eye[ID].fSizeX;
			pVtx[2].pos.y =	BATTLE_BOSS_ALERT_EYE_POS_Y + BATTLE_BOSS_ALERT_EYE_SIZE_Y * g_Alert_Eye[ID].fSizeX;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = BATTLE_BOSS_ALERT_EYE_POS_X + BATTLE_BOSS_ALERT_EYE_SIZE_X * g_Alert_Eye[ID].fSizeX;
			pVtx[3].pos.y = BATTLE_BOSS_ALERT_EYE_POS_Y + BATTLE_BOSS_ALERT_EYE_SIZE_Y * g_Alert_Eye[ID].fSizeX;
			pVtx[3].pos.z = 0.0f;

			// rhwの設定
			pVtx[0].rhw =
			pVtx[1].rhw =
			pVtx[2].rhw =
			pVtx[3].rhw = 1.0f;

			// 頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Alert_Eye[ID].fAlpha);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Alert_Eye[ID].fAlpha);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Alert_Eye[ID].fAlpha);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Alert_Eye[ID].fAlpha);

			pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

	g_pVtxBuffBattle_Alert_Eye->Unlock();
}
//=============================================================================
// Shining頂点の作成
//=============================================================================
HRESULT MakeVertexBattle_Alert_Line(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * BATTLE_BOSS_ALERT_LINE_MAX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
		FVF_VERTEX_2D,						// 使用する頂点フォーマット
		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
		&g_pVtxBuffBattle_Alert_Line,					// 頂点バッファインターフェースへのポインタ
		NULL)))								// NULLに設定
	{
		return E_FAIL;
	}
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffBattle_Alert_Line->Lock(0, 0, (void**)&pVtx, 0);
			// 頂点座標の設定
		for (int nCntAlert_Line = 0; nCntAlert_Line < BATTLE_BOSS_ALERT_LINE_MAX;nCntAlert_Line++,pVtx += 4)
		{
			//=======================LeftSide
			pVtx[0].pos.x = g_Alert_Line[nCntAlert_Line].pos.x - BATTLE_BOSS_ALERT_LINE_SIZE_X;
			pVtx[0].pos.y = g_Alert_Line[nCntAlert_Line].pos.y - BATTLE_BOSS_ALERT_LINE_SIZE_Y;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_Alert_Line[nCntAlert_Line].pos.x + BATTLE_BOSS_ALERT_LINE_SIZE_X;
			pVtx[1].pos.y = g_Alert_Line[nCntAlert_Line].pos.y - BATTLE_BOSS_ALERT_LINE_SIZE_Y;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_Alert_Line[nCntAlert_Line].pos.x - BATTLE_BOSS_ALERT_LINE_SIZE_X ;
			pVtx[2].pos.y =	g_Alert_Line[nCntAlert_Line].pos.y + BATTLE_BOSS_ALERT_LINE_SIZE_Y;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_Alert_Line[nCntAlert_Line].pos.x + BATTLE_BOSS_ALERT_LINE_SIZE_X ;
			pVtx[3].pos.y = g_Alert_Line[nCntAlert_Line].pos.y + BATTLE_BOSS_ALERT_LINE_SIZE_Y;
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
		g_pVtxBuffBattle_Alert_Line->Unlock();
	}
	return S_OK;

}
void SetVertexBattle_Alert_Line(int ID)
{//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffBattle_Alert_Line->Lock(0, 0, (void**)&pVtx, 0);
		// 頂点座標の設定
	pVtx += ID * 4;
		//=======================LeftSide
			pVtx[0].pos.x = g_Alert_Line[ID].pos.x - BATTLE_BOSS_ALERT_LINE_SIZE_X;
			pVtx[0].pos.y = g_Alert_Line[ID].pos.y - BATTLE_BOSS_ALERT_LINE_SIZE_Y;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_Alert_Line[ID].pos.x + BATTLE_BOSS_ALERT_LINE_SIZE_X;
			pVtx[1].pos.y = g_Alert_Line[ID].pos.y - BATTLE_BOSS_ALERT_LINE_SIZE_Y;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_Alert_Line[ID].pos.x - BATTLE_BOSS_ALERT_LINE_SIZE_X;
			pVtx[2].pos.y =	g_Alert_Line[ID].pos.y + BATTLE_BOSS_ALERT_LINE_SIZE_Y;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_Alert_Line[ID].pos.x + BATTLE_BOSS_ALERT_LINE_SIZE_X;
			pVtx[3].pos.y = g_Alert_Line[ID].pos.y + BATTLE_BOSS_ALERT_LINE_SIZE_Y;
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

			pVtx[0].tex = D3DXVECTOR2(0.0f + g_Alert_Line[ID].VerTexPos,0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f + g_Alert_Line[ID].VerTexPos,0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + g_Alert_Line[ID].VerTexPos,1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f + g_Alert_Line[ID].VerTexPos,1.0f);
	g_pVtxBuffBattle_Alert_Line->Unlock();
}
ALERT_PHASE GetBOSS_Alert_Phase(void)
{
	return g_AlertPhase;
}
void	SetBossAlertStart(void)
{
	g_AlertPhase = ALERT_PHASE_START;
}
