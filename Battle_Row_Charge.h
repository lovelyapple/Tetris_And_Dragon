//=============================================================================
// ブロック破壊効果 [BlockShining.h]
//=============================================================================
#ifndef _ROW_CHARGE_
#define _ROW_CHARGE_

#include "main.h"
typedef enum
{
	ROW_CHARGE_KEEP = 0,
	ROW_CHARGE_RESET,
	ROW_CHARGE_INCREASING,
	ROW_CHARGE_DECREASE,
	ROW_CHARGE_MAX_TYPE,
}ROW_CHARGE_TYPE;

#define ROW_CHARGE_MAX_VALUE		(100)
#define ROW_VALUE_RESET				((float)ROW_CHARGE_MAX_VALUE / (60.0f * 5.0f))//毎フレームごとに減る量
#define ROW_MAX_ROW					(10)
#define ROW_VALUE_PER_ROW			(ROW_CHARGE_MAX_VALUE / ROW_MAX_ROW)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitRowCharge(void);
void UninitRowCharge(void);
void UpdateRowCharge(void);
void DrawRowCharge(void);
void SetRowCharge(int Row,int nType);
int	GetRowChargeType(void);
int	GetRowChargeValue(void);
#endif
