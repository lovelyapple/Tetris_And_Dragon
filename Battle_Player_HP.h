//====================================================
//Battle_Player_HP.h
//====================================================
#ifndef _BATTLE_PLAYER_HP_
#define _BATTLE_PLAYER_HP_
#include "main.h"
#include "Battle_Main.h"
#define BATTLE_PLAYER_HP_POS_X		(SYS_BATTLE_POS_START_X + 20.0f)
#define BATTLE_PLAYER_HP_POS_Y		(VALUE_FIELD_END_Y - BATTLE_TEAM_SIZE * 1.4f)
#define BATTLE_PLAYER_HP_SIZE		(20.0f)
#define BATTLE_PLAYER_HP_ENTITY_POS_X	(BATTLE_PLAYER_HP_POS_X + BATTLE_PLAYER_HP_SIZE)
#define BATTLE_PLAYER_HP_ENTITY_POS_Y	(BATTLE_PLAYER_HP_POS_Y - BATTLE_PLAYER_HP_SIZE * 0.2f)
#define BATTLE_PLAYER_HP_ENTITY_SIZE_X	(SYS_BATTLE_SIZE_X - (BATTLE_PLAYER_HP_SIZE + 10.0f) * 2)
#define BATTLE_PLAYER_HP_ENTITY_SIZE_Y	(BATTLE_PLAYER_HP_SIZE * 0.5f)
#define BATTLE_PLAYER_HP_RIGHT_POS		(BATTLE_PLAYER_HP_ENTITY_POS_X + BATTLE_PLAYER_HP_ENTITY_SIZE_X)
#define	BATTLE_PLAYER_HP_LIGHT_SPEED	(10.0f)
#define TEXTURE_PLAYER_HP_CAGE		("data/TEXTURE/Player_HP_Outline.png")
#define TEXTURE_PLAYER_HP_ENTITY	("data/TEXTURE/CageEntity.png")
#define TEXTURE_PLAYER_HP_CAGE_UNIT_SIZE	(1.0f / 4.0f)
#define TEXTURE_PLAYER_HP_ENTITY_UNIT_SIZE	(1.0f / 3.0f)

#define TEXTURE_BATTLE_PLAYER_HP_NUMBER	("data/TEXTURE/Number_Big.png")
#define BATTLE_PLAYER_HP_NUMBER_DIGIT	(8)
#define BATTLE_PLAYER_HP_NUMBER_SIZE_X	(8.0f)
#define BATTLE_PLAYER_HP_NUMBER_SIZE_Y	(16.0f)
#define BATTLE_PLAYER_HP_NUMBER_POS_X_REC	(BATTLE_PLAYER_HP_RIGHT_POS - 50.0f)
#define BATTLE_PLAYER_HP_NUMBER_UNIT_SIZE	(1.0f / 10.0f)
#define BATTLE_PLAYER_HP_NUMBER_POS_Y_REC	(BATTLE_PLAYER_HP_POS_Y + BATTLE_PLAYER_HP_NUMBER_SIZE_Y)
#define BATTLE_PLAYER_HP_NUMBER_POS_X_MAX	(BATTLE_PLAYER_HP_NUMBER_POS_X_REC + BATTLE_PLAYER_HP_NUMBER_SIZE_X * BATTLE_PLAYER_HP_NUMBER_DIGIT)

#define BATTLE_PLAYER_HP_SHAKE_TIME		(60)

HRESULT InitBattle_Player_HP(void);
void UninitBattle_Player_HP(void);
void UpdateBattle_Player_HP(void);
void DrawBattle_Player_HP(void);
void SetBattle_Player_HP(void);
void SetBattle_Player_HP_Shake(void);
#endif