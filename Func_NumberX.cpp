//=============================================================================
//
// 数列表示処理
// MADE BY hALNAGOYA.GP11A172.17 LIN LI 
// Ver 1.3
//=============================================================================
#include "Func_NumberX.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT	MakeVertexNumberD(LPDIRECT3DDEVICE9 pDevice);
void SetTextureNumberD(int nNumberD_ID,int nBox,float Alpha);
//*****************************************************************************
// グローバル変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureNumberD[MAX_TEX] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffNumberD = NULL;		// 頂点バッファスへのポインタ

NUMBER_X	g_aNumberX[MAX_NUMBER_X];					// 数列組のワーク
NUMBER_DATA g_aNumberD[MAX_NUMBER_COUNT];				// 数字のワーク
const char *g_pFileNameNumberTex[MAX_TEX] =
{
	"data/TEXTURE/Num_base.png",
	"data/TEXTURE/Num_Digital.png",
	"data/TEXTURE/Num_FulColor.png",
};
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitNumberX(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	int i = MAX_NUMBER_COUNT;

	for(int nCnt_NumberD = 0;nCnt_NumberD < MAX_NUMBER_COUNT; nCnt_NumberD++)
	{
		g_aNumberD[nCnt_NumberD].bUse		= false;
		g_aNumberD[nCnt_NumberD].nSize_X	= 0.0f;
		g_aNumberD[nCnt_NumberD].nSize_Y	= 0.0f;
		g_aNumberD[nCnt_NumberD].NumBox		= 0;
		g_aNumberD[nCnt_NumberD].pos		= D3DXVECTOR3(0.0f,0.0f,0.0f);
	}


	for(int nCnt_NumberX = 0;nCnt_NumberX < MAX_NUMBER_X;nCnt_NumberX++)
	{
		g_aNumberX[nCnt_NumberX].nDigit		= 0;
		g_aNumberX[nCnt_NumberX].bUse		= false;
		g_aNumberX[nCnt_NumberX].nSize_X	= 0.0f;
		g_aNumberX[nCnt_NumberX].nSize_Y	= 0.0f;
		g_aNumberX[nCnt_NumberX].pos		= D3DXVECTOR3(0.0f,0.0f,0.0f);
		g_aNumberX[nCnt_NumberX].nNumber	= 0; 
		g_aNumberX[nCnt_NumberX].bFade_Out	= false;	
		g_aNumberX[nCnt_NumberX].fAphla		= 1.0f;
		g_aNumberX[nCnt_NumberX].TexName	= 0;
		g_aNumberX[nCnt_NumberX].nLife		= 60;
	}

	// 頂点情報の作成
	MakeVertexNumberD(pDevice);
	for(int nCntTex = 0; nCntTex < MAX_TEX ;nCntTex++)
	{
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,		// デバイスへのポインタ
		g_pFileNameNumberTex[nCntTex],		// ファイルの名前
		&g_pTextureNumberD[nCntTex]);		// 読み込むメモリー
	}
		//MessageBox(NULL,"Xファイルの読み込みが失敗しました",NULL,MB_OK);
		//return E_FAIL;


	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void UninitNumberX(void)
{
	for(int nCntTex = 0; nCntTex < MAX_TEX ;nCntTex++)
	{
	if(g_pTextureNumberD[nCntTex] != NULL)
	{// テクスチャの開放
		g_pTextureNumberD[nCntTex]->Release();
		g_pTextureNumberD[nCntTex] = NULL;
	}
	}
	if(g_pVtxBuffNumberD != NULL)
	{// テクスチャの開放
		g_pVtxBuffNumberD->Release();
		g_pVtxBuffNumberD = NULL;
	}
}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateNumberX(void)
{

	//if(GetKeyboardTrigger(DIK_G))
	//{
	//	Set_NumberX(TIME_LEFT,				// 数列の名前
	//	true,								// 使用の状態設定
	//	false,								// Fade状態
	//	BASICALWB,							// 使用するテクスチャの名前
	//	4,									// 入力最大桁数
	//	0000,								// 入力値
	//	30.0f,								// 文字組の大きさX
	//	50.0f,								// 文字組の大きさY
	//	1.0f,								// 透明度の設定
	//	D3DXVECTOR3(1020.0f,300.0f,0.0f)	// 描画開始位置
	//	);

	//}


	int nCnt_NumberD = 0;

	for(int nCnt_NumberX = 0;nCnt_NumberX < MAX_NUMBER_X; nCnt_NumberX++)
	{
		if(g_aNumberX[nCnt_NumberX].bUse)
		{
			int nCntDigit = 0;//桁数のカウント
			int NumberD_Start = nCnt_NumberX * MAX_DIGIT; // 現在numXが確報しているNumD列の所在始点

			for(int nCntD = NumberD_Start;									//始点の位置を開始位置に設定		
				nCntD < NumberD_Start + g_aNumberX[nCnt_NumberX].nDigit;	//今の番号から最大桁数までの番号												
				nCntD ++)													//相対番号の増加
			{
				// NumberX内の数字を分解し，NumberDに一個ずつ入れる
				g_aNumberD[nCntD].bUse = true;
				g_aNumberD[nCntD].NumBox =  g_aNumberX[nCnt_NumberX].nNumber %
					(int)(powf(10.0f,(float)(nCntDigit + 1)))/
					(int)(powf(10.0f,(float)(nCntDigit)));

				//*************この下から　色々な効果を追加できるよ*****************//

				
				//***********************************ここからは fade Outで消える処理
				if(g_aNumberX[nCnt_NumberX].bFade_Out == true)
				{
					g_aNumberX[nCnt_NumberX].nSize_X *= VALUE_SIZE_CHANGE_X;
					g_aNumberX[nCnt_NumberX].nSize_Y *= VALUE_SIZE_CHANGE_Y;
					g_aNumberX[nCnt_NumberX].fAphla  -=	VALUE_FADE_OUT;
					g_aNumberX[nCnt_NumberX].nLife--;
					// もし最後までフェイドしたら，数列を無効にする
					if(g_aNumberX[nCnt_NumberX].nLife <= 0)
					{
						g_aNumberX[nCnt_NumberX].fAphla  = 0.0f;
						g_aNumberX[nCnt_NumberX].nLife	 = 60;
						g_aNumberX[nCnt_NumberX].bFade_Out =false;
						Diffuse_Number(PLAYER_SCORE,false);
					}
				}
				//***********************************ここまではfade Outで消える処理
				//*************効果の追加はここまでだよ******************************//

				// NumberDの数字大きさ反映
				g_aNumberD[nCntD].nSize_X = g_aNumberX[nCnt_NumberX].nSize_X;
				g_aNumberD[nCntD].nSize_Y = g_aNumberX[nCnt_NumberX].nSize_Y;

				// NumberDの数字の描画場所反映
				g_aNumberD[nCntD].pos.x = g_aNumberX[nCnt_NumberX].pos.x - (nCntD - nCnt_NumberX * 8)*g_aNumberD[nCntD].nSize_X;
				g_aNumberD[nCntD].pos.y = g_aNumberX[nCnt_NumberX].pos.y;
				nCntDigit++;
				SetTextureNumberD(nCntD,g_aNumberD[nCntD].NumBox,g_aNumberX[nCnt_NumberX].fAphla);
			}
		}




	}


}
//=============================================================================
// 描画処理
//=============================================================================
void DrawNumberX(void)
{
	int nCnt_NumberX = -1;//初期値は-1にしないとおかしくなるよ，理由は下へ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffNumberD, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);



	// ポリゴンの描画
	for(int nCntNumberD = 0; nCntNumberD < MAX_NUMBER_COUNT; nCntNumberD++)
	{
		if(nCntNumberD % MAX_DIGIT ==0)
			nCnt_NumberX++;//ここで一回目の計算で，0番目の数列二セットできるからね

		if(g_aNumberD[nCntNumberD].bUse == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureNumberD[g_aNumberX[nCnt_NumberX].TexName]);
			//*********************テクスチャワーク//該該当数列          テクスチャ名

			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,
				(nCntNumberD * 4),
				NUM_POLYGON);
		}
	}	
}
//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT	MakeVertexNumberD(LPDIRECT3DDEVICE9 pDevice)
{
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * MAX_NUMBER_COUNT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffNumberD,
		NULL)))
	{
		return E_FAIL;
	}
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffNumberD->Lock(0, 0, (void**)&pVtx, 0);


		for(int nCnt_NumberD = 0;nCnt_NumberD < MAX_NUMBER_COUNT; nCnt_NumberD++,pVtx += 4)

		{
			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aNumberD[nCnt_NumberD].pos.x - g_aNumberD[nCnt_NumberD].nSize_X,
				g_aNumberD[nCnt_NumberD].pos.y,
				0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aNumberD[nCnt_NumberD].pos.x, 
				g_aNumberD[nCnt_NumberD].pos.y,
				0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aNumberD[nCnt_NumberD].pos.x - g_aNumberD[nCnt_NumberD].nSize_X,
				g_aNumberD[nCnt_NumberD].pos.y + g_aNumberD[nCnt_NumberD].nSize_Y,
				0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aNumberD[nCnt_NumberD].pos.x,
				g_aNumberD[nCnt_NumberD].pos.y + g_aNumberD[nCnt_NumberD].nSize_Y,
				0.0f);//中心設定は右上
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
	g_pVtxBuffNumberD->Unlock();


	return S_OK;


}
//=============================================================================
// テクスチャ座標設定
//=============================================================================
void SetTextureNumberD(int nNumberD_ID,int nBox,float Alpha)
{
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffNumberD->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (nNumberD_ID * 4);

	// 頂点座標の設定
	pVtx[0].tex = D3DXVECTOR2((float)nBox * 0.1f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2((float)nBox * 0.1f + 0.1f, 0.0f);	
	pVtx[2].tex = D3DXVECTOR2((float)nBox * 0.1f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2((float)nBox * 0.1f + 0.1f, 1.0f);

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, Alpha);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, Alpha);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, Alpha);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, Alpha);

	if(nNumberD_ID >= TURN_ENEMY_0*MAX_DIGIT && nNumberD_ID < DISTENCE*MAX_DIGIT)
	{
		if(rand()%4 ==0 && nBox == 1)
		{
			// 頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, Alpha);
			pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, Alpha);
			pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, Alpha);
			pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, Alpha);
		}
	}


	pVtx[0].pos = D3DXVECTOR3(g_aNumberD[nNumberD_ID].pos.x - g_aNumberD[nNumberD_ID].nSize_X,
		g_aNumberD[nNumberD_ID].pos.y,
		0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aNumberD[nNumberD_ID].pos.x, 
		g_aNumberD[nNumberD_ID].pos.y,
		0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aNumberD[nNumberD_ID].pos.x - g_aNumberD[nNumberD_ID].nSize_X,
		g_aNumberD[nNumberD_ID].pos.y + g_aNumberD[nNumberD_ID].nSize_Y,
		0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aNumberD[nNumberD_ID].pos.x,
		g_aNumberD[nNumberD_ID].pos.y + g_aNumberD[nNumberD_ID].nSize_Y,
		0.0f);//中心設定は右上


	// 頂点データをアンロックする
	g_pVtxBuffNumberD->Unlock();

}
//=============================================================================
//Numberの設定
//=============================================================================
void Set_NumberX(
				int		nName,			// 数列の名前
				bool	bUse,			// 使用の状態設定
				bool	bFade,			// Fadeの制御
				int		TexName,		// 使用するテクスチャの名前
				int		nMax_Digit,		// 入力最大桁数
				int		nInput_Data,	// 入力値
				float	fSize_X,		// 文字組の大きさX
				float	fSize_Y,		// 文字組の大きさY
				float	fAlpha,			// 透明度の設定
				D3DXVECTOR3 pos)		// 描画開始位置

{
	g_aNumberX[nName].bUse		= true;
	g_aNumberX[nName].bFade_Out = bFade;
	g_aNumberX[nName].TexName	= TexName;
	g_aNumberX[nName].nDigit	= nMax_Digit,
	g_aNumberX[nName].nNumber	= nInput_Data;
	g_aNumberX[nName].nSize_X	= fSize_X;
	g_aNumberX[nName].nSize_Y	= fSize_Y;
	g_aNumberX[nName].pos		= pos;
	g_aNumberX[nName].fAphla	= fAlpha;
}
//=============================================================================
//Numberの解除
//=============================================================================
void Diffuse_Number(int nName,bool Fade)
{	

	if(Fade == false)
	{
		int nCntDigit = 0;
		int NumberD_Start = nName * MAX_DIGIT;			// 現在numXが確報しているNumD列の所在始点

		for(int nCntD = NumberD_Start;					//始点の位置を開始位置に設定		
			nCntD < NumberD_Start + MAX_DIGIT;			//今の番号から最大桁数までの番号												
			nCntD ++)									//相対番号の増加
			g_aNumberD[nCntD].bUse = false;
	}
	g_aNumberX[nName].bFade_Out = true;
}
//===================================================================================
//NumberXの取得
//===================================================================================
NUMBER_X *GetNumberX(void)
{
	return &g_aNumberX[0];
}

