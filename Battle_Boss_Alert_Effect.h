//====================================================
//Battle_Effect_Alert.h
//====================================================
#ifndef _BATTLE_EFFECT_BOSS_ALERT_
#define _BATTLE_EFFECT_BOSS_ALERT_
#include "main.h"

#define BATTLE_BOSS_ALERT_EYE_POS_X			(SYS_BATTLE_POS_START_X + SYS_BATTLE_SIZE_X/2)
#define BATTLE_BOSS_ALERT_EYE_POS_Y			(SYS_BATTLE_BATTLE_SIZE_Y/3)
#define BATTLE_BOSS_ALERT_EYE_SIZE_X		(SYS_BATTLE_SIZE_X/2)
#define BATTLE_BOSS_ALERT_EYE_SIZE_Y		(SYS_BATTLE_BATTLE_SIZE_Y/4)
#define BATTLE_BOSS_ALERT_EYE_MAX			(8)
#define BATTLE_BOSS_ALERT_EYE_LIFE			(30)
#define BATTLE_BOSS_ALERT_LINE_SIZE_X		(SYS_BATTLE_SIZE_X /2)
#define BATTLE_BOSS_ALERT_LINE_SIZE_Y		(25.0f)
#define BATTLE_BOSS_ALERT_LINE_POS_X		(SYS_BATTLE_POS_START_X + SYS_BATTLE_SIZE_X/2)
#define BATTLE_BOSS_ALERT_LINE_POS_Y		(40.0f)
#define BATTLE_BOSS_ALERT_LINE_MAX			(2)
#define ATTLE_BOSS_ALERT_START_TIME			(60)
typedef enum
{
	ALERT_PHASE_START = 0,
	ALERT_PHASE_EYE_SHINE,
	ALERT_PHASE_OUT,
	ALERT_PHASE_RESET,
	ALERT_PHASE_MAX,
}ALERT_PHASE;
HRESULT InitBattle_Effect_Alert(void);
void UninitBattle_Effect_Alert(void);
void UpdateBattle_Effect_Alert(void);
void DrawBattle_Effect_Alert(void);
ALERT_PHASE	GetBOSS_Alert_Phase(void);
void	SetBossAlertStart(void);


#endif
