//=============================================================================
//
// サウンド処理 [sound.h]
// HAL名古屋
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*************************************
// サウンドファイル
//*************************************
typedef enum
{
	SOUND_LABEL_BGM000 = 0,		// BGM0
	SOUND_LABEL_BGM001,			// BGM1
	SOUND_LABEL_BGM002,			// BGM2
	SOUND_LABEL_BGM003,			// BGM3
	SOUND_LABEL_SE_DECIDE,		// 決定音
	SOUND_LABEL_SE_CANCEL,		// キャンセル音
	SOUND_LABEL_SE_SELECT,		// 選択音
	SOUND_LABEL_SE_SHOT,		// 弾発射音
	SOUND_LABEL_SE_LASER,		// レーザー発射音
	SOUND_LABEL_SE_LOCKON,		// ロックオン音
	SOUND_LABEL_SE_HIT000,		// ヒット音
	SOUND_LABEL_SE_HIT001,		// ヒット音
	SOUND_LABEL_SE_GUARD,		// ガード音
	SOUND_LABEL_SE_EXPLOSION,	// 爆発音
	SOUND_LABEL_SE_JUMP,		// ジャンプ音
	SOUND_LABEL_SE_ICE_SLASH,	// アイスのスラッシュ音
	SOUND_LABEL_SE_BLOCK_MOVE,  // ブロックの操作音
	SOUND_LABEL_SE_BLOCK_ROT,	// ブロック回転の音
	SOUND_LABEL_SE_BLOCK_STOP,	// ブロック停止の音
	SOUND_LABEL_OPENING_BGM,	// 開始BGM
	SOUND_LABEL_STAGE_BGM,		// ステージ中
	SOUND_LABEL_SE_BOSS_ALERT,	// ボス戦の警告
	SOUND_LABEL_BGM_BOSS,		//ボス戦BGM
	SOUND_LABEL_SE_ATTACK_FIRE,
	SOUND_LABEL_SE_ATTACK_WIND,
	SOUND_LABEL_SE_ATTACK_WATER,
	SOUND_LABEL_SE_ATTACK_LIGHT,
	SOUND_LABEL_SE_ATTACK_DARK,
	SOUND_LABEL_SE_ATTACK_ENEMY_NORMAL,
	SOUND_LABEL_SE_ENEMY_SKILL_USE,
	SOUND_LABEL_SE_TETRIS_PIECE_CHANGE,
	SOUND_LABEL_SE_BATTLE_PLAYER_USE_SKILL,
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
