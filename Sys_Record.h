//=============================================================================
//
// RecordMain処理[Sys_Record.h]
//
//=============================================================================
#ifndef _BATTLE_SYS_RECORD_
#define _BATTLE_SYS_RECORD_

#include "main.h"

typedef enum
{
	SYS_RECORD_START = 0,
	SYS_RECORD_STOP,
	SYS_RECORD_RESET,
	SYS_RECORD_MAX,
}SYS_RECORD_MODE;
void	SetSysRecordMode(SYS_RECORD_MODE g_Mode);

HRESULT InitRecord_System(void);
void	UninitRecord_System(void);
void	UpdateRecord_System(void);
void	DrawRecord_System(void);
HRESULT	SaveRecord(void);

#endif
