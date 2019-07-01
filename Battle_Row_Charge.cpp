
//=============================================================================
// ブロック破壊効果 [RowCharge.cpp]
// HAL 名古屋　GP11A172 17 50323 リ リン
//=============================================================================
#include "Battle_Row_Charge.h"
#include "Tetris_main.h"
#include "main.h"
#include "Function.h"
#include "game.h"
#include "input.h"
//*****************************************************************************
//使用注意
//*****************************************************************************
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_ROW_CHARGE ("data/TEXTURE/Rowcharge.png")
#define	ROW_CHARGE_POS_X			(SYS_BATTLE_POS_START_X+90.0f)
#define ROW_CHARGE_POS_Y			(0.0f)
#define ROW_CHARGE_SIZE				(20.0f)
#define ROW_CHARGE_ENTITY_POS		(ROW_CHARGE_POS_X + ROW_CHARGE_SIZE)
#define ROW_CHARGE_ENTITY_SIZE		(280.0f)
#define ROW_CHARGE_RIGHT_POS		(ROW_CHARGE_ENTITY_POS + ROW_CHARGE_ENTITY_SIZE)
#define TEXRURE_CHARGE_UNIT			(1.0f/3.0f)
#define ROW_TEXTURE_PER_VALUE		(ROW_CHARGE_ENTITY_SIZE/(float)ROW_CHARGE_MAX_VALUE)
#define ROW_CHARGE_CHANGE_TIME		(60)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexRowCharge(LPDIRECT3DDEVICE9 pDevice);
void SetVerTexTureRowCharge(void);
//*****************************************************************************
// 構造体宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureRowCharge	= NULL;	//テクスチャメモリ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRowCharge = NULL;	//頂点座標バッファ
float		g_RowRec_Value;
float		g_RowDest_Value;
float		g_RowBuff_Value;
float		g_RowTime;
float	g_RowValue_Per;
int		g_RowPhase;
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitRowCharge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	g_RowRec_Value = 0;
	g_RowDest_Value = 0;
	g_RowBuff_Value = 0;
	g_RowValue_Per = 0;
	g_RowTime = ROW_CHARGE_CHANGE_TIME;
	g_RowPhase = ROW_CHARGE_RESET;

	D3DXCreateTextureFromFile(pDevice,
							TEXTURE_ROW_CHARGE,
							&g_pTextureRowCharge);

	MakeVertexRowCharge(pDevice);
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitRowCharge(void)
{
	if(g_pTextureRowCharge != NULL)
	{// テクスチャの開放
		g_pTextureRowCharge->Release();
		g_pTextureRowCharge = NULL;
	}

	if(g_pVtxBuffRowCharge != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffRowCharge->Release();
		g_pVtxBuffRowCharge = NULL;
	}
}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateRowCharge(void)
{

	switch(g_RowPhase)
	{
	case ROW_CHARGE_KEEP:
		g_RowBuff_Value = 0;
		g_RowValue_Per = 0;
		g_RowTime = ROW_CHARGE_CHANGE_TIME;
		g_RowPhase = ROW_CHARGE_MAX_TYPE;
		break;
	case ROW_CHARGE_RESET:
		g_RowTime --;
		g_RowValue_Per = ROW_VALUE_RESET;
		g_RowRec_Value -= g_RowValue_Per;
		g_RowDest_Value = 0;
		if(g_RowRec_Value <= 0)
		{
			g_RowRec_Value = 0;
			g_RowPhase = ROW_CHARGE_KEEP;
		}
		break;
	case ROW_CHARGE_INCREASING:
		g_RowTime --;
		g_RowRec_Value += g_RowValue_Per;
		if(g_RowTime<=0)
		{
			g_RowRec_Value = g_RowDest_Value;
			g_RowPhase = ROW_CHARGE_KEEP;
		}
		break;
	case ROW_CHARGE_DECREASE:
		g_RowTime --;

		g_RowRec_Value += g_RowValue_Per;
		if(g_RowTime<=0)
		{
			g_RowRec_Value = g_RowDest_Value;
			g_RowPhase = ROW_CHARGE_KEEP;
		}
		break;
	case ROW_CHARGE_MAX_TYPE:
		break;
	}

	g_RowRec_Value  =SetMAX(g_RowRec_Value,ROW_CHARGE_MAX_VALUE,0,2);
	SetVerTexTureRowCharge();
}
void SetRowCharge(int Row,int nType)
{
	int Value = 0;// Row * ROW_VALUE_PER_ROW;
	switch(Row)
	{
	case 1:
		Value = ROW_CHARGE_MAX_VALUE * 0;
	break;
	case 2:
		Value = ROW_CHARGE_MAX_VALUE * 0.10f;
	break;
	case 3:
		Value = ROW_CHARGE_MAX_VALUE * 0.20f;
	break;
	case 4:
		Value = ROW_CHARGE_MAX_VALUE * 0.35f;
	break;
	}

	g_RowTime = ROW_CHARGE_CHANGE_TIME;
	
	if(g_RowValue_Per != 0)
	{
		g_RowDest_Value += Value;
		g_RowBuff_Value = g_RowDest_Value - g_RowRec_Value;
		g_RowValue_Per = g_RowBuff_Value / ROW_CHARGE_CHANGE_TIME;
	}
	else
	{
		g_RowDest_Value = g_RowRec_Value + Value;
		g_RowValue_Per = (float)Value / (float)ROW_CHARGE_CHANGE_TIME;
	}
	//if(nType == ROW_CHARGE_DECREASE)
	//	g_RowValue_Per = -(100.0f / (5.0f * 60.0f));//MAX_Value / (MaxTime(sec) * Fps)
	g_RowPhase = nType;
}
int	GetRowChargeValue(void)
{
	return g_RowRec_Value;
}

int	GetRowChargeType(void)
{
	return g_RowPhase;
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawRowCharge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//フィールドの描画
	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffRowCharge,0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetTexture(0,g_pTextureRowCharge);

	// ポリゴンの描画

	//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);//set
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);//in
	//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);//set
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);// out
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	pDevice->SetTexture(0,NULL);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, NUM_POLYGON);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 8, NUM_POLYGON);
	pDevice->SetTexture(0,g_pTextureRowCharge);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 12, NUM_POLYGON);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 16, NUM_POLYGON);
	

}
//=============================================================================
// Shining頂点の作成
//=============================================================================
HRESULT MakeVertexRowCharge(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * 5,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
		FVF_VERTEX_2D,						// 使用する頂点フォーマット
		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
		&g_pVtxBuffRowCharge,					// 頂点バッファインターフェースへのポインタ
		NULL)))								// NULLに設定
	{
		return E_FAIL;
	}
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffRowCharge->Lock(0, 0, (void**)&pVtx, 0);
			// 頂点座標の設定

		//=======================LeftSide
		pVtx[0].pos.x = ROW_CHARGE_POS_X;
		pVtx[0].pos.y = ROW_CHARGE_POS_Y;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = ROW_CHARGE_POS_X + ROW_CHARGE_SIZE;
		pVtx[1].pos.y = ROW_CHARGE_POS_Y;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = ROW_CHARGE_POS_X;
		pVtx[2].pos.y =	ROW_CHARGE_POS_Y + ROW_CHARGE_SIZE;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = ROW_CHARGE_POS_X + ROW_CHARGE_SIZE;
		pVtx[3].pos.y = ROW_CHARGE_POS_Y + ROW_CHARGE_SIZE;
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
		pVtx[1].tex = D3DXVECTOR2(TEXRURE_CHARGE_UNIT,0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
		pVtx[3].tex = D3DXVECTOR2(TEXRURE_CHARGE_UNIT,1.0f);

		//=======================EntityBG
		pVtx+= 4;
		pVtx[0].pos.x = ROW_CHARGE_ENTITY_POS;
		pVtx[0].pos.y = ROW_CHARGE_POS_Y + ROW_CHARGE_SIZE*0.1f;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = ROW_CHARGE_ENTITY_POS + ROW_CHARGE_ENTITY_SIZE;
		pVtx[1].pos.y = ROW_CHARGE_POS_Y + ROW_CHARGE_SIZE*0.1f;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = ROW_CHARGE_ENTITY_POS;
		pVtx[2].pos.y =	ROW_CHARGE_POS_Y + ROW_CHARGE_SIZE - ROW_CHARGE_SIZE*0.1f;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = ROW_CHARGE_ENTITY_POS + ROW_CHARGE_ENTITY_SIZE;
		pVtx[3].pos.y = ROW_CHARGE_POS_Y + ROW_CHARGE_SIZE - ROW_CHARGE_SIZE*0.1f;
		pVtx[3].pos.z = 0.0f;
		// rhwの設定
		pVtx[0].rhw =
			pVtx[1].rhw =
			pVtx[2].rhw =
			pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

		//=======================Entity
		pVtx+= 4;
		pVtx[0].pos.x = ROW_CHARGE_ENTITY_POS;
		pVtx[0].pos.y = ROW_CHARGE_POS_Y + ROW_CHARGE_SIZE*0.1f;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = ROW_CHARGE_ENTITY_POS + g_RowRec_Value * ROW_TEXTURE_PER_VALUE;
		pVtx[1].pos.y = ROW_CHARGE_POS_Y + ROW_CHARGE_SIZE*0.1f;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = ROW_CHARGE_ENTITY_POS;
		pVtx[2].pos.y =	ROW_CHARGE_POS_Y + ROW_CHARGE_SIZE - ROW_CHARGE_SIZE*0.1f;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = ROW_CHARGE_ENTITY_POS + g_RowRec_Value * ROW_TEXTURE_PER_VALUE;
		pVtx[3].pos.y = ROW_CHARGE_POS_Y + ROW_CHARGE_SIZE - ROW_CHARGE_SIZE*0.1f;
		pVtx[3].pos.z = 0.0f;
		// rhwの設定
		pVtx[0].rhw =
			pVtx[1].rhw =
			pVtx[2].rhw =
			pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//=======================OutLine
		pVtx+= 4;
		pVtx[0].pos.x = ROW_CHARGE_ENTITY_POS;
		pVtx[0].pos.y = ROW_CHARGE_POS_Y;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = ROW_CHARGE_ENTITY_POS + ROW_CHARGE_ENTITY_SIZE;
		pVtx[1].pos.y = ROW_CHARGE_POS_Y;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = ROW_CHARGE_ENTITY_POS;
		pVtx[2].pos.y =	ROW_CHARGE_POS_Y + ROW_CHARGE_SIZE;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = ROW_CHARGE_ENTITY_POS + ROW_CHARGE_ENTITY_SIZE;
		pVtx[3].pos.y = ROW_CHARGE_POS_Y + ROW_CHARGE_SIZE;
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

		pVtx[0].tex = D3DXVECTOR2(TEXRURE_CHARGE_UNIT,0.0f);
		pVtx[1].tex = D3DXVECTOR2(TEXRURE_CHARGE_UNIT*2,0.0f);
		pVtx[2].tex = D3DXVECTOR2(TEXRURE_CHARGE_UNIT,1.0f);
		pVtx[3].tex = D3DXVECTOR2(TEXRURE_CHARGE_UNIT*2,1.0f);


		//=======================Rightside
		pVtx+= 4;
		pVtx[0].pos.x = ROW_CHARGE_RIGHT_POS;
		pVtx[0].pos.y = ROW_CHARGE_POS_Y;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = ROW_CHARGE_RIGHT_POS + ROW_CHARGE_SIZE;
		pVtx[1].pos.y = ROW_CHARGE_POS_Y;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = ROW_CHARGE_RIGHT_POS;
		pVtx[2].pos.y =	ROW_CHARGE_POS_Y + ROW_CHARGE_SIZE;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = ROW_CHARGE_RIGHT_POS + ROW_CHARGE_SIZE;
		pVtx[3].pos.y = ROW_CHARGE_POS_Y + ROW_CHARGE_SIZE;
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

		pVtx[0].tex = D3DXVECTOR2(TEXRURE_CHARGE_UNIT*2,0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
		pVtx[2].tex = D3DXVECTOR2(TEXRURE_CHARGE_UNIT*2,1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);
		// 頂点データをアンロックする
		g_pVtxBuffRowCharge->Unlock();
	}
	return S_OK;

}
void SetVerTexTureRowCharge(void)
{
//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;
	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffRowCharge->Lock(0, 0, (void**)&pVtx, 0);
	pVtx += 8;

	pVtx[0].pos.x = ROW_CHARGE_ENTITY_POS;
	pVtx[0].pos.y = ROW_CHARGE_POS_Y + ROW_CHARGE_SIZE*0.1f;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = ROW_CHARGE_ENTITY_POS + g_RowRec_Value * ROW_TEXTURE_PER_VALUE;
	pVtx[1].pos.y = ROW_CHARGE_POS_Y + ROW_CHARGE_SIZE*0.1f;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = ROW_CHARGE_ENTITY_POS;
	pVtx[2].pos.y =	ROW_CHARGE_POS_Y + ROW_CHARGE_SIZE - ROW_CHARGE_SIZE*0.1f;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = ROW_CHARGE_ENTITY_POS + g_RowRec_Value * ROW_TEXTURE_PER_VALUE;
	pVtx[3].pos.y = ROW_CHARGE_POS_Y + ROW_CHARGE_SIZE - ROW_CHARGE_SIZE*0.1f;
	pVtx[3].pos.z = 0.0f;
	// rhwの設定
	pVtx[0].rhw =
		pVtx[1].rhw =
		pVtx[2].rhw =
		pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	// 頂点データをアンロックする
	g_pVtxBuffRowCharge->Unlock();
}
