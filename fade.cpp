////=============================================================================
////
//// フェード処理 [fade.cpp]
//// HAL名古屋
////
////=============================================================================
////#include "fade.h"
//#include "score.h"
////#include "Fade_loading.h"
////*****************************************************************************
//// マクロ定義
////*****************************************************************************
//#define	TEXTURE_FADE	"data/TEXTURE/fade000.jpg"	// 読み込むテクスチャファイル名
//#define	FADE_RATE		(0.02f)						// フェード係数
//
////*****************************************************************************
//// プロトタイプ宣言
////*****************************************************************************
//HRESULT MakeVertexFade(LPDIRECT3DDEVICE9 pDevice);
//void SetColorFade(D3DXCOLOR col);
//
////*****************************************************************************
//// グローバル変数
////*****************************************************************************
//LPDIRECT3DTEXTURE9		g_pTextureFade = NULL;		// テクスチャへのポインタ
//LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFade = NULL;		// 頂点バッファインターフェースへのポインタ
////FADE					g_fade = FADE_IN;			// フェード状態
//D3DXCOLOR				g_colorFade;				// フェード色
////MODE					g_modeNext = MODE_TITLE;	// 次のモード
//
////=============================================================================
//// 初期化処理
////=============================================================================
//HRESULT InitFade(void)
//{
//	LPDIRECT3DDEVICE9 pDevice = GetDevice();
//
//	// 頂点情報の設定
//	MakeVertexFade(pDevice);
//
//	// テクスチャの読み込み
//	D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
//								TEXTURE_FADE,		// ファイルの名前
//								&g_pTextureFade);	// 読み込むメモリー
//
//	//g_fade = FADE_IN;
//	//g_modeNext = MODE_TITLE;
//	
//	//InitFade_loading();
//	return S_OK;
//}
//
////=============================================================================
//// 終了処理
////=============================================================================
//void UninitFade(void)
//{
//	if(g_pTextureFade != NULL)
//	{// テクスチャの開放
//		g_pTextureFade->Release();
//		g_pTextureFade = NULL;
//	}
//
//	if(g_pVtxBuffFade != NULL)
//	{// 頂点バッファの開放
//		g_pVtxBuffFade->Release();
//		g_pVtxBuffFade = NULL;
//	}
//	//UninitFade_loading();
//}
//
////=============================================================================
//// 更新処理
////=============================================================================
//void UpdateFade(void)
//{
//	//UpdateFade_loading();
//	if(g_fade != FADE_NONE)
//	{// フェード処理中
//		if(g_fade == FADE_IN)			
//
//		{// フェードイン処理
//			g_colorFade.a -= FADE_RATE;		// α値を減算して画面を浮き上がらせる
//			if(g_colorFade.a <= 0.0f)
//			{
//				// フェード処理終了
//				g_colorFade.a = 0.0f;
//				g_fade = FADE_NONE;
//
//			}
//
//			// 頂点カラーの設定
//			SetColorFade(g_colorFade);
//		}
//		else if(g_fade == FADE_OUT)
//		{// フェードアウト処理
//			//演出始める
//			if(g_colorFade.a == 0.0f)
//			//StartFadeLoad();
//			g_colorFade.a += FADE_RATE;		// α値を加算して画面を消していく
//			//int LoadFadePhase = GetFadeLoadPhase();
//			
//
//			if(g_colorFade.a >= 1.0f)
//				g_colorFade.a = 1.0f;
//			//if(LoadFadePhase == LOADING_BLOCK_PHASE_MAX)
//			{
//				// フェードイン処理に切り替え
//				g_colorFade.a = 1.0f;
//				g_fade = FADE_IN;
//				// 次のモードを設定
//				//SetMode(g_modeNext);
//			}
//
//			// 頂点カラーの設定
//			SetColorFade(g_colorFade);
//		}
//	}
//}
//
////=============================================================================
//// タイトル画面
////=============================================================================
//void DrawFade(void)
//{
//	LPDIRECT3DDEVICE9 pDevice = GetDevice();
//
//	// 頂点バッファをデバイスのデータストリームにバインド
//    pDevice->SetStreamSource(0, g_pVtxBuffFade, 0, sizeof(VERTEX_2D));
//
//	// 頂点フォーマットの設定
//	pDevice->SetFVF(FVF_VERTEX_2D);
//
//	// テクスチャの設定
//	pDevice->SetTexture(0, g_pTextureFade);
//
//	// ポリゴンの描画
//	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
//	//DrawFade_loading();
//}
//
////=============================================================================
//// 頂点の作成
////=============================================================================
//HRESULT MakeVertexFade(LPDIRECT3DDEVICE9 pDevice)
//{
//	// オブジェクトの頂点バッファを生成
//    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
//												D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
//												FVF_VERTEX_2D,				// 使用する頂点フォーマット
//												D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
//												&g_pVtxBuffFade,			// 頂点バッファインターフェースへのポインタ
//												NULL)))						// NULLに設定
//	{
//        return E_FAIL;
//	}
//
//	{//頂点バッファの中身を埋める
//		VERTEX_2D *pVtx;
//
//		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
//		g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);
//
//		// 頂点座標の設定
//		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//		pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
//		pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
//		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
//
//		// テクスチャのパースペクティブコレクト用
//		pVtx[0].rhw =
//		pVtx[1].rhw =
//		pVtx[2].rhw =
//		pVtx[3].rhw = 1.0f;
//
//		// 頂点カラーの設定
//		g_colorFade = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//		pVtx[0].col = g_colorFade;
//		pVtx[1].col = g_colorFade;
//		pVtx[2].col = g_colorFade;
//		pVtx[3].col = g_colorFade;
//
//		// テクスチャ座標の設定
//		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
//		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
//		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
//		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);	
//
//		// 頂点データをアンロックする
//		g_pVtxBuffFade->Unlock();
//	}
//
//	return S_OK;
//}
//
////=============================================================================
//// 頂点カラーの設定
////=============================================================================
//void SetColorFade(D3DXCOLOR col)
//{
//	VERTEX_2D *pVtx;
//
//	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
//	g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);
//
//	// 頂点カラーの設定
//	pVtx[0].col = col;
//	pVtx[1].col = col;
//	pVtx[2].col = col;
//	pVtx[3].col = col;
//
//	// 頂点データをアンロックする
//	g_pVtxBuffFade->Unlock();
//}
//
//////=============================================================================
////// フェードの状態設定
//////=============================================================================
////void SetFade(FADE fade, MODE modeNext)
////{
////	g_fade = fade;
////	//g_modeNext = modeNext;
////}
////
//////=============================================================================
////// フェードの状態取得
//////=============================================================================
////FADE GetFade(void)
////{
////	return g_fade;
////}
//
