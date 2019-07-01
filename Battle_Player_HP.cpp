//====================================================
//Battle_Player_HP.cpp
//====================================================
#include "main.h"
#include "game.h"
#include "Battle_Player_HP.h"
#include "Function.h"
#include "Battle_Player.h"
#include "Battle_Main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexBattle_Player_HP(LPDIRECT3DDEVICE9 pDevice);
HRESULT MakeVertexBattle_Player_HP_NumRec(LPDIRECT3DDEVICE9 pDevice);
HRESULT MakeVertexBattle_Player_HP_NumMax(LPDIRECT3DDEVICE9 pDevice);
void SetVerTexBattle_Player_HP(void);
void SetVerTexBattle_Player_HP_NumRec(int ID);
void SetVerTexBattle_Player_HP_NumMAX(int ID);
void InitRecHPNumberSingle(int ID);
void InitMAXHPNumberSingle(int ID);
//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureBattle_Player_HP_Cage		= NULL;		// テクスチャへのポインタ
LPDIRECT3DTEXTURE9		g_pTextureBattle_Player_HP_Entity	= NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBattle_Player_HP_Cage		= NULL;		// 頂点バッファインターフェースへのポインタ

LPDIRECT3DTEXTURE9		g_pTextureBattle_Player_HP_Num	= NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBattle_Player_HP_NumRec	= NULL;		// 頂点バッファインターフェースへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBattle_Player_HP_NumMAX	= NULL;		// 頂点バッファインターフェースへのポインタ

//HP ケージに関する変数宣言
D3DXVECTOR3	Left_HP_Pos;
D3DXVECTOR3	Entity_HP_Pos;
D3DXVECTOR3 OutLine_HP_Pos;
D3DXVECTOR3	Right_HP_Pos;
int			nMaxHP;
float		fSize_per_HP;
int			nRecHp;
bool		bLight;
int			HPColorType;
D3DXVECTOR3	Right_Light_Pos;

D3DXVECTOR3 DamageShake;
int			ShakeTime;
//HPの数値に関する変数宣言
typedef struct
{
	bool		bUse;
	D3DXVECTOR3	DrawPos;
	float		SizeX;
	float		SizeY;
	int			nBox;
}HP_VALUE_D;
HP_VALUE_D g_PlayerRecHP[BATTLE_PLAYER_HP_NUMBER_DIGIT];
HP_VALUE_D g_PlayerMAXHP[BATTLE_PLAYER_HP_NUMBER_DIGIT];
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBattle_Player_HP(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXCreateTextureFromFile(pDevice,
	TEXTURE_PLAYER_HP_CAGE,
	&g_pTextureBattle_Player_HP_Cage);

	D3DXCreateTextureFromFile(pDevice,
	TEXTURE_PLAYER_HP_ENTITY,
	&g_pTextureBattle_Player_HP_Entity);

	D3DXCreateTextureFromFile(pDevice,
	TEXTURE_BATTLE_PLAYER_HP_NUMBER,
	&g_pTextureBattle_Player_HP_Num);

	Left_HP_Pos		= D3DXVECTOR3(BATTLE_PLAYER_HP_POS_X,BATTLE_PLAYER_HP_POS_Y,0.0f);
	Entity_HP_Pos	= D3DXVECTOR3(BATTLE_PLAYER_HP_ENTITY_POS_X,BATTLE_PLAYER_HP_ENTITY_POS_Y,0.0f);
	OutLine_HP_Pos	= D3DXVECTOR3(BATTLE_PLAYER_HP_ENTITY_POS_X,BATTLE_PLAYER_HP_POS_Y,0.0f);
	Right_HP_Pos	= D3DXVECTOR3(BATTLE_PLAYER_HP_RIGHT_POS,BATTLE_PLAYER_HP_POS_Y,0.0f);
	Right_Light_Pos	= D3DXVECTOR3(BATTLE_PLAYER_HP_POS_X,BATTLE_PLAYER_HP_ENTITY_POS_Y,0.0f);
	DamageShake		= D3DXVECTOR3(0.0f,0.0f,0.0f);
	ShakeTime		= 0;
	nMaxHP			= 0.0f;
	fSize_per_HP	= 0.0f;
	nRecHp			= 0.0f;
	bLight			= false;
	HPColorType		= 0;
	for(int nCntD = 0; nCntD< BATTLE_PLAYER_HP_NUMBER_DIGIT;nCntD ++)
	{
		InitRecHPNumberSingle(nCntD);
		InitMAXHPNumberSingle(nCntD);
	}



	MakeVertexBattle_Player_HP(pDevice);
	MakeVertexBattle_Player_HP_NumRec(pDevice);
	MakeVertexBattle_Player_HP_NumMax(pDevice);
	return S_OK;
}
void InitRecHPNumberSingle(int ID)
{
	g_PlayerRecHP[ID].bUse = false;
	g_PlayerRecHP[ID].SizeX = BATTLE_PLAYER_HP_NUMBER_SIZE_X;
	g_PlayerRecHP[ID].SizeY = BATTLE_PLAYER_HP_NUMBER_SIZE_Y;
	g_PlayerRecHP[ID].nBox	= 0;
	g_PlayerRecHP[ID].DrawPos = D3DXVECTOR3(BATTLE_PLAYER_HP_NUMBER_POS_X_REC - ID * (BATTLE_PLAYER_HP_NUMBER_SIZE_X + 2.0f),
											BATTLE_PLAYER_HP_NUMBER_POS_Y_REC,
											0.0f);
}

void InitMAXHPNumberSingle(int ID)
{
	g_PlayerMAXHP[ID].bUse = false;
	g_PlayerMAXHP[ID].SizeX = BATTLE_PLAYER_HP_NUMBER_SIZE_X;
	g_PlayerMAXHP[ID].SizeY = BATTLE_PLAYER_HP_NUMBER_SIZE_Y;
	g_PlayerMAXHP[ID].nBox	= 0;
	g_PlayerMAXHP[ID].DrawPos = D3DXVECTOR3(BATTLE_PLAYER_HP_NUMBER_POS_X_MAX - ID * BATTLE_PLAYER_HP_NUMBER_SIZE_X,
											BATTLE_PLAYER_HP_NUMBER_POS_Y_REC,
											0.0f);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBattle_Player_HP(void)
{
	if(g_pTextureBattle_Player_HP_Cage != NULL)
	{// テクスチャの開放
		g_pTextureBattle_Player_HP_Cage->Release();
		g_pTextureBattle_Player_HP_Cage = NULL;
	}
	if(g_pTextureBattle_Player_HP_Entity != NULL)
	{// テクスチャの開放
		g_pTextureBattle_Player_HP_Entity->Release();
		g_pTextureBattle_Player_HP_Entity = NULL;
	}
	if(g_pVtxBuffBattle_Player_HP_Cage != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffBattle_Player_HP_Cage->Release();
		g_pVtxBuffBattle_Player_HP_Cage = NULL;
	}


	if(g_pTextureBattle_Player_HP_Num != NULL)
	{// テクスチャの開放
		g_pTextureBattle_Player_HP_Num->Release();
		g_pTextureBattle_Player_HP_Num = NULL;
	}
	//Buff
	if(g_pVtxBuffBattle_Player_HP_NumRec != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffBattle_Player_HP_NumRec->Release();
		g_pVtxBuffBattle_Player_HP_NumRec = NULL;
	}
	if(g_pVtxBuffBattle_Player_HP_NumMAX != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffBattle_Player_HP_NumMAX->Release();
		g_pVtxBuffBattle_Player_HP_NumMAX = NULL;
	}
}
void SetBattle_Player_HP(void)
{
	BATTLE_PLAYER *pPlayer = GetBattlePlayer();

	nMaxHP			= pPlayer->fMaxHP;
	fSize_per_HP	= (float)BATTLE_PLAYER_HP_ENTITY_SIZE_X / (float)nMaxHP;
	nRecHp			= pPlayer->fRecHP;
	
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBattle_Player_HP(void)
{
	BATTLE_PLAYER *pPlayer = GetBattlePlayer();
	nRecHp			= pPlayer->fRecHP;
	nMaxHP			= pPlayer->fMaxHP;

	int nBoxRecWk[BATTLE_PLAYER_HP_NUMBER_DIGIT]	= {};
	int nBoxMAXWk[BATTLE_PLAYER_HP_NUMBER_DIGIT]	= {};
	bool DrawStartRec = false;
	bool DrawStartMAX = false;
	int	VisualRec = 0;
	int VisualMAX = 0;
	//=====================ワーク配列に値を入れる===============================//
	for(int nCntIdx = BATTLE_PLAYER_HP_NUMBER_DIGIT; nCntIdx> 0;nCntIdx --)
	{
		int Value = nRecHp % (int)(powf(10.0f,nCntIdx))/(int)(powf(10.0f,(nCntIdx-1)));
		if(Value != 0 )
			DrawStartRec = true;
		if(DrawStartRec == true)
		{
			nBoxRecWk[nCntIdx-1] = Value;
			VisualRec++;
		}
		else
		{
			nBoxRecWk[nCntIdx-1] = 99;
		}
	}
	for(int nCntIdx = BATTLE_PLAYER_HP_NUMBER_DIGIT; nCntIdx> 0;nCntIdx --)
	{
		int Value = nMaxHP % (int)(powf(10.0f,nCntIdx))/(int)(powf(10.0f,(nCntIdx-1)));
		if(Value != 0 )
			DrawStartMAX = true;
		if(DrawStartMAX == true)
		{
			nBoxMAXWk[nCntIdx-1] = Value;
			VisualMAX ++;
		}
		else
		{
			nBoxMAXWk[nCntIdx-1] = 99;
		}
	}
	//======================MAXHPの描画位置を再設定する==============================//
	for(int nCntD = 0; nCntD< VisualMAX;nCntD ++)
	{
		g_PlayerMAXHP[nCntD].DrawPos.x = BATTLE_PLAYER_HP_NUMBER_POS_X_REC + BATTLE_PLAYER_HP_NUMBER_SIZE_X * (VisualMAX+1)
										- nCntD * (BATTLE_PLAYER_HP_NUMBER_SIZE_X+2.0f);
	}
	//=======================それぞれのDの中に数値を入れ,ｂUseを決める=============================//
	for(int nCntD = 0; nCntD< BATTLE_PLAYER_HP_NUMBER_DIGIT;nCntD ++)
	{
		if(nBoxRecWk[nCntD] == 99)
		{
			g_PlayerRecHP[nCntD].bUse = false;
		}
		else
		{
			g_PlayerRecHP[nCntD].bUse = true;
			g_PlayerRecHP[nCntD].nBox = nBoxRecWk[nCntD];
			SetVerTexBattle_Player_HP_NumRec(nCntD);
		}

		if(nBoxMAXWk[nCntD] == 99)
		{
			g_PlayerMAXHP[nCntD].bUse = false;

		}
		else
		{
			g_PlayerMAXHP[nCntD].bUse = true;
			g_PlayerMAXHP[nCntD].nBox = nBoxMAXWk[nCntD];
			SetVerTexBattle_Player_HP_NumMAX(nCntD);
		}	
	}
	if(ShakeTime >0)
	{
		ShakeTime --;
		DamageShake.x = rand()%10 -5 ;
		DamageShake.y = rand()%10 -5 ;
	}
	else
	{
		DamageShake		= D3DXVECTOR3(0.0f,0.0f,0.0f);
	}




	//=====================HPの色を決める===============================//
	HPColorType = (int)((nRecHp-2) / (nMaxHP /3));
	HPColorType = SetMAX(HPColorType,2,0,2);
	SetVerTexBattle_Player_HP();
}
void SetBattle_Player_HP_Shake(void)
{
	ShakeTime = BATTLE_PLAYER_HP_SHAKE_TIME;
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBattle_Player_HP(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//フィールドの描画
	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffBattle_Player_HP_Cage, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetTexture(0,g_pTextureBattle_Player_HP_Cage);
	// ポリゴンの描画
	//left
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	//bg
	pDevice->SetTexture(0,NULL);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, NUM_POLYGON);

	//entity
	pDevice->SetTexture(0,g_pTextureBattle_Player_HP_Entity);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 8, NUM_POLYGON);

	//outLine
	pDevice->SetTexture(0,g_pTextureBattle_Player_HP_Cage);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 12, NUM_POLYGON);
	//Right
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 16, NUM_POLYGON);
	//light
	if(bLight == true)
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 20, NUM_POLYGON);


	//数字RECの描画
	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffBattle_Player_HP_NumRec, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetTexture(0,g_pTextureBattle_Player_HP_Num);
	// ポリゴンの描画
	for(int nCntD = 0; nCntD< BATTLE_PLAYER_HP_NUMBER_DIGIT;nCntD ++)
	{
		if(g_PlayerRecHP[nCntD].bUse == false)
			continue;
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntD * 4, NUM_POLYGON);
	}

	//数字MAXの描画
	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffBattle_Player_HP_NumMAX, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetTexture(0,g_pTextureBattle_Player_HP_Num);
	// ポリゴンの描画
	for(int nCntD = 0; nCntD< BATTLE_PLAYER_HP_NUMBER_DIGIT;nCntD ++)
	{
		if(g_PlayerMAXHP[nCntD].bUse == false)
			continue;
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntD * 4, NUM_POLYGON);
	}


}
//=============================================================================
// 頂点の設定
//=============================================================================
HRESULT MakeVertexBattle_Player_HP_NumRec(LPDIRECT3DDEVICE9 pDevice)
{
// オブジェクトの頂点バッファを生成
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * BATTLE_PLAYER_HP_NUMBER_DIGIT,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
		FVF_VERTEX_2D,						// 使用する頂点フォーマット
		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
		&g_pVtxBuffBattle_Player_HP_NumRec,			// 頂点バッファインターフェースへのポインタ
		NULL)))								// NULLに設定
	{
		return E_FAIL;
	}
	{	//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffBattle_Player_HP_NumRec->Lock(0, 0, (void**)&pVtx, 0);
		for(int nCntD = 0; nCntD< BATTLE_PLAYER_HP_NUMBER_DIGIT;nCntD ++,pVtx += 4)
			{
				// 頂点座標の設定
				pVtx[0].pos.x = g_PlayerRecHP[nCntD].DrawPos.x; 
				pVtx[0].pos.y =	g_PlayerRecHP[nCntD].DrawPos.y - BATTLE_PLAYER_HP_NUMBER_SIZE_Y;
				pVtx[0].pos.z = 0.0f;

				pVtx[1].pos.x = g_PlayerRecHP[nCntD].DrawPos.x + BATTLE_PLAYER_HP_NUMBER_SIZE_X; 
				pVtx[1].pos.y =	g_PlayerRecHP[nCntD].DrawPos.y - BATTLE_PLAYER_HP_NUMBER_SIZE_Y;
				pVtx[1].pos.z = 0.0f;

				pVtx[2].pos.x = g_PlayerRecHP[nCntD].DrawPos.x; 
				pVtx[2].pos.y =	g_PlayerRecHP[nCntD].DrawPos.y;
				pVtx[2].pos.z = 0.0f;

				pVtx[3].pos.x = g_PlayerRecHP[nCntD].DrawPos.x + BATTLE_PLAYER_HP_NUMBER_SIZE_X; 
				pVtx[3].pos.y =	g_PlayerRecHP[nCntD].DrawPos.y;
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

				int nBox = g_PlayerRecHP[nCntD].nBox;
				pVtx[0].tex = D3DXVECTOR2(BATTLE_PLAYER_HP_NUMBER_UNIT_SIZE * nBox,0.0f);
				pVtx[1].tex = D3DXVECTOR2(BATTLE_PLAYER_HP_NUMBER_UNIT_SIZE * nBox + BATTLE_PLAYER_HP_NUMBER_UNIT_SIZE,0.0f);
				pVtx[2].tex = D3DXVECTOR2(BATTLE_PLAYER_HP_NUMBER_UNIT_SIZE * nBox,1.0f);
				pVtx[3].tex = D3DXVECTOR2(BATTLE_PLAYER_HP_NUMBER_UNIT_SIZE * nBox + BATTLE_PLAYER_HP_NUMBER_UNIT_SIZE,1.0f);
		}
		// 頂点データをアンロックする
		g_pVtxBuffBattle_Player_HP_NumRec->Unlock();
	}
	return S_OK;
}
HRESULT MakeVertexBattle_Player_HP_NumMax(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * BATTLE_PLAYER_HP_NUMBER_DIGIT,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
		FVF_VERTEX_2D,						// 使用する頂点フォーマット
		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
		&g_pVtxBuffBattle_Player_HP_NumMAX,			// 頂点バッファインターフェースへのポインタ
		NULL)))								// NULLに設定
	{
		return E_FAIL;
	}
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffBattle_Player_HP_NumMAX->Lock(0, 0, (void**)&pVtx, 0);
		for(int nCntD = 0; nCntD< BATTLE_PLAYER_HP_NUMBER_DIGIT;nCntD ++,pVtx += 4)
			{
				// 頂点座標の設定
				pVtx[0].pos.x = g_PlayerMAXHP[nCntD].DrawPos.x; 
				pVtx[0].pos.y =	g_PlayerMAXHP[nCntD].DrawPos.y - BATTLE_PLAYER_HP_NUMBER_SIZE_Y;
				pVtx[0].pos.z = 0.0f;

				pVtx[1].pos.x = g_PlayerMAXHP[nCntD].DrawPos.x + BATTLE_PLAYER_HP_NUMBER_SIZE_X; 
				pVtx[1].pos.y =	g_PlayerMAXHP[nCntD].DrawPos.y - BATTLE_PLAYER_HP_NUMBER_SIZE_Y;
				pVtx[1].pos.z = 0.0f;

				pVtx[2].pos.x = g_PlayerMAXHP[nCntD].DrawPos.x; 
				pVtx[2].pos.y =	g_PlayerMAXHP[nCntD].DrawPos.y;
				pVtx[2].pos.z = 0.0f;

				pVtx[3].pos.x = g_PlayerMAXHP[nCntD].DrawPos.x + BATTLE_PLAYER_HP_NUMBER_SIZE_X; 
				pVtx[3].pos.y =	g_PlayerMAXHP[nCntD].DrawPos.y;
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

				int nBox = g_PlayerRecHP[nCntD].nBox;
				pVtx[0].tex = D3DXVECTOR2(BATTLE_PLAYER_HP_NUMBER_UNIT_SIZE * nBox,0.0f);
				pVtx[1].tex = D3DXVECTOR2(BATTLE_PLAYER_HP_NUMBER_UNIT_SIZE * nBox + BATTLE_PLAYER_HP_NUMBER_UNIT_SIZE,0.0f);
				pVtx[2].tex = D3DXVECTOR2(BATTLE_PLAYER_HP_NUMBER_UNIT_SIZE * nBox,1.0f);
				pVtx[3].tex = D3DXVECTOR2(BATTLE_PLAYER_HP_NUMBER_UNIT_SIZE * nBox + BATTLE_PLAYER_HP_NUMBER_UNIT_SIZE,1.0f);
		}
		// 頂点データをアンロックする
		g_pVtxBuffBattle_Player_HP_NumMAX->Unlock();
	}
	return S_OK;
}
void SetVerTexBattle_Player_HP_NumRec(int ID)
{
	//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffBattle_Player_HP_NumRec->Lock(0, 0, (void**)&pVtx, 0);
	pVtx += ID * 4;

		// 頂点座標の設定
		pVtx[0].pos.x = g_PlayerRecHP[ID].DrawPos.x; 
		pVtx[0].pos.y =	g_PlayerRecHP[ID].DrawPos.y - BATTLE_PLAYER_HP_NUMBER_SIZE_Y*1.2;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_PlayerRecHP[ID].DrawPos.x + BATTLE_PLAYER_HP_NUMBER_SIZE_X*1.2; 
		pVtx[1].pos.y =	g_PlayerRecHP[ID].DrawPos.y - BATTLE_PLAYER_HP_NUMBER_SIZE_Y*1.2;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_PlayerRecHP[ID].DrawPos.x; 
		pVtx[2].pos.y =	g_PlayerRecHP[ID].DrawPos.y;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_PlayerRecHP[ID].DrawPos.x + BATTLE_PLAYER_HP_NUMBER_SIZE_X*1.2; 
		pVtx[3].pos.y =	g_PlayerRecHP[ID].DrawPos.y;
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

		int nBox = g_PlayerRecHP[ID].nBox;
		pVtx[0].tex = D3DXVECTOR2(BATTLE_PLAYER_HP_NUMBER_UNIT_SIZE * nBox,0.0f);
		pVtx[1].tex = D3DXVECTOR2(BATTLE_PLAYER_HP_NUMBER_UNIT_SIZE * nBox + BATTLE_PLAYER_HP_NUMBER_UNIT_SIZE,0.0f);
		pVtx[2].tex = D3DXVECTOR2(BATTLE_PLAYER_HP_NUMBER_UNIT_SIZE * nBox,1.0f);
		pVtx[3].tex = D3DXVECTOR2(BATTLE_PLAYER_HP_NUMBER_UNIT_SIZE * nBox + BATTLE_PLAYER_HP_NUMBER_UNIT_SIZE,1.0f);
		// 頂点データをアンロックする
	g_pVtxBuffBattle_Player_HP_NumRec->Unlock();
}
void SetVerTexBattle_Player_HP_NumMAX(int ID)
{
	//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffBattle_Player_HP_NumMAX->Lock(0, 0, (void**)&pVtx, 0);
	pVtx += ID * 4;

		// 頂点座標の設定
		pVtx[0].pos.x = g_PlayerMAXHP[ID].DrawPos.x; 
		pVtx[0].pos.y =	g_PlayerMAXHP[ID].DrawPos.y - BATTLE_PLAYER_HP_NUMBER_SIZE_Y;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_PlayerMAXHP[ID].DrawPos.x + BATTLE_PLAYER_HP_NUMBER_SIZE_X; 
		pVtx[1].pos.y =	g_PlayerMAXHP[ID].DrawPos.y - BATTLE_PLAYER_HP_NUMBER_SIZE_Y;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_PlayerMAXHP[ID].DrawPos.x; 
		pVtx[2].pos.y =	g_PlayerMAXHP[ID].DrawPos.y;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_PlayerMAXHP[ID].DrawPos.x + BATTLE_PLAYER_HP_NUMBER_SIZE_X; 
		pVtx[3].pos.y =	g_PlayerMAXHP[ID].DrawPos.y;
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

		int nBox = g_PlayerMAXHP[ID].nBox;
		pVtx[0].tex = D3DXVECTOR2(BATTLE_PLAYER_HP_NUMBER_UNIT_SIZE * nBox,0.0f);
		pVtx[1].tex = D3DXVECTOR2(BATTLE_PLAYER_HP_NUMBER_UNIT_SIZE * nBox + BATTLE_PLAYER_HP_NUMBER_UNIT_SIZE,0.0f);
		pVtx[2].tex = D3DXVECTOR2(BATTLE_PLAYER_HP_NUMBER_UNIT_SIZE * nBox,1.0f);
		pVtx[3].tex = D3DXVECTOR2(BATTLE_PLAYER_HP_NUMBER_UNIT_SIZE * nBox + BATTLE_PLAYER_HP_NUMBER_UNIT_SIZE,1.0f);
		// 頂点データをアンロックする
	g_pVtxBuffBattle_Player_HP_NumMAX->Unlock();
}
HRESULT MakeVertexBattle_Player_HP(LPDIRECT3DDEVICE9 pDevice)
{
// オブジェクトの頂点バッファを生成
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * 6,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
		FVF_VERTEX_2D,						// 使用する頂点フォーマット
		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
		&g_pVtxBuffBattle_Player_HP_Cage,			// 頂点バッファインターフェースへのポインタ
		NULL)))								// NULLに設定
	{
		return E_FAIL;
	}
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffBattle_Player_HP_Cage->Lock(0, 0, (void**)&pVtx, 0);

		//Leftside
		// 頂点座標の設定
		pVtx[0].pos.x = Left_HP_Pos.x; 
		pVtx[0].pos.y =	Left_HP_Pos.y - BATTLE_PLAYER_HP_SIZE;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = Left_HP_Pos.x + BATTLE_PLAYER_HP_SIZE; 
		pVtx[1].pos.y =	Left_HP_Pos.y - BATTLE_PLAYER_HP_SIZE;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = Left_HP_Pos.x; 
		pVtx[2].pos.y =	Left_HP_Pos.y;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = Left_HP_Pos.x + BATTLE_PLAYER_HP_SIZE; 
		pVtx[3].pos.y =	Left_HP_Pos.y;
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


		pVtx[0].tex = D3DXVECTOR2(TEXTURE_PLAYER_HP_CAGE_UNIT_SIZE * 0,0.0f);
		pVtx[1].tex = D3DXVECTOR2(TEXTURE_PLAYER_HP_CAGE_UNIT_SIZE * 0 + TEXTURE_PLAYER_HP_CAGE_UNIT_SIZE,0.0f);
		pVtx[2].tex = D3DXVECTOR2(TEXTURE_PLAYER_HP_CAGE_UNIT_SIZE * 0,1.0f);
		pVtx[3].tex = D3DXVECTOR2(TEXTURE_PLAYER_HP_CAGE_UNIT_SIZE * 0 + TEXTURE_PLAYER_HP_CAGE_UNIT_SIZE,1.0f);

pVtx+= 4;
		//BG
		// 頂点座標の設定
		pVtx[0].pos.x = Entity_HP_Pos.x; 
		pVtx[0].pos.y =	Entity_HP_Pos.y - BATTLE_PLAYER_HP_ENTITY_SIZE_Y;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = Entity_HP_Pos.x + BATTLE_PLAYER_HP_ENTITY_SIZE_X; 
		pVtx[1].pos.y =	Entity_HP_Pos.y - BATTLE_PLAYER_HP_ENTITY_SIZE_Y;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = Entity_HP_Pos.x; 
		pVtx[2].pos.y =	Entity_HP_Pos.y;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = Entity_HP_Pos.x + BATTLE_PLAYER_HP_ENTITY_SIZE_X; 
		pVtx[3].pos.y =	Entity_HP_Pos.y;
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

pVtx+= 4;
		//Entity
		// 頂点座標の設定
		pVtx[0].pos.x = Entity_HP_Pos.x; 
		pVtx[0].pos.y =	Entity_HP_Pos.y - BATTLE_PLAYER_HP_ENTITY_SIZE_Y;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = Entity_HP_Pos.x + nRecHp * fSize_per_HP; 
		pVtx[1].pos.y =	Entity_HP_Pos.y - BATTLE_PLAYER_HP_ENTITY_SIZE_Y;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = Entity_HP_Pos.x; 
		pVtx[2].pos.y =	Entity_HP_Pos.y;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = Entity_HP_Pos.x + nRecHp * fSize_per_HP; 
		pVtx[3].pos.y =	Entity_HP_Pos.y;
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

		pVtx[0].tex = D3DXVECTOR2(TEXTURE_PLAYER_HP_ENTITY_UNIT_SIZE * 0,0.0f);
		pVtx[1].tex = D3DXVECTOR2(TEXTURE_PLAYER_HP_ENTITY_UNIT_SIZE * 0 + TEXTURE_PLAYER_HP_ENTITY_UNIT_SIZE,0.0f);
		pVtx[2].tex = D3DXVECTOR2(TEXTURE_PLAYER_HP_ENTITY_UNIT_SIZE * 0,1.0f);
		pVtx[3].tex = D3DXVECTOR2(TEXTURE_PLAYER_HP_ENTITY_UNIT_SIZE * 0 + TEXTURE_PLAYER_HP_ENTITY_UNIT_SIZE,1.0f);

pVtx+= 4;
		//Outline
		// 頂点座標の設定
		pVtx[0].pos.x = OutLine_HP_Pos.x; 
		pVtx[0].pos.y =	OutLine_HP_Pos.y - BATTLE_PLAYER_HP_SIZE;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = OutLine_HP_Pos.x + BATTLE_PLAYER_HP_ENTITY_SIZE_X; 
		pVtx[1].pos.y =	OutLine_HP_Pos.y - BATTLE_PLAYER_HP_SIZE;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = OutLine_HP_Pos.x; 
		pVtx[2].pos.y =	OutLine_HP_Pos.y;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = OutLine_HP_Pos.x + BATTLE_PLAYER_HP_ENTITY_SIZE_X; 
		pVtx[3].pos.y =	OutLine_HP_Pos.y;
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

		pVtx[0].tex = D3DXVECTOR2(TEXTURE_PLAYER_HP_ENTITY_UNIT_SIZE * 1,0.0f);
		pVtx[1].tex = D3DXVECTOR2(TEXTURE_PLAYER_HP_ENTITY_UNIT_SIZE * 1 + TEXTURE_PLAYER_HP_ENTITY_UNIT_SIZE,0.0f);
		pVtx[2].tex = D3DXVECTOR2(TEXTURE_PLAYER_HP_ENTITY_UNIT_SIZE * 1,1.0f);
		pVtx[3].tex = D3DXVECTOR2(TEXTURE_PLAYER_HP_ENTITY_UNIT_SIZE * 1 + TEXTURE_PLAYER_HP_ENTITY_UNIT_SIZE,1.0f);

pVtx+= 4;
		//Right
		// 頂点座標の設定
		pVtx[0].pos.x = Right_HP_Pos.x; 
		pVtx[0].pos.y =	Right_HP_Pos.y - BATTLE_PLAYER_HP_SIZE;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = Right_HP_Pos.x + BATTLE_PLAYER_HP_SIZE; 
		pVtx[1].pos.y =	Right_HP_Pos.y - BATTLE_PLAYER_HP_SIZE;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = Right_HP_Pos.x; 
		pVtx[2].pos.y =	Right_HP_Pos.y;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = Right_HP_Pos.x + BATTLE_PLAYER_HP_SIZE; 
		pVtx[3].pos.y =	Right_HP_Pos.y;
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

		pVtx[0].tex = D3DXVECTOR2(TEXTURE_PLAYER_HP_ENTITY_UNIT_SIZE * 2,0.0f);
		pVtx[1].tex = D3DXVECTOR2(TEXTURE_PLAYER_HP_ENTITY_UNIT_SIZE * 2 + TEXTURE_PLAYER_HP_ENTITY_UNIT_SIZE,0.0f);
		pVtx[2].tex = D3DXVECTOR2(TEXTURE_PLAYER_HP_ENTITY_UNIT_SIZE * 2,1.0f);
		pVtx[3].tex = D3DXVECTOR2(TEXTURE_PLAYER_HP_ENTITY_UNIT_SIZE * 2 + TEXTURE_PLAYER_HP_ENTITY_UNIT_SIZE,1.0f);
pVtx+= 4;
		//Light
		// 頂点座標の設定
		pVtx[0].pos.x = Right_Light_Pos.x; 
		pVtx[0].pos.y =	Right_Light_Pos.y - BATTLE_PLAYER_HP_ENTITY_SIZE_Y;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = Right_Light_Pos.x + BATTLE_PLAYER_HP_SIZE; 
		pVtx[1].pos.y =	Right_Light_Pos.y - BATTLE_PLAYER_HP_ENTITY_SIZE_Y;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = Right_Light_Pos.x; 
		pVtx[2].pos.y =	Right_Light_Pos.y;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = Right_Light_Pos.x + BATTLE_PLAYER_HP_SIZE; 
		pVtx[3].pos.y =	Right_Light_Pos.y;
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

		pVtx[0].tex = D3DXVECTOR2(TEXTURE_PLAYER_HP_ENTITY_UNIT_SIZE * 3,0.0f);
		pVtx[1].tex = D3DXVECTOR2(TEXTURE_PLAYER_HP_ENTITY_UNIT_SIZE * 3 + TEXTURE_PLAYER_HP_ENTITY_UNIT_SIZE,0.0f);
		pVtx[2].tex = D3DXVECTOR2(TEXTURE_PLAYER_HP_ENTITY_UNIT_SIZE * 3,1.0f);
		pVtx[3].tex = D3DXVECTOR2(TEXTURE_PLAYER_HP_ENTITY_UNIT_SIZE * 3 + TEXTURE_PLAYER_HP_ENTITY_UNIT_SIZE,1.0f);
		// 頂点データをアンロックする
		g_pVtxBuffBattle_Player_HP_Cage->Unlock();
	}
	return S_OK;
}
void SetVerTexBattle_Player_HP(void)

{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffBattle_Player_HP_Cage->Lock(0, 0, (void**)&pVtx, 0);

		//Leftside
		// 頂点座標の設定
		pVtx[0].pos.x = Left_HP_Pos.x+ DamageShake.x; 
		pVtx[0].pos.y =	Left_HP_Pos.y - BATTLE_PLAYER_HP_SIZE+ DamageShake.x;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = Left_HP_Pos.x + BATTLE_PLAYER_HP_SIZE+ DamageShake.x; 
		pVtx[1].pos.y =	Left_HP_Pos.y - BATTLE_PLAYER_HP_SIZE+ DamageShake.x;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = Left_HP_Pos.x+ DamageShake.x; 
		pVtx[2].pos.y =	Left_HP_Pos.y+ DamageShake.x;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = Left_HP_Pos.x + BATTLE_PLAYER_HP_SIZE+ DamageShake.x; 
		pVtx[3].pos.y =	Left_HP_Pos.y+ DamageShake.x;
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


		pVtx[0].tex = D3DXVECTOR2(TEXTURE_PLAYER_HP_CAGE_UNIT_SIZE * 0,0.0f);
		pVtx[1].tex = D3DXVECTOR2(TEXTURE_PLAYER_HP_CAGE_UNIT_SIZE * 0 + TEXTURE_PLAYER_HP_CAGE_UNIT_SIZE,0.0f);
		pVtx[2].tex = D3DXVECTOR2(TEXTURE_PLAYER_HP_CAGE_UNIT_SIZE * 0,1.0f);
		pVtx[3].tex = D3DXVECTOR2(TEXTURE_PLAYER_HP_CAGE_UNIT_SIZE * 0 + TEXTURE_PLAYER_HP_CAGE_UNIT_SIZE,1.0f);

pVtx+= 4;
		//BG
		// 頂点座標の設定
		pVtx[0].pos.x = Entity_HP_Pos.x+ DamageShake.x; 
		pVtx[0].pos.y =	Entity_HP_Pos.y - BATTLE_PLAYER_HP_ENTITY_SIZE_Y+ DamageShake.x;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = Entity_HP_Pos.x + BATTLE_PLAYER_HP_ENTITY_SIZE_X+ DamageShake.x; 
		pVtx[1].pos.y =	Entity_HP_Pos.y - BATTLE_PLAYER_HP_ENTITY_SIZE_Y+ DamageShake.x;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = Entity_HP_Pos.x+ DamageShake.x; 
		pVtx[2].pos.y =	Entity_HP_Pos.y+ DamageShake.x;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = Entity_HP_Pos.x + BATTLE_PLAYER_HP_ENTITY_SIZE_X+ DamageShake.x; 
		pVtx[3].pos.y =	Entity_HP_Pos.y+ DamageShake.x;
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

pVtx+= 4;
		//Entity
		// 頂点座標の設定
		pVtx[0].pos.x = Entity_HP_Pos.x+ DamageShake.x; 
		pVtx[0].pos.y =	Entity_HP_Pos.y - BATTLE_PLAYER_HP_ENTITY_SIZE_Y+ DamageShake.x;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = Entity_HP_Pos.x + nRecHp * fSize_per_HP+ DamageShake.x; 
		pVtx[1].pos.y =	Entity_HP_Pos.y - BATTLE_PLAYER_HP_ENTITY_SIZE_Y+ DamageShake.x;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = Entity_HP_Pos.x+ DamageShake.x; 
		pVtx[2].pos.y =	Entity_HP_Pos.y+ DamageShake.x;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = Entity_HP_Pos.x + nRecHp * fSize_per_HP+ DamageShake.x; 
		pVtx[3].pos.y =	Entity_HP_Pos.y+ DamageShake.x;
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

		pVtx[0].tex = D3DXVECTOR2(TEXTURE_PLAYER_HP_ENTITY_UNIT_SIZE * HPColorType,0.0f);
		pVtx[1].tex = D3DXVECTOR2(TEXTURE_PLAYER_HP_ENTITY_UNIT_SIZE * HPColorType + TEXTURE_PLAYER_HP_ENTITY_UNIT_SIZE,0.0f);
		pVtx[2].tex = D3DXVECTOR2(TEXTURE_PLAYER_HP_ENTITY_UNIT_SIZE * HPColorType,1.0f);
		pVtx[3].tex = D3DXVECTOR2(TEXTURE_PLAYER_HP_ENTITY_UNIT_SIZE * HPColorType + TEXTURE_PLAYER_HP_ENTITY_UNIT_SIZE,1.0f);

pVtx+= 4;
		//Outline
		// 頂点座標の設定
		pVtx[0].pos.x = OutLine_HP_Pos.x+ DamageShake.x; 
		pVtx[0].pos.y =	OutLine_HP_Pos.y - BATTLE_PLAYER_HP_SIZE+ DamageShake.x;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = OutLine_HP_Pos.x + BATTLE_PLAYER_HP_ENTITY_SIZE_X+ DamageShake.x; 
		pVtx[1].pos.y =	OutLine_HP_Pos.y - BATTLE_PLAYER_HP_SIZE+ DamageShake.x;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = OutLine_HP_Pos.x+ DamageShake.x; 
		pVtx[2].pos.y =	OutLine_HP_Pos.y+ DamageShake.x;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = OutLine_HP_Pos.x + BATTLE_PLAYER_HP_ENTITY_SIZE_X+ DamageShake.x; 
		pVtx[3].pos.y =	OutLine_HP_Pos.y+ DamageShake.x;
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

		pVtx[0].tex = D3DXVECTOR2(TEXTURE_PLAYER_HP_CAGE_UNIT_SIZE * 1,0.0f);
		pVtx[1].tex = D3DXVECTOR2(TEXTURE_PLAYER_HP_CAGE_UNIT_SIZE * 1 + TEXTURE_PLAYER_HP_CAGE_UNIT_SIZE,0.0f);
		pVtx[2].tex = D3DXVECTOR2(TEXTURE_PLAYER_HP_CAGE_UNIT_SIZE * 1,1.0f);
		pVtx[3].tex = D3DXVECTOR2(TEXTURE_PLAYER_HP_CAGE_UNIT_SIZE * 1 + TEXTURE_PLAYER_HP_CAGE_UNIT_SIZE,1.0f);

pVtx+= 4;
		//Right
		// 頂点座標の設定
		pVtx[0].pos.x = Right_HP_Pos.x+ DamageShake.x; 
		pVtx[0].pos.y =	Right_HP_Pos.y - BATTLE_PLAYER_HP_SIZE+ DamageShake.x;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = Right_HP_Pos.x + BATTLE_PLAYER_HP_SIZE+ DamageShake.x; 
		pVtx[1].pos.y =	Right_HP_Pos.y - BATTLE_PLAYER_HP_SIZE+ DamageShake.x;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = Right_HP_Pos.x+ DamageShake.x; 
		pVtx[2].pos.y =	Right_HP_Pos.y+ DamageShake.x;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = Right_HP_Pos.x + BATTLE_PLAYER_HP_SIZE; 
		pVtx[3].pos.y =	Right_HP_Pos.y+ DamageShake.x;
		pVtx[3].pos.z = 0.0f+ DamageShake.x;

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

		pVtx[0].tex = D3DXVECTOR2(TEXTURE_PLAYER_HP_CAGE_UNIT_SIZE * 2,0.0f);
		pVtx[1].tex = D3DXVECTOR2(TEXTURE_PLAYER_HP_CAGE_UNIT_SIZE * 2 + TEXTURE_PLAYER_HP_CAGE_UNIT_SIZE,0.0f);
		pVtx[2].tex = D3DXVECTOR2(TEXTURE_PLAYER_HP_CAGE_UNIT_SIZE * 2,1.0f);
		pVtx[3].tex = D3DXVECTOR2(TEXTURE_PLAYER_HP_CAGE_UNIT_SIZE * 2 + TEXTURE_PLAYER_HP_CAGE_UNIT_SIZE,1.0f);
pVtx+= 4;
		//Light
		// 頂点座標の設定
		pVtx[0].pos.x = Right_Light_Pos.x+ DamageShake.x; 
		pVtx[0].pos.y =	Right_Light_Pos.y - BATTLE_PLAYER_HP_ENTITY_SIZE_Y+ DamageShake.x;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = Right_Light_Pos.x + BATTLE_PLAYER_HP_SIZE+ DamageShake.x; 
		pVtx[1].pos.y =	Right_Light_Pos.y - BATTLE_PLAYER_HP_ENTITY_SIZE_Y+ DamageShake.x;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = Right_Light_Pos.x+ DamageShake.x; 
		pVtx[2].pos.y =	Right_Light_Pos.y+ DamageShake.x;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = Right_Light_Pos.x + BATTLE_PLAYER_HP_SIZE+ DamageShake.x; 
		pVtx[3].pos.y =	Right_Light_Pos.y + DamageShake.x;
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

		pVtx[0].tex = D3DXVECTOR2(TEXTURE_PLAYER_HP_CAGE_UNIT_SIZE * 3,0.0f);
		pVtx[1].tex = D3DXVECTOR2(TEXTURE_PLAYER_HP_CAGE_UNIT_SIZE * 3 + TEXTURE_PLAYER_HP_CAGE_UNIT_SIZE,0.0f);
		pVtx[2].tex = D3DXVECTOR2(TEXTURE_PLAYER_HP_CAGE_UNIT_SIZE * 3,1.0f);
		pVtx[3].tex = D3DXVECTOR2(TEXTURE_PLAYER_HP_CAGE_UNIT_SIZE * 3 + TEXTURE_PLAYER_HP_CAGE_UNIT_SIZE,1.0f);
		// 頂点データをアンロックする
		g_pVtxBuffBattle_Player_HP_Cage->Unlock();
}	


