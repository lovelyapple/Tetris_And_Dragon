//=============================================================================
//combo counter damage number
//=============================================================================
#include "Battle_Member_Damage_Count.h"
#include "main.h"
#include "game.h"
#include "input.h"
#include "Battle_Player.h"
#include "Battle_Main.h"
#include "Function.h"
#include "sound.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT	MakeVertexCombo_Damage(LPDIRECT3DDEVICE9 pDevice);
void SetTextureCombo_Damage(int nNumberD_ID,int nBox,float fSize,float fAlpa);
void InitCombo_DamageX(int X_ID);
void InitCombo_DamageD(int D_ID,int X_ID,int Idx);
//*****************************************************************************
// グローバル変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureCombo_DamageD = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCombo_DamageD = NULL;		// 頂点バッファスへのポインタ

COMBO_DAMAGE_X		g_Combo_DamageX[COMBO_DAMAGE_MAX_GROUP];
COMBO_DAMAGE_D		g_Combo_DamageD[MAX_COMBO_DAMAGE_D];
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT	InitCombo_Damage(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	for(int nDamageX = 0; nDamageX < COMBO_DAMAGE_MAX_GROUP;nDamageX++)
	{
		InitCombo_DamageX(nDamageX);
	}

	D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
								TEXTURE_COMBO_DAMAGE,		// ファイルの名前
									&g_pTextureCombo_DamageD);
	MakeVertexCombo_Damage( pDevice);
	return S_OK;
}
void InitCombo_DamageX(int X_ID)
{
	BATTLE_T_MEMBER *pMember = GetTMember();
	g_Combo_DamageX[X_ID].BaseDamage	= pMember[X_ID].fATK;
	g_Combo_DamageX[X_ID].DrawPos.x		= pMember[X_ID].pos.x + BATTLE_TEAM_SIZE;
	g_Combo_DamageX[X_ID].DrawPos.y		= pMember[X_ID].pos.y;
	g_Combo_DamageX[X_ID].DrawPos.z		= 0.0f;
	g_Combo_DamageX[X_ID].RecValue		= 0;
	g_Combo_DamageX[X_ID].DestValue		= 0;
	g_Combo_DamageX[X_ID].ChangeValue	= 0;	
	g_Combo_DamageX[X_ID].nStart		= COMBO_DAMAGE_MAX_DIGIT * X_ID;
	g_Combo_DamageX[X_ID].nEnd			= COMBO_DAMAGE_MAX_DIGIT * (X_ID + 1) - 1;
	g_Combo_DamageX[X_ID].fSizeX		= BATTLE_TEAM_SIZE / COMBO_DAMAGE_MAX_DIGIT;
	g_Combo_DamageX[X_ID].fSizeY		= 1.0f;
	g_Combo_DamageX[X_ID].fAlpha		= 1.0f;
	g_Combo_DamageX[X_ID].TimeCount		= COMBO_DAMAGE_TIME;	
	g_Combo_DamageX[X_ID].nPhase		= 0;
	for(int DamgaeD = g_Combo_DamageX[X_ID].nStart,Idx = 0;
		DamgaeD <= g_Combo_DamageX[X_ID].nEnd;DamgaeD ++,Idx++)
		InitCombo_DamageD(DamgaeD,X_ID,Idx);
}

void InitCombo_DamageD(int D_ID,int X_ID,int Idx)
{
	g_Combo_DamageD[D_ID].bUse = false;
	g_Combo_DamageD[D_ID].nBox = 99;
	g_Combo_DamageD[D_ID].fSizeX = g_Combo_DamageX[X_ID].fSizeX;
	g_Combo_DamageD[D_ID].fSizeY = g_Combo_DamageX[X_ID].fSizeY;
	g_Combo_DamageD[D_ID].DrawPos.x = 
		g_Combo_DamageX[X_ID].DrawPos.x - Idx * g_Combo_DamageD[D_ID].fSizeX;
	g_Combo_DamageD[D_ID].DrawPos.y = g_Combo_DamageX[X_ID].DrawPos.y;
	g_Combo_DamageD[D_ID].fAlpha	= g_Combo_DamageX[D_ID].fAlpha;
}
void	UninitCombo_Damage(void)
{
	if(g_pTextureCombo_DamageD != NULL)
	{// テクスチャの開放
		g_pTextureCombo_DamageD->Release();
		g_pTextureCombo_DamageD = NULL;
		g_pTextureCombo_DamageD;
	}
	if(g_pVtxBuffCombo_DamageD != NULL)
	{// テクスチャの開放
		g_pVtxBuffCombo_DamageD->Release();
		g_pVtxBuffCombo_DamageD = NULL;
	}
}
//*****************************************************************************
// 更新処理
//*****************************************************************************

void	UpdateCombo_Damage(void)
{

	for(int nDamageX = 0; nDamageX < COMBO_DAMAGE_MAX_GROUP;nDamageX++)
	{
		switch(g_Combo_DamageX[nDamageX].nPhase)
		{
		case COMBO_DAMAGE_RESET:
			InitCombo_DamageX(nDamageX);
			g_Combo_DamageX[nDamageX].nPhase = COMBO_DAMAGE_KEEP;
			break;
		case COMBO_DAMAGE_KEEP:
			break;
		case COMBO_DAMAGE_UP:
			g_Combo_DamageX[nDamageX].TimeCount--;

			if(g_Combo_DamageX[nDamageX].TimeCount%4 == 0)
				PlaySound(SOUND_LABEL_SE_BLOCK_STOP);

				g_Combo_DamageX[nDamageX].fSizeY *= 1.08f;//サイズをアップ
				g_Combo_DamageX[nDamageX].fSizeY = SetMAX(g_Combo_DamageX[nDamageX].fSizeY,2.0f,1.0f,2);
				g_Combo_DamageX[nDamageX].RecValue += g_Combo_DamageX[nDamageX].ChangeValue;
			if(g_Combo_DamageX[nDamageX].TimeCount <= 0 || g_Combo_DamageX[nDamageX].RecValue > g_Combo_DamageX[nDamageX].DestValue)
			{
				g_Combo_DamageX[nDamageX].RecValue = g_Combo_DamageX[nDamageX].DestValue;
				g_Combo_DamageX[nDamageX].TimeCount = COMBO_DAMAGE_TIME;
				g_Combo_DamageX[nDamageX].nPhase = COMBO_DAMAGE_KEEP;
			}
			break;
		case COMBO_DAMAGE_REDUCE:
			g_Combo_DamageX[nDamageX].TimeCount--;
			g_Combo_DamageX[nDamageX].fAlpha *= 0.96f;
			if(g_Combo_DamageX[nDamageX].TimeCount <= 0)
			{
				g_Combo_DamageX[nDamageX].nPhase = COMBO_DAMAGE_RESET;
			}
			break;
		case COMBO_DAMAGE_MAX:
			break;
			}
			//サイズの常時回復を設定
			if(g_Combo_DamageX[nDamageX].fSizeY != 1.0f)// * COMBO_DAMAGE_SIZE_Y)
			{
				if(g_Combo_DamageX[nDamageX].TimeCount%2 == 0)
					g_Combo_DamageX[nDamageX].fSizeY *= 0.93f;
				if(g_Combo_DamageX[nDamageX].fSizeY < 1.05f)// * COMBO_DAMAGE_SIZE_Y)
					g_Combo_DamageX[nDamageX].fSizeY = 1.0f;// * COMBO_DAMAGE_SIZE_Y;
			}
	}
	for(int nDamageX = 0; nDamageX < COMBO_DAMAGE_MAX_GROUP;nDamageX++)
	{
		if(g_Combo_DamageX[nDamageX].RecValue == 0)
			continue;
		int nBoxWk[COMBO_DAMAGE_MAX_DIGIT]	= {99};
		bool DrawStart = false;//描画
		int VisualDigit	= 0;//使われている桁の数
		for(int Idx  = COMBO_DAMAGE_MAX_DIGIT; Idx > 0; Idx --)
		{
			int Value = (int)(g_Combo_DamageX[nDamageX].RecValue) %
				(int)(powf(10.0f,(float)Idx))/(int)(powf(10.0f,(float)(Idx - 1)));
			if(Value != 0 )
				DrawStart = true;
			
			if(DrawStart == true)
			{
				nBoxWk[Idx -1] = Value;
				VisualDigit ++ ;
			}
			else
				nBoxWk[Idx -1] = 99;
		}
		float DrawSize = BATTLE_TEAM_SIZE / VisualDigit;//描画のサイズを再調整する
		g_Combo_DamageX[nDamageX].fSizeX = DrawSize;
		if(DrawSize > BATTLE_TEAM_SIZE /4)
			DrawSize = BATTLE_TEAM_SIZE /4;
		for(int nCntD = g_Combo_DamageX[nDamageX].nStart,Idx = 0;
			nCntD <= g_Combo_DamageX[nDamageX].nEnd;
			nCntD++,Idx++)
		{
			if(nBoxWk[Idx] == 99)
			{
				g_Combo_DamageD[nCntD].bUse = false;
			}
			else
			{
				g_Combo_DamageD[nCntD].bUse = true;
				g_Combo_DamageD[nCntD].nBox = nBoxWk[Idx];
				g_Combo_DamageD[nCntD].fSizeX = DrawSize;
				g_Combo_DamageD[nCntD].fSizeY = g_Combo_DamageX[nDamageX].fSizeY;
				g_Combo_DamageD[nCntD].fAlpha	= g_Combo_DamageX[nDamageX].fAlpha;
				g_Combo_DamageD[nCntD].DrawPos.x = g_Combo_DamageX[nDamageX].DrawPos.x - Idx * DrawSize;
				SetTextureCombo_Damage(nCntD,nBoxWk[Idx],DrawSize,g_Combo_DamageD[nCntD].fAlpha);
			}
		}
	}
}
bool GetComboDamageKeep(void)
{
	bool Res = true;
	for(int nDamageX = 0; nDamageX < COMBO_DAMAGE_MAX_GROUP;nDamageX++)
	{
		if(g_Combo_DamageX[nDamageX].nPhase != COMBO_DAMAGE_KEEP)
		{
			Res = false;
			break;
		}
	}
	return Res;
}

void SetComboDamageType(int MemberID,int ChangeValue,int Type,int Time)
{
	g_Combo_DamageX[MemberID].nPhase = Type;
	switch(Type)
	{
	case COMBO_DAMAGE_RESET:
			break;
	case COMBO_DAMAGE_UP:
		if(g_Combo_DamageX[MemberID].ChangeValue != 0 )
		{
			g_Combo_DamageX[MemberID].DestValue += ChangeValue;
			g_Combo_DamageX[MemberID].ChangeValue = 
				(g_Combo_DamageX[MemberID].DestValue - g_Combo_DamageX[MemberID].RecValue)/(float)Time;
		}
		else
		{
		g_Combo_DamageX[MemberID].ChangeValue = ChangeValue / (float)Time;
		g_Combo_DamageX[MemberID].DestValue = g_Combo_DamageX[MemberID].RecValue + ChangeValue;
		}
		if(g_Combo_DamageX[MemberID].ChangeValue < 1.0f)
			g_Combo_DamageX[MemberID].ChangeValue = 1.0f;
		g_Combo_DamageX[MemberID].TimeCount = Time;
			break;
	case COMBO_DAMAGE_REDUCE:
		break;
	}

}
void ResetCombo(void)
{
	for(int nDamageX = 0; nDamageX < COMBO_DAMAGE_MAX_GROUP;nDamageX++)
	{
		if(g_Combo_DamageX[nDamageX].nPhase != COMBO_DAMAGE_KEEP)
			SetComboDamageType(nDamageX,0,COMBO_DAMAGE_REDUCE,0);
	}

}

//*****************************************************************************
// 描画処理
//*****************************************************************************
void	DrawCombo_Damage(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffCombo_DamageD, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetTexture(0, g_pTextureCombo_DamageD);

	// ポリゴンの描画
	for(int nDamageD = 0; nDamageD < MAX_COMBO_DAMAGE_D; nDamageD++)
	{
		if(g_Combo_DamageD[nDamageD].bUse == false)
			continue;
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,(nDamageD * 4),NUM_POLYGON);
	}	
}
//*****************************************************************************
// 頂点作成
//*****************************************************************************
HRESULT	MakeVertexCombo_Damage(LPDIRECT3DDEVICE9 pDevice)
{
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * MAX_COMBO_DAMAGE_D,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffCombo_DamageD,
		NULL)))
	{
		return E_FAIL;
	}
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffCombo_DamageD->Lock(0, 0, (void**)&pVtx, 0);

		for(int nDamageD = 0; nDamageD < MAX_COMBO_DAMAGE_D; nDamageD++,pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].pos.x = g_Combo_DamageD[nDamageD].DrawPos.x - g_Combo_DamageD[nDamageD].fSizeX;
			pVtx[0].pos.y = g_Combo_DamageD[nDamageD].DrawPos.y - COMBO_DAMAGE_SIZE_Y;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_Combo_DamageD[nDamageD].DrawPos.x;
			pVtx[1].pos.y = g_Combo_DamageD[nDamageD].DrawPos.y - COMBO_DAMAGE_SIZE_Y;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_Combo_DamageD[nDamageD].DrawPos.x - g_Combo_DamageD[nDamageD].fSizeX;;
			pVtx[2].pos.y = g_Combo_DamageD[nDamageD].DrawPos.y;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_Combo_DamageD[nDamageD].DrawPos.x;
			pVtx[3].pos.y = g_Combo_DamageD[nDamageD].DrawPos.y;
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
			pVtx[0].tex = D3DXVECTOR2((float)g_Combo_DamageD[nDamageD].nBox * 0.1f,			0.0f);
			pVtx[1].tex = D3DXVECTOR2((float)g_Combo_DamageD[nDamageD].nBox * 0.1f + 0.1f,	0.0f);	
			pVtx[2].tex = D3DXVECTOR2((float)g_Combo_DamageD[nDamageD].nBox * 0.1f,			1.0f);
			pVtx[3].tex = D3DXVECTOR2((float)g_Combo_DamageD[nDamageD].nBox * 0.1f + 0.1f,	1.0f);
		}
		// 頂点データをアンロックする
		g_pVtxBuffCombo_DamageD->Unlock();
	}
	return S_OK;

}
void SetTextureCombo_Damage(int nNumberD_ID,int nBox,float fSize,float fAlpa)
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffCombo_DamageD->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += nNumberD_ID *4;
			// 頂点座標の設定
			pVtx[0].pos.x = g_Combo_DamageD[nNumberD_ID].DrawPos.x - fSize;
			pVtx[0].pos.y = g_Combo_DamageD[nNumberD_ID].DrawPos.y - COMBO_DAMAGE_SIZE_Y * g_Combo_DamageD[nNumberD_ID].fSizeY;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_Combo_DamageD[nNumberD_ID].DrawPos.x;
			pVtx[1].pos.y = g_Combo_DamageD[nNumberD_ID].DrawPos.y - COMBO_DAMAGE_SIZE_Y * g_Combo_DamageD[nNumberD_ID].fSizeY;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_Combo_DamageD[nNumberD_ID].DrawPos.x - fSize;
			pVtx[2].pos.y = g_Combo_DamageD[nNumberD_ID].DrawPos.y;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_Combo_DamageD[nNumberD_ID].DrawPos.x;
			pVtx[3].pos.y = g_Combo_DamageD[nNumberD_ID].DrawPos.y;
			pVtx[3].pos.z = 0.0f;
			// rhwの設定
			pVtx[0].rhw =
				pVtx[1].rhw =
				pVtx[2].rhw =
				pVtx[3].rhw = 1.0f;

			// 頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpa);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpa);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpa);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpa);

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2((float)g_Combo_DamageD[nNumberD_ID].nBox * 0.1f,			0.0f);
			pVtx[1].tex = D3DXVECTOR2((float)g_Combo_DamageD[nNumberD_ID].nBox * 0.1f + 0.1f,	0.0f);	
			pVtx[2].tex = D3DXVECTOR2((float)g_Combo_DamageD[nNumberD_ID].nBox * 0.1f,			1.0f);
			pVtx[3].tex = D3DXVECTOR2((float)g_Combo_DamageD[nNumberD_ID].nBox * 0.1f + 0.1f,	1.0f);


		//頂点データをアンロックする
		g_pVtxBuffCombo_DamageD->Unlock();
	}
}
COMBO_DAMAGE_X	*GetDamageCombo(void)
{
	return &g_Combo_DamageX[0];
}
