//=============================================================================
//[MessageBox.cpp]
// HAL 名古屋　GP11A172 17 50323 リ リン
//=============================================================================

#include "MessageBox.h"
#include "main.h"
#include "game.h"
#include "Battle_Main.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


const char *FileNameMessage[MESSAGE_WORDS_MAX]=
{
	"data/MESSAGE/HOROBIYO.png",
	"data/MESSAGE/NAMETERUNO.png",
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexMessageBox(LPDIRECT3DDEVICE9 pDevice);
HRESULT MakeVertexMessageWords(LPDIRECT3DDEVICE9 pDevice);
#define MESSAGE_WORDS_SIZE	(50)
void SetVertexMessageWords(int WordsID);
void SetVertexMessageBox(int BoxID);

void InitMessageBoxSingle(int BoxID);
void InitMessageWordsSingle(int WordsID);
//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureMessageBox	= NULL;	//テクスチャメモリ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMessageBox	= NULL;	//頂点座標バッファ
LPDIRECT3DTEXTURE9		g_pTextureMessageWords[MESSAGE_WORDS_MAX]	= {};	//テクスチャメモリ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMessageWords	= NULL;	//頂点座標バッファ

MESSAGE_BOX				g_MessageBox[MESSAGE_BOX_MAX];
MESSAGE_WORDS_STRUCT	g_MessageWords[MESSAGE_WORDS_MAX];
int MessageBoxID = 0;
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitMessageBox(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	for(int nWords = 0; nWords < MESSAGE_WORDS_MAX;nWords++)
	{
		InitMessageWordsSingle(nWords);
		D3DXCreateTextureFromFile(pDevice,
							FileNameMessage[nWords],
							&g_pTextureMessageWords[nWords]);
	}


	for(int BoxID = 0;BoxID < MESSAGE_BOX_MAX;BoxID++)
	{
		InitMessageBoxSingle(BoxID);
	}

	D3DXCreateTextureFromFile(pDevice,
							TEXTURE_MESSAGE_BOX_BLOCK,
							&g_pTextureMessageBox);

	MakeVertexMessageBox(pDevice);
	MakeVertexMessageWords(pDevice);
	return S_OK;
}
void InitMessageBoxSingle(int BoxID)
{
	g_MessageBox[BoxID].bUse		= false;
	g_MessageBox[BoxID].nLife		= MESSAGE_MAX_LIFE;					// 時間制御
	g_MessageBox[BoxID].phase		= MESSAGE_BOX_PHASE_MAX;			// 進み度合い
	g_MessageBox[BoxID].posCenter	= D3DXVECTOR3(0.0f,0.0f,0.0f);		// 中心位置
	g_MessageBox[BoxID].posUp		= D3DXVECTOR3(0.0f,0.0f,0.0f);		// 上段位置
	g_MessageBox[BoxID].posDown		= D3DXVECTOR3(0.0f,0.0f,0.0f);		// 下段位置
	g_MessageBox[BoxID].move		= D3DXVECTOR3(0.0f,0.0f,0.0f);
	g_MessageBox[BoxID].fAlpha		= 1.0f;								// 透明度
	g_MessageBox[BoxID].fHeight		= 0.0f;
	g_MessageBox[BoxID].fWidth		= 0.0f;

}
void InitMessageWordsSingle(int WordsID)
{
	g_MessageWords[WordsID].bUse	= false;
	g_MessageWords[WordsID].fHeight	= 0.0f;
	g_MessageWords[WordsID].fWidth	= 0.0f;
	g_MessageWords[WordsID].pos		= D3DXVECTOR3(0.0f,0.0f,0.0f);
	g_MessageWords[WordsID].MessageWordsID = MESSAGE_WORDS_MAX;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitMessageBox(void)
{
	if(g_pTextureMessageBox != NULL)
	{// テクスチャの開放
		g_pTextureMessageBox->Release();
		g_pTextureMessageBox = NULL;
	}

	if(g_pVtxBuffMessageBox != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffMessageBox->Release();
		g_pVtxBuffMessageBox = NULL;
	}

	for(int nMessage = 0; nMessage < MESSAGE_WORDS_MAX;nMessage++)
	{
		if(g_pTextureMessageWords[nMessage] != NULL)
		{// テクスチャの開放
			g_pTextureMessageWords[nMessage]->Release();
			g_pTextureMessageWords[nMessage] = NULL;
		}
	}
	if(g_pVtxBuffMessageWords != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffMessageWords->Release();
		g_pVtxBuffMessageWords = NULL;
	}

}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateMessageBox(void)
{



	for(int BoxID = 0;BoxID < MESSAGE_BOX_MAX;BoxID++)
	{
		if(g_MessageBox[BoxID].bUse == false)
			continue;
		switch (g_MessageBox[BoxID].phase)
		{
		case MESSAGE_BOX_PHASE_START:
			g_MessageBox[BoxID].phase	=  MESSAGE_BOX_PHASE_OPEN_X;
			break;
		case MESSAGE_BOX_PHASE_OPEN_X:
			g_MessageBox[BoxID].nLife--;
			g_MessageBox[BoxID].posUp.x -= g_MessageBox[BoxID].move.x;
			g_MessageBox[BoxID].posDown.x += g_MessageBox[BoxID].move.x;
			if(g_MessageBox[BoxID].nLife < 0)
			{
				g_MessageBox[BoxID].nLife = 0;//MESSAGE_MAX_LIFE_KEEP*0.5f;
				g_MessageBox[BoxID].posUp.x		= -g_MessageBox[BoxID].fWidth * 0.5f;
				g_MessageBox[BoxID].posDown.x	=  g_MessageBox[BoxID].fWidth * 0.5f;
				g_MessageBox[BoxID].phase		=  MESSAGE_BOX_PHASE_OPEN_KEEP;
			}

			break;
		case MESSAGE_BOX_PHASE_OPEN_KEEP:
			g_MessageBox[BoxID].nLife--;
			if(g_MessageBox[BoxID].nLife < 0)
			{
				g_MessageBox[BoxID].nLife = MESSAGE_MAX_LIFE;
				g_MessageBox[BoxID].phase		=  MESSAGE_BOX_PHASE_OPEN_Y;
			}
			break;
		case MESSAGE_BOX_PHASE_OPEN_Y:
			g_MessageBox[BoxID].nLife--;
			g_MessageBox[BoxID].posUp.y -= g_MessageBox[BoxID].move.y;
			g_MessageBox[BoxID].posDown.y += g_MessageBox[BoxID].move.y;
			if(g_MessageBox[BoxID].nLife < 0)
			{
				g_MessageBox[BoxID].nLife = MESSAGE_MAX_LIFE;
				g_MessageBox[BoxID].posUp.y		= -g_MessageBox[BoxID].fHeight * 0.5f;
				g_MessageBox[BoxID].posDown.y	=  g_MessageBox[BoxID].fHeight * 0.5f;
				g_MessageBox[BoxID].phase		=  MESSAGE_BOX_PHASE_KEEP00;
			}
			break;
		case MESSAGE_BOX_PHASE_KEEP00:
			//g_MessageBox[BoxID].nLife--;
			//if(g_MessageBox[BoxID].nLife < 0)
			//{
			//	g_MessageBox[BoxID].nLife = MESSAGE_MAX_LIFE;
			//	g_MessageBox[BoxID].phase		=  MESSAGE_BOX_PHASE_CLOSE_Y;
			//}

			break;
		case MESSAGE_BOX_PHASE_CLOSE_Y:
	
			g_MessageBox[BoxID].nLife--;
			g_MessageBox[BoxID].posUp.y += g_MessageBox[BoxID].move.y;
			g_MessageBox[BoxID].posDown.y -= g_MessageBox[BoxID].move.y;
			if(g_MessageBox[BoxID].nLife < 0)
			{
				g_MessageBox[BoxID].nLife = 0;//MESSAGE_MAX_LIFE_KEEP*0.5f;
				g_MessageBox[BoxID].posUp.y		=  0.0f;
				g_MessageBox[BoxID].posDown.y	=  0.0f;
				g_MessageBox[BoxID].phase		=  MESSAGE_BOX_PHASE_CLOSE_KEEP;
			}
			break;
		case MESSAGE_BOX_PHASE_CLOSE_KEEP:
			g_MessageBox[BoxID].nLife--;
			if(g_MessageBox[BoxID].nLife < 0)
			{
				g_MessageBox[BoxID].nLife = MESSAGE_MAX_LIFE;
				g_MessageBox[BoxID].phase		=  MESSAGE_BOX_PHASE_CLOSE_X;
			}
			break;
		case MESSAGE_BOX_PHASE_CLOSE_X:
			g_MessageBox[BoxID].nLife--;
			g_MessageBox[BoxID].posUp.x += g_MessageBox[BoxID].move.x;
			g_MessageBox[BoxID].posDown.x -= g_MessageBox[BoxID].move.x;
			if(g_MessageBox[BoxID].nLife < 0)
			{
				g_MessageBox[BoxID].nLife = MESSAGE_MAX_LIFE_KEEP*0.5f;
				g_MessageBox[BoxID].posUp.x		=  0.0f;
				g_MessageBox[BoxID].posDown.x	=  0.0f;
				g_MessageBox[BoxID].phase		=  MESSAGE_BOX_PHASE_KEEP01;
			}
			break;
		case MESSAGE_BOX_PHASE_KEEP01:
			g_MessageBox[BoxID].nLife--;
			if(g_MessageBox[BoxID].nLife < 0)
			{
				g_MessageBox[BoxID].phase		= MESSAGE_BOX_PHASE_INIT;
			}
			break;
		case MESSAGE_BOX_PHASE_INIT:
			InitMessageBoxSingle(BoxID);
			break;
		case MESSAGE_BOX_PHASE_MAX:
			break;
		}
		SetVertexMessageBox(BoxID);
	}
	for(int nWords = 0; nWords < MESSAGE_WORDS_MAX;nWords++)
	{
		if(g_MessageWords[nWords].bUse == false)
			continue;
		SetVertexMessageWords(nWords);
	}


}
int OpeBoxIDBox(D3DXVECTOR3 Centerpos,
					float		Widht,
					float		Height,
					MESSAGE_WORDS	MessageWordsID
					)
{
	int Idx = 0;
	for(int BoxID = 0;BoxID < MESSAGE_BOX_MAX;BoxID++)
	{
		if(g_MessageBox[BoxID].bUse == true)
			continue;
			g_MessageBox[BoxID].bUse = true;
			g_MessageBox[BoxID].fHeight = Height;
			g_MessageBox[BoxID].fWidth  = Widht;
			g_MessageBox[BoxID].posCenter = Centerpos;
			g_MessageBox[BoxID].MessageWordsID = MessageWordsID;
			g_MessageBox[BoxID].move = D3DXVECTOR3(Widht * 0.5f / MESSAGE_MAX_LIFE,
													  Height * 0.5f / MESSAGE_MAX_LIFE,
													  0.0f);
			g_MessageBox[BoxID].phase = MESSAGE_BOX_PHASE_START;
			Idx = BoxID;
			break;
	}

	if(MessageWordsID != MESSAGE_WORDS_MAX)
	{

		g_MessageWords[MessageWordsID].bUse = true;
		g_MessageWords[MessageWordsID].pos = Centerpos;
		g_MessageWords[MessageWordsID].MessageWordsID = MessageWordsID;
		g_MessageWords[MessageWordsID].fHeight	= MESSAGE_WORDS_SIZE;
		//switch ( MessageWordsID)
		//
		//case MESSAGE_WORDS_HOROBIYO:
			g_MessageWords[MessageWordsID].fWidth = MESSAGE_WORDS_SIZE * 5;
		//	break;
		//}
	}


	return Idx;

}

void CloseMessageBox(int BoxID)
{
	g_MessageBox[BoxID].phase = MESSAGE_BOX_PHASE_CLOSE_Y;
	InitMessageWordsSingle(g_MessageBox[BoxID].MessageWordsID);//一回のみ実行
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawMessageBox(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//フィールドの描画
	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffMessageBox,0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	
	for(int BoxID = 0;BoxID < MESSAGE_BOX_MAX;BoxID++)
	{
		if(g_MessageBox[BoxID].bUse == false)
			continue;
		// ポリゴンの描画
		pDevice->SetTexture(0,NULL);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, BoxID * 12, NUM_POLYGON);
		pDevice->SetTexture(0,g_pTextureMessageBox);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, BoxID * 12+4, NUM_POLYGON);
		
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, BoxID * 12+8, NUM_POLYGON);
	}

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffMessageWords,0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	
	for(int WordsID = 0;WordsID < MESSAGE_WORDS_MAX;WordsID++)
	{
		if(g_MessageWords[WordsID].bUse == false)
			continue;
		// ポリゴンの描画
		pDevice->SetTexture(0,g_pTextureMessageWords[g_MessageWords[WordsID].MessageWordsID]);

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, WordsID * 4, NUM_POLYGON);
	}
}
//=============================================================================
// Shining頂点の作成
//=============================================================================
HRESULT MakeVertexMessageBox(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * MESSAGE_BOX_MAX*3,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
		FVF_VERTEX_2D,						// 使用する頂点フォーマット
		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
		&g_pVtxBuffMessageBox,					// 頂点バッファインターフェースへのポインタ
		NULL)))								// NULLに設定
	{
		return E_FAIL;
	}
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffMessageBox->Lock(0, 0, (void**)&pVtx, 0);
			// 頂点座標の設定
		for(int BoxID = 0;BoxID < MESSAGE_BOX_MAX;BoxID++,pVtx += 4)
		{


		//=======================LeftSide
			pVtx[0].pos.x = g_MessageBox[BoxID].posCenter.x + g_MessageBox[BoxID].posUp.x - MESSAGE_BOX_SIZE_X;
			pVtx[0].pos.y = g_MessageBox[BoxID].posCenter.y + g_MessageBox[BoxID].posUp.y - MESSAGE_BOX_SIZE_Y;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_MessageBox[BoxID].posCenter.x + g_MessageBox[BoxID].posDown.x + MESSAGE_BOX_SIZE_X;
			pVtx[1].pos.y = g_MessageBox[BoxID].posCenter.y + g_MessageBox[BoxID].posUp.y - MESSAGE_BOX_SIZE_Y;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_MessageBox[BoxID].posCenter.x + g_MessageBox[BoxID].posUp.x - MESSAGE_BOX_SIZE_X;
			pVtx[2].pos.y =	g_MessageBox[BoxID].posCenter.y + g_MessageBox[BoxID].posDown.y + MESSAGE_BOX_SIZE_Y;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_MessageBox[BoxID].posCenter.x + g_MessageBox[BoxID].posDown.x + MESSAGE_BOX_SIZE_X;
			pVtx[3].pos.y = g_MessageBox[BoxID].posCenter.y + g_MessageBox[BoxID].posDown.y + MESSAGE_BOX_SIZE_Y;
			pVtx[3].pos.z = 0.0f;
			// rhwの設定
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
			pVtx += 4;
		//=======================LeftSide
			pVtx[0].pos.x = g_MessageBox[BoxID].posCenter.x + g_MessageBox[BoxID].posUp.x - MESSAGE_BOX_SIZE_X;
			pVtx[0].pos.y = g_MessageBox[BoxID].posCenter.y + g_MessageBox[BoxID].posUp.y - MESSAGE_BOX_SIZE_Y;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_MessageBox[BoxID].posCenter.x + g_MessageBox[BoxID].posUp.x + MESSAGE_BOX_SIZE_X;
			pVtx[1].pos.y = g_MessageBox[BoxID].posCenter.y + g_MessageBox[BoxID].posUp.y - MESSAGE_BOX_SIZE_Y;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_MessageBox[BoxID].posCenter.x + g_MessageBox[BoxID].posUp.x - MESSAGE_BOX_SIZE_X;
			pVtx[2].pos.y =	g_MessageBox[BoxID].posCenter.y + g_MessageBox[BoxID].posUp.y + MESSAGE_BOX_SIZE_Y;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_MessageBox[BoxID].posCenter.x + g_MessageBox[BoxID].posUp.x + MESSAGE_BOX_SIZE_X;
			pVtx[3].pos.y = g_MessageBox[BoxID].posCenter.y + g_MessageBox[BoxID].posUp.y + MESSAGE_BOX_SIZE_Y;
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
			pVtx[1].tex = D3DXVECTOR2(0.5f,0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.5f,1.0f);
			pVtx += 4;
		//=======================LeftSide
			pVtx[0].pos.x = g_MessageBox[BoxID].posCenter.x + g_MessageBox[BoxID].posDown.x - MESSAGE_BOX_SIZE_X;
			pVtx[0].pos.y = g_MessageBox[BoxID].posCenter.y + g_MessageBox[BoxID].posDown.y - MESSAGE_BOX_SIZE_Y;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_MessageBox[BoxID].posCenter.x + g_MessageBox[BoxID].posDown.x + MESSAGE_BOX_SIZE_X;
			pVtx[1].pos.y = g_MessageBox[BoxID].posCenter.y + g_MessageBox[BoxID].posDown.y - MESSAGE_BOX_SIZE_Y;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_MessageBox[BoxID].posCenter.x + g_MessageBox[BoxID].posDown.x - MESSAGE_BOX_SIZE_X;
			pVtx[2].pos.y =	g_MessageBox[BoxID].posCenter.y + g_MessageBox[BoxID].posDown.y + MESSAGE_BOX_SIZE_Y;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_MessageBox[BoxID].posCenter.x + g_MessageBox[BoxID].posDown.x + MESSAGE_BOX_SIZE_X;
			pVtx[3].pos.y = g_MessageBox[BoxID].posCenter.y + g_MessageBox[BoxID].posDown.y + MESSAGE_BOX_SIZE_Y;
			pVtx[3].pos.z = 0.0f;
			// rhwの設定
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

			pVtx[0].tex = D3DXVECTOR2(0.5f,0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.5f,1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);
		}

		// 頂点データをアンロックする
		g_pVtxBuffMessageBox->Unlock();
	}
	return S_OK;

}
//=============================================================================
// 頂点の設定
//=============================================================================
void SetVertexMessageBox(int BoxID)
{//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffMessageBox->Lock(0, 0, (void**)&pVtx, 0);
		// 頂点座標の設定
	pVtx += BoxID*12;
//=======================LeftSide
	pVtx[0].pos.x = g_MessageBox[BoxID].posCenter.x + g_MessageBox[BoxID].posUp.x - MESSAGE_BOX_SIZE_X;
	pVtx[0].pos.y = g_MessageBox[BoxID].posCenter.y + g_MessageBox[BoxID].posUp.y - MESSAGE_BOX_SIZE_Y + MESSAGE_BOX_SIZE_Y/2;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_MessageBox[BoxID].posCenter.x + g_MessageBox[BoxID].posDown.x + MESSAGE_BOX_SIZE_X;
	pVtx[1].pos.y = g_MessageBox[BoxID].posCenter.y + g_MessageBox[BoxID].posUp.y - MESSAGE_BOX_SIZE_Y + MESSAGE_BOX_SIZE_Y/2;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_MessageBox[BoxID].posCenter.x + g_MessageBox[BoxID].posUp.x - MESSAGE_BOX_SIZE_X;
	pVtx[2].pos.y =	g_MessageBox[BoxID].posCenter.y + g_MessageBox[BoxID].posDown.y + MESSAGE_BOX_SIZE_Y - MESSAGE_BOX_SIZE_Y/2;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_MessageBox[BoxID].posCenter.x + g_MessageBox[BoxID].posDown.x + MESSAGE_BOX_SIZE_X;
	pVtx[3].pos.y = g_MessageBox[BoxID].posCenter.y + g_MessageBox[BoxID].posDown.y + MESSAGE_BOX_SIZE_Y - MESSAGE_BOX_SIZE_Y/2;
	pVtx[3].pos.z = 0.0f;
	// rhwの設定
	// rhwの設定
	pVtx[0].rhw =
	pVtx[1].rhw =
	pVtx[2].rhw =
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
	pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);


	pVtx += 4;
		//=======================LeftSide
	pVtx[0].pos.x = g_MessageBox[BoxID].posCenter.x + g_MessageBox[BoxID].posUp.x - MESSAGE_BOX_SIZE_X;
	pVtx[0].pos.y = g_MessageBox[BoxID].posCenter.y + g_MessageBox[BoxID].posUp.y - MESSAGE_BOX_SIZE_Y;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_MessageBox[BoxID].posCenter.x + g_MessageBox[BoxID].posUp.x + MESSAGE_BOX_SIZE_X;
	pVtx[1].pos.y = g_MessageBox[BoxID].posCenter.y + g_MessageBox[BoxID].posUp.y - MESSAGE_BOX_SIZE_Y;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_MessageBox[BoxID].posCenter.x + g_MessageBox[BoxID].posUp.x - MESSAGE_BOX_SIZE_X;
	pVtx[2].pos.y =	g_MessageBox[BoxID].posCenter.y + g_MessageBox[BoxID].posUp.y + MESSAGE_BOX_SIZE_Y;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_MessageBox[BoxID].posCenter.x + g_MessageBox[BoxID].posUp.x + MESSAGE_BOX_SIZE_X;
	pVtx[3].pos.y = g_MessageBox[BoxID].posCenter.y + g_MessageBox[BoxID].posUp.y + MESSAGE_BOX_SIZE_Y;
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
	pVtx[1].tex = D3DXVECTOR2(0.5f,0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.5f,1.0f);

	pVtx += 4;


//=======================LeftSide
	pVtx[0].pos.x = g_MessageBox[BoxID].posCenter.x + g_MessageBox[BoxID].posDown.x - MESSAGE_BOX_SIZE_X;
	pVtx[0].pos.y = g_MessageBox[BoxID].posCenter.y + g_MessageBox[BoxID].posDown.y - MESSAGE_BOX_SIZE_Y;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_MessageBox[BoxID].posCenter.x + g_MessageBox[BoxID].posDown.x + MESSAGE_BOX_SIZE_X;
	pVtx[1].pos.y = g_MessageBox[BoxID].posCenter.y + g_MessageBox[BoxID].posDown.y - MESSAGE_BOX_SIZE_Y;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_MessageBox[BoxID].posCenter.x + g_MessageBox[BoxID].posDown.x - MESSAGE_BOX_SIZE_X;
	pVtx[2].pos.y =	g_MessageBox[BoxID].posCenter.y + g_MessageBox[BoxID].posDown.y + MESSAGE_BOX_SIZE_Y;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_MessageBox[BoxID].posCenter.x + g_MessageBox[BoxID].posDown.x + MESSAGE_BOX_SIZE_X;
	pVtx[3].pos.y = g_MessageBox[BoxID].posCenter.y + g_MessageBox[BoxID].posDown.y + MESSAGE_BOX_SIZE_Y;
	pVtx[3].pos.z = 0.0f;
	// rhwの設定
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

	pVtx[0].tex = D3DXVECTOR2(0.5f,0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.5f,1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

	// 頂点データをアンロックする
	g_pVtxBuffMessageBox->Unlock();
}
HRESULT MakeVertexMessageWords(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * MESSAGE_WORDS_MAX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
		FVF_VERTEX_2D,						// 使用する頂点フォーマット
		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
		&g_pVtxBuffMessageWords,					// 頂点バッファインターフェースへのポインタ
		NULL)))								// NULLに設定
	{
		return E_FAIL;
	}
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffMessageWords->Lock(0, 0, (void**)&pVtx, 0);
			// 頂点座標の設定
		for(int WordsID = 0;WordsID < MESSAGE_WORDS_MAX;WordsID++,pVtx += 4)
		{
		//=======================LeftSide
			pVtx[0].pos.x = g_MessageWords[WordsID].pos.x + g_MessageWords[WordsID].fWidth;
			pVtx[0].pos.y = g_MessageWords[WordsID].pos.y + g_MessageWords[WordsID].fHeight; 
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_MessageWords[WordsID].pos.x + g_MessageWords[WordsID].fWidth; 
			pVtx[1].pos.y = g_MessageWords[WordsID].pos.y + g_MessageWords[WordsID].fHeight; 
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_MessageWords[WordsID].pos.x + g_MessageWords[WordsID].fWidth; 
			pVtx[2].pos.y =	g_MessageWords[WordsID].pos.y + g_MessageWords[WordsID].fHeight; 
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_MessageWords[WordsID].pos.x + g_MessageWords[WordsID].fWidth; 
			pVtx[3].pos.y = g_MessageWords[WordsID].pos.y + g_MessageWords[WordsID].fHeight; 
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
	g_pVtxBuffMessageWords->Unlock();
	}
	return S_OK;
}
void SetVertexMessageWords(int WordsID)
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffMessageWords->Lock(0, 0, (void**)&pVtx, 0);
			// 頂点座標の設定
		pVtx += WordsID*4;

		//=======================v
			pVtx[0].pos.x = g_MessageWords[WordsID].pos.x - g_MessageWords[WordsID].fWidth/2;
			pVtx[0].pos.y = g_MessageWords[WordsID].pos.y - g_MessageWords[WordsID].fHeight; 
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_MessageWords[WordsID].pos.x + g_MessageWords[WordsID].fWidth/2; 
			pVtx[1].pos.y = g_MessageWords[WordsID].pos.y - g_MessageWords[WordsID].fHeight; 
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_MessageWords[WordsID].pos.x - g_MessageWords[WordsID].fWidth/2; 
			pVtx[2].pos.y =	g_MessageWords[WordsID].pos.y + g_MessageWords[WordsID].fHeight; 
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_MessageWords[WordsID].pos.x + g_MessageWords[WordsID].fWidth/2; 
			pVtx[3].pos.y = g_MessageWords[WordsID].pos.y + g_MessageWords[WordsID].fHeight; 
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
			// 頂点データをアンロックする
	g_pVtxBuffMessageWords->Unlock();
	}
}
