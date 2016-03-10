
//=============================================================================
// ブロック破壊効果 [Block_Light.cpp]
// HAL 名古屋　GP11A172 17 50323 リ リン
//=============================================================================
#include "Block_Light.h"
#include "main.h"
#include "Tetris_main.h"
#include "input.h"

#include <time.h>
#include <stdlib.h>

//*****************************************************************************
//使用注意
//*****************************************************************************
//マクロデ定義したVALUE_TIME_LIGHTは☆が消えるまでの時間，60は1秒
//もし，自動生成する場合，自動生成間隔と星生存時間を十分に注意しましょう。
//その時,段階的にプログラムを止め，g_starの使用状況をクイックウォッチで観察してみましょう
//もし，trueとなている星のナンバーが一定していなければ，バランスをとれていないことだ。
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_BLOCK_LIGHT				(256)				// 出現できるライトの最大数
#define VALUE_BLOCK_LIGHT_LIFE		(30)			// 描画時間
#define TEXTURE_EFFECT_LIGHT "data/TEXTURE/underLight.png"
#define LIGHT_SIZE_Y		(150.0f)
#define LIGHT_SIZE_X		(200.0f)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexBlock_Light(LPDIRECT3DDEVICE9 pDevice);
void SetVerTexBlock_Light(int LightID);
//*****************************************************************************
// 構造体宣言
//*****************************************************************************
typedef struct
{
	bool	bUse;			//使用しているかどうか
	int		nLife;			//使用時間
	float	fSpeed;			//移動速度
	float	fSize;			//大きさ
	float	fLeft;			//移動限界左
	float	fRight;			//移動限界右
	float	fAlpha;			//透明度
	D3DXVECTOR3 pos;		//描画位置中心　底の真ん中
}EFFECT_LIGHT;
//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureBlock_Light	= NULL;	//テクスチャメモリ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBlock_Light	= NULL;	//頂点座標バッファ
EFFECT_LIGHT g_Light[MAX_BLOCK_LIGHT];			//分裂ブロックの構造体


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBlock_Light(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for(int nCntLight = 0;nCntLight < MAX_BLOCK_LIGHT;nCntLight++)
	{
		g_Light[nCntLight].bUse		= false;
		g_Light[nCntLight].fLeft	= 0.0f;
		g_Light[nCntLight].nLife	= VALUE_BLOCK_LIGHT_LIFE;
		g_Light[nCntLight].fRight	= 0.0f;
		g_Light[nCntLight].fSpeed	= 0.0f;
		g_Light[nCntLight].fSize	= 1.0f;
		g_Light[nCntLight].pos		= D3DXVECTOR3(0.0f,0.0f,0.0f);
		g_Light[nCntLight].fAlpha	= 1.0f;
	}

	D3DXCreateTextureFromFile(pDevice,
		TEXTURE_EFFECT_LIGHT,
		&g_pTextureBlock_Light);

	MakeVertexBlock_Light(pDevice);
	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void UninitBlock_Light(void)
{
	if(g_pTextureBlock_Light != NULL)
	{// テクスチャの開放
		g_pTextureBlock_Light->Release();
		g_pTextureBlock_Light = NULL;
	}

	if(g_pVtxBuffBlock_Light != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffBlock_Light->Release();
		g_pVtxBuffBlock_Light = NULL;
	}
}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateBlock_Light(void)
{
	//if(GetKeyboardTrigger(DIK_O))
		//SetLight(D3DXVECTOR3(500.0f,500.0f,0.0f));
	for(int nCntLight = 0;nCntLight < MAX_BLOCK_LIGHT;nCntLight ++)
	{
		if(g_Light[nCntLight].bUse == false)
			continue;
	//=======ここから更新方法を書く=======//


		//if(g_Light[nCntLight].nLife > VALUE_BLOCK_LIGHT_LIFE * 0.8f)
			g_Light[nCntLight].fSize *= 1.1f;
		//if(g_Light[nCntLight].nLife <= VALUE_BLOCK_LIGHT_LIFE * 0.8f)
		//{
		//	g_Light[nCntLight].fSize *= 0.90f;
			g_Light[nCntLight].fAlpha *= 0.90;
		//}

	//=======ここまで更新方法を書く=======//
		g_Light[nCntLight].nLife -- ;
		if(g_Light[nCntLight].nLife == 0)
			g_Light[nCntLight].bUse = false;
		SetVerTexBlock_Light(nCntLight);

	}
}
//=============================================================================
// 描画処理
//=============================================================================
void DrawBlock_Light(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//フィールドの描画
	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffBlock_Light, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetTexture(0,g_pTextureBlock_Light);
	for(int nCntLight = 0;nCntLight < MAX_BLOCK_LIGHT;nCntLight++)
	{
		if(g_Light[nCntLight].bUse == false)
			continue;
		// ポリゴンの描画
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);//set
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);//in
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntLight * 4), NUM_POLYGON);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);//set
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);// out
	}

}
//=============================================================================
// Light頂点の作成
//=============================================================================
HRESULT MakeVertexBlock_Light(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * MAX_BLOCK_LIGHT,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
		FVF_VERTEX_2D,						// 使用する頂点フォーマット
		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
		&g_pVtxBuffBlock_Light,					// 頂点バッファインターフェースへのポインタ
		NULL)))								// NULLに設定
	{
		return E_FAIL;
	}
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffBlock_Light->Lock(0, 0, (void**)&pVtx, 0);

		for(int nCntLight = 0;nCntLight < MAX_BLOCK_LIGHT;nCntLight++,pVtx += 4)	
		{
			// 頂点座標の設定
			pVtx[0].pos.x = g_Light[nCntLight].pos.x - LIGHT_SIZE_Y;
			pVtx[0].pos.y = g_Light[nCntLight].pos.y - LIGHT_SIZE_Y;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_Light[nCntLight].pos.x + LIGHT_SIZE_Y;
			pVtx[1].pos.y = g_Light[nCntLight].pos.y - LIGHT_SIZE_Y;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_Light[nCntLight].pos.x;
			pVtx[2].pos.y = g_Light[nCntLight].pos.y;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_Light[nCntLight].pos.x + LIGHT_SIZE_Y;
			pVtx[3].pos.y = g_Light[nCntLight].pos.y;
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
		g_pVtxBuffBlock_Light->Unlock();
	}
	return S_OK;

}
//=============================================================================
// Lightnの設定
//=============================================================================
void SetVerTexBlock_Light(int Lightn_ID)
{
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffBlock_Light->Lock(0, 0, (void**)&pVtx, 0);
		// 頂点座標の設定
		pVtx += (Lightn_ID *4);
			// 頂点座標の設定
			pVtx[0].pos.x = g_Light[Lightn_ID].pos.x - LIGHT_SIZE_X;
			pVtx[0].pos.y = g_Light[Lightn_ID].pos.y - LIGHT_SIZE_Y * g_Light[Lightn_ID].fSize;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_Light[Lightn_ID].pos.x + LIGHT_SIZE_X;
			pVtx[1].pos.y = g_Light[Lightn_ID].pos.y - LIGHT_SIZE_Y * g_Light[Lightn_ID].fSize;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_Light[Lightn_ID].pos.x - LIGHT_SIZE_X;
			pVtx[2].pos.y = g_Light[Lightn_ID].pos.y;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_Light[Lightn_ID].pos.x + LIGHT_SIZE_X;
			pVtx[3].pos.y = g_Light[Lightn_ID].pos.y;
			pVtx[3].pos.z = 0.0f;

			// rhwの設定
			pVtx[0].rhw =
			pVtx[1].rhw =
			pVtx[2].rhw =
			pVtx[3].rhw = 1.0f;
			// 頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Light[Lightn_ID].fAlpha);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Light[Lightn_ID].fAlpha);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Light[Lightn_ID].fAlpha);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Light[Lightn_ID].fAlpha);


			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);
	
	// 頂点データをアンロックする
	g_pVtxBuffBlock_Light->Unlock();
	
}
//=============================================================================
// Lightの設定
//=============================================================================
void SetLight(D3DXVECTOR3 Pos)
{
	int nLight	= 0;

	for(int nCntLight = 0;nCntLight < MAX_BLOCK_LIGHT && nLight<2; nCntLight++)
	{
		if(g_Light[nCntLight].bUse == true)
			continue;
		
		g_Light[nCntLight].bUse		= true;
		g_Light[nCntLight].pos		= Pos;
		g_Light[nCntLight].fSize	= 1.0f;
		g_Light[nCntLight].fAlpha	= 1.0f;
		g_Light[nCntLight].nLife	= VALUE_BLOCK_LIGHT_LIFE;
		
		nLight++;
	}
}

