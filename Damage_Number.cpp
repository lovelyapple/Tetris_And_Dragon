//=============================================================================
//
// 数列表示処理
// 
//
//=============================================================================
#include "main.h"
#include "game.h"
#include "Damage_Number.h"
#include "input.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_DAMAGE_NUMBER_X		(32)
#define MAX_DAMAGE_DIGIT		(8)
#define MAX_DAMAGE_NUMBER_D		(MAX_DAMAGE_DIGIT*MAX_DAMAGE_NUMBER_X)
#define DEFAUL_DAMAGE_NUMBER_SIZE_X	(20)
#define DEFAUL_DAMAGE_NUMBER_SIZE_Y	(40)
#define TEXTURE_DAMAGED			"data/TEXTURE/Number_Big.png"
#define DAMAGE_TIME_CHANGRING	(20)
#define DAMAGE_TIME_CHARGE		(60)
#define DAMAGE_TIME_LAST		(30)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT	MakeVertexDamageD(LPDIRECT3DDEVICE9 pDevice);
void SetTextureDamageD(int nNumberD_ID,int nBox,float fSize);

void InitDamageD(int DamageDID); 
void InitDamageX(int DamageXID);
//*****************************************************************************
// グローバル変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureDamageD = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffDamageD = NULL;		// 頂点バッファスへのポインタ

DAMAGE_NUMBER_X	g_DamageX[MAX_DAMAGE_NUMBER_X];
DAMAGE_NUMBER_D	g_DamageD[MAX_DAMAGE_NUMBER_D];
//=============================================================================
// 初期化処理
//=============================================================================
void InitDamageX(int DamageXID)
{
	g_DamageX[DamageXID].bUse	= false;
	g_DamageX[DamageXID].fChangePerTime	= 0.0f;
	g_DamageX[DamageXID].nChangePhase	= DAMAGE_PHASE_CHARGE;
	g_DamageX[DamageXID].nColor			= 0;
	g_DamageX[DamageXID].InitDamage		= 0;
	g_DamageX[DamageXID].nDamageType	= DAMAGE_NORMAL;
	g_DamageX[DamageXID].nRecDamage		= 0;
	g_DamageX[DamageXID].nDestDamage	= 0;
	g_DamageX[DamageXID].nD_End			= 0;
	g_DamageX[DamageXID].nD_Start		= 0;
	g_DamageX[DamageXID].nLife			= DAMAGE_TIME_CHARGE;
	g_DamageX[DamageXID].pos			= D3DXVECTOR3(0.0f,0.0f,0.0f);
	g_DamageX[DamageXID].fSize			= 1.0f;
	g_DamageX[DamageXID].fMove			= D3DXVECTOR3(0.0f,0.0f,0.0f);;
}
void InitDamageD(int DamageDID)
{
	g_DamageD[DamageDID].bUse	= false;
	g_DamageD[DamageDID].bDraw	= false;
	g_DamageD[DamageDID].nBox	= 99;
	g_DamageD[DamageDID].pos		= D3DXVECTOR3(0.0f,0.0f,0.0f);

}

HRESULT	InitDamage_NumberD(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	for(int nDamageX = 0; nDamageX < MAX_DAMAGE_NUMBER_X; nDamageX++)
	{
		InitDamageX(nDamageX);
	}

	for(int nDamageD = 0; nDamageD < MAX_DAMAGE_NUMBER_D;nDamageD++)
	{
		InitDamageD(nDamageD);
	}
	D3DXCreateTextureFromFile(pDevice,		// デバイスへのポインタ
		TEXTURE_DAMAGED,		// ファイルの名前
		&g_pTextureDamageD);
	MakeVertexDamageD(pDevice);
	return S_OK;

}
//=============================================================================
// 終了処理
//=============================================================================
void UninitDamage_NumberD(void)
{

	if(g_pTextureDamageD != NULL)
	{// テクスチャの開放
		g_pTextureDamageD->Release();
		g_pTextureDamageD = NULL;
		g_pTextureDamageD;
	}
	if(g_pVtxBuffDamageD != NULL)
	{// テクスチャの開放
		g_pVtxBuffDamageD->Release();
		g_pVtxBuffDamageD = NULL;
	}

}
void SetDamageNumberX(int DestDamage,int InitDamage,int DamageType,D3DXVECTOR3 pos)
{
	int Idx_X = 0;
	int DamageDest = DestDamage;
	for(int nDamageX = 0; nDamageX < MAX_DAMAGE_NUMBER_X; nDamageX++)
	{
		if(g_DamageX[nDamageX].bUse == true)
			continue;
		else
		{
		if(nDamageX == MAX_DAMAGE_NUMBER_X - 1)
			Idx_X = 0;
		else
			Idx_X = nDamageX;
		break;
		}
	}
	g_DamageX[Idx_X].bUse = true;
	g_DamageX[Idx_X].nDestDamage = DestDamage;
	switch(DamageType)
	{
	case DAMAGE_NORMAL:
		InitDamage = DestDamage;
		break;
	case DAMAGE_GREAT:
		InitDamage = DestDamage /2 ;
		break;
	case DAMAGE_BAD:
		InitDamage = DestDamage *2 ;
	break;
	}

	g_DamageX[Idx_X].InitDamage	 = InitDamage;
	g_DamageX[Idx_X].nRecDamage	 = 0;
	g_DamageX[Idx_X].nDamageType = DamageType;
	g_DamageX[Idx_X].pos = pos;
	g_DamageX[Idx_X].fMove.x = (rand()%11 - 5.0f)/10.0f;
	g_DamageX[Idx_X].fMove.y = (rand()%11 - 5.0f)/10.0f;

	g_DamageX[Idx_X].fChangePerTime = (float)InitDamage/(float)DAMAGE_TIME_CHARGE;


//	開始点を検索


	for(int nDamageD = 0,Digit = 0; nDamageD < MAX_DAMAGE_NUMBER_D && Digit < MAX_DAMAGE_DIGIT ;nDamageD++)
	{
		if(g_DamageD[nDamageD].bUse == true)
			continue;
		if(Digit == 0)
			g_DamageX[Idx_X].nD_Start = nDamageD;//0番目の時に開始点を初期化
		else
			g_DamageX[Idx_X].nD_End = nDamageD;//ほかの時に終点を更新

		g_DamageD[nDamageD].bUse = true;
		Digit ++ ;
	}


}

//=============================================================================
// 更新処理
//=============================================================================
void	UpdateDamage_NumberD(void)
{
	for(int nDamageX = 0; nDamageX < MAX_DAMAGE_NUMBER_X; nDamageX++)
	{
		if(g_DamageX[nDamageX].bUse == false)
			continue;

		else
		{
			int NumberBox[MAX_DAMAGE_DIGIT]= {};
			g_DamageX[nDamageX].pos += g_DamageX[nDamageX].fMove;
			D3DXVECTOR3 pos = g_DamageX[nDamageX].pos;
			bool DrawStart = false;
			//Xデータを設定
			for(int Idx = MAX_DAMAGE_DIGIT; Idx > 0; Idx --)
			{

				int BoxWK = (int)(g_DamageX[nDamageX].nRecDamage) %
					(int)(powf(10.0f,(float)(Idx))) / (int)(powf(10.0f,(float)(Idx-1)));

				//最大桁から数え０ではない時から数字を入れる
				if(BoxWK != 0)
					DrawStart = true;

				if(DrawStart == true)
					NumberBox[Idx-1] = BoxWK;
				else
					NumberBox[Idx-1] = 99;
			}
			//Dのデータを設定
			for(int nCntD = g_DamageX[nDamageX].nD_Start,Idx = 0;
				nCntD <= g_DamageX[nDamageX].nD_End;
				nCntD++,Idx++)
			{
				g_DamageD[nCntD].bUse = true;
				g_DamageD[nCntD].nBox = NumberBox[Idx];
				if(g_DamageD[nCntD].nBox == 99)
					g_DamageD[nCntD].bDraw = false;
				else 
					g_DamageD[nCntD].bDraw = true;
				g_DamageD[nCntD].pos.x = pos.x - (Idx+1)*DEFAUL_DAMAGE_NUMBER_SIZE_X * g_DamageX[nDamageX].fSize;
				g_DamageD[nCntD].pos.y = pos.y;
				SetTextureDamageD(nCntD,g_DamageD[nCntD].nBox,g_DamageX[nDamageX].fSize);
			}
			//状態遷移
			switch(g_DamageX[nDamageX].nChangePhase)
			{
			case DAMAGE_PHASE_CHARGE:
				g_DamageX[nDamageX].nLife--;
				g_DamageX[nDamageX].fSize += 0.005;
				g_DamageX[nDamageX].nRecDamage += g_DamageX[nDamageX].fChangePerTime;
				if(g_DamageX[nDamageX].nLife <= 0)
				{
					g_DamageX[nDamageX].nRecDamage = g_DamageX[nDamageX].InitDamage;
					g_DamageX[nDamageX].nLife = DAMAGE_TIME_CHANGRING;
					g_DamageX[nDamageX].nChangePhase = DAMAGE_PHASE_PAUSE;
				}
				break;
			case DAMAGE_PHASE_PAUSE:
				g_DamageX[nDamageX].nLife --;
				if(g_DamageX[nDamageX].nLife <= 0)
				{
					g_DamageX[nDamageX].nLife = DAMAGE_TIME_CHANGRING;
					g_DamageX[nDamageX].nChangePhase = DAMAGE_PHASE_CHANGE;
				}

				break;
			case DAMAGE_PHASE_CHANGE://ここはもうちょっと詳しくみあしょう
				switch(g_DamageX[nDamageX].nDamageType)
				{
				case DAMAGE_NORMAL:
					g_DamageX[nDamageX].nChangePhase = DAMAGE_PHASE_KEEP;
					g_DamageX[nDamageX].nLife = 10;
					break;
				case DAMAGE_GREAT:
					g_DamageX[nDamageX].fChangePerTime = float((g_DamageX[nDamageX].nDestDamage - g_DamageX[nDamageX].nRecDamage)/DAMAGE_TIME_CHANGRING);
					g_DamageX[nDamageX].nChangePhase = DAMAGE_PHASE_INCREASE;
					break;
				case DAMAGE_BAD:
					g_DamageX[nDamageX].fChangePerTime = float((g_DamageX[nDamageX].nDestDamage - g_DamageX[nDamageX].nRecDamage)/DAMAGE_TIME_CHANGRING);
					g_DamageX[nDamageX].nChangePhase = DAMAGE_PHASE_LOSING;
					break;
				}				
				break;
			case DAMAGE_PHASE_INCREASE:
				g_DamageX[nDamageX].nLife --;
				g_DamageX[nDamageX].fSize += 0.008;
				g_DamageX[nDamageX].nRecDamage += g_DamageX[nDamageX].fChangePerTime;
				if(g_DamageX[nDamageX].nLife <= 0)
				{
					g_DamageX[nDamageX].nRecDamage = g_DamageX[nDamageX].nDestDamage;
					g_DamageX[nDamageX].nLife = DAMAGE_TIME_LAST;
					g_DamageX[nDamageX].nChangePhase = DAMAGE_PHASE_STOP;
				}
				break;

			case DAMAGE_PHASE_LOSING:
				g_DamageX[nDamageX].nLife --;
				g_DamageX[nDamageX].fSize -= 0.003;
				g_DamageX[nDamageX].nRecDamage += g_DamageX[nDamageX].fChangePerTime;
				if(g_DamageX[nDamageX].nLife <= 0)
				{
					g_DamageX[nDamageX].nRecDamage = g_DamageX[nDamageX].nDestDamage;
					g_DamageX[nDamageX].nLife = DAMAGE_TIME_LAST;
					g_DamageX[nDamageX].nChangePhase = DAMAGE_PHASE_STOP;
				}	
				break;
			case DAMAGE_PHASE_KEEP:
				g_DamageX[nDamageX].nLife --;
				if(g_DamageX[nDamageX].nLife <= 0)
				{
					g_DamageX[nDamageX].nLife = DAMAGE_TIME_LAST;
					g_DamageX[nDamageX].nChangePhase = DAMAGE_PHASE_STOP;
				}	
				break;
			case DAMAGE_PHASE_STOP:
				g_DamageX[nDamageX].nLife --;
				if(g_DamageX[nDamageX].nLife <= 0)
					g_DamageX[nDamageX].nChangePhase = DAMAGE_PHASE_FADE_OUT;
				break;
			case DAMAGE_PHASE_FADE_OUT:
				g_DamageX[nDamageX].bUse =false;

				//使いきったら，現在のＸとＤを初期化する
				for(int nCntD = g_DamageX[nDamageX].nD_Start,Idx = 0;
					nCntD <= g_DamageX[nDamageX].nD_End;
					nCntD++,Idx++)
					InitDamageD(nCntD);
				InitDamageX(nDamageX);
				break;
			}

		}

	}

}
//=============================================================================
// 描画処理
//=============================================================================
void DrawDamage_NumberD(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffDamageD, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetTexture(0, g_pTextureDamageD);

	// ポリゴンの描画
	for(int nDamageD = 0; nDamageD < MAX_DAMAGE_NUMBER_D; nDamageD++)
	{
		if(g_DamageD[nDamageD].bUse == false || g_DamageD[nDamageD].bDraw == false)
			continue;
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,(nDamageD * 4),NUM_POLYGON);
	}	
}
//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT	MakeVertexDamageD(LPDIRECT3DDEVICE9 pDevice)
{
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * MAX_DAMAGE_NUMBER_D,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffDamageD,
		NULL)))
	{
		return E_FAIL;
	}
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffDamageD->Lock(0, 0, (void**)&pVtx, 0);


		for(int nDamageD = 0; nDamageD < MAX_DAMAGE_NUMBER_D; nDamageD++,pVtx += 4)

		{
			// 頂点座標の設定
			pVtx[0].pos.x = g_DamageD[nDamageD].pos.x - DEFAUL_DAMAGE_NUMBER_SIZE_X;
			pVtx[0].pos.y = g_DamageD[nDamageD].pos.y - DEFAUL_DAMAGE_NUMBER_SIZE_Y;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_DamageD[nDamageD].pos.x;// - DEFAUL_DAMAGE_NUMBER_SIZE_X;
			pVtx[1].pos.y = g_DamageD[nDamageD].pos.y - DEFAUL_DAMAGE_NUMBER_SIZE_Y;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_DamageD[nDamageD].pos.x - DEFAUL_DAMAGE_NUMBER_SIZE_X;
			pVtx[2].pos.y = g_DamageD[nDamageD].pos.y;// - DEFAUL_DAMAGE_NUMBER_SIZE_Y;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_DamageD[nDamageD].pos.x;// - DEFAUL_DAMAGE_NUMBER_SIZE_X;
			pVtx[3].pos.y = g_DamageD[nDamageD].pos.y;// - DEFAUL_DAMAGE_NUMBER_SIZE_Y;
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
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
		}

	}
	// 頂点データをアンロックする
	g_pVtxBuffDamageD->Unlock();


	return S_OK;


}
//=============================================================================
// 頂点の変更
//=============================================================================
void SetTextureDamageD(int nNumberD_ID,int nBox,float fSize)
{
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffDamageD->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nNumberD_ID*4;
	// 頂点座標の設定
	pVtx[0].pos.x = (g_DamageD[nNumberD_ID].pos.x - DEFAUL_DAMAGE_NUMBER_SIZE_X * fSize);
	pVtx[0].pos.y = (g_DamageD[nNumberD_ID].pos.y - DEFAUL_DAMAGE_NUMBER_SIZE_Y * fSize);
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = (g_DamageD[nNumberD_ID].pos.x);// - DEFAUL_DAMAGE_NUMBER_SIZE_X;
	pVtx[1].pos.y = (g_DamageD[nNumberD_ID].pos.y - DEFAUL_DAMAGE_NUMBER_SIZE_Y * fSize);
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = (g_DamageD[nNumberD_ID].pos.x - DEFAUL_DAMAGE_NUMBER_SIZE_X * fSize);
	pVtx[2].pos.y = (g_DamageD[nNumberD_ID].pos.y);// - DEFAUL_DAMAGE_NUMBER_SIZE_Y;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = (g_DamageD[nNumberD_ID].pos.x);// - DEFAUL_DAMAGE_NUMBER_SIZE_X;
	pVtx[3].pos.y = (g_DamageD[nNumberD_ID].pos.y);// - DEFAUL_DAMAGE_NUMBER_SIZE_Y;
	pVtx[3].pos.z = 0.0f;
	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// 頂点座標の設定
	pVtx[0].tex = D3DXVECTOR2((float)nBox * 0.1f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2((float)nBox * 0.1f + 0.1f, 0.0f);	
	pVtx[2].tex = D3DXVECTOR2((float)nBox * 0.1f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2((float)nBox * 0.1f + 0.1f, 1.0f);
	// 頂点データをアンロックする
	g_pVtxBuffDamageD->Unlock();
}
