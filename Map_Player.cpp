//=============================================================================
//
// [Map_Player.cpp]
//
//=============================================================================
#include "Map_Main.h"
#include "main.h"
#include "Map_Player.h"
#include "input.h"
#include "Function.h"
//#include "fade.h"
#include "Battle_Main.h"
#include "Func_NumberX.h"

#define TEXTUREMAP_PLAYER	("data/TEXTURE/mouse_01.png")

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexMap_Player(LPDIRECT3DDEVICE9 pDevice);
void SetVerTexMap_Player(void);
void SetVerTexMap_Player(void);
//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_apTextureMap_Player = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMap_Player = NULL;// 頂点バッファインターフェースへのポインタ
MAP_PLAYER		g_MapPlayer;
D3DXVECTOR2		g_SpeedWk;
int				g_StageMasterData;
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitMap_Player(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	g_MapPlayer.DrawPos	= D3DXVECTOR3(0.0f,0.0f,0.0f);
	g_MapPlayer.fSpeed	= 0.0f;
	g_MapPlayer.Pos.nX	= 0;
	g_MapPlayer.Pos.nY	= 0;
	g_MapPlayer.Move	= D3DXVECTOR3(0.0f,0.0f,0.0f);
	g_SpeedWk			= D3DXVECTOR2(0.0f,0.0f);
	D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
								TEXTUREMAP_PLAYER,			// ファイルの名前
								&g_apTextureMap_Player);	// 読み込むメモリー
	MakeVertexMap_Player(pDevice);
	g_StageMasterData = 0;

		Set_NumberX(MAP_PLAYER_COORDNATION,	// 数列の名前
		true,									// 使用の状態設定
		false,									// Fade状態
		BASICALWB,								// 使用するテクスチャの名前
		4,										// 入力最大桁数
		00,										// 入力値
		15.0f,									// 文字組の大きさX
		30.0f,									// 文字組の大きさY
		1.0f,									// 透明度の設定
		// 描画開始位置
		D3DXVECTOR3(600.0f,100.0f,0.0f)
		);

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void UninitMap_Player(void)
{
	if(g_apTextureMap_Player != NULL)
	{// テクスチャの開放
		g_apTextureMap_Player->Release();
		g_apTextureMap_Player = NULL;
	}

	if(g_pVtxBuffMap_Player != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffMap_Player->Release();
		g_pVtxBuffMap_Player = NULL;
	}
}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateMap_Player(void)
{
	bool bMove = false;
	D3DXVECTOR3	*MapLimitPos = GetMapRangeLimit();
	FIELD_UNIT *pFieldUnit = GetFieldUnit();
	if(GetKeyboardPress(DIK_A))
	{
		g_SpeedWk.x	-= MAP_PLAYER_COS;

	}
	else if(GetKeyboardPress(DIK_D))
	{
		g_SpeedWk.x	+= MAP_PLAYER_COS;
	}
	else
	{
		g_SpeedWk.x *= 0.8f;
		if(g_SpeedWk.x > -0.1f && g_SpeedWk.x < 0.1f)
			g_SpeedWk.x = 0.0f;
	}

	if(GetKeyboardPress(DIK_W))
	{
		g_SpeedWk.y	-= MAP_PLAYER_COS;
	}
	else if(GetKeyboardPress(DIK_S))
	{
		g_SpeedWk.y	+= MAP_PLAYER_COS;
	}
	else
	{
		g_SpeedWk.y *= 0.8f;
		if(g_SpeedWk.y > -0.1f && g_SpeedWk.y < 0.1f)
			g_SpeedWk.y = 0.0f;
	}

	g_SpeedWk.x = SetMAX(g_SpeedWk.x,D3DX_PI * 0.5f,-D3DX_PI * 0.5f,2);
	g_SpeedWk.y = SetMAX(g_SpeedWk.y,D3DX_PI * 0.5f,-D3DX_PI * 0.5f,2);
	
	g_MapPlayer.Move.x = MAP_PALYER_SPEED * g_SpeedWk.x;
	g_MapPlayer.Move.y = MAP_PALYER_SPEED * g_SpeedWk.y;
	g_MapPlayer.DrawPos += g_MapPlayer.Move;
	g_MapPlayer.DrawPos.x = SetMAX(g_MapPlayer.DrawPos.x,MapLimitPos[1].x,MapLimitPos[0].x,2);
	g_MapPlayer.DrawPos.y = SetMAX(g_MapPlayer.DrawPos.y,MapLimitPos[1].y,MapLimitPos[0].y,2);
	g_MapPlayer.Pos.nX = (int)(g_MapPlayer.DrawPos.x / MAP_UNIT_SIZE - 1);
	g_MapPlayer.Pos.nY = (int)(g_MapPlayer.DrawPos.y / MAP_UNIT_SIZE - 1);
	int Unitype = (pFieldUnit + g_MapPlayer.Pos.nY)[g_MapPlayer.Pos.nX].nType;
	SetVerTexMap_Player();
	NUMBER_X *pNumberX = GetNumberX();
	pNumberX[MAP_PLAYER_COORDNATION].nNumber = g_MapPlayer.Pos.nY * 10 + g_MapPlayer.Pos.nX;

}
//=============================================================================
// 　プレイヤーの位置情報の取得
//=============================================================================
COORDINATION_INT GetMapPlayerPos(void)
{
	return g_MapPlayer.Pos;
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawMap_Player(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffMap_Player, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_apTextureMap_Player);

	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0 , NUM_POLYGON);
}
//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexMap_Player(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX *1,// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
												FVF_VERTEX_2D,						// 使用する頂点フォーマット
												D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
												&g_pVtxBuffMap_Player,					// 頂点バッファインターフェースへのポインタ
												NULL)))								// NULLに設定
	{
        return E_FAIL;
	}
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffMap_Player->Lock(0, 0, (void**)&pVtx, 0);


				// 頂点座標の設定
				pVtx[0].pos.x = g_MapPlayer.DrawPos.x;
				pVtx[0].pos.y = g_MapPlayer.DrawPos.y;
				pVtx[0].pos.z = 0.0f;

				pVtx[1].pos.x = g_MapPlayer.DrawPos.x + MAP_UNIT_SIZE;
				pVtx[1].pos.y = g_MapPlayer.DrawPos.y;
				pVtx[1].pos.z = 0.0f;

				pVtx[2].pos.x = g_MapPlayer.DrawPos.x ;//+ MAP_UNIT_SIZE;
				pVtx[2].pos.y = g_MapPlayer.DrawPos.y + MAP_UNIT_SIZE;
				pVtx[2].pos.z = 0.0f;
			
				pVtx[3].pos.x = g_MapPlayer.DrawPos.x + MAP_UNIT_SIZE;
				pVtx[3].pos.y = g_MapPlayer.DrawPos.y + MAP_UNIT_SIZE;
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
		g_pVtxBuffMap_Player->Unlock();
	}

	return S_OK;
}
//=============================================================================
// Fieldの設定
//=============================================================================
void SetVerTexMap_Player(void)
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffMap_Player->Lock(0, 0, (void**)&pVtx, 0);

				// 頂点座標の設定
				pVtx[0].pos.x = g_MapPlayer.DrawPos.x - MAP_PLAYER_SIZE;
				pVtx[0].pos.y = g_MapPlayer.DrawPos.y - MAP_PLAYER_SIZE;
				pVtx[0].pos.z = 0.0f;

				pVtx[1].pos.x = g_MapPlayer.DrawPos.x + MAP_PLAYER_SIZE;
				pVtx[1].pos.y = g_MapPlayer.DrawPos.y -	MAP_PLAYER_SIZE;
				pVtx[1].pos.z = 0.0f;

				pVtx[2].pos.x = g_MapPlayer.DrawPos.x - MAP_PLAYER_SIZE;
				pVtx[2].pos.y = g_MapPlayer.DrawPos.y + MAP_PLAYER_SIZE;
				pVtx[2].pos.z = 0.0f;
			
				pVtx[3].pos.x = g_MapPlayer.DrawPos.x + MAP_PLAYER_SIZE;
				pVtx[3].pos.y = g_MapPlayer.DrawPos.y + MAP_PLAYER_SIZE;
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
		g_pVtxBuffMap_Player->Unlock();
	}
}
