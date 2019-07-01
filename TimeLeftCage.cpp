//=============================================================================
// テトリスのメイン処理[TimeLeftCage.cpp]
//=============================================================================
#include "TimeLeftCage.h"
#include "main.h"
#include "input.h"
#include "Function.h"
#include "effect_star.h"
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexMoveTimeLeft(LPDIRECT3DDEVICE9 pDevice);
HRESULT MakeVertexMoveTimeLeftCage(LPDIRECT3DDEVICE9 pDevice);
void SetVerTexMoveTimeLeft(int VerTex);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureMoveTimeLeft	= NULL;//テクスチャメモリ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMoveTimeLeft = NULL;//頂点座標バッファ
LPDIRECT3DTEXTURE9		g_pTextureMoveTimeLeftCage	= NULL;//テクスチャメモリ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMoveTimeLeftCage = NULL;//頂点座標バッファ
 
float g_TimeCageLenght;
int	  g_VerTexPosX;
float g_VerTexPosY;
float g_VerColCageBG;

MOVETIME g_aMoveTime;
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTimeLeft(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_VerTexPosX = 0;
	g_VerTexPosY = 0.5f;
	g_VerColCageBG = 0.0f;

	g_aMoveTime.fMAX_Move_Time	= VALUE_DEFAULT_MOVE_TIME;
	g_aMoveTime.fMoveTimeLeft	= 0.0f;
	g_aMoveTime.nType			= TIME_MOVE_KEEPING;
	g_aMoveTime.VecTime			= 0.0f;

	D3DXCreateTextureFromFile(pDevice,
		TEXTURE_MOVETIME,
		&g_pTextureMoveTimeLeft);
	MakeVertexMoveTimeLeft(pDevice);

	D3DXCreateTextureFromFile(pDevice,
		TEXTURE_MOVETIME_CAGE,
		&g_pTextureMoveTimeLeftCage);
	MakeVertexMoveTimeLeftCage(pDevice);
	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void UninitTimeLeft(void)
{
	if(g_pTextureMoveTimeLeft != NULL)
	{// テクスチャの開放
		g_pTextureMoveTimeLeft->Release();
		g_pTextureMoveTimeLeft = NULL;
	}
	if(g_pVtxBuffMoveTimeLeft != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffMoveTimeLeft->Release();
		g_pVtxBuffMoveTimeLeft = NULL;
	}
	if(g_pTextureMoveTimeLeftCage != NULL)
	{// テクスチャの開放
		g_pTextureMoveTimeLeftCage->Release();
		g_pTextureMoveTimeLeftCage = NULL;
	}
	if(g_pVtxBuffMoveTimeLeftCage != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffMoveTimeLeftCage->Release();
		g_pVtxBuffMoveTimeLeftCage = NULL;
	}
}
//=============================================================================
// 移動時間設定
//=============================================================================
void SetMoveTime(int Type,float MaxTimeLeft)
{
	MOVETIME *pMoveTime;
	pMoveTime = GetMoveTimeLeft();
	pMoveTime->nType = Type;
	pMoveTime->VecTime = (MaxTimeLeft - pMoveTime->fMoveTimeLeft) / VALUE_RECOVER_TIME;
	pMoveTime->fMAX_Move_Time = MaxTimeLeft;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTimeLeft(void)
{
	MOVETIME *pMoveTime;
	pMoveTime = GetMoveTimeLeft();

	SetVerTexMoveTimeLeft(g_VerTexPosX);


	float VecTime = pMoveTime->VecTime;

	switch (pMoveTime->nType)
	{
	case TIME_MOVE_KEEPING:break;
	case TIME_MOVE_FULL:break;
	case TIME_MOVE_RECOVER:
		if(pMoveTime->fMoveTimeLeft < pMoveTime->fMAX_Move_Time)
			pMoveTime->fMoveTimeLeft += VecTime;
		if(pMoveTime->fMoveTimeLeft >= pMoveTime->fMAX_Move_Time)
		{
			D3DXVECTOR3 pos;
			pos.x = TIMELEFT_POS_X + TIMELEFT_SIZE_X/2;
			pos.y = TIMELEFT_POS_Y + TIMELEFT_SIZE_Y;
			pos.z = 0.0f;
			SetEffect_Star(pos,BLOCK_GREEN,0);
			pMoveTime->nType = TIME_MOVE_FULL;
		}
		break;
	case TIME_MOVE_LOSING:
		if(pMoveTime->fMoveTimeLeft > 0.0f)
			pMoveTime->fMoveTimeLeft -= 1.0f;
		if(pMoveTime->fMoveTimeLeft <= 0.0f)
			pMoveTime->nType = TIME_MOVE_KEEPING;
		break;
	}
	pMoveTime->fMoveTimeLeft = SetMAX(pMoveTime->fMoveTimeLeft,VALUE_DEFAULT_MOVE_TIME,0.0f,2);

	if(pMoveTime->fMoveTimeLeft < VALUE_DEFAULT_MOVE_TIME * (1.0f/3.0f))
	{
		g_VerTexPosX = 0;	
	}
	if(pMoveTime->fMoveTimeLeft >= VALUE_DEFAULT_MOVE_TIME * (1.0f/3.0f) && pMoveTime->fMoveTimeLeft < VALUE_DEFAULT_MOVE_TIME * (2.0f/3.0f))
		g_VerTexPosX = 1;
	if(pMoveTime->fMoveTimeLeft >= VALUE_DEFAULT_MOVE_TIME * (2.0f/3.0f))
		g_VerTexPosX = 2;
	if (pMoveTime->fMoveTimeLeft < VALUE_DEFAULT_MOVE_TIME * 0.15f)
	{
		if(rand()%2 == 0)
		{
			g_VerTexPosY = 0.5f;g_VerColCageBG = 0.5f;
		}
		else
		{
			g_VerTexPosY = 0.0f;g_VerColCageBG = 0.0f;
		}
	}
	else
	{
		g_VerTexPosY = 0.0f;g_VerColCageBG=0.0f;
	}

	
}
int GetTimeLeft(void)
{
	return g_aMoveTime.fMoveTimeLeft;
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTimeLeft(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffMoveTimeLeft, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定(空部分)
	pDevice->SetTexture(0, NULL);

	// ポリゴンの描画(空部分)
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);


	// テクスチャの設定(輪郭
	pDevice->SetTexture(0,g_pTextureMoveTimeLeft);

	// ポリゴンの描画(輪郭)
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,4, NUM_POLYGON);


	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffMoveTimeLeftCage, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetTexture(0,g_pTextureMoveTimeLeftCage);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);


}
//=============================================================================
// MoveTimeLeft頂点の作成
//=============================================================================
HRESULT MakeVertexMoveTimeLeft(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * 2,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
		FVF_VERTEX_2D,						// 使用する頂点フォーマット
		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
		&g_pVtxBuffMoveTimeLeft,					// 頂点バッファインターフェースへのポインタ
		NULL)))								// NULLに設定
	{
		return E_FAIL;
	}
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;


		//頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffMoveTimeLeft->Lock(0, 0, (void**)&pVtx, 0);

		pVtx[0].pos.x = TIMELEFT_POS_ENTITY_X;
		pVtx[0].pos.y = TIMELEFT_POS_ENTITY_Y;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = TIMELEFT_POS_ENTITY_X + TIMELEFT_SIZE_ENTITY_X;
		pVtx[1].pos.y = TIMELEFT_POS_ENTITY_Y;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = TIMELEFT_POS_ENTITY_X;
		pVtx[2].pos.y = TIMELEFT_POS_ENTITY_Y + TIMELEFT_SIZE_ENTITY_Y;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = TIMELEFT_POS_ENTITY_X + TIMELEFT_SIZE_ENTITY_X;
		pVtx[3].pos.y = TIMELEFT_POS_ENTITY_Y + TIMELEFT_SIZE_ENTITY_Y;
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
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
		// 頂点座標の設定
		pVtx[0].pos.x = TIMELEFT_POS_ENTITY_X;
		pVtx[0].pos.y = TIMELEFT_POS_ENTITY_Y;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = TIMELEFT_POS_ENTITY_X + TIMELEFT_SIZE_ENTITY_X;
		pVtx[1].pos.y = TIMELEFT_POS_ENTITY_Y;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = TIMELEFT_POS_ENTITY_X;
		pVtx[2].pos.y = TIMELEFT_POS_ENTITY_Y + TIMELEFT_SIZE_ENTITY_Y;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = TIMELEFT_POS_ENTITY_X + TIMELEFT_SIZE_ENTITY_X;
		pVtx[3].pos.y = TIMELEFT_POS_ENTITY_Y + TIMELEFT_SIZE_ENTITY_Y;
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
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		// 頂点データをアンロックする
		g_pVtxBuffMoveTimeLeft->Unlock();	
	}
	return S_OK;
}
HRESULT MakeVertexMoveTimeLeftCage(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * 1,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
		FVF_VERTEX_2D,						// 使用する頂点フォーマット
		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
		&g_pVtxBuffMoveTimeLeftCage,		// 頂点バッファインターフェースへのポインタ
		NULL)))								// NULLに設定
	{
		return E_FAIL;
	}
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;
		//頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffMoveTimeLeftCage->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].pos.x = TIMELEFT_POS_X;
		pVtx[0].pos.y = TIMELEFT_POS_Y;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = TIMELEFT_POS_X + TIMELEFT_SIZE_X;
		pVtx[1].pos.y = TIMELEFT_POS_Y;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = TIMELEFT_POS_X;
		pVtx[2].pos.y = TIMELEFT_POS_Y+ TIMELEFT_SIZE_Y;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = TIMELEFT_POS_X + TIMELEFT_SIZE_X;
		pVtx[3].pos.y = TIMELEFT_POS_Y + TIMELEFT_SIZE_Y;
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
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		// 頂点データをアンロックする
		g_pVtxBuffMoveTimeLeftCage->Unlock();	
	}
	return S_OK;
}
//=============================================================================
// MoveTimeLeftの設定
//=============================================================================
void SetVerTexMoveTimeLeft(int VerTex)
{
	MOVETIME *pMoveTime;
	pMoveTime = GetMoveTimeLeft();

	//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;
	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffMoveTimeLeft->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(g_VerColCageBG, g_VerColCageBG, g_VerColCageBG, 1.0f);
		pVtx[1].col = D3DXCOLOR(g_VerColCageBG, g_VerColCageBG, g_VerColCageBG, 1.0f);
		pVtx[2].col = D3DXCOLOR(g_VerColCageBG, g_VerColCageBG, g_VerColCageBG, 1.0f);
		pVtx[3].col = D3DXCOLOR(g_VerColCageBG, g_VerColCageBG, g_VerColCageBG, 1.0f);

		pVtx += 4;

	pVtx[0].pos.x = TIMELEFT_POS_ENTITY_X;
	pVtx[0].pos.y = TIMELEFT_POS_ENTITY_Y;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = TIMELEFT_POS_ENTITY_X + TIMELEFT_SIZE_ENTITY_X;
	pVtx[1].pos.y = TIMELEFT_POS_ENTITY_Y;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = TIMELEFT_POS_ENTITY_X;
	pVtx[2].pos.y = TIMELEFT_POS_ENTITY_Y + pMoveTime->fMoveTimeLeft * VALUE_HEIHT_PER_TIME;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = TIMELEFT_POS_ENTITY_X + TIMELEFT_SIZE_ENTITY_X;
	pVtx[3].pos.y = TIMELEFT_POS_ENTITY_Y + pMoveTime->fMoveTimeLeft * VALUE_HEIHT_PER_TIME;
	pVtx[3].pos.z = 0.0f;

	pVtx[0].tex = D3DXVECTOR2(VerTex * (1.0f/3.0f),				g_VerTexPosY);
	pVtx[1].tex = D3DXVECTOR2(VerTex * (1.0f/3.0f)+(1.0f/3.0f), g_VerTexPosY);
	pVtx[2].tex = D3DXVECTOR2(VerTex * (1.0f/3.0f),				g_VerTexPosY+0.5f);
	pVtx[3].tex = D3DXVECTOR2(VerTex * (1.0f/3.0f)+(1.0f/3.0f), g_VerTexPosY+0.5f);

	// 頂点データをアンロックする
	g_pVtxBuffMoveTimeLeft->Unlock();

}
//===========================================================
//時間の取得
//===========================================================
MOVETIME *GetMoveTimeLeft(void)
{ 
	return &g_aMoveTime;
}
