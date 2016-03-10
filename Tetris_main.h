//=============================================================================
//
// テトリスのメイン処理[Tetris_main.h]
//
//=============================================================================
#ifndef _TETRIS_MAIN_
#define _TETRIS_MAIN_


#include "main.h"
#include "game.h"
#include "Battle_Main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_GROUP			(2)				// 最大ブロック組数
#define MAX_GROUP_BLOCK		(7)				// 最大グループのブロック数
#define MAX_BLOCK			(64)			// 最大ブロック数
#define MAX_FIELD_ROW		(19)			// フィールド最大行数
#define MAX_FIELD_COLUMN	(13)			// フィールド最大列数
#define MAX_FIELD_BLOCK		(MAX_FIELD_ROW * MAX_FIELD_COLUMN)
											//フィールド上最大ブロック数
#define MAX_FORM_PLACE		(4)				// 形が持つ最大のブロック数

#define TEX_FIELD_BG		("data/TEXTURE/Tetris_Main02.jpg")
#define TEX_BLOCK_TEXTURE	"data/TEXTURE/TETRIS-Block.png"	// ブロックの画像データ
#define TEX_BLOCK_TEXTURE_TEST	"data/TEXTURE/player000.png"	// ブロックの画像データ
#define	KEY_LEFT			(DIK_A)			// left入力
#define KEY_RIGHT			(DIK_D)			// Right入力
#define KEY_DOWN			(DIK_S)			// down 入力
#define KEY_ROT_LEFT		(DIK_SPACE)			// Rot入力
#define KEY_ROT_RIGHT		(DIK_RIGHT)			// Rot入力
#define KEY_START			(DIK_SPACE)			// テトリススタート
#define KEY_SETUP			(DIK_UP)			// テトリスリセット
#define KEY_DROP			(DIK_W)			// ブロック瞬間落ち
#define KEY_CHEAT_UP		(DIK_Q)			// ブロックを上昇させるキー
#define VALUE_TIME_DELAY	(60)			// 延遅時間
#define VALUE_MAX_ROW_COMBO	(10)
#define GROUP_USING			(0)				//使用グループのナンバー
#define GROUP_SHADOW		(1)				//グループの射影

#define TETRIS_BG_SIZE_X	(VALUE_FIELD_SIZE_X)
#define TETRIS_BG_SIZE_Y	(VALUE_FIELD_SIZE_Y)
#define TETRIS_BG_POS_X		(VALUE_FIELD_START_X)
#define TETRIS_BG_POS_Y		(VALUE_FIELD_START_Y)

//*****************************************************************************
// 構造体宣言
//*****************************************************************************
typedef enum
{
	TETRIS_START = 0,
	TETRIS_STOP,
	TETRIS_SET_Q,
	TETRIS_WAIT_SET,
	TETRIS_WAIT_TIME,
	TETRIS_WAIT_DELETE,
	TETRIS_UPDATE,
	TETRIS_ROW_CHARGE,
	TETRIS_DELETE_WAIT,
	TETRIS_TURN_WAIT,
	TETRIS_NOTH,
}TETRIS_STATS;
typedef struct
{
	int nType;			// フィール上ブロックの状態
	int nColor;			// フィール上ブロックの色
	D3DXVECTOR2 pos;	// フィール上ブロックの描画位置
	float g_fAlpha;			// 透明度
}FIELD;//フィールド
typedef enum
{
	FIELD_NONE = 0,		// フィールドブロックが使われていない状態
	FIELD_USE,			// フィールドブロックがブロックとして使われている状態
	FIELD_WALL,			// フィールドブロックが壁と床ブロックとして使われている状態
	FIELD_MP,		// フィールドブロックがスキルブロックの状態
}FIELD_TYPE;//フィールドの状態
typedef struct
{
	bool bUse;			// 単体ブロックの使用状態
	int Type;			// 単体ブロックの分類実ブロックか，射影ブロックか
	int nColor;			// 単体ブロックの色
	D3DXVECTOR2 FormPos;// 単体ブロックが持つ形の位置
	D3DXVECTOR2	Place;	// 単体ブロックのフィールド上位置
	D3DXVECTOR2 Pos;	// 単体ブロックの描画位置

}BLOCK;//ブロック
typedef struct 
{
	int nCount;							// 形状が持つブロック数
	int Rot;							// 形状が持つ回転種類
	D3DXVECTOR2 Place[MAX_FORM_PLACE];	// 形状の各ブロックの相対位置
}GROUP_FORM;
typedef struct
{
	int nType;			// グループの状態
	int nColor;			// グループの色
	int B_start;		// グループが持つブロックのスタートナンバー
	int B_end;			// グループが持つブロックのエンドナンバー
	D3DXVECTOR2 Place;	// グループがフィールド上の位置
	D3DXVECTOR2 Pos;	// グループの描画位置
	int			NumForm;// グループの形状ナンバー
	GROUP_FORM	Form;	// グループの形状詳細設定
	int	Rotation;		// 回転可能の最大パターン数
}GROUP;//グループ
typedef enum
{
	GROUP_BACK = 0,		// グループが使われていない状態
	GROUP_USE,			// グループが使われている状態
	GROUP_WAIT,			// グループが待機状態
}GROUP_TYPE;//グループの状態
typedef enum
{
	BLOCK_RED = 0,
	BLOCK_GREEN,
	BLOCK_BLUE,
	BLOCK_LIGHT,
	BLOCK_DARK,
	BLOCK_PINK,
	BLOCK_WALL,
	BLOCK_MP,
	BLOCK_MAX_COLOR,
	BLOCK_RAND_COLOR,
}BLOCK_COLOR;//色の情報の分類
typedef enum
{
	FIELD_MOVE_IN = 0,
	FIELD_MOVE_OUT,
	FIELD_MOVE_KEEP,
}FIELD_MOVE_TYPE;//フィールドの移動タイプ
typedef enum
{	
	FORM_La=0,
	FORM_Lb,
	FORM_O,
	FORM_Sa,
	FORM_Sb,
	FORM_I,
	FORM_T,
	FORM__NONE,
	MAX_FORM_TYPE,
}FORM_TYPE;//形構造体の分類
typedef enum
{	
	BLOCK_NORMAL = 0,
	BLOCK_GRAVITY,
	BLOCK_SHADOW,
	//BLOCK_MP,
	MAX_BLOCK_TYPE,
}BLOCK_TYPE;//形構造体の分類

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
GROUP_FORM GetGroupForm(int Form_ID);
HRESULT InitTetris_Main(void);
int	GetGroupLeft(void);
int GetGameStats(void);
int SetGroupLeft(int Count);
void SetTetrisFieldMove(FIELD_MOVE_TYPE type);
FIELD_MOVE_TYPE GetFieldMoveType(void);
void SetTerisMainStats(int STATS);
void SetTetrisInput(bool input);
void UninitTetris_Main(void);
void UpdateTetris_Main(void);
void DrawTetris_Main(void);
void SetVerTexField(void);
FIELD *GetField(void);
void ChangeFieldColor(int RecColor,int TargetColor);
void SetSpecialBlockEffect(void);

#endif
////=============================================================================
//// テトリスのメイン処理[Tetris_main.cpp]
////
////
////=============================================================================
//#include "Tetris_main.h"
//#include "main.h"
//#include "input.h"
//#include "Function.h"
//#include "group_in_queue.h"
//#include "effect_block_broke.h"
//#include "sound.h"
//#include "TimeLeftCage.h"
//#include "Func_NumberX.h"
//#include "Block_Light.h"
//#include <stdlib.h>
//#include <time.h>
//#include "effect_star.h"
//#include "Battle_Main.h"
//#include "Battle_Attack.h"
////*****************************************************************************
//// プロトタイプ宣言
////*****************************************************************************
//HRESULT MakeVertexField(LPDIRECT3DDEVICE9 pDevice);
//HRESULT MakeVertexBlock(LPDIRECT3DDEVICE9 pDevice);
//void SetVerTexField(void);
//void SetVerTexBlock(void);
//void InitGroup(int Group_ID);
//void InitBlockAll(void);
//void SetGroupTop(int Group_ID);
//void Check_Row(void);
//void Rotation(int Group_ID);
//void DeleteRow(void);
//void GroupShadowMove(void);
////*****************************************************************************
//// グローバル変数
////*****************************************************************************
//
//LPDIRECT3DTEXTURE9		g_pTextureField	= NULL;//テクスチャメモリ
//LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffField = NULL;//頂点座標バッファ
//LPDIRECT3DTEXTURE9		g_pTextureBlock	= NULL;//テクスチャメモリ
//LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBlock = NULL;//頂点座標バッファ
//
//FIELD	g_aField[MAX_FIELD_ROW][MAX_FIELD_COLUMN];	//Field構造体
//BLOCK	g_aBlock[MAX_BLOCK];						//Blockの構造体
//GROUP	g_aGroup[MAX_GROUP];						//Groupの構造体
//bool	g_bInput;									//操作可能か
//NUMBER_X *pNumberx;
//
//float g_MonsterHP = 100.0f;
//float g_PlayerHP  = 100.0f;
//
//int nCntTime;										//テトリス内時間制御変数
//int	g_TimeAutoDrop;									// テトリス内自動落下間隔
//bool g_bAutoDrop;									// 自動落下ON/OFFスイッチ
//int	g_AutoTime;										// 連動移動の速度
//
//int	StoreGroup[MAX_GROUP_BLOCK]={10,10,10,10,10,10,10};		//形の出現率を保存する場所
//int	StoreColor[BLOCK_MAX_COLOR]={10,10,10,10,10,10,10};	//形の出現率を保存する場所s
//int g_NumGroup;										// 現在使っているグループの番号		
//int g_Counter	=0;
//D3DXVECTOR2 g_BlockMove;
//
//int g_GroupLeft;		//残りのＱグループ数
//int g_GroupUseQ;		//現在使っているＱグループのナンバー
//int g_nDeleteTime;		//行削除時間制御
//int g_Wk_Delete[MAX_FIELD_ROW];//削除すべき行を保存するワーク 0 番目は行の総数，1からは行の番目
//int g_TetrisStats;	//ゲームの進行状態
//
//GROUP_FORM g_aForm[MAX_FORM_TYPE]=
//{
//	/*0*/{1,1,{D3DXVECTOR2(0,0)}},//NONE
//	/*1*/{4,3,{D3DXVECTOR2(0,0),D3DXVECTOR2(-1, 1), D3DXVECTOR2(-1, 0), D3DXVECTOR2( 1, 0)}},// La
//	/*2*/{4,3,{D3DXVECTOR2(0,0),D3DXVECTOR2( 1, 1), D3DXVECTOR2(-1, 0), D3DXVECTOR2( 1, 0)}},// Lb
//	/*3*/{4,0,{D3DXVECTOR2(0,0),D3DXVECTOR2( 0,-1), D3DXVECTOR2( 1,-1), D3DXVECTOR2( 1, 0)}},// O
//	/*4*/{4,1,{D3DXVECTOR2(0,0),D3DXVECTOR2(-1,-1), D3DXVECTOR2( 0,-1), D3DXVECTOR2( 1, 0)}},// Sa
//	/*5*/{4,1,{D3DXVECTOR2(0,0),D3DXVECTOR2(-1, 0), D3DXVECTOR2( 0,-1), D3DXVECTOR2( 1,-1)}},// Sb
//	/*6*/{4,1,{D3DXVECTOR2(0,0),D3DXVECTOR2( 0,-1), D3DXVECTOR2( 0, 1), D3DXVECTOR2( 0, 2)}},// I
//	/*7*/{4,3,{D3DXVECTOR2(0,0),D3DXVECTOR2( 0,-1), D3DXVECTOR2(-1, 0), D3DXVECTOR2( 1, 0)}} // T
//};
////=============================================================================
//// 初期化処理
////=============================================================================
//HRESULT InitTetris_Main(void)
//{
//	LPDIRECT3DDEVICE9 pDevice = GetDevice();
//
//	MOVETIME *pMoveTime;
//	pMoveTime = GetMoveTimeLeft();
//	//フィールド初期化
//	for(int nCntField_Y = 0; nCntField_Y < MAX_FIELD_ROW; nCntField_Y++)
//	{
//		for(int nCntField_X = 0; nCntField_X < MAX_FIELD_COLUMN; nCntField_X++)
//		{
//			g_aField[nCntField_Y][nCntField_X].nColor = 0;
//			g_aField[nCntField_Y][nCntField_X].g_fAlpha = 1.0f;
//			g_aField[nCntField_Y][nCntField_X].pos.x  = VALUE_FIELD_START_X + nCntField_X * VALUE_BLOCK_SIZE;
//			g_aField[nCntField_Y][nCntField_X].pos.y  =	VALUE_FIELD_START_Y - nCntField_Y * VALUE_BLOCK_SIZE;
//
//			if(nCntField_Y == 0 || nCntField_X == 0 || nCntField_X == MAX_FIELD_COLUMN - 1)
//			{
//				g_aField[nCntField_Y][nCntField_X].nType = FIELD_WALL;
//				g_aField[nCntField_Y][nCntField_X].nColor= BLOCK_WALL;
//			}
//			else
//				g_aField[nCntField_Y][nCntField_X].nType  = FIELD_NONE;
//		}
//	}
//	//ブロック初期化
//	InitBlockAll();
//	//グループ初期化
//	for(int nCntGroup = 0; nCntGroup < MAX_GROUP; nCntGroup++)
//	{
//		g_aGroup[nCntGroup].B_start = 0;
//		g_aGroup[nCntGroup].B_end	= 0;
//		g_aGroup[nCntGroup].nColor	= BLOCK_MP;
//		g_aGroup[nCntGroup].nType	= GROUP_BACK;
//		g_aGroup[nCntGroup].NumForm	= FORM__NONE;
//		g_aGroup[nCntGroup].Place	= D3DXVECTOR2(0.0f,0.0f);
//		g_aGroup[nCntGroup].Pos		= D3DXVECTOR2(0.0f,0.0f);
//		g_aGroup[nCntGroup].Form.nCount = 0;
//		g_aGroup[nCntGroup].Form.Rot	= 1;
//		g_aGroup[nCntGroup].Rotation	= 0;
//		for(int nPlace = 0; nPlace < MAX_FORM_PLACE; nPlace++)
//			g_aGroup[nCntGroup].Form.Place[nPlace]=D3DXVECTOR2(0.0f,0.0f);
//
//	}
//	for(int nDel_WK = 0; nDel_WK < MAX_FIELD_ROW;nDel_WK++)
//	{
//		g_Wk_Delete[nDel_WK] = 0;
//	}
//
//
//
//	nCntTime = 0;
//	g_TimeAutoDrop = 60;
//	g_bAutoDrop = true;
//	g_AutoTime	= 8;
//
//	MakeVertexField(pDevice);
//	MakeVertexBlock(pDevice);
//
//	g_NumGroup	= 0;
//	g_BlockMove = D3DXVECTOR2(0,0);
//
//	//ゲーム制御変数
//	g_GroupLeft = 0;
//	g_GroupUseQ	= 0;
//	g_nDeleteTime = 0;
//	pMoveTime->fMoveTimeLeft = 0.0f;
//	g_TetrisStats = SITUATION_START;
//	g_bInput	= false;
//	pNumberx = GetNumberX(PLAYER_SCORE);
//	pNumberx->nNumber = 0;
//	Set_NumberX(TIME_LEFT,				// 数列の名前
//		true,								// 使用の状態設定
//		false,								// Fade状態
//		FULLCOLOR,							// 使用するテクスチャの名前
//		4,									// 入力最大桁数
//		pNumberx->nNumber,					// 入力値
//		30.0f,								// 文字組の大きさX
//		50.0f,								// 文字組の大きさY
//		1.0f,								// 透明度の設定
//		D3DXVECTOR3(1020.0f,100.0f,0.0f)	// 描画開始位置
//		);
//
//	D3DXCreateTextureFromFile(pDevice,
//		TEX_BLOCK_TEXTURE,
//		&g_pTextureField);
//
//	D3DXCreateTextureFromFile(pDevice,
//		TEX_BLOCK_TEXTURE,
//		&g_pTextureBlock);
//	return S_OK;
//}
////=============================================================================
//// 終了処理
////=============================================================================
//void UninitTetris_Main(void)
//{
//	if(g_pTextureField != NULL)
//	{// テクスチャの開放
//		g_pTextureField->Release();
//		g_pTextureField = NULL;
//	}
//
//	if(g_pVtxBuffField != NULL)
//	{// 頂点バッファの開放
//		g_pVtxBuffField->Release();
//		g_pVtxBuffField = NULL;
//	}
//
//	if(g_pTextureBlock != NULL)
//	{// テクスチャの開放
//		g_pTextureBlock->Release();
//		g_pTextureBlock = NULL;
//	}
//
//	if(g_pVtxBuffBlock != NULL)
//	{// 頂点バッファの開放
//		g_pVtxBuffBlock->Release();
//		g_pVtxBuffBlock = NULL;
//	}
//}
////=============================================================================
//// 更新処理
////=============================================================================
//void UpdateTetris_Main(void)
//{
//
//	MOVETIME *pMoveTime;
//	pMoveTime = GetMoveTimeLeft();
//	if(g_TetrisStats == SITUATION_START)//開始時一回のみ実行
//	{
//		if(pMoveTime->nType == TIME_MOVE_KEEPING)
//			SetMoveTime(TIME_MOVE_RECOVER,VALUE_DEFAULT_MOVE_TIME);
//		if(pMoveTime->nType == TIME_MOVE_FULL)
//		{
//			g_TetrisStats = SITUATION_STOP;
//		}
//	}
//
//	if(g_TetrisStats == SITUATION_WAIT_TIMECAGE)//操作時間リチャージ
//	{
//		if(pMoveTime->nType == TIME_MOVE_KEEPING)
//			SetMoveTime(TIME_MOVE_RECOVER,VALUE_DEFAULT_MOVE_TIME);
//		int BattleStatsWK = GetBattleStatsWK();
//			//*********攻撃演出へ移行******************//
//			/**/if(BattleStatsWK == BATTLE_TETRIS)	 /**/
//			/**/{SetBattleStats(BATTLE_PLAYER_ATTACK);}/**/
//			//*********攻撃演出へ移行******************//
//
//		if(pMoveTime->fMoveTimeLeft == VALUE_DEFAULT_MOVE_TIME)
//		{
//			g_TetrisStats = SITUATION_STOP;
//		}
//	}
//
//	if(g_TetrisStats == SITUATION_STOP && g_nDeleteTime == 0)
//	{
//		g_GroupUseQ = 0;
//		if(GetKeyboardTrigger(DIK_M)&&g_bInput == true)
//		{
//			for(int Q_ID = 0;Q_ID < MAX_GROUP_Q;Q_ID++)
//				SetGroup_Q(Q_ID);
//			g_TetrisStats = SITUATION_WAIT_SETGROUP;
//		}
//	}
//	if(GetKeyboardRelease(DIK_M)&& g_TetrisStats == SITUATION_WAIT_SETGROUP&&g_bInput == true)
//	{
//		g_GroupLeft = 4;
//		InitGroup(GROUP_USING);
//		SetGroupTop(GROUP_USING);
//		InitGroup(GROUP_SHADOW);
//		SetGroup_Q_Type(g_GroupUseQ,GROUP_BACK);
//		SetMoveTime(TIME_MOVE_LOSING,VALUE_DEFAULT_MOVE_TIME);
//		g_TetrisStats = SITUATION_UPDATE;
//
//	}
//
//	if(g_TetrisStats == SITUATION_UPDATE && g_nDeleteTime == 0)
//	{
//		if(g_bInput == true)
//		{
//		if(GetKeyboardTrigger(KEY_ROT))
//			Rotation(GROUP_USING);
//
//		nCntTime++;
//		//=================================左への移動
//		if(GetKeyboardTrigger(KEY_LEFT))
//		{
//			g_Counter = 0;
//			g_BlockMove.x = -1;
//			PlaySound(SOUND_LABEL_SE_BLOCK_MOVE);
//		}
//		if(GetKeyboardPress(KEY_LEFT))
//		{
//			g_Counter ++;
//			if(g_Counter > 10&&nCntTime %g_AutoTime==0)
//			{
//				PlaySound(SOUND_LABEL_SE_BLOCK_MOVE);
//				g_BlockMove.x = -1;
//			}
//		}
//		if(GetKeyboardRelease(KEY_LEFT))
//		{
//			PlaySound(SOUND_LABEL_SE_BLOCK_MOVE);
//
//			g_Counter = 0;
//		}
//		//=================================右への移動
//		if(GetKeyboardTrigger(KEY_RIGHT))
//		{
//			PlaySound(SOUND_LABEL_SE_BLOCK_MOVE);
//			g_Counter = 0;
//			g_BlockMove.x =  1;
//		}
//		if(GetKeyboardPress(KEY_RIGHT))
//		{
//			g_Counter ++;
//			if(g_Counter > 10 && nCntTime %g_AutoTime==0)
//			{
//				PlaySound(SOUND_LABEL_SE_BLOCK_MOVE);
//				g_BlockMove.x =  1;
//			}
//		}
//		if(GetKeyboardRelease(KEY_RIGHT))
//		{
//			g_Counter = 0;
//		}
//		//=================================下への移動
//		if(GetKeyboardTrigger(KEY_DOWN))
//		{
//			PlaySound(SOUND_LABEL_SE_BLOCK_MOVE);
//			g_Counter = 0;
//			g_BlockMove.y = -1;
//		}
//		if(GetKeyboardPress(KEY_DOWN))
//		{
//
//			g_Counter ++;
//			if(g_Counter > 10 && nCntTime %(g_AutoTime/2)==0)
//			{
//				PlaySound(SOUND_LABEL_SE_BLOCK_MOVE);
//				g_BlockMove.y = -1;
//			}
//			g_bAutoDrop = false;
//		}
//		else
//		{g_bAutoDrop = true;}
//
//		if(GetKeyboardRelease(KEY_DOWN))
//		{
//			g_Counter = 0;
//
//		}
//		if(GetKeyboardTrigger(KEY_CHEAT_UP))
//
//			g_BlockMove.y =  1;
//		//=================================自動移動
//		if(nCntTime%g_TimeAutoDrop==0&&g_bAutoDrop == true)
//		{
//			PlaySound(SOUND_LABEL_SE_BLOCK_MOVE);
//			g_BlockMove.y = -1;
//		}
//
//		if(GetKeyboardTrigger(KEY_DROP)&&g_aGroup[GROUP_USING].Place.y > g_aGroup[GROUP_SHADOW].Place.y + 2)
//		{
//			PlaySound(SOUND_LABEL_SE_BLOCK_STOP);
//			g_aGroup[GROUP_USING].Place.y = g_aGroup[GROUP_SHADOW].Place.y + 2;
//			D3DXVECTOR3 pos;
//			  
//			pos.x = VALUE_FIELD_START_X + (g_aGroup[GROUP_USING].Place.x+1) * VALUE_BLOCK_SIZE; 
//			pos.y = VALUE_FIELD_START_Y - g_aGroup[GROUP_USING].Place.y * VALUE_BLOCK_SIZE;
//			pos.z = 0.0f;
//			SetLight(pos);
//			SetEffect_Star(pos,g_aGroup[GROUP_USING].nColor,1);
//
//		}
//		}//input ok??
//		//左右移動の処理
//		for(int nCntBlock = g_aGroup[GROUP_USING].B_start;
//			nCntBlock <= g_aGroup[GROUP_USING].B_end;
//			nCntBlock ++)
//		{
//			int NowPlaceX	= (int)g_aBlock[nCntBlock].Place.x;
//			int NowPlaceY	= (int)g_aBlock[nCntBlock].Place.y;//現在位置保存ワーク
//			int DestPlaceX	= (int)g_aBlock[nCntBlock].Place.x + (int)g_BlockMove.x;
//			int DestPlaceY	= (int)g_aBlock[nCntBlock].Place.y + (int)g_BlockMove.y;//目的位置保存ワーク
//			if(g_aField[NowPlaceY][DestPlaceX].nType != FIELD_NONE)
//				g_BlockMove.x = 0;
//		}
//		//上下移動の処理
//		if(g_BlockMove.y != 0)
//		{
//			int NowPlaceX	= 0;
//			int NowPlaceY	= 0;
//			int DestPlaceX	= 0;
//			int DestPlaceY	= 0;
//			for(int nCntBlock = g_aGroup[GROUP_USING].B_start;
//				nCntBlock <= g_aGroup[GROUP_USING].B_end;
//				nCntBlock ++)
//			{
//				NowPlaceX	= (int)g_aBlock[nCntBlock].Place.x;
//				NowPlaceY	= (int)g_aBlock[nCntBlock].Place.y;//現在位置保存ワーク
//				DestPlaceX	= NowPlaceX + (int)g_BlockMove.x;
//				DestPlaceY	= NowPlaceY + (int)g_BlockMove.y;//目的位置保存ワーク
//
//				if(g_aField[DestPlaceY][NowPlaceX].nType != FIELD_NONE)
//					g_BlockMove.y = 0;
//
//			}
//			//==========================引っかかった時にフィールドに反映=========================//
//			if(g_BlockMove.y == 0)
//			{
//				PlaySound(SOUND_LABEL_SE_BLOCK_STOP); // 停止音
//				for(int nCntBlock  = g_aGroup[GROUP_USING].B_start;
//					nCntBlock <= g_aGroup[GROUP_USING].B_end;
//					nCntBlock ++)
//				{
//
//					NowPlaceX = (int)g_aBlock[nCntBlock].Place.x;
//					NowPlaceY = (int)g_aBlock[nCntBlock].Place.y;
//
//					g_aField[NowPlaceY][NowPlaceX].nType	= FIELD_USE;
//					g_aField[NowPlaceY][NowPlaceX].nColor	= g_aGroup[GROUP_USING].nColor;
//
//					//エフェクトを付けようか
//					D3DXVECTOR3 Epos = D3DXVECTOR3(0.0f,0.0f,0.0f);
//					Epos.x = g_aField[NowPlaceY][NowPlaceX].pos.x;
//					Epos.y = g_aField[NowPlaceY][NowPlaceX].pos.y + 15.0f;//15.0fは修正値
//					int EColor = g_aGroup[GROUP_USING].nColor;
//					SetEffect_Broke(Epos,EColor);
//					 
//				}
//				g_GroupLeft--;//使用可能ブロック数を減らす
//				Check_Row();//行列チェック
//				if(g_GroupLeft != 0)
//				{
//					if(g_Wk_Delete[0]==0)
//					{
//						g_GroupUseQ ++;
//						SetGroupTop(GROUP_USING);
//						SetGroup_Q_Type(g_GroupUseQ,GROUP_BACK);
//						InitGroup(GROUP_USING);
//						InitGroup(GROUP_SHADOW);
//					}
//				}
//				else
//				{   
//					g_TetrisStats = SITUATION_WAIT_TIMECAGE;
//					pMoveTime->nType = TIME_MOVE_KEEPING;
//					SetGroupTop(GROUP_USING);
//					InitBlockAll();
//					InitGroup_Q_ALL();
//					InitBlock_Q_ALL();
//				}
//
//			}
//
//		}
//		//中心グループの絶対位置を反映
//		g_aGroup[GROUP_USING].Place += g_BlockMove;
//		for(int nCntBlock = g_aGroup[GROUP_USING].B_start;
//			nCntBlock <= g_aGroup[GROUP_USING].B_end;
//			nCntBlock ++)
//		{
//			g_aBlock[nCntBlock].Place = g_aGroup[GROUP_USING].Place + g_aBlock[nCntBlock].FormPos;
//
//			g_aBlock[nCntBlock].Pos.x = VALUE_FIELD_START_X + g_aBlock[nCntBlock].Place.x * VALUE_BLOCK_SIZE; 
//			g_aBlock[nCntBlock].Pos.y = VALUE_FIELD_START_Y - g_aBlock[nCntBlock].Place.y * VALUE_BLOCK_SIZE;
//		}
//		//==============ここからはブロックの射影処理================//
//		
//		GroupShadowMove();
//		
//		//==============ここまではブロックの射影処理================//
//		//==============以上はブロックのmain処理================//
//		nCntTime = (int)SetMAX((float)nCntTime,(float)120,0.0f,1);
//		g_BlockMove = D3DXVECTOR2(0,0);
//
//	}
//	//==============ここからははターン中移動時間切れの処理================//
//	if(g_GroupLeft != 0 && pMoveTime->fMoveTimeLeft == 0)
//	{
//		g_TetrisStats = SITUATION_MOVE_TIME_OUT;
//		g_GroupLeft = 0;					
//		InitBlockAll();
//		InitGroup_Q_ALL();
//		InitBlock_Q_ALL();
//		//一行ずつ上に移す
//		for(int nRow = MAX_FIELD_ROW -1; nRow > 1; nRow--)
//		{
//			for(int nCol = 1; nCol < MAX_FIELD_COLUMN-1;nCol ++)
//			{
//				g_aField[nRow][nCol].nType  = g_aField[nRow-1][nCol].nType;
//				g_aField[nRow][nCol].nColor = g_aField[nRow-1][nCol].nColor;
//			}
//
//		}
//		//最下行をランダムに色を入れる
//		int UseCount = 0;
//		for(int nCol = 1; nCol < MAX_FIELD_COLUMN-1;nCol ++)
//		{
//			if(rand()%100<10)
//				g_aField[1][nCol].nType = FIELD_NONE;
//			else
//			{
//				g_aField[1][nCol].nType = FIELD_USE;
//				g_aField[1][nCol].nColor = rand()%6;
//				UseCount ++;
//			}
//
//		}
//		//もしすべて有効と判断した場合，ランダムに一個を無効にする
//		if(UseCount = MAX_FIELD_COLUMN - 2)
//		{
//			int Dest = rand()%(MAX_FIELD_COLUMN-1) + 1;
//			g_aField[1][Dest].nType = FIELD_NONE;
//		}
//		g_TetrisStats = SITUATION_WAIT_TIMECAGE;
//	}
//
//
//		//引っかかった時に野エフェクト処理
//		if(g_Wk_Delete[0] != 0)
//		{	
//			g_nDeleteTime --;
//			PlaySound(SOUND_LABEL_SE_ICE_SLASH);
//			for(int nCount = g_Wk_Delete[0],Idx = 1;nCount >0 ;nCount--,Idx++)
//			{
//
//				for(int nCloumn = 1; nCloumn < MAX_FIELD_COLUMN -1;nCloumn++)
//				{
//					if(g_nDeleteTime % 4 == 0)
//						g_aField[g_Wk_Delete[Idx]][nCloumn].g_fAlpha = 1.0f;
//					else
//						g_aField[g_Wk_Delete[Idx]][nCloumn].g_fAlpha = 0.1f;
//
//				}
//			}
//			if(g_nDeleteTime ==0)
//			{
//				for(int nCount = g_Wk_Delete[0],Idx = 1;nCount >0 ;nCount--,Idx++)
//				{
//
//					for(int nCloumn = 1; nCloumn < MAX_FIELD_COLUMN -1;nCloumn++)
//					{
//						D3DXVECTOR3 Pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
//						g_aField[g_Wk_Delete[Idx]][nCloumn].g_fAlpha = 1.0f;
//						Pos.x = g_aField[g_Wk_Delete[Idx]][nCloumn].pos.x;
//						Pos.y = g_aField[g_Wk_Delete[Idx]][nCloumn].pos.y;
//						Pos.z = 0.0f;
//						int Color = g_aField[g_Wk_Delete[Idx]][nCloumn].nColor;
//
//						SetEffect_Broke(Pos,Color);
//					}
//				}
//				InitBlockAll();		
//				DeleteRow();
//				g_GroupUseQ ++;
//				SetGroupTop(GROUP_USING);
//				SetGroup_Q_Type(g_GroupUseQ,GROUP_BACK);
//				InitGroup(GROUP_USING);
//				InitGroup(GROUP_SHADOW);
//
//			}
//
//		}
//
//		SetVerTexBlock();
//		SetVerTexField();
//
//}
//
////=============================================================================
//// 射影グループの処理
////=============================================================================
//void GroupShadowMove(void)
//{
//	D3DXVECTOR2 Place = g_aGroup[GROUP_USING].Place;
//	int FormPosX[MAX_FORM_PLACE] = {0,0,0,0};
//	int FormPosY[MAX_FORM_PLACE] = {0,0,0,0};
//
//		for(int nCntBlock = g_aGroup[GROUP_USING].B_start,FormIdx = 0;
//			nCntBlock <= g_aGroup[GROUP_USING].B_end;
//			nCntBlock ++,FormIdx++)
//		{
//			FormPosX[FormIdx] = g_aBlock[nCntBlock].FormPos.x;
//			FormPosY[FormIdx] = g_aBlock[nCntBlock].FormPos.y;
//		}
//		int PlaceY = 0;
//		for(PlaceY = Place.y;PlaceY>1;PlaceY--)
//		{
//			bool check = true;
//			for(int FormIdx = 0; FormIdx < MAX_FORM_PLACE; FormIdx ++)
//			{
//				int DestPlaceX = Place.x + FormPosX[FormIdx];
//				int DestPlaceY = PlaceY + FormPosY[FormIdx];
//
//				if(g_aField[DestPlaceY-1][DestPlaceX].nType != FIELD_NONE)
//				{
//					check = false;
//					break;
//				}
//			}
//			if(check == false)
//				break;
//		}
//		g_aGroup[GROUP_SHADOW].Place.x = g_aGroup[GROUP_USING].Place.x;
//		g_aGroup[GROUP_SHADOW].Place.y = PlaceY;
//		for(int AnCntBlock = g_aGroup[GROUP_SHADOW].B_start,BnCntBlock = g_aGroup[GROUP_USING].B_start;
//			AnCntBlock <= g_aGroup[GROUP_SHADOW].B_end;
//			AnCntBlock ++,BnCntBlock++)
//		{
//			g_aBlock[AnCntBlock].FormPos = g_aBlock[BnCntBlock].FormPos;
//			g_aBlock[AnCntBlock].Place.x = g_aGroup[GROUP_SHADOW].Place.x + g_aBlock[AnCntBlock].FormPos.x;
//			g_aBlock[AnCntBlock].Place.y = g_aGroup[GROUP_SHADOW].Place.y + g_aBlock[AnCntBlock].FormPos.y;
//			g_aBlock[AnCntBlock].Pos.x = VALUE_FIELD_START_X + g_aBlock[AnCntBlock].Place.x * VALUE_BLOCK_SIZE; 
//			g_aBlock[AnCntBlock].Pos.y = VALUE_FIELD_START_Y - g_aBlock[AnCntBlock].Place.y * VALUE_BLOCK_SIZE;
//		}
//
//
//}
////=============================================================================
//// Blockのチェック
////=============================================================================
//void Check_Row(void)
//{
//	int nDel_WK = 1;
//	//=================ここからは行のチェック及び消滅=================//
//	for(int nCheck_Row = 1; nCheck_Row < MAX_FIELD_ROW -1;nCheck_Row++)
//	{
//		int nCheck = 0;
//		for(int nCheck_Column = 1;nCheck_Column < MAX_FIELD_COLUMN -1;nCheck_Column++)
//		{
//			if(g_aField[nCheck_Row][nCheck_Column].nType == FIELD_USE)
//				nCheck ++;
//		}
//		if(nCheck == MAX_FIELD_COLUMN -2)
//		{	
//
//			g_Wk_Delete[0]++;//ワーク内の行総数更新
//			g_Wk_Delete[nDel_WK] = nCheck_Row;//現在の行をワークに保存
//			nDel_WK++;//インデックスの更新
//			g_nDeleteTime = VALUE_TIME_DELAY;//更新ストップ時間セット
//		}
//	}
//	//=================ここまでは行のチェック及び消滅=================//
//}
//void DeleteRow(void)
//{
//	MOVETIME *pMoveTime;
//	pMoveTime = GetMoveTimeLeft();
//	DAMAGE_STRUCT	*pDamage;
//	pDamage = GetDamageStruct();
//
//	int Idx = 1;
//	for(int IndexRow = g_Wk_Delete[Idx];g_Wk_Delete[0] > 0;Idx++,g_Wk_Delete[0]--)
//	{
//
//		for(int nCopy_Row = IndexRow;nCopy_Row<MAX_FIELD_ROW -1 ;nCopy_Row++)
//		{
//			for(int nCopy_Column = 1;nCopy_Column<MAX_FIELD_COLUMN -1;nCopy_Column++)
//			{
//				if(nCopy_Row == IndexRow)//コンボの計算
//				{
//				int Color= g_aField[IndexRow][nCopy_Column].nColor ;
//				pDamage[Color].nCombo ++;
//				}
//
//				g_aField[nCopy_Row][nCopy_Column].nColor = g_aField[nCopy_Row + 1][nCopy_Column].nColor;
//				g_aField[nCopy_Row][nCopy_Column].nType = g_aField[nCopy_Row + 1][nCopy_Column].nType;
//			}
//		}
//		pNumberx->nNumber += 100;		//スコア更新
//		pMoveTime->fMoveTimeLeft += 60;	//移動時間を少し復元，ボーナスとして
//	}
//	//ダメージを一応計しておく
//	DamageAnalysBase();
//
//}
////=============================================================================
//// 描画処理
////=============================================================================
//void DrawTetris_Main(void)
//{
//	LPDIRECT3DDEVICE9 pDevice = GetDevice();
//
//	//フィールドの描画
//	// 頂点バッファをデバイスのデータストリームにバインド
//	pDevice->SetStreamSource(0, g_pVtxBuffField, 0, sizeof(VERTEX_2D));
//
//	// 頂点フォーマットの設定
//	pDevice->SetFVF(FVF_VERTEX_2D);
//
//	pDevice->SetTexture(0,g_pTextureField);
//	for(int nCntField_Y = 0; nCntField_Y < MAX_FIELD_ROW; nCntField_Y++)
//	{
//		for(int nCntField_X = 0; nCntField_X < MAX_FIELD_COLUMN; nCntField_X++)
//		{
//			if(g_aField[nCntField_Y][nCntField_X].nType == FIELD_NONE)
//				continue;
//			// ポリゴンの描画
//			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, ((nCntField_Y* MAX_FIELD_COLUMN + nCntField_X) * 4), NUM_POLYGON);
//		}
//	}
//	//エフェクトの描画
//	DrawBlock_Light();
//
//
//	//ロックの描画
//	// 頂点バッファをデバイスのデータストリームにバインド
//	pDevice->SetStreamSource(0, g_pVtxBuffBlock, 0, sizeof(VERTEX_2D));
//	// 頂点フォーマットの設定
//	pDevice->SetFVF(FVF_VERTEX_2D);
//	// テクスチャの設定
//	pDevice->SetTexture(0,g_pTextureBlock);
//	for(int nCnt_Block = 0; nCnt_Block < MAX_BLOCK;nCnt_Block ++)
//	{
//		if(g_aBlock[nCnt_Block].bUse == false)
//			continue;
//		// ポリゴンの描画
//		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,nCnt_Block*4, NUM_POLYGON);
//	}
//
//}
////=============================================================================
//// Groupの初期化処理
////=============================================================================
//void InitGroup(int Group_ID)
//{
//	int start = 0;
//	int nCntBlock = 0;
//
//	if(Group_ID == GROUP_SHADOW)
//	{
//		g_aGroup[Group_ID] = g_aGroup[GROUP_USING];
//
//	}
//	else
//	{
//		g_aGroup[Group_ID].nType	= GROUP_WAIT;
//		g_aGroup[Group_ID].NumForm	= GetGroup_Q_NumForm(g_GroupUseQ);
//		g_aGroup[Group_ID].nColor	= GetGroup_Q_Color(g_GroupUseQ);
//		g_aGroup[Group_ID].Form		= g_aForm[g_aGroup[Group_ID].NumForm];
//		g_aGroup[Group_ID].Rotation = 0;
//	}
//
//	while(g_aBlock[nCntBlock].bUse == true)
//		nCntBlock++;
//
//	//======念のために　これを書いとく======//
//	if(nCntBlock > MAX_BLOCK - 10)
//	{
//		for(int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock ++)
//		{
//			g_aBlock[nCntBlock].bUse	= false;
//			g_aBlock[nCntBlock].nColor	= BLOCK_RED;
//			g_aBlock[nCntBlock].Place	= D3DXVECTOR2(0,0);
//			g_aBlock[nCntBlock].Pos		= D3DXVECTOR2(0.0f,0.0f);
//			g_aBlock[nCntBlock].FormPos	= D3DXVECTOR2(0,0);
//		}
//
//	}
//	//======念のために　これを書いとく======//
//
//	g_aGroup[Group_ID].B_start	= nCntBlock;
//	start	= nCntBlock;
//	g_aGroup[Group_ID].B_end	= start + g_aGroup[Group_ID].Form.nCount-1;
//
//
//
//	for(int nBlock = 0; nBlock < g_aGroup[Group_ID].Form.nCount;nBlock++)
//	{
//		g_aBlock[nBlock + start].bUse	 = true;
//		if(Group_ID == GROUP_SHADOW)
//			g_aBlock[nBlock + start].Type = BLOCK_SHADOW;
//		g_aBlock[nBlock + start].nColor	 = g_aGroup[Group_ID].nColor;
//		g_aBlock[nBlock + start].FormPos = g_aGroup[Group_ID].Form.Place[nBlock];
//		g_aBlock[nBlock + start].Place	 = g_aGroup[Group_ID].Place + g_aBlock[nBlock + start].FormPos;
//		g_aBlock[nBlock + start].Pos.x	 = g_aGroup[Group_ID].Pos.x + g_aBlock[nBlock + start].Place.x * VALUE_BLOCK_SIZE;
//		g_aBlock[nBlock + start].Pos.y	 = g_aGroup[Group_ID].Pos.y + g_aBlock[nBlock + start].Place.y * VALUE_BLOCK_SIZE;
//	}
//	//SetVerTexBlock();
//}
////=============================================================================
//// Groupのセット処理
////=============================================================================
//void SetGroupTop(int Group_ID)
//{
//	g_aGroup[Group_ID].nType	= GROUP_USE;
//	g_aGroup[Group_ID].Place.x	= (int)MAX_FIELD_COLUMN/2;
//	g_aGroup[Group_ID].Place.y	= (int)MAX_FIELD_ROW - 2;
//}
////=============================================================================
//// Field頂点の作成
////=============================================================================
//HRESULT MakeVertexField(LPDIRECT3DDEVICE9 pDevice)
//{
//	// オブジェクトの頂点バッファを生成
//	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * MAX_FIELD_BLOCK,	// 頂点データ用に確保するバッファサイズ(バイト単位)
//		D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
//		FVF_VERTEX_2D,						// 使用する頂点フォーマット
//		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
//		&g_pVtxBuffField,					// 頂点バッファインターフェースへのポインタ
//		NULL)))								// NULLに設定
//	{
//		return E_FAIL;
//	}
//
//	{//頂点バッファの中身を埋める
//		VERTEX_2D *pVtx;
//
//		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
//		g_pVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);
//
//		for(int nCntField_Y = 0; nCntField_Y < MAX_FIELD_ROW; nCntField_Y++)
//		{
//			for(int nCntField_X = 0; nCntField_X < MAX_FIELD_COLUMN; nCntField_X++,pVtx += 4)
//			{
//				// 頂点座標の設定
//				pVtx[0].pos.x = g_aField[nCntField_Y][nCntField_X].pos.x;
//				pVtx[0].pos.y = g_aField[nCntField_Y][nCntField_X].pos.y - VALUE_BLOCK_SIZE;
//				pVtx[0].pos.z = 0.0f;
//
//				pVtx[1].pos.x = g_aField[nCntField_Y][nCntField_X].pos.x + VALUE_BLOCK_SIZE;
//				pVtx[1].pos.y = g_aField[nCntField_Y][nCntField_X].pos.y - VALUE_BLOCK_SIZE;
//				pVtx[1].pos.z = 0.0f;
//
//				pVtx[2].pos.x = g_aField[nCntField_Y][nCntField_X].pos.x;
//				pVtx[2].pos.y = g_aField[nCntField_Y][nCntField_X].pos.y;
//				pVtx[2].pos.z = 0.0f;
//
//				pVtx[3].pos.x = g_aField[nCntField_Y][nCntField_X].pos.x + VALUE_BLOCK_SIZE;
//				pVtx[3].pos.y = g_aField[nCntField_Y][nCntField_X].pos.y;
//				pVtx[3].pos.z = 0.0f;
//				// rhwの設定
//				pVtx[0].rhw =
//					pVtx[1].rhw =
//					pVtx[2].rhw =
//					pVtx[3].rhw = 1.0f;
//
//				// 頂点カラーの設定
//				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//
//				// テクスチャ座標の設定
//				pVtx[0].tex = D3DXVECTOR2(g_aField[nCntField_Y][nCntField_X].nColor * (1.0f/BLOCK_MAX_COLOR), 0.0f);
//				pVtx[1].tex = D3DXVECTOR2(g_aField[nCntField_Y][nCntField_X].nColor * (1.0f/BLOCK_MAX_COLOR)+(1.0f/BLOCK_MAX_COLOR), 0.0f);
//				pVtx[2].tex = D3DXVECTOR2(g_aField[nCntField_Y][nCntField_X].nColor * (1.0f/BLOCK_MAX_COLOR), 0.5f);
//				pVtx[3].tex = D3DXVECTOR2(g_aField[nCntField_Y][nCntField_X].nColor * (1.0f/BLOCK_MAX_COLOR)+(1.0f/BLOCK_MAX_COLOR), 0.5f);
//
//			}
//		}
//		// 頂点データをアンロックする
//		g_pVtxBuffField->Unlock();
//	}
//	return S_OK;
//}
////=============================================================================
//// Block頂点の作成
////=============================================================================
//HRESULT MakeVertexBlock(LPDIRECT3DDEVICE9 pDevice)
//{
//	// オブジェクトの頂点バッファを生成
//	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * MAX_BLOCK,	// 頂点データ用に確保するバッファサイズ(バイト単位)
//		D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
//		FVF_VERTEX_2D,						// 使用する頂点フォーマット
//		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
//		&g_pVtxBuffBlock,					// 頂点バッファインターフェースへのポインタ
//		NULL)))								// NULLに設定
//	{
//		return E_FAIL;
//	}
//
//	{
//		//頂点バッファの中身を埋める
//		VERTEX_2D *pVtx;
//
//		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
//		g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);
//
//		for(int nCnt_Block = 0; nCnt_Block < MAX_BLOCK;nCnt_Block ++,pVtx += 4)
//		{
//
//			// 頂点座標の設定
//			pVtx[0].pos.x = g_aBlock[nCnt_Block].Pos.x;
//			pVtx[0].pos.y = g_aBlock[nCnt_Block].Pos.y - VALUE_BLOCK_SIZE;
//			pVtx[0].pos.z = 0.0f;
//
//			pVtx[1].pos.x = g_aBlock[nCnt_Block].Pos.x + VALUE_BLOCK_SIZE;
//			pVtx[1].pos.y = g_aBlock[nCnt_Block].Pos.y - VALUE_BLOCK_SIZE;
//			pVtx[1].pos.z = 0.0f;
//
//			pVtx[2].pos.x = g_aBlock[nCnt_Block].Pos.y;
//			pVtx[2].pos.y = g_aBlock[nCnt_Block].Pos.y;
//			pVtx[2].pos.z = 0.0f;
//
//			pVtx[3].pos.x = g_aBlock[nCnt_Block].Pos.y + VALUE_BLOCK_SIZE;
//			pVtx[3].pos.y = g_aBlock[nCnt_Block].Pos.y;
//			pVtx[3].pos.z = 0.0f;
//			// rhwの設定
//			pVtx[0].rhw =
//				pVtx[1].rhw =
//				pVtx[2].rhw =
//				pVtx[3].rhw = 1.0f;
//
//			// 頂点カラーの設定
//			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//
//			float TexY = 0.0f;
//			if(g_aBlock[nCnt_Block].Type == BLOCK_SHADOW)
//			 TexY = 0.5f;
//			// テクスチャ座標の設定
//			pVtx[0].tex = D3DXVECTOR2(g_aBlock[nCnt_Block].nColor * (1.0f/BLOCK_MAX_COLOR), TexY+0.0f);
//			pVtx[1].tex = D3DXVECTOR2(g_aBlock[nCnt_Block].nColor * (1.0f/BLOCK_MAX_COLOR)+(1.0f/BLOCK_MAX_COLOR), TexY+0.0f);
//			pVtx[2].tex = D3DXVECTOR2(g_aBlock[nCnt_Block].nColor * (1.0f/BLOCK_MAX_COLOR), TexY+0.5f);
//			pVtx[3].tex = D3DXVECTOR2(g_aBlock[nCnt_Block].nColor * (1.0f/BLOCK_MAX_COLOR)+(1.0f/BLOCK_MAX_COLOR), TexY+0.5f);
//		}
//
//		// 頂点データをアンロックする
//		g_pVtxBuffBlock->Unlock();
//		return S_OK;
//	}
//}
////=============================================================================
//// Fieldの設定
////=============================================================================
//void SetVerTexField(void)
//{
//	//頂点バッファの中身を埋める
//	VERTEX_2D *pVtx;
//	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
//	g_pVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);
//
//	for(int nCntField_Y = 0; nCntField_Y < MAX_FIELD_ROW; nCntField_Y++)
//	{
//		for(int nCntField_X = 0; nCntField_X < MAX_FIELD_COLUMN; nCntField_X++,pVtx += 4)
//		{
//			if(g_aField[nCntField_Y][nCntField_X].nType != FIELD_USE)
//				continue;
//			// テクスチャ座標の設定
//
//			//pVtx += (EnemyID * 4);
//			pVtx[0].tex = D3DXVECTOR2(g_aField[nCntField_Y][nCntField_X].nColor * (1.0f/BLOCK_MAX_COLOR),							0.0f);
//			pVtx[1].tex = D3DXVECTOR2(g_aField[nCntField_Y][nCntField_X].nColor * (1.0f/BLOCK_MAX_COLOR)+(1.0f/BLOCK_MAX_COLOR),	0.0f);
//			pVtx[2].tex = D3DXVECTOR2(g_aField[nCntField_Y][nCntField_X].nColor * (1.0f/BLOCK_MAX_COLOR),							0.5f);
//			pVtx[3].tex = D3DXVECTOR2(g_aField[nCntField_Y][nCntField_X].nColor * (1.0f/BLOCK_MAX_COLOR)+(1.0f/BLOCK_MAX_COLOR),	0.5f);
//
//			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aField[nCntField_Y][nCntField_X].g_fAlpha);
//			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aField[nCntField_Y][nCntField_X].g_fAlpha);
//			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aField[nCntField_Y][nCntField_X].g_fAlpha);
//			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aField[nCntField_Y][nCntField_X].g_fAlpha);
//
//		}
//	}
//	// 頂点データをアンロックする
//	g_pVtxBuffField->Unlock();        
//
//}
////=============================================================================
//// Blockの設定
////=============================================================================
//void SetVerTexBlock(void)
//{
//	//頂点バッファの中身を埋める
//	VERTEX_2D *pVtx;
//
//	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
//	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);
//
//	for(int nCnt_Block = 0; nCnt_Block < MAX_BLOCK;nCnt_Block ++,pVtx += 4)
//	{
//		if(g_aBlock[nCnt_Block].bUse == false)
//			continue;
//		//頂点座標設定
//		pVtx[0].pos.x = g_aBlock[nCnt_Block].Pos.x;
//		pVtx[0].pos.y = g_aBlock[nCnt_Block].Pos.y - VALUE_BLOCK_SIZE;
//		pVtx[0].pos.z = 0.0f;
//
//		pVtx[1].pos.x = g_aBlock[nCnt_Block].Pos.x + VALUE_BLOCK_SIZE;
//		pVtx[1].pos.y = g_aBlock[nCnt_Block].Pos.y - VALUE_BLOCK_SIZE;
//		pVtx[1].pos.z = 0.0f;
//
//		pVtx[2].pos.x = g_aBlock[nCnt_Block].Pos.x;
//		pVtx[2].pos.y = g_aBlock[nCnt_Block].Pos.y;
//		pVtx[2].pos.z = 0.0f;
//
//		pVtx[3].pos.x = g_aBlock[nCnt_Block].Pos.x + VALUE_BLOCK_SIZE;
//		pVtx[3].pos.y = g_aBlock[nCnt_Block].Pos.y;
//		pVtx[3].pos.z = 0.0f;
//
//			float TexY = 0.0f;
//			if(g_aBlock[nCnt_Block].Type == BLOCK_SHADOW)
//			 TexY = 0.5f;
//			// テクスチャ座標の設定
//			pVtx[0].tex = D3DXVECTOR2(g_aBlock[nCnt_Block].nColor * (1.0f/BLOCK_MAX_COLOR), TexY+0.0f);
//			pVtx[1].tex = D3DXVECTOR2(g_aBlock[nCnt_Block].nColor * (1.0f/BLOCK_MAX_COLOR)+(1.0f/BLOCK_MAX_COLOR), TexY+0.0f);
//			pVtx[2].tex = D3DXVECTOR2(g_aBlock[nCnt_Block].nColor * (1.0f/BLOCK_MAX_COLOR), TexY+0.5f);
//			pVtx[3].tex = D3DXVECTOR2(g_aBlock[nCnt_Block].nColor * (1.0f/BLOCK_MAX_COLOR)+(1.0f/BLOCK_MAX_COLOR), TexY+0.5f);
//	}
//	// 頂点データをアンロックする
//	g_pVtxBuffBlock->Unlock();
//}
//
////=============================================================================
//// 回転処理
////=============================================================================
//void Rotation(int Group_ID)
//{
//	bool bRot = true;
//	GROUP_FORM NowForm = g_aGroup[Group_ID].Form;
//	GROUP_FORM DestForm = NowForm;
//	int DestPlaceX	= 0;
//	int DestPlaceY	= 0;
//	g_aGroup[Group_ID].Rotation++;
//	g_aGroup[Group_ID].Rotation = (int)SetMAX((float)g_aGroup[Group_ID].Rotation,(float)g_aGroup[Group_ID].Form.Rot,0.0f,1);
//
//	//*********************回転
//	if(g_aGroup[Group_ID].Rotation == 0)
//	{
//		DestForm = g_aForm[g_aGroup[Group_ID].NumForm];
//	}
//	else
//	{
//		for(int nCntPlace = 0; nCntPlace < NowForm.nCount; nCntPlace++)
//		{
//			DestForm.Place[nCntPlace].x = 0 * NowForm.Place[nCntPlace].x - 1 * NowForm.Place[nCntPlace].y;	
//			DestForm.Place[nCntPlace].y	= 0 * NowForm.Place[nCntPlace].y + 1 * NowForm.Place[nCntPlace].x;
//		}
//	}
//	//********************仮回転後のチェッ
//	for(int nCntPlace = 0; nCntPlace < NowForm.nCount; nCntPlace++)
//	{
//		DestPlaceX = (int)DestForm.Place[nCntPlace].x + (int)g_aGroup[Group_ID].Place.x;
//		DestPlaceY = (int)DestForm.Place[nCntPlace].y + (int)g_aGroup[Group_ID].Place.y;
//
//		if(g_aField[DestPlaceY][DestPlaceX].nType != FIELD_NONE)
//
//		{
//			g_aGroup[Group_ID].Rotation--;
//			bRot = false;
//			break;
//		}
//	}
//	//********************回転の反映
//	if(bRot == true)
//	{
//		PlaySound(SOUND_LABEL_SE_BLOCK_ROT);
//		g_aGroup[Group_ID].Form = DestForm;
//		for(int nCntBlock = g_aGroup[Group_ID].B_start,nCntPlace = 0;
//			nCntBlock <= g_aGroup[Group_ID].B_end;
//			nCntBlock ++,nCntPlace ++)
//		{
//			g_aBlock[nCntBlock].FormPos = DestForm.Place[nCntPlace];
//			g_aBlock[nCntBlock].Place = g_aGroup[GROUP_USING].Place + g_aBlock[nCntBlock].FormPos;
//		}			
//	}
//}
////================================================================
////GrouFormの取得
////================================================================
//GROUP_FORM GetGroupForm(int Form_ID)
//{
//	return g_aForm[Form_ID];
//}
////================================================================
////残組の取得と設定
////================================================================
//int	GetGroupLeft(void)
//{
//	return g_GroupLeft;
//}
//
////================================================================
////すべてのブロックを初期化
////================================================================
//void InitBlockAll(void)
//{
//	for(int nCntBlock = 0; nCntBlock < MAX_BLOCK;nCntBlock++)
//	{
//		g_aBlock[nCntBlock].Type	= BLOCK_NORMAL;
//		g_aBlock[nCntBlock].bUse	= false;
//		g_aBlock[nCntBlock].nColor	= BLOCK_MP;
//		g_aBlock[nCntBlock].Place	= D3DXVECTOR2(0,0);
//		g_aBlock[nCntBlock].Pos		= D3DXVECTOR2(0.0f,0.0f);
//		g_aBlock[nCntBlock].FormPos	= D3DXVECTOR2(0,0);
//	}
//}
////================================================================
////入力可能の設定
////================================================================
//void SetTetrisInput(bool input)
//{
//	g_bInput = input;
//}
