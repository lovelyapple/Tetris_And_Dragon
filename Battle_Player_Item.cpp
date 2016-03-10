
//=============================================================================
// ブロック破壊効果 [Battle_Player_Item.cpp]
// HAL 名古屋　GP11A172 17 50323 リ リン
//=============================================================================
#include "Battle_Player_Item.h"
#include "Battle_Main.h"
#include "input.h"
//*****************************************************************************
//使用注意
//*****************************************************************************
//*****************************************************************************
// マクロ定義
//*****************************************************************************
typedef enum
{
	BATTLE_ITEM_HP_RECOVER_SMALL = 0,
	BATTLE_ITEM_MP_RECOVER_SMALL,
	BATTLE_ITEM_MAGIC_CARD_NORMAL,
	BATTLE_ITEM_MAX,
}BATTLE_ITEM_NAME;

#define TEXTURE_ITEM		("data/TEXTURE/Item.png")
#define TEXTURE_ITEM_COUNT	("data/TEXTURE/Number_Big.png")
#define BATTLE_ITEM_POS_X	(SYS_BATTLE_POS_START_X + 10.0f)
#define BATTLE_ITEM_POS_Y	(VALUE_FIELD_END_Y + 50.0f)
#define BATTLE_ITEM_SIZE	(50.0f)
#define BATTLE_ITEM_COUNT_SIZE_X	(10)
#define BATTLE_ITEM_COUNT_SIZE_Y	(20)
#define BATTLE_ITEM_COUNT_POS_X	(BATTLE_ITEM_POS_X + BATTLE_ITEM_SIZE*1.2f)
#define BATTLE_ITEM_COUNT_ALPHA_TIME	(70)
#define BATTLE_ITEM_COUNT_ALPHA_ACTIVE_TIME	(80)
#define BATTLE_ITEM_COUNT_DIGIT	(2)
#define BATTLE_ITEM_COUNT_UNIT_SIZE	(1.0f/10)
#define BATTLE_ITEM_UNIT_SIZE	(1.0/ BATTLE_ITEM_MAX)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexBattle_Player_Item(LPDIRECT3DDEVICE9 pDevice);
void SetVerTexBattle_Player_Item(void);
void SetBattle_Player_Item(void);
//*****************************************************************************
// 構造体宣言
//*****************************************************************************
typedef enum
{
	ITEM_PAHSE_WAIT = 0,
	ITEM_PAHSE_CHANGE,
	ITEM_PAHSE_MAX,
}BATTLE_ITEM_PAHSE;
typedef struct
{
	int Count;	//残数
	int Alpha;
	int	AlphaTime;
	BATTLE_ITEM_PAHSE phase; 
	D3DXVECTOR3	ItemPos;
	D3DXVECTOR3 CountPos[BATTLE_ITEM_COUNT_DIGIT];
}BATTLE_ITEM;


//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureBattle_Player_Item = NULL;	//テクスチャメモリ
LPDIRECT3DTEXTURE9		g_pTextureBattle_Player_Item_Count = NULL;	//テクスチャメモリ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBattle_Player_Item = NULL;	//頂点座標バッファ
BATTLE_ITEM g_Item[BATTLE_ITEM_MAX];			//分裂ブロックの構造体

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBattle_Player_Item(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for(int nCntItem = 0;nCntItem < BATTLE_ITEM_MAX;nCntItem++)
	{
		g_Item[nCntItem].Count	= 3;
		g_Item[nCntItem].Alpha	= 0;
		g_Item[nCntItem].AlphaTime	= 0;
		g_Item[nCntItem].phase		= ITEM_PAHSE_WAIT;
		g_Item[nCntItem].ItemPos	= D3DXVECTOR3(0.0f,0.0f,0.0f);
		g_Item[nCntItem].CountPos[0]	= D3DXVECTOR3(0.0f,0.0f,0.0f);
		g_Item[nCntItem].CountPos[1]	= D3DXVECTOR3(0.0f,0.0f,0.0f);
	}


	D3DXCreateTextureFromFile(pDevice,
							TEXTURE_ITEM,
							&g_pTextureBattle_Player_Item);
	D3DXCreateTextureFromFile(pDevice,
							TEXTURE_ITEM_COUNT,
							&g_pTextureBattle_Player_Item_Count);
	SetBattle_Player_Item();

	MakeVertexBattle_Player_Item(pDevice);
	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void UninitBattle_Player_Item(void)
{
	if(g_pTextureBattle_Player_Item != NULL)
	{// テクスチャの開放
		g_pTextureBattle_Player_Item->Release();
		g_pTextureBattle_Player_Item = NULL;
	}
	if(g_pTextureBattle_Player_Item_Count != NULL)
	{// テクスチャの開放
		g_pTextureBattle_Player_Item_Count->Release();
		g_pTextureBattle_Player_Item_Count = NULL;
	}
	if(g_pVtxBuffBattle_Player_Item != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffBattle_Player_Item->Release();
		g_pVtxBuffBattle_Player_Item = NULL;
	}
}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateBattle_Player_Item(void)
{

	if(rand()% 100 < 5 && g_Item[BATTLE_ITEM_MAGIC_CARD_NORMAL].phase == ITEM_PAHSE_MAX)
	{
		for(int nCntItem = 0;nCntItem < BATTLE_ITEM_MAX;nCntItem++)
		{
			g_Item[nCntItem].AlphaTime = BATTLE_ITEM_COUNT_ALPHA_TIME * (nCntItem+1);
			g_Item[nCntItem].phase = ITEM_PAHSE_WAIT;
		}

	}
	if(GetKeyboardTrigger(DIK_L))
		g_Item[1].phase = ITEM_PAHSE_WAIT;
	for(int nCntItem = 0;nCntItem < BATTLE_ITEM_MAX;nCntItem++)
	{
		switch(g_Item[nCntItem].phase)
		{
		case ITEM_PAHSE_WAIT:
			g_Item[nCntItem].AlphaTime --;
			if(g_Item[nCntItem].AlphaTime <= 0 )
			{
				g_Item[nCntItem].phase = ITEM_PAHSE_CHANGE;
				g_Item[nCntItem].AlphaTime = 0;
			}

			break;
		case ITEM_PAHSE_CHANGE:
			g_Item[nCntItem].Alpha ++;
			if(g_Item[nCntItem].Alpha >= BATTLE_ITEM_COUNT_ALPHA_ACTIVE_TIME)
			{
				g_Item[nCntItem].Alpha = 0;
				g_Item[nCntItem].phase = ITEM_PAHSE_MAX;
			}

			break;
		case ITEM_PAHSE_MAX:
			break;
		}

	}

	//SetBattle_Player_Item();
	SetVerTexBattle_Player_Item();
}
//=============================================================================
// Itemの設定
//=============================================================================
void SetBattle_Player_Item(void)
{
	for(int nCntItem = 0;nCntItem < BATTLE_ITEM_MAX;nCntItem++)
	{
		g_Item[nCntItem].Count	= 99;

		g_Item[nCntItem].Alpha	= 0;;
		g_Item[nCntItem].AlphaTime	= 0;

		g_Item[nCntItem].ItemPos		= D3DXVECTOR3(BATTLE_ITEM_POS_X,
														BATTLE_ITEM_POS_Y + nCntItem * (BATTLE_ITEM_SIZE + 10.0f),
														0.0f);
		g_Item[nCntItem].CountPos[0]	= D3DXVECTOR3(BATTLE_ITEM_COUNT_POS_X,
														BATTLE_ITEM_POS_Y + nCntItem * (BATTLE_ITEM_SIZE + 10.0f),
														0.0f);
		g_Item[nCntItem].CountPos[1]	= D3DXVECTOR3(BATTLE_ITEM_COUNT_POS_X + BATTLE_ITEM_COUNT_SIZE_X,
														BATTLE_ITEM_POS_Y + nCntItem * (BATTLE_ITEM_SIZE + 10.0f),
														0.0f);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBattle_Player_Item(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//フィールドの描画
	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffBattle_Player_Item, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for(int nCntItem = 0;nCntItem < BATTLE_ITEM_MAX;nCntItem++)
	{
		
		pDevice->SetTexture(0,g_pTextureBattle_Player_Item);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntItem * 12), NUM_POLYGON);
		pDevice->SetTexture(0,g_pTextureBattle_Player_Item_Count);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntItem * 12 + 4), NUM_POLYGON);


		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntItem * 12 + 8), NUM_POLYGON);
		// ポリゴンの描画

		//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);//set
		//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);//in
		//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);//set
		//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);// out
	}

}
//=============================================================================
// Item頂点の作成
//=============================================================================
HRESULT MakeVertexBattle_Player_Item(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * BATTLE_ITEM_MAX * 3,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
		FVF_VERTEX_2D,						// 使用する頂点フォーマット
		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
		&g_pVtxBuffBattle_Player_Item,					// 頂点バッファインターフェースへのポインタ
		NULL)))								// NULLに設定
	{
		return E_FAIL;
	}
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffBattle_Player_Item->Lock(0, 0, (void**)&pVtx, 0);

		for(int nCntItem = 0;nCntItem < BATTLE_ITEM_MAX;nCntItem++,pVtx += 4)	
		{
			// 頂点座標の設定
			pVtx[0].pos.x = g_Item[nCntItem].ItemPos.x - BATTLE_ITEM_SIZE;
			pVtx[0].pos.y = g_Item[nCntItem].ItemPos.y - BATTLE_ITEM_SIZE;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_Item[nCntItem].ItemPos.x + BATTLE_ITEM_SIZE;
			pVtx[1].pos.y = g_Item[nCntItem].ItemPos.y - BATTLE_ITEM_SIZE;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_Item[nCntItem].ItemPos.x;
			pVtx[2].pos.y = g_Item[nCntItem].ItemPos.y;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_Item[nCntItem].ItemPos.x + BATTLE_ITEM_SIZE;
			pVtx[3].pos.y = g_Item[nCntItem].ItemPos.y;
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
			int Pones = g_Item[nCntItem].Count % 10;
			int Ptens = g_Item[nCntItem].Count / 10;;
			pVtx += 4;
			// 頂点座標の設定
			pVtx[0].pos.x = g_Item[nCntItem].CountPos[0].x;// - BATTLE_ITEM_COUNT_SIZE_X;
			pVtx[0].pos.y = g_Item[nCntItem].CountPos[0].y - BATTLE_ITEM_COUNT_SIZE_Y;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_Item[nCntItem].CountPos[0].x + BATTLE_ITEM_COUNT_SIZE_X;
			pVtx[1].pos.y = g_Item[nCntItem].CountPos[0].y - BATTLE_ITEM_COUNT_SIZE_Y;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_Item[nCntItem].CountPos[0].x;
			pVtx[2].pos.y = g_Item[nCntItem].CountPos[0].y;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_Item[nCntItem].CountPos[0].x + BATTLE_ITEM_COUNT_SIZE_X;
			pVtx[3].pos.y = g_Item[nCntItem].CountPos[0].y;
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

			pVtx[0].tex = D3DXVECTOR2(BATTLE_ITEM_COUNT_UNIT_SIZE * Pones,0.0f);
			pVtx[1].tex = D3DXVECTOR2(BATTLE_ITEM_COUNT_UNIT_SIZE * Pones + BATTLE_ITEM_COUNT_UNIT_SIZE,0.0f);
			pVtx[2].tex = D3DXVECTOR2(BATTLE_ITEM_COUNT_UNIT_SIZE * Pones,1.0f);
			pVtx[3].tex = D3DXVECTOR2(BATTLE_ITEM_COUNT_UNIT_SIZE * Pones + BATTLE_ITEM_COUNT_UNIT_SIZE,1.0f);

			pVtx += 4;
			// 頂点座標の設定
			pVtx[0].pos.x = g_Item[nCntItem].CountPos[1].x;// - BATTLE_ITEM_COUNT_SIZE_X;
			pVtx[0].pos.y = g_Item[nCntItem].CountPos[1].y - BATTLE_ITEM_COUNT_SIZE_Y;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_Item[nCntItem].CountPos[1].x + BATTLE_ITEM_COUNT_SIZE_X;
			pVtx[1].pos.y = g_Item[nCntItem].CountPos[1].y - BATTLE_ITEM_COUNT_SIZE_Y;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_Item[nCntItem].CountPos[1].x;
			pVtx[2].pos.y = g_Item[nCntItem].CountPos[1].y;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_Item[nCntItem].CountPos[1].x + BATTLE_ITEM_COUNT_SIZE_X;
			pVtx[3].pos.y = g_Item[nCntItem].CountPos[1].y;
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

			pVtx[0].tex = D3DXVECTOR2(BATTLE_ITEM_COUNT_UNIT_SIZE * Ptens,0.0f);
			pVtx[1].tex = D3DXVECTOR2(BATTLE_ITEM_COUNT_UNIT_SIZE * Ptens + BATTLE_ITEM_COUNT_UNIT_SIZE,0.0f);
			pVtx[2].tex = D3DXVECTOR2(BATTLE_ITEM_COUNT_UNIT_SIZE * Ptens,1.0f);
			pVtx[3].tex = D3DXVECTOR2(BATTLE_ITEM_COUNT_UNIT_SIZE * Ptens + BATTLE_ITEM_COUNT_UNIT_SIZE,1.0f);
		}
		// 頂点データをアンロックする
		g_pVtxBuffBattle_Player_Item->Unlock();
	}
	return S_OK;

}
//=============================================================================
// Itemnの設定
//=============================================================================
void SetVerTexBattle_Player_Item(void)

{//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffBattle_Player_Item->Lock(0, 0, (void**)&pVtx, 0);

	for(int nCntItem = 0;nCntItem < BATTLE_ITEM_MAX;nCntItem++,pVtx += 4)	
	{
		// 頂点座標の設定
		pVtx[0].pos.x = g_Item[nCntItem].ItemPos.x;
		pVtx[0].pos.y = g_Item[nCntItem].ItemPos.y - BATTLE_ITEM_SIZE;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_Item[nCntItem].ItemPos.x + BATTLE_ITEM_SIZE;
		pVtx[1].pos.y = g_Item[nCntItem].ItemPos.y - BATTLE_ITEM_SIZE;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_Item[nCntItem].ItemPos.x;
		pVtx[2].pos.y = g_Item[nCntItem].ItemPos.y;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_Item[nCntItem].ItemPos.x + BATTLE_ITEM_SIZE;
		pVtx[3].pos.y = g_Item[nCntItem].ItemPos.y;
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

		pVtx[0].tex = D3DXVECTOR2(BATTLE_ITEM_UNIT_SIZE * nCntItem,0.0f);
		pVtx[1].tex = D3DXVECTOR2(BATTLE_ITEM_UNIT_SIZE * nCntItem + BATTLE_ITEM_UNIT_SIZE,0.0f);
		pVtx[2].tex = D3DXVECTOR2(BATTLE_ITEM_UNIT_SIZE * nCntItem,1.0f);
		pVtx[3].tex = D3DXVECTOR2(BATTLE_ITEM_UNIT_SIZE * nCntItem + BATTLE_ITEM_UNIT_SIZE,1.0f);
		int Pones = g_Item[nCntItem].Count % 10;
		int Ptens = g_Item[nCntItem].Count / 10;;
		pVtx += 4;
		float Afpha = (sinf(g_Item[nCntItem].Alpha * (D3DX_PI * 4 ) / BATTLE_ITEM_COUNT_ALPHA_ACTIVE_TIME +   D3DX_PI * 0.5f) + 1.0f) * 0.5f;
		
		// 頂点座標の設定
		pVtx[0].pos.x = g_Item[nCntItem].CountPos[0].x;// - BATTLE_ITEM_COUNT_SIZE_X;
		pVtx[0].pos.y = g_Item[nCntItem].CountPos[0].y - BATTLE_ITEM_COUNT_SIZE_Y;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_Item[nCntItem].CountPos[0].x + BATTLE_ITEM_COUNT_SIZE_X;
		pVtx[1].pos.y = g_Item[nCntItem].CountPos[0].y - BATTLE_ITEM_COUNT_SIZE_Y;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_Item[nCntItem].CountPos[0].x;
		pVtx[2].pos.y = g_Item[nCntItem].CountPos[0].y;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_Item[nCntItem].CountPos[0].x + BATTLE_ITEM_COUNT_SIZE_X;
		pVtx[3].pos.y = g_Item[nCntItem].CountPos[0].y;
		pVtx[3].pos.z = 0.0f;
		// rhwの設定
		pVtx[0].rhw =
			pVtx[1].rhw =
			pVtx[2].rhw =
			pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, Afpha);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, Afpha);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, Afpha);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, Afpha);

		pVtx[0].tex = D3DXVECTOR2(BATTLE_ITEM_COUNT_UNIT_SIZE * Pones,0.0f);
		pVtx[1].tex = D3DXVECTOR2(BATTLE_ITEM_COUNT_UNIT_SIZE * Pones + BATTLE_ITEM_COUNT_UNIT_SIZE,0.0f);
		pVtx[2].tex = D3DXVECTOR2(BATTLE_ITEM_COUNT_UNIT_SIZE * Pones,1.0f);
		pVtx[3].tex = D3DXVECTOR2(BATTLE_ITEM_COUNT_UNIT_SIZE * Pones + BATTLE_ITEM_COUNT_UNIT_SIZE,1.0f);

		pVtx += 4;
		// 頂点座標の設定
		pVtx[0].pos.x = g_Item[nCntItem].CountPos[1].x;// - BATTLE_ITEM_COUNT_SIZE_X;
		pVtx[0].pos.y = g_Item[nCntItem].CountPos[1].y - BATTLE_ITEM_COUNT_SIZE_Y;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_Item[nCntItem].CountPos[1].x + BATTLE_ITEM_COUNT_SIZE_X;
		pVtx[1].pos.y = g_Item[nCntItem].CountPos[1].y - BATTLE_ITEM_COUNT_SIZE_Y;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_Item[nCntItem].CountPos[1].x;
		pVtx[2].pos.y = g_Item[nCntItem].CountPos[1].y;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_Item[nCntItem].CountPos[1].x + BATTLE_ITEM_COUNT_SIZE_X;
		pVtx[3].pos.y = g_Item[nCntItem].CountPos[1].y;
		pVtx[3].pos.z = 0.0f;
		// rhwの設定
		pVtx[0].rhw =
			pVtx[1].rhw =
			pVtx[2].rhw =
			pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, Afpha);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, Afpha);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, Afpha);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, Afpha);

		pVtx[0].tex = D3DXVECTOR2(BATTLE_ITEM_COUNT_UNIT_SIZE * Ptens,0.0f);
		pVtx[1].tex = D3DXVECTOR2(BATTLE_ITEM_COUNT_UNIT_SIZE * Ptens + BATTLE_ITEM_COUNT_UNIT_SIZE,0.0f);
		pVtx[2].tex = D3DXVECTOR2(BATTLE_ITEM_COUNT_UNIT_SIZE * Ptens,1.0f);
		pVtx[3].tex = D3DXVECTOR2(BATTLE_ITEM_COUNT_UNIT_SIZE * Ptens + BATTLE_ITEM_COUNT_UNIT_SIZE,1.0f);
	}
	// 頂点データをアンロックする
	g_pVtxBuffBattle_Player_Item->Unlock();
}

