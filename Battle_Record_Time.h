//=============================================================================
//
// recordTime処理[Battle_Record_Time.h]
//
//=============================================================================
#ifndef _BATTLE_RECORD_TIME_
#define _BATTLE_RECORD_TIME_
#include "main.h"
#include "Battle_Main.h"
#define TIMEUNIT	(3) 
#define FIELNAME_TIME_R_SETTING	("Game_Record_Time_Setting.csv")
#define FIELNAME_RECORD			("Game_Record_Time_Data.csv")
typedef enum
{
	RECORD_MODE_START = 0,
	RECORD_MODE_STOP,
	RECORD_MODE_RESET,
	RECORD_MODE_MAX,
}RECORD_MODE;
typedef struct
{
	int	Decimal;
	int Second;
	int	Minute;
}TIME_STRUCT;

typedef enum
{
	RECORD_UNIT_DECIMAL00 = 0,
	RECORD_UNIT_DECIMAL01,
	RECORD_UNIT_SEC00,
	RECORD_UNIT_SEC01,
	RECORD_UNIT_MIN00,
	RECORD_UNIT_MIN01,
	RECORD_UNIT_MAX,
}RECORD_UNIT;
class CBattle_Record_Time
{
private:
	bool	m_bUse;							// 使用中が
	RECORD_MODE m_TimeMode;					// 現在のモード
	D3DXVECTOR3	m_pos[RECORD_UNIT_MAX];		// 各時間要素の描画場所
	int			m_nValue[RECORD_UNIT_MAX];	// 各時間要素の値
	float		m_fSize[RECORD_UNIT_MAX];	// 各時間要素の大きさ
	TIME_STRUCT m_TimeLive;					// 現在の時間

	LPDIRECT3DTEXTURE9	m_pTextureBattle_Record_Time;	// テクスチャバッファ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffBattle_Record_Time;// テクスチャ頂点バッファ
	void SetVerTexBattle_Record_Time(void);				// テクスチャ頂点バッファの設定
	HRESULT MakeVerTexBattle_Record_Time(LPDIRECT3DDEVICE9 pDevice);// テクスチャ頂点の作成
public:
	TIME_STRUCT	m_RecordTime;				// 記録の時間
		
	void ResetTime(void);					// 現在の時間をれセット
	HRESULT InitBattle_Record_Time(void);	// 初期化処理
	void UpdateBattle_Record_Time(void);	// 更新処理
	TIME_STRUCT GetRecordLively(void);		// 現在時間の取得
	void UninitBattle_Record_Time(void);	// 終了処理
	void Draw_Battle_Record_Time(void);		// 描画処理
	void Set_Time_Mode(RECORD_MODE mode);	//状態変化設定
	void SetUp_Record_Time(D3DXVECTOR3 Inputpos,float fInputSize[]);
					//初期設定,初期化と違うよ
};
#endif
