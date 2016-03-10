
//=============================================================================
// ブロック破壊効果 [Effect_Curve.cpp]
// HAL 名古屋　GP11A172 17 50323 リ リン
//=============================================================================
#include "effect_Curve.h"
#include "Tetris_main.h"
#include "main.h"
#include "input.h"
#include "Function.h"
#include <math.h>
//*****************************************************************************
//使用注意
//*****************************************************************************
//マクロデ定義したVALUE_TIME_CURVEは☆が消えるまでの時間，60は1秒
//もし，自動生成する場合，自動生成間隔と星生存時間を十分に注意しましょう。
//その時,段階的にプログラムを止め，g_curveの使用状況をクイックウォッチで観察してみましょう
//もし，trueとなている星のナンバーが一定していなければ，バランスをとれていないことだ。
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define VALUE_SPRITE_SIZE	    (10.0f)				// 分裂するブロックのサイズ
#define VALUE_LIFE_SPRITE		(120)				// スプライトの時間
#define MAX_CURVE				(12)				// カーブの差大本数
#define MAX_CURVE_SPRITE		(256)				// 最大分裂数
#define SPRITE_SIZE_UP_TIME		(0.1)				// サイズが大きくなる段階の分量
#define SPRITE_SIZE_DOWN_TIME	(1-SPRITE_SIZE_UP_TIME)//上の逆

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexEffect_Sprite(LPDIRECT3DDEVICE9 pDevice);
void SetVerTexEffect_Sprite(int Curven_ID);
void InitCurveSpriteSingle( int Sprite_ID);
void InitCurveSingle( int Curve_ID);
void SetUpSprite(D3DXVECTOR3 pos,D3DXVECTOR3 move);
void SetUpCurve(D3DXVECTOR3 Start,
				D3DXVECTOR3	End,
				float		Time,
				int			Frequency);
//*****************************************************************************
// 構造体宣言
//*****************************************************************************
typedef struct
{
	bool bUse;				// 使用フラグ
	D3DXVECTOR3 DestPos;	// 目的位置
	D3DXVECTOR3 StartPos;	// 開始位置
	D3DXVECTOR3 RecPos;		// 現在位置
	D3DXVECTOR3 Move;		// 移動速度
	int			nTime;		// 単位時間
	int			nFrequency;	// Sprite出現頻度
}EFFECT_CURVE;
typedef enum
{
	SPRITE_PHASE_START = 0,
	SPRITE_PHASE_REDUCE,
	SPRITE_PHASE_RESET,
	SPRITE_PHASE_MAX,
}SPRITE_PHASE;
typedef struct
{
	bool bUse;				// 使用フラグ
	int	nLife;
	D3DXVECTOR3	DrawPos;	// 描画位置
	D3DXVECTOR3 Move;		// 移動
	SPRITE_PHASE Phase;		// 現在の状態
	float Size;				// 大きさ
	float Alpha;			// アルファ値
}EFFECT_CURVE_SPRITE;

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureEffect_Sprite	= NULL;	//テクスチャメモリ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect_Sprite = NULL;	//頂点座標バッファ
EFFECT_CURVE g_Curve[MAX_CURVE];					//　カーブ
EFFECT_CURVE_SPRITE g_Sprite[MAX_CURVE_SPRITE];		//　スプライト
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEffect_Curve(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for(int nCntCurve = 0;nCntCurve < MAX_CURVE;nCntCurve++)
	{
		InitCurveSingle(nCntCurve);
	}
	for(int nCntSprite = 0;MAX_CURVE_SPRITE < MAX_CURVE;nCntSprite++)
	{
		InitCurveSpriteSingle(nCntSprite);
	}

	D3DXCreateTextureFromFile(pDevice,
		TEXTURE_CURVE,
		&g_pTextureEffect_Sprite);

	MakeVertexEffect_Sprite(pDevice);
	return S_OK;
}
//=============================================================================
// 特定のPointを初期化する
//=============================================================================
void 	InitCurveSingle( int Curve_ID)
{
	g_Curve[Curve_ID].bUse = false;
	g_Curve[Curve_ID].DestPos	= D3DXVECTOR3(0.0f,0.0f,0.0f);
	g_Curve[Curve_ID].Move		= D3DXVECTOR3(0.0f,0.0f,0.0f);
	g_Curve[Curve_ID].RecPos	= D3DXVECTOR3(0.0f,0.0f,0.0f);
	g_Curve[Curve_ID].StartPos	= D3DXVECTOR3(0.0f,0.0f,0.0f);
	g_Curve[Curve_ID].nFrequency = 0;
	g_Curve[Curve_ID].nTime		= 0;

}
void InitCurveSpriteSingle( int Sprite_ID)
{
	g_Sprite[Sprite_ID].bUse = false;
	g_Sprite[Sprite_ID].Alpha	= 1.0f;
	g_Sprite[Sprite_ID].Size	= 0.0f;
	g_Sprite[Sprite_ID].nLife	= VALUE_LIFE_SPRITE;
	g_Sprite[Sprite_ID].DrawPos	= D3DXVECTOR3(0.0f,0.0f,0.0f);
	g_Sprite[Sprite_ID].Move	= D3DXVECTOR3(0.0f,0.0f,0.0f);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEffect_Curve(void)
{
	if(g_pTextureEffect_Sprite != NULL)
	{// テクスチャの開放
		g_pTextureEffect_Sprite->Release();
		g_pTextureEffect_Sprite = NULL;
	}

	if(g_pVtxBuffEffect_Sprite != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffEffect_Sprite->Release();
		g_pVtxBuffEffect_Sprite = NULL;
	}
}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateEffect_Curve(void)
{
//	if(GetKeyboardTrigger(DIK_O))
	//	SetUpCurve(D3DXVECTOR3(1000.0f,0.0f,0.0f),D3DXVECTOR3(1000.0f,500.0f,0.0f),0.5f,1);
	for(int nCntCurve = 0;nCntCurve < MAX_CURVE;nCntCurve++)
	{
		if(g_Curve[nCntCurve].bUse == false)
			continue;
		g_Curve[nCntCurve].nTime --;
		g_Curve[nCntCurve].RecPos += g_Curve[nCntCurve].Move;// ;
		if(g_Curve[nCntCurve].nTime % g_Curve[nCntCurve].nFrequency == 0)
			SetUpSprite(g_Curve[nCntCurve].RecPos,g_Curve[nCntCurve].Move);//* 0.3f);
		if(g_Curve[nCntCurve].nTime < 0)
			InitCurveSingle(nCntCurve);
	}

	for(int nCntSprite = 0;nCntSprite < MAX_CURVE_SPRITE;nCntSprite++)
	{
		if(g_Sprite[nCntSprite].bUse == false)
			continue;
		g_Sprite[nCntSprite].DrawPos += g_Sprite[nCntSprite].Move;
		g_Sprite[nCntSprite].Move *= 0.9f;
		switch(g_Sprite[nCntSprite].Phase)
		{
		case SPRITE_PHASE_START :
			g_Sprite[nCntSprite].nLife--;
			g_Sprite[nCntSprite].Size += 1.0f/(VALUE_LIFE_SPRITE * SPRITE_SIZE_UP_TIME);
			if(g_Sprite[nCntSprite].nLife < (VALUE_LIFE_SPRITE * SPRITE_SIZE_DOWN_TIME))
				g_Sprite[nCntSprite].Phase = SPRITE_PHASE_REDUCE;
			break;
		case SPRITE_PHASE_REDUCE:
			g_Sprite[nCntSprite].nLife--;
			g_Sprite[nCntSprite].Size -= 1.0f/(VALUE_LIFE_SPRITE * SPRITE_SIZE_DOWN_TIME);
			if(g_Sprite[nCntSprite].nLife < 0)
				g_Sprite[nCntSprite].Phase = SPRITE_PHASE_RESET;
			break;
		case SPRITE_PHASE_RESET:
			InitCurveSpriteSingle(nCntSprite);
			break;
		case SPRITE_PHASE_MAX:
			break;
		}
		SetVerTexEffect_Sprite(nCntSprite);
	
	}


}
void SetUpSprite(D3DXVECTOR3 pos,D3DXVECTOR3 move)
{
	for(int nCntSprite = 0;nCntSprite < MAX_CURVE_SPRITE;nCntSprite++)	
	{
		if(g_Sprite[nCntSprite].bUse == true)
			continue;
		g_Sprite[nCntSprite].bUse = true;
		g_Sprite[nCntSprite].DrawPos = pos;
		g_Sprite[nCntSprite].Phase = SPRITE_PHASE_START;
		g_Sprite[nCntSprite].Move = move;
		break;
	}

}
void SetUpCurve(D3DXVECTOR3 Start,
				D3DXVECTOR3	End,
				float		Time,
				int			Frequency)
{
	for(int nCntCurve = 0;nCntCurve < MAX_CURVE;nCntCurve++)
	{
		if(g_Curve[nCntCurve].bUse == true)
			continue;
		g_Curve[nCntCurve].bUse = true;
		g_Curve[nCntCurve].StartPos	= Start;
		g_Curve[nCntCurve].DestPos	= End;
		g_Curve[nCntCurve].nTime	= Time * 60;
		g_Curve[nCntCurve].nFrequency	= Frequency;
		g_Curve[nCntCurve].RecPos	= Start;
		g_Curve[nCntCurve].Move		= (End - Start)/(Time * 60);
		break;
	}

	
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEffect_Curve(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//フィールドの描画
	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffEffect_Sprite, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetTexture(0,g_pTextureEffect_Sprite);
	for(int nCntSprite = 0;nCntSprite < MAX_CURVE_SPRITE;nCntSprite++)
	{
		if(g_Sprite[nCntSprite].bUse == false)
			continue;
		// ポリゴンの描画
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);//set
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);//in
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntSprite * 4), NUM_POLYGON);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);//set
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);// out
	}

}
//=============================================================================
// Curve頂点の作成
//=============================================================================
HRESULT MakeVertexEffect_Sprite(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * MAX_CURVE_SPRITE,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
		FVF_VERTEX_2D,						// 使用する頂点フォーマット
		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
		&g_pVtxBuffEffect_Sprite,					// 頂点バッファインターフェースへのポインタ
		NULL)))								// NULLに設定
	{
		return E_FAIL;
	}
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffEffect_Sprite->Lock(0, 0, (void**)&pVtx, 0);

		for(int nCntSprite = 0;nCntSprite < MAX_CURVE_SPRITE;nCntSprite++,pVtx += 4)	
		{
			// 頂点座標の設定
			pVtx[0].pos.x = g_Sprite[nCntSprite].DrawPos.x - VALUE_SPRITE_SIZE * g_Sprite[nCntSprite].Size;
			pVtx[0].pos.y = g_Sprite[nCntSprite].DrawPos.y - VALUE_SPRITE_SIZE * g_Sprite[nCntSprite].Size;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_Sprite[nCntSprite].DrawPos.x + VALUE_SPRITE_SIZE * g_Sprite[nCntSprite].Size;
			pVtx[1].pos.y = g_Sprite[nCntSprite].DrawPos.y - VALUE_SPRITE_SIZE * g_Sprite[nCntSprite].Size;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_Sprite[nCntSprite].DrawPos.x - VALUE_SPRITE_SIZE * g_Sprite[nCntSprite].Size;
			pVtx[2].pos.y = g_Sprite[nCntSprite].DrawPos.y + VALUE_SPRITE_SIZE * g_Sprite[nCntSprite].Size;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_Sprite[nCntSprite].DrawPos.x + VALUE_SPRITE_SIZE * g_Sprite[nCntSprite].Size;
			pVtx[3].pos.y = g_Sprite[nCntSprite].DrawPos.y + VALUE_SPRITE_SIZE * g_Sprite[nCntSprite].Size;
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
		g_pVtxBuffEffect_Sprite->Unlock();
	}
	return S_OK;

}
//=============================================================================
// Curvenの設定
//=============================================================================
void SetVerTexEffect_Sprite(int Sprite_ID)
{
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffEffect_Sprite->Lock(0, 0, (void**)&pVtx, 0);
	// 頂点座標の設定
	pVtx += (Sprite_ID *4);
	// 頂点座標の設定
	pVtx[0].pos.x = g_Sprite[Sprite_ID].DrawPos.x - VALUE_SPRITE_SIZE * g_Sprite[Sprite_ID].Size;
	pVtx[0].pos.y = g_Sprite[Sprite_ID].DrawPos.y - VALUE_SPRITE_SIZE * g_Sprite[Sprite_ID].Size;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_Sprite[Sprite_ID].DrawPos.x + VALUE_SPRITE_SIZE * g_Sprite[Sprite_ID].Size;
	pVtx[1].pos.y = g_Sprite[Sprite_ID].DrawPos.y - VALUE_SPRITE_SIZE * g_Sprite[Sprite_ID].Size;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_Sprite[Sprite_ID].DrawPos.x - VALUE_SPRITE_SIZE * g_Sprite[Sprite_ID].Size;
	pVtx[2].pos.y = g_Sprite[Sprite_ID].DrawPos.y + VALUE_SPRITE_SIZE * g_Sprite[Sprite_ID].Size;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_Sprite[Sprite_ID].DrawPos.x + VALUE_SPRITE_SIZE * g_Sprite[Sprite_ID].Size;
	pVtx[3].pos.y = g_Sprite[Sprite_ID].DrawPos.y + VALUE_SPRITE_SIZE * g_Sprite[Sprite_ID].Size;
	pVtx[3].pos.z = 0.0f;

	// rhwの設定
	pVtx[0].rhw =
		pVtx[1].rhw =
		pVtx[2].rhw =
		pVtx[3].rhw = 1.0f;
	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Sprite[Sprite_ID].Alpha);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Sprite[Sprite_ID].Alpha);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Sprite[Sprite_ID].Alpha);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Sprite[Sprite_ID].Alpha);


	// テクスチャ座標の設定
	
	pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

	// 頂点データをアンロックする
	g_pVtxBuffEffect_Sprite->Unlock();

}
