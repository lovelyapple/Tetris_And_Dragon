//=============================================================================
//
// 数列表示処理
// 
//
//=============================================================================
#ifndef _DAMAGE_NUMBER_
#define _DAMAGE_NUMBER_

#include "main.h"


//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	bool	bUse;			//使われているかどうか
	int		InitDamage;		//ダメージ値
	int		nChangePhase;	//現在の状態
	int		nDamageType;	//属性の相性
	int		nDestDamage;	//最終ダメージ
	float	nRecDamage;		//現在のダメージ
	float	fChangePerTime;	//時間単位の変化量
	int		nColor;			//色
	int		nD_Start;		//所有するＤの開始点
	int		nD_End;			//所有するＤの終止点
	int		nLife;			//ダメージ出現時間
	D3DXVECTOR3	pos;		//描画開始場所
	float	fSize;			//大きさの倍数
	D3DXVECTOR3 fMove;		//移動方向
}DAMAGE_NUMBER_X;
typedef struct
{
	bool	bUse;
	bool	bDraw;
	int		nBox;
	D3DXVECTOR3	pos;
}DAMAGE_NUMBER_D;

typedef enum 
{
	DAMAGE_NORMAL = 0,
	DAMAGE_GREAT,
	DAMAGE_BAD,
}DAMAGE_TYPE;
typedef enum
{
	DAMAGE_PHASE_CHARGE = 0,
	DAMAGE_PHASE_PAUSE,
	DAMAGE_PHASE_CHANGE,
	DAMAGE_PHASE_INCREASE,
	DAMAGE_PHASE_LOSING,
	DAMAGE_PHASE_KEEP,
	DAMAGE_PHASE_STOP,
	DAMAGE_PHASE_FADE_OUT,
	DAMAGE_PHASE_MAX,
}DAMAGE_PHASE;

//*****************************************************************************
// プロトタイプ宣
//*****************************************************************************
HRESULT	InitDamage_NumberD(void);
void	UpdateDamage_NumberD(void);
void	UninitDamage_NumberD(void);
void	DrawDamage_NumberD(void);
void SetDamageNumberX(int DestDamage,int InitDamage,int DamageType,D3DXVECTOR3 pos);

#endif
