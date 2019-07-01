//=============================================================================
// ブロック破壊効果 [Effect_Curve.h]
//=============================================================================
#ifndef _CURVE_H_
#define _CURVE_H_

#include "main.h"
#define TEXTURE_CURVE "data/TEXTURE/effect_ball.png"
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEffect_Curve(void);
void UninitEffect_Curve(void);
void UpdateEffect_Curve(void);
void DrawEffect_Curve(void);

#endif
	//if(GetKeyboardPress(DIK_O))
	//{
	// SetEffect_Curve 
	//(	D3DXVECTOR3 (  0.0f,400.0f,0.0f),	// 開始位置
	//	D3DXVECTOR3 (800.0f,400.0f,0.0f),	// 終了位置
	//	D3DXVECTOR3 (  0.0f,000.0f,0.0f),	// 制御Ａ
	//	D3DXVECTOR3 (800.0f,800.0f,0.0f)	// 制御Ｂ
	//);
	//}
	////PosN(t) = PosA(2*t^3 - 3*t^2 + 1) + PosM(t^3 - 2*t^2 + t) + PosB(-2 * t^3 + 3*t^2) + PosN(t^3 - t^2)
	//for(int nCntCurve = 0; nCntCurve < MAX_CURVE;nCntCurve++)
	//{
	//	if(g_Curve[nCntCurve].bUse == false)
	//		continue;
	//	g_Curve[nCntCurve].life += 0.0011;
	//	//***********今からエルミート曲線を描く

	//	float t = g_Curve[nCntCurve].life;
	//	float tC = 1 - t;

	//	D3DXVECTOR3 SP = g_Curve[nCntCurve].StartPos;
	//	D3DXVECTOR3 EP = g_Curve[nCntCurve].EndPos;
	//	D3DXVECTOR3 CA = g_Curve[nCntCurve].ConPosA;
	//	D3DXVECTOR3 CB = g_Curve[nCntCurve].ConPosB;

	//	g_Curve[nCntCurve].DrawPos.x = 
	//		tC * tC * tC * SP.x + 
	//		3 * tC * tC * t * CA.x + 
	//		3 * tC * t * t * CB.x +
	//		t * t* t * EP.x;
	//	g_Curve[nCntCurve].DrawPos.y = 
	//		tC * tC * tC * SP.y + 
	//		3 * tC * tC * t * CA.y + 
	//		3 * tC * t * t * CB.y +
	//		t * t* t * EP.y;


	//	//最後初期化する
	//	if(g_Curve[nCntCurve].life > VALUE_TIME_CURVE)
	//	{
	//		InitCurvePoint(nCntCurve);
	//	}
	//	SetVerTexEffect_Curve(nCntCurve);

	//}
