
//=============================================================================
// ブロック破壊効果 [Effect_Star.cpp]
// HAL 名古屋　GP11A172 17 50323 リ リン
//=============================================================================
#include "effect_star.h"
#include "Tetris_main.h"
#include "main.h"
#include "input.h"
#include "Function.h"
#include "Func_NumberX.h"
//*****************************************************************************
//使用注意
//*****************************************************************************
//マクロデ定義したVALUE_TIME_STARは☆が消えるまでの時間，60は1秒
//もし，自動生成する場合，自動生成間隔と星生存時間を十分に注意しましょう。
//その時,段階的にプログラムを止め，g_starの使用状況をクイックウォッチで観察してみましょう
//もし，trueとなている星のナンバーが一定していなければ，バランスをとれていないことだ。
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define VALUE_STAR_SIZE	(30)// 分裂するブロックのサイズ
#define VALUE_TIME_STAR		(140)				// 1秒
#define MAX_STAR				(256)				// 分裂できるブロックの最大数
#define VALUE_DEFAULT_SPEED_STAR (1.5f)				// 初期速度

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexEffect_Star(LPDIRECT3DDEVICE9 pDevice);
void SetVerTexEffect_Star(int Starn_ID);
D3DXVECTOR3 g_MousePos;
D3DXVECTOR3 g_MouseMove;
//*****************************************************************************
// 構造体宣言
//*****************************************************************************
typedef struct
{
	bool bUse;			//使用しているかどうか
	int nType;			//行動タイプ
	int	life;			//消えるまでの時間
	int nPart;			//どの部分か
	D3DXVECTOR3 pos;	//描画位置(中心)
	D3DXVECTOR3	move;	//移動方向
	float fChange_Size; //大きさの変化倍数
	int color;			//使用している画像
	float	Alpha;		//透明度
}EFFECT_STAR;

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureEffect_Star[BLOCK_MAX_COLOR-2]	= {};	//テクスチャメモリ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect_Star = NULL;	//頂点座標バッファ
EFFECT_STAR g_Star[MAX_STAR];			//分裂ブロックの構造体

const char *c_aFileNameStar[BLOCK_MAX_COLOR-1] =
	{
	"data/TEXTURE/star_red.png",	
	"data/TEXTURE/star_green.png",	
	"data/TEXTURE/star_blue.png",
	"data/TEXTURE/star_light.png",
	"data/TEXTURE/star_purple.png",	
	"data/TEXTURE/star_pink.png",
	"data/TEXTURE/star_pink.png",
	};
LPDIRECT3DTEXTURE9 GetMemoryStar(int color)
{
	return g_pTextureEffect_Star[color];
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEffect_Star(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for(int nCntStar = 0;nCntStar < MAX_STAR;nCntStar++)
	{
		g_Star[nCntStar].bUse	= false;
		g_Star[nCntStar].color	= BLOCK_MP;
		g_Star[nCntStar].move	= D3DXVECTOR3(0.0f,VALUE_DEFAULT_SPEED_STAR,0.0f);
		g_Star[nCntStar].pos	= D3DXVECTOR3(0.0f,0.0f,0.0f);
		g_Star[nCntStar].life	= VALUE_TIME_STAR;
		g_Star[nCntStar].nPart	= 0;
		g_Star[nCntStar].Alpha	= 1.0f;
		g_Star[nCntStar].fChange_Size =1.0f;
		g_Star[nCntStar].nType  = 0;
	}

	g_MousePos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	g_MouseMove= D3DXVECTOR3(0.0f,0.0f,0.0f);

	for(int nCntTexStar = 0;nCntTexStar<BLOCK_MAX_COLOR-2;nCntTexStar++)
	{
	D3DXCreateTextureFromFile(pDevice,
		c_aFileNameStar[nCntTexStar],
		&g_pTextureEffect_Star[nCntTexStar]);
	}

	MakeVertexEffect_Star(pDevice);
	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void UninitEffect_Star(void)
{
	for(int nCntTexStar = 0;nCntTexStar<BLOCK_MAX_COLOR-2;nCntTexStar++)
	if(g_pTextureEffect_Star[nCntTexStar] != NULL)
	{// テクスチャの開放
		g_pTextureEffect_Star[nCntTexStar]->Release();
		g_pTextureEffect_Star[nCntTexStar] = NULL;
	}

	if(g_pVtxBuffEffect_Star != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffEffect_Star->Release();
		g_pVtxBuffEffect_Star = NULL;
	}
}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateEffect_Star(void)
{
	for(int nCntStar = 0; nCntStar < MAX_STAR;nCntStar++)
	{
		if(g_Star[nCntStar].bUse == false)
			continue;
		g_Star[nCntStar].life --;

			//横行きの調整
			switch(g_Star[nCntStar].nPart)
			{
			case 0:g_Star[nCntStar].move.x	= -2.0f;
				break;
			case 1:g_Star[nCntStar].move.x	= -1.0f;
				break;
			case 2:g_Star[nCntStar].move.x	=  2.0f;
				break;
			case 3:g_Star[nCntStar].move.x	=  1.0f;
				break;
			};
		switch (g_Star[nCntStar].nType)
		{
		case 0:
			g_Star[nCntStar].fChange_Size = ((float)(rand()%10 + 1))/10 + 0.5f;
			//高さの調整
			if(g_Star[nCntStar].life > VALUE_TIME_STAR*0.7f)
			{
				g_Star[nCntStar].move.y *= -0.99f;
			}
			if(g_Star[nCntStar].life == VALUE_TIME_STAR*0.7f)
				g_Star[nCntStar].move.y = VALUE_DEFAULT_SPEED_STAR;
			if(g_Star[nCntStar].life < VALUE_TIME_STAR*0.7f)
			{
				g_Star[nCntStar].move.y *= 1.1f;
			}

			if(g_Star[nCntStar].life > VALUE_TIME_STAR * (4/5))
				g_Star[nCntStar].Alpha -=0.004f;

			break;
		case 1:
			g_Star[nCntStar].move.y -= 0.5f;//移動量
			g_Star[nCntStar].Alpha -=0.005f;//α変化値
			g_Star[nCntStar].fChange_Size *= 0.99f; 
			break;
		}	
		
		g_Star[nCntStar].pos += g_Star[nCntStar].move;
			if(g_Star[nCntStar].life == 0)
			{
				g_Star[nCntStar].bUse = false;
				//使わなくなると，初期化
				g_Star[nCntStar].move = D3DXVECTOR3(0.0f,VALUE_DEFAULT_SPEED_STAR,0.0f);
				g_Star[nCntStar].Alpha	= 1.0f;
				g_Star[nCntStar].life = VALUE_TIME_STAR;
				g_Star[nCntStar].fChange_Size = 1.0f;

			}
		SetVerTexEffect_Star(nCntStar);
	}
}
//=============================================================================
// 描画処理
//=============================================================================
void DrawEffect_Star(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//フィールドの描画
	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffEffect_Star, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for(int nCntStar = 0;nCntStar < MAX_STAR;nCntStar++)
	{
		if(g_Star[nCntStar].bUse == false)
			continue;
		pDevice->SetTexture(0,g_pTextureEffect_Star[g_Star[nCntStar].color]);
		// ポリゴンの描画

		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);//set
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);//in
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntStar * 4), NUM_POLYGON);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);//set
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);// out
	}

}
//=============================================================================
// Star頂点の作成
//=============================================================================
HRESULT MakeVertexEffect_Star(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * MAX_STAR,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
		FVF_VERTEX_2D,						// 使用する頂点フォーマット
		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
		&g_pVtxBuffEffect_Star,					// 頂点バッファインターフェースへのポインタ
		NULL)))								// NULLに設定
	{
		return E_FAIL;
	}
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffEffect_Star->Lock(0, 0, (void**)&pVtx, 0);

		for(int nCntStar = 0;nCntStar < MAX_STAR;nCntStar++,pVtx += 4)	
		{
			// 頂点座標の設定
			pVtx[0].pos.x = g_Star[nCntStar].pos.x - VALUE_STAR_SIZE;
			pVtx[0].pos.y = g_Star[nCntStar].pos.y - VALUE_STAR_SIZE;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_Star[nCntStar].pos.x + VALUE_STAR_SIZE;
			pVtx[1].pos.y = g_Star[nCntStar].pos.y - VALUE_STAR_SIZE;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_Star[nCntStar].pos.x;
			pVtx[2].pos.y = g_Star[nCntStar].pos.y;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_Star[nCntStar].pos.x + VALUE_STAR_SIZE;
			pVtx[3].pos.y = g_Star[nCntStar].pos.y;
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
		g_pVtxBuffEffect_Star->Unlock();
	}
	return S_OK;

}
//=============================================================================
// Starnの設定
//=============================================================================
void SetVerTexEffect_Star(int Starn_ID)
{
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffEffect_Star->Lock(0, 0, (void**)&pVtx, 0);
	// 頂点座標の設定
	pVtx += (Starn_ID *4);
	// 頂点座標の設定
	pVtx[0].pos.x = g_Star[Starn_ID].pos.x - 0.5f*VALUE_STAR_SIZE*3*g_Star[Starn_ID].fChange_Size;
	pVtx[0].pos.y = g_Star[Starn_ID].pos.y - VALUE_STAR_SIZE*3*g_Star[Starn_ID].fChange_Size;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_Star[Starn_ID].pos.x + 0.5f*VALUE_STAR_SIZE*3*g_Star[Starn_ID].fChange_Size;
	pVtx[1].pos.y = g_Star[Starn_ID].pos.y - VALUE_STAR_SIZE*3*g_Star[Starn_ID].fChange_Size;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_Star[Starn_ID].pos.x - 0.5f*VALUE_STAR_SIZE*3*g_Star[Starn_ID].fChange_Size;;
	pVtx[2].pos.y = g_Star[Starn_ID].pos.y;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_Star[Starn_ID].pos.x + 0.5f*VALUE_STAR_SIZE*3*g_Star[Starn_ID].fChange_Size;
	pVtx[3].pos.y = g_Star[Starn_ID].pos.y;
	pVtx[3].pos.z = 0.0f;

	// rhwの設定
	pVtx[0].rhw =
		pVtx[1].rhw =
		pVtx[2].rhw =
		pVtx[3].rhw = 1.0f;
	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Star[Starn_ID].Alpha);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Star[Starn_ID].Alpha);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Star[Starn_ID].Alpha);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Star[Starn_ID].Alpha);


	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

	// 頂点データをアンロックする
	g_pVtxBuffEffect_Star->Unlock();

}
//=============================================================================
// Starの設定
//=============================================================================
void SetEffect_Star(D3DXVECTOR3 Pos,int Color,int nType)
{
	int nStar	= 0;

	for(int nCntStar = 0;nCntStar < MAX_STAR&&nStar<4;nCntStar++)
	{
		if(g_Star[nCntStar].bUse == true)
			continue;

		g_Star[nCntStar].nPart = nStar;


		g_Star[nCntStar].bUse	= true;
		g_Star[nCntStar].color  = Color;
			if(Color == BLOCK_RAND_COLOR)
				g_Star[nCntStar].color = rand()%6;
		g_Star[nCntStar].nType  = nType;

		switch(nStar)
		{
		case 0:	
			g_Star[nCntStar].pos.x = Pos.x;
			g_Star[nCntStar].pos.y = Pos.y;// - VALUE_STAR_SIZE;
			break;
		case 1:	
			g_Star[nCntStar].pos.x = Pos.x; 
			g_Star[nCntStar].pos.y = Pos.y;
			break;
		case 2:
			g_Star[nCntStar].pos.x = Pos.x;// + VALUE_STAR_SIZE;
			g_Star[nCntStar].pos.y = Pos.y;// - VALUE_STAR_SIZE;
			break;
		case 3:
			g_Star[nCntStar].pos.x = Pos.x;// + VALUE_STAR_SIZE;
			g_Star[nCntStar].pos.y = Pos.y;
			break;
		}
		nStar++;
	}
}

