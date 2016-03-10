//=============================================================================
//
// 背景処理 [bg.h]
// HAL名古屋
//
//=============================================================================
#ifndef _FUNCTION_H_
#define _FUNCTION_H_

#include "main.h"

#define D3DX3_INIT ( D3DXVECTOR3(0.0f,0.0f,0.0f))
typedef struct
{
	bool bUse;
	int	Name;
	int BonusValue;
}RAND_STRUCT;
float SetMAX(float NOW, float MAX, float MIN,int nType);
int GetRandColor(RAND_STRUCT input[],int Count);

HRESULT InitFun(void);
void UninitFun(void);
void UpdateFunction(void);

#endif
