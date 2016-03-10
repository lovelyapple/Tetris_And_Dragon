//=============================================================================
// 処理[Battle_Record_Time.cpp]
//=============================================================================
#include "Battle_Record_Time.h"
#include "main.h"

#include "Function.h"
#include <stdio.h>
#include <string.h>

#define TEXTURE_RECORD_TIME	("data/TEXTURE/Num_base.png")
#define MAX_TIME_DECIMAL_NUMBER		(2)
#define MAX_TIME_SEC_NUMBER			(2)
#define MAX_TIME_MIN_NUMBER			(2)
#define MAX_TIME_NUMBER				(MAX_TIME_DECIMAL_NUMBER + MAX_TIME_SEC_NUMBER + MAX_TIME_MIN_NUMBER)
#define TIME_UNIT_SIZE		(1.0f / 10)
#define TIME_TEX_SIZE_X	(10.0f)
#define TIME_TEX_SIZE_Y	(20.0f)
#define TIME_POS_DEFAULT_X		(1200.0f)
#define TIME_POS_DEFAULT_Y		(100.0f)
#define TIME_DECIMAL_SIZE (1.0f)
#define TIME_SEC_SIZE (1.0f)
#define TIME_MIN_SIZE (1.0f)
#define TIME_INTERVAL_SIZE	(0.4f)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
//*****************************************************************************
// グローバル変数D3DX3_INIT
//*****************************************************************************

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CBattle_Record_Time::InitBattle_Record_Time(void)
{
	//========================================================
	//ファイルの読み込み
	//
	//一回読み取り専用で、失敗した場合,ファイルを作成し,デファオール値を入れる
	//========================================================
	float	fGetSize[3] = {0.0f,0.0f,0.0f};
	char	strWk[256];
	D3DXVECTOR3	LoadPos = D3DX3_INIT;
	//レコード描画のセッティング
	FILE *fp;
	if((fp = fopen(FIELNAME_TIME_R_SETTING,"r")) ==NULL)
	{


		float DefaultSize[TIMEUNIT] = {2.0f,2.5f,2.5f};//初期値倍数
		D3DXVECTOR3	Defaultpos = D3DXVECTOR3(TIME_POS_DEFAULT_X,TIME_POS_DEFAULT_Y,0.0f);
		if((fp = fopen(FIELNAME_TIME_R_SETTING,"w")) ==NULL)
			return S_FALSE;
			fprintf(fp,"TimeDecimalSize(X_10.0fTimes_Y_20.0fTimes):,%4.3f\n \
					   TimeSecSize(X_10.0fTimes_Y_20.0fTimes):,%4.3f\n \
					   TimeMinSize(X_10.0fTimes_Y_20.0fTimes):,%4.3f\n \
					   D3DXVECTOR--PosXY:,%4.3f,%4.3f\n",
						DefaultSize[0],
						DefaultSize[1],
						DefaultSize[2],
						(float)Defaultpos.x,(float)Defaultpos.y);
		fclose(fp);


		if((fp =  fopen(FIELNAME_TIME_R_SETTING,"r"))==NULL)
			return S_FALSE;
		for(int Unit = 0; Unit < TIMEUNIT;Unit ++)
		fscanf(fp,"%[^,],%f\n",strWk,&fGetSize[Unit]);

		fscanf(fp,"%[^,],%f,%f\n",strWk,&LoadPos.x,&LoadPos.y);

		fclose(fp);

	}
	else
	{
		if((fp =  fopen(FIELNAME_TIME_R_SETTING,"r"))==NULL)
			return S_FALSE;
		for(int Unit = 0; Unit < TIMEUNIT;Unit ++)
		fscanf(fp,"%[^,],%f\n",strWk,&fGetSize[Unit]);

		fscanf(fp,"%[^,],%f,%f\n",strWk,&LoadPos.x,&LoadPos.y);

		fclose(fp);
	}


	//======================記録のデータ取得
	FILE *fc;
	if((fc = fopen(FIELNAME_RECORD,"r")) ==NULL)
	{//失敗したら新しくデフォルトデータを入れる
		m_RecordTime.Decimal = 99;
		m_RecordTime.Second	= 59;
		m_RecordTime.Minute	= 59;		
		if((fc = fopen(FIELNAME_RECORD,"w")) ==NULL)
			return S_FALSE;

		fprintf(fc,"RecentlyTimeRecord:, %d,%d,%d\n",m_RecordTime.Decimal,m_RecordTime.Second,m_RecordTime.Minute);
		fclose(fc);

	//読み取りの開き
		if((fc = fopen(FIELNAME_RECORD,"r")) ==NULL)
			return S_FALSE;
		{
			fscanf(fc,"%[^,], %d,%d,%d\n",strWk,&m_RecordTime.Decimal,&m_RecordTime.Second,&m_RecordTime.Minute);
		}
		fclose(fc);
	}
	else
	{
	//何もなければ普通に読み取り
		if((fc = fopen(FIELNAME_RECORD,"r")) ==NULL)
			return S_FALSE;
		{
			fscanf(fc,"%[^,], %d,%d,%d\n",strWk,&m_RecordTime.Decimal,&m_RecordTime.Second,&m_RecordTime.Minute);
		}
		fclose(fc);
	}



	//========================================================
	//ファイルの読み込み終了
	//========================================================


	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	m_TimeMode = RECORD_MODE_MAX;
	for(int nCntUnit = 0; nCntUnit < RECORD_UNIT_MAX;nCntUnit++)
	{
		m_pos[nCntUnit]		= D3DX3_INIT;
		m_nValue[nCntUnit]	= 0;
		m_fSize[nCntUnit]		= 0.0f;
	}
	m_TimeLive.Decimal	= 0;
	m_TimeLive.Minute	= 0;
	m_TimeLive.Second	= 0;

	m_bUse = true;
	m_pTextureBattle_Record_Time = NULL;
	m_pVtxBuffBattle_Record_Time = NULL;


	D3DXCreateTextureFromFile(pDevice,
								TEXTURE_RECORD_TIME,
								&m_pTextureBattle_Record_Time);
	SetUp_Record_Time(LoadPos,fGetSize);
	MakeVerTexBattle_Record_Time(pDevice);
	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CBattle_Record_Time::UninitBattle_Record_Time(void)
{
	if(m_pTextureBattle_Record_Time != NULL)
	{// テクスチャの開放
		m_pTextureBattle_Record_Time->Release();
		m_pTextureBattle_Record_Time = NULL;
	}
	if(m_pVtxBuffBattle_Record_Time != NULL)
	{// 頂点バッファの開放
		m_pVtxBuffBattle_Record_Time->Release();
		m_pVtxBuffBattle_Record_Time = NULL;
	}

}
//=============================================================================
// 更新処理
//=============================================================================
void CBattle_Record_Time::UpdateBattle_Record_Time(void)
{
	switch(m_TimeMode)
	{
	case RECORD_MODE_START:
		m_TimeLive.Decimal ++;
		if(m_TimeLive.Decimal >=  100)
		{
			m_TimeLive.Second ++ ;
			m_TimeLive.Decimal  =0;	
		}
		if(m_TimeLive.Second >= 60)
		{
			m_TimeLive.Second = 0;
			m_TimeLive.Minute ++ ;
			m_TimeLive.Minute = SetMAX(m_TimeLive.Minute,60,0,2);
		}
		break;
	case RECORD_MODE_STOP:
		break;
	case RECORD_MODE_RESET:
		m_TimeLive.Decimal = 0;
		m_TimeLive.Second = 0;
		m_TimeLive.Minute = 0;
		break;
	case RECORD_MODE_MAX:
		break;
	}


	m_nValue[RECORD_UNIT_DECIMAL00] = (int)m_TimeLive.Decimal % 10;
	m_nValue[RECORD_UNIT_DECIMAL01] = (int)m_TimeLive.Decimal / 10;
	m_nValue[RECORD_UNIT_SEC00] = (int)m_TimeLive.Second % 10;
	m_nValue[RECORD_UNIT_SEC01] = (int)m_TimeLive.Second / 10;
	m_nValue[RECORD_UNIT_MIN00] = (int)m_TimeLive.Minute % 10;
	m_nValue[RECORD_UNIT_MIN01] = (int)m_TimeLive.Minute / 10;
	SetVerTexBattle_Record_Time();
}
//=============================================================================
// その他の関数
//=============================================================================
void CBattle_Record_Time::Set_Time_Mode(RECORD_MODE mode)
{
	m_TimeMode = mode;
}
void CBattle_Record_Time::SetUp_Record_Time(D3DXVECTOR3 Inputpos,float fInputSize[3])
{

	m_fSize[RECORD_UNIT_DECIMAL00] = fInputSize[0];
	m_fSize[RECORD_UNIT_DECIMAL01] = fInputSize[0];
	m_fSize[RECORD_UNIT_SEC00] = fInputSize[1];
	m_fSize[RECORD_UNIT_SEC01] = fInputSize[1];
	m_fSize[RECORD_UNIT_MIN00] = fInputSize[2];
	m_fSize[RECORD_UNIT_MIN01] = fInputSize[2];

	for(int nCntUnit = 0; nCntUnit < RECORD_UNIT_MAX;nCntUnit++)
	m_pos[nCntUnit].y = Inputpos.y;

	float fInterval = TIME_TEX_SIZE_X * fInputSize[0] * TIME_INTERVAL_SIZE;
	m_pos[RECORD_UNIT_DECIMAL00].x = Inputpos.x;
	m_pos[RECORD_UNIT_DECIMAL01].x =	m_pos[RECORD_UNIT_DECIMAL00].x - TIME_TEX_SIZE_X * TIME_DECIMAL_SIZE - fInterval;

	m_pos[RECORD_UNIT_SEC00].x =		m_pos[RECORD_UNIT_DECIMAL01].x - TIME_TEX_SIZE_X * TIME_DECIMAL_SIZE - fInterval;
	fInterval = TIME_TEX_SIZE_X * fInputSize[1] * TIME_INTERVAL_SIZE;
	m_pos[RECORD_UNIT_SEC01].x =		m_pos[RECORD_UNIT_SEC00].x - TIME_TEX_SIZE_X * TIME_SEC_SIZE - fInterval;



	m_pos[RECORD_UNIT_MIN00].x =		m_pos[RECORD_UNIT_SEC01].x - TIME_TEX_SIZE_X * TIME_SEC_SIZE - fInterval;
	fInterval = TIME_TEX_SIZE_X * fInputSize[2] * TIME_INTERVAL_SIZE;
	m_pos[RECORD_UNIT_MIN01].x =		m_pos[RECORD_UNIT_MIN00].x - TIME_TEX_SIZE_X * TIME_MIN_SIZE - fInterval;
}
void CBattle_Record_Time::ResetTime(void)
{
	m_TimeLive.Decimal = 0;
	m_TimeLive.Second = 0;
	m_TimeLive.Minute = 0;
}
TIME_STRUCT CBattle_Record_Time::GetRecordLively(void)
{
	TIME_STRUCT TimeLively;
	TimeLively.Decimal = m_TimeLive.Decimal;
	TimeLively.Minute = m_TimeLive.Minute;
	TimeLively.Second = m_TimeLive.Second;
	return TimeLively;
}

//=============================================================================
// 描画処理
//=============================================================================
void CBattle_Record_Time::Draw_Battle_Record_Time(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, m_pVtxBuffBattle_Record_Time, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定()
	pDevice->SetTexture(0, m_pTextureBattle_Record_Time);
	for(int nCntUnit = 0; nCntUnit < RECORD_UNIT_MAX;nCntUnit++)
	// ポリゴンの描画()
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntUnit * 4, NUM_POLYGON);




}
//=============================================================================
// Battle_Score_And_Time_Record頂点の作成
//=============================================================================

HRESULT CBattle_Record_Time::MakeVerTexBattle_Record_Time(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * (MAX_TIME_NUMBER),// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
		FVF_VERTEX_2D,						// 使用する頂点フォーマット
		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
		&m_pVtxBuffBattle_Record_Time,		// 頂点バッファインターフェースへのポインタ
		NULL)))								// NULLに設定
	{
		return E_FAIL;
	}
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;
		//頂点データの範囲をロックし、頂点バッファへのポインタを取得
		m_pVtxBuffBattle_Record_Time->Lock(0, 0, (void**)&pVtx, 0);
		for(int nCntUnit = 0; nCntUnit < RECORD_UNIT_MAX;nCntUnit++,pVtx+=4)
		{	
			pVtx[0].pos.x = m_pos[nCntUnit].x - TIME_TEX_SIZE_X;
			pVtx[0].pos.y = m_pos[nCntUnit].y - TIME_TEX_SIZE_Y;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = m_pos[nCntUnit].x;
			pVtx[1].pos.y = m_pos[nCntUnit].y - TIME_TEX_SIZE_Y;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = m_pos[nCntUnit].x - TIME_TEX_SIZE_X;
			pVtx[2].pos.y = m_pos[nCntUnit].y;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = m_pos[nCntUnit].x;
			pVtx[3].pos.y = m_pos[nCntUnit].y;
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

			int Unit = m_nValue[nCntUnit];
			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(TIME_UNIT_SIZE * Unit, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(TIME_UNIT_SIZE * Unit + TIME_UNIT_SIZE, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(TIME_UNIT_SIZE * Unit, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(TIME_UNIT_SIZE * Unit + TIME_UNIT_SIZE, 1.0f);
	
		}
		// 頂点データをアンロックする
		m_pVtxBuffBattle_Record_Time->Unlock();	
	}
	return S_OK;
}
//=============================================================================
// Battle_Score_And_Time_Recordの設定
//=============================================================================
void CBattle_Record_Time::SetVerTexBattle_Record_Time(void)
{//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;
	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	m_pVtxBuffBattle_Record_Time->Lock(0, 0, (void**)&pVtx, 0);
	for(int nCntUnit = 0; nCntUnit < RECORD_UNIT_MAX;nCntUnit++,pVtx+=4)
	{	
		pVtx[0].pos.x = m_pos[nCntUnit].x - TIME_TEX_SIZE_X * m_fSize[nCntUnit];
		pVtx[0].pos.y = m_pos[nCntUnit].y - TIME_TEX_SIZE_Y * m_fSize[nCntUnit];
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = m_pos[nCntUnit].x;
		pVtx[1].pos.y = m_pos[nCntUnit].y - TIME_TEX_SIZE_Y * m_fSize[nCntUnit];
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = m_pos[nCntUnit].x - TIME_TEX_SIZE_X * m_fSize[nCntUnit];
		pVtx[2].pos.y = m_pos[nCntUnit].y;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = m_pos[nCntUnit].x;
		pVtx[3].pos.y = m_pos[nCntUnit].y;
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

		int Unit = m_nValue[nCntUnit];
		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(TIME_UNIT_SIZE * Unit, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(TIME_UNIT_SIZE * Unit + TIME_UNIT_SIZE, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(TIME_UNIT_SIZE * Unit, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(TIME_UNIT_SIZE * Unit + TIME_UNIT_SIZE, 1.0f);
	
	}
	// 頂点データをアンロックする
	m_pVtxBuffBattle_Record_Time->Unlock();	
}
