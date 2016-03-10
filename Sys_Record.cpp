//=============================================================================
//RecordMain処理[Sys_Record.cpp]
//=============================================================================
#include "Battle_Record_Time.h"
#include "Sys_Record.h"
#include "main.h"
#include "input.h"
#include <stdio.h>
//*****************************************************************************
// グローバル変数D3DX3_INIT
//*****************************************************************************
void DrawTimeRecord(void);
CBattle_Record_Time	g_Time_Record;
SYS_RECORD_MODE		g_Sys_Record_Mode;
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitRecord_System(void)
{
	g_Sys_Record_Mode = SYS_RECORD_MAX;
	g_Time_Record.InitBattle_Record_Time();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void	UninitRecord_System(void)
{
	g_Time_Record.UninitBattle_Record_Time();
}

//=============================================================================
// 更新処理
//=============================================================================
void	UpdateRecord_System(void)
{
	if(GetKeyboardPress(DIK_LSHIFT))
	{
		if(GetKeyboardTrigger(DIK_O))
			g_Time_Record.InitBattle_Record_Time();
	}
	//stop key debug
	switch(g_Sys_Record_Mode)
	{
	case SYS_RECORD_START:
		g_Time_Record.Set_Time_Mode(RECORD_MODE_START);
		break;
	case SYS_RECORD_STOP:
		g_Time_Record.Set_Time_Mode(RECORD_MODE_STOP);
		break;
	case SYS_RECORD_RESET:
		g_Time_Record.Set_Time_Mode(RECORD_MODE_RESET);
		break;
	case SYS_RECORD_MAX:
		break;
	}



	g_Time_Record.UpdateBattle_Record_Time();
}
HRESULT	SaveRecord(void)
{
	TIME_STRUCT Record = g_Time_Record.GetRecordLively();
	float SizeWK[TIMEUNIT] = {};
	D3DXVECTOR3 posWK;
	char	strWK[256] = {};

	if(Record.Minute < g_Time_Record.m_RecordTime.Minute)
		if(Record.Second < g_Time_Record.m_RecordTime.Second)
			if(Record.Decimal < g_Time_Record.m_RecordTime.Decimal)
			{
				g_Time_Record.m_RecordTime = Record;

				FILE *fp;
				if((fp = fopen(FIELNAME_RECORD,"w")) == NULL)
					return S_FALSE;

				fprintf(fp," RecentlyTimeRecord:,%d,%d,%d\n",Record.Decimal,Record.Second,Record.Minute);


				fclose(fp);

			}
			return S_OK;
}

//=============================================================================
// システムレコードモードの変更
//=============================================================================
void SetSysRecordMode(SYS_RECORD_MODE g_Mode)
{
	g_Sys_Record_Mode = g_Mode;
}

//=============================================================================
// 描画処理
//=============================================================================
void	DrawRecord_System(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	g_Time_Record.Draw_Battle_Record_Time();
	DrawTimeRecord();
}
//=============================================================================
// FPS表示処理
//=============================================================================
void DrawTimeRecord(void)
{
	LPD3DXFONT	Font = GetGetFont();
	RECT rect = {1000, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	char aStr[256];

	wsprintf(&aStr[0], "現在の記録 %d 分%d 秒 %d \n", g_Time_Record.m_RecordTime.Minute,
		g_Time_Record.m_RecordTime.Second,
		g_Time_Record.m_RecordTime.Decimal);

	// テキスト描画
	Font->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(0xff, 0xff, 0xff, 0xff));
}
