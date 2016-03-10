//====================================================
//Battle_InterFaseBG.h
//====================================================
#ifndef _BATTLE_INTERFACE_BG_
#define _BATTLE_INTERFACE_BG_
#include "main.h"
typedef enum
{
	INTERFACE_BG_PHASE_KEEP = 0,
	INTERFACE_BG_PHASE_IN,
	INTERFACE_BG_PHASE_OUT,
	INTERFACE_BG_PHASE_RESET,
	INTERFACE_BG_PHASE_MAX,
}INTERFACE_BG_PHASE;

HRESULT InitBattle_InterFaseBG(void);
void UninitBattle_InterFaseBG(void);
void UpdateBattle_InterFaseBG(void);
void DrawBattle_InterFaseBG(void);
void SetInterPhaseChange(INTERFACE_BG_PHASE phase);

#endif
