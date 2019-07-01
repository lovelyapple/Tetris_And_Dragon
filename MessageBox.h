//====================================================
//MessageBox.h
//====================================================
#ifndef _MASSAGE_BOX_
#define _MASSAGE_BOX_
#include "main.h"

//******************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_MESSAGE_BOX_BLOCK ("data/TEXTURE/MessageBoxBlock.png")
#define MESSAGE_BOX_SIZE_X	(60)
#define MESSAGE_BOX_SIZE_Y	(24)
#define	MESSAGE_BOX_MAX		(16)
#define MESSAGE_MAX_LIFE	(5)
#define MESSAGE_MAX_LIFE_KEEP	(30)
typedef enum
{
	MESSAGE_BOX_PHASE_START = 0,
	MESSAGE_BOX_PHASE_OPEN_X,
	MESSAGE_BOX_PHASE_OPEN_KEEP,
	MESSAGE_BOX_PHASE_OPEN_Y,
	MESSAGE_BOX_PHASE_KEEP00,
	MESSAGE_BOX_PHASE_CLOSE_Y,
	MESSAGE_BOX_PHASE_CLOSE_KEEP,
	MESSAGE_BOX_PHASE_CLOSE_X,
	MESSAGE_BOX_PHASE_KEEP01,
	MESSAGE_BOX_PHASE_INIT,
	MESSAGE_BOX_PHASE_MAX,
}MESSAGE_BOX_PHASE;
typedef enum
{
	MESSAGE_WORDS_HOROBIYO = 0,
	MESSAGE_WORDS_NAMETERU,
	MESSAGE_WORDS_MAX,
}MESSAGE_WORDS;
typedef struct
{
	bool bUse;
	int			nLife;			// 時間制御
	MESSAGE_BOX_PHASE phase;	// 進み度合い
	D3DXVECTOR3	posCenter;		// 中心位置
	D3DXVECTOR3	posUp;			// 上段位置
	D3DXVECTOR3	posDown;		// 下段位置
	D3DXVECTOR3 move;
	float		fAlpha;			// 透明度
	float		fWidth;
	float		fHeight;
	int			MessageWordsID;	//メッセージ内容の番号
}MESSAGE_BOX;

typedef struct
{
	bool bUse;
	D3DXVECTOR3 pos;
	float	fWidth;
	float	fHeight;
	MESSAGE_WORDS	MessageWordsID;	//メッセージの内容
}MESSAGE_WORDS_STRUCT;

//******************************************************************************
// プロトタイプ宣言
//*****************************************************************************


HRESULT InitMessageBox(void);
void UninitMessageBox(void);
void UpdateMessageBox(void);
void DrawMessageBox(void);
int OpeBoxIDBox(D3DXVECTOR3 Centerpos,
					float		Widht,
					float		Height,
					MESSAGE_WORDS	MessageWordsID
					);
void CloseMessageBox(int BoxID);

#endif
