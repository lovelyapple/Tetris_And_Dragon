//=============================================================================
// テトリスのメイン処理[Tetris_main.cpp]
//
//
//=============================================================================
#include "Tetris_main.h"
#include "main.h"
#include "input.h"
#include "Function.h"
#include "group_in_queue.h"
#include "effect_block_broke.h"
#include "sound.h"
#include "TimeLeftCage.h"
#include "Func_NumberX.h"
#include "Block_Light.h"
#include <stdlib.h>
#include <time.h>
#include "effect_star.h"
#include "Battle_Main.h"
#include "Battle_Attack.h"
#include "Battle_Player.h"
#include "Effect_Tetris_Block_Shining.h"
#include "Battle_Member_Damage_Count.h"
#include "Battle_Row_Charge.h"
#include "Battle_Effect_TimeLeftCount.h"
#include "Battle_ComboCheers.h"
#include "Battle_InterFaseBG.h"
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexFieldBG(LPDIRECT3DDEVICE9 pDevice);
void SetVerTexFieldBG(void);
HRESULT MakeVertexFieldFade(LPDIRECT3DDEVICE9 pDevice);
void SetVerTexFieldFade(void);
HRESULT MakeVertexField(LPDIRECT3DDEVICE9 pDevice);

HRESULT MakeVertexFieldWait(LPDIRECT3DDEVICE9 pDevice);
void SetVerTexFieldWait(void);
HRESULT MakeVertexBlock(LPDIRECT3DDEVICE9 pDevice);
void SetVerTexBlock(void);

void InitGroup(int Group_ID);
void InitBlockAll(void);
void SetGroupTop(int Group_ID);
void Check_Row(void);
void Rotation(int Group_ID,int Rot);
void DeleteRow(void);
void GroupShadowMove(void);

#define TEXTURE_FIELD_WAIT	("data/TEXTURE/Waiting.png")
//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureFieldWait	= NULL;//テクスチャメモリ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFieldWait = NULL;//頂点座標バッファ

LPDIRECT3DTEXTURE9		g_pTextureFieldBG	= NULL;//テクスチャメモリ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFieldBG = NULL;//頂点座標バッファ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFieldFade = NULL;//頂点座標バッファ

LPDIRECT3DTEXTURE9		g_pTextureField	= NULL;//テクスチャメモリ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffField = NULL;//頂点座標バッファ
LPDIRECT3DTEXTURE9		g_pTextureBlock	= NULL;//テクスチャメモリ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBlock = NULL;//頂点座標バッファ



FIELD	g_aField[MAX_FIELD_ROW][MAX_FIELD_COLUMN];	//Field構造体
BLOCK	g_aBlock[MAX_BLOCK];						//Blockの構造体
GROUP	g_aGroup[MAX_GROUP];						//Groupの構造体
bool	g_bInput;									//操作可能か
NUMBER_X *pNumberx;

float g_MonsterHP = 100.0f;
float g_PlayerHP  = 100.0f;

int nCntTime;										//テトリス内時間制御変数
int	g_TimeAutoDrop;									// テトリス内自動落下間隔
bool g_bAutoDrop;									// 自動落下ON/OFFスイッチ
int	g_AutoTime;										// 連動移動の速度

int	StoreGroup[MAX_GROUP_BLOCK]={10,10,10,10,10,10,10};		//形の出現率を保存する場所
int	StoreColor[BLOCK_MAX_COLOR]={10,10,10,10,10,10,10};	//形の出現率を保存する場所s
int g_NumGroup;										// 現在使っているグループの番号		
int g_Counter	=0;
D3DXVECTOR2 g_BlockMove;



FIELD_MOVE_TYPE g_FieldMove_Type = FIELD_MOVE_KEEP;
D3DXVECTOR3 g_FieldMove;		//フィールドの全体移動」

int g_GroupLeft;		//残りのＱグループ数
int g_GroupUseQ;		//現在使っているＱグループのナンバー
int g_nDeleteTime;		//行削除時間制御
int g_Wk_Delete[MAX_FIELD_ROW];//削除すべき行を保存するワーク 0 番目は行の総数，1からは行の番目
int g_TetrisStats;	//ゲームの進行状態
bool g_CheckDeath;	//落下の衝突フラグ

int g_RowComboCount;	//行けしコンボカウント

float g_fieldWaitAlpha; //待機マークのアルファ
float g_FieldFadeAlpha; //フィールドの暗黒する処理
GROUP_FORM g_aForm[MAX_FORM_TYPE]=
{
	/*0*/{1,1,{D3DXVECTOR2(0,0)}},//NONE
	/*1*/{4,3,{D3DXVECTOR2(0,0),D3DXVECTOR2(-1, 1), D3DXVECTOR2(-1, 0), D3DXVECTOR2( 1, 0)}},// La
	/*2*/{4,3,{D3DXVECTOR2(0,0),D3DXVECTOR2( 1, 1), D3DXVECTOR2(-1, 0), D3DXVECTOR2( 1, 0)}},// Lb
	/*3*/{4,3,{D3DXVECTOR2(0,0),D3DXVECTOR2( 0,-1), D3DXVECTOR2( 1,-1), D3DXVECTOR2( 1, 0)}},// O
	/*4*/{4,3,{D3DXVECTOR2(0,0),D3DXVECTOR2(-1,-1), D3DXVECTOR2( 0,-1), D3DXVECTOR2( 1, 0)}},// Sa
	/*5*/{4,3,{D3DXVECTOR2(0,0),D3DXVECTOR2(-1, 0), D3DXVECTOR2( 0,-1), D3DXVECTOR2( 1,-1)}},// Sb
	/*6*/{4,3,{D3DXVECTOR2(0,0),D3DXVECTOR2( 0,-1), D3DXVECTOR2( 0, 1), D3DXVECTOR2( 0, 2)}},// I
	/*7*/{4,3,{D3DXVECTOR2(0,0),D3DXVECTOR2( 0,-1), D3DXVECTOR2(-1, 0), D3DXVECTOR2( 1, 0)}} // T
};
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTetris_Main(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	MOVETIME *pMoveTime;
	pMoveTime = GetMoveTimeLeft();
	//フィールド初期化
	for(int nCntField_Y = 0; nCntField_Y < MAX_FIELD_ROW; nCntField_Y++)
	{
		for(int nCntField_X = 0; nCntField_X < MAX_FIELD_COLUMN; nCntField_X++)
		{
			g_aField[nCntField_Y][nCntField_X].nColor = 0;
			g_aField[nCntField_Y][nCntField_X].g_fAlpha = 1.0f;
			g_aField[nCntField_Y][nCntField_X].pos.x  = VALUE_FIELD_START_X + nCntField_X * VALUE_BLOCK_SIZE;
			g_aField[nCntField_Y][nCntField_X].pos.y  =	VALUE_FIELD_START_Y - nCntField_Y * VALUE_BLOCK_SIZE;

			if(nCntField_Y == 0 || nCntField_X == 0 || nCntField_X == MAX_FIELD_COLUMN - 1)
			{
				g_aField[nCntField_Y][nCntField_X].nType = FIELD_WALL;
				g_aField[nCntField_Y][nCntField_X].nColor= BLOCK_WALL;
			}
			else
				g_aField[nCntField_Y][nCntField_X].nType  = FIELD_NONE;
		}
	}
	//ブロック初期化
	InitBlockAll();
	//グループ初期化
	for(int nCntGroup = 0; nCntGroup < MAX_GROUP; nCntGroup++)
	{
		g_aGroup[nCntGroup].B_start = 0;
		g_aGroup[nCntGroup].B_end	= 0;
		g_aGroup[nCntGroup].nColor	= BLOCK_MP;
		g_aGroup[nCntGroup].nType	= GROUP_BACK;
		g_aGroup[nCntGroup].NumForm	= FORM__NONE;
		g_aGroup[nCntGroup].Place	= D3DXVECTOR2(0.0f,0.0f);
		g_aGroup[nCntGroup].Pos		= D3DXVECTOR2(0.0f,0.0f);
		g_aGroup[nCntGroup].Form.nCount = 0;
		g_aGroup[nCntGroup].Form.Rot	= 1;
		g_aGroup[nCntGroup].Rotation	= 0;
		for(int nPlace = 0; nPlace < MAX_FORM_PLACE; nPlace++)
			g_aGroup[nCntGroup].Form.Place[nPlace]=D3DXVECTOR2(0.0f,0.0f);

	}
	for(int nDel_WK = 0; nDel_WK < MAX_FIELD_ROW;nDel_WK++)
	{
		g_Wk_Delete[nDel_WK] = 0;
	}



	nCntTime = 0;
	g_TimeAutoDrop = 60;
	g_bAutoDrop = true;
	g_AutoTime	= 8;
	MakeVertexFieldBG(pDevice);
	MakeVertexFieldFade(pDevice);
	MakeVertexField(pDevice);
	MakeVertexFieldWait(pDevice);
	MakeVertexBlock(pDevice);

	g_FieldMove_Type = FIELD_MOVE_KEEP;
	g_NumGroup	= 0;
	g_BlockMove = D3DXVECTOR2(0,0);
	g_FieldMove = D3DXVECTOR3(0.0f,0.0f,0.0f);
	g_RowComboCount = 0;
	g_fieldWaitAlpha = 0.0f;
	g_FieldFadeAlpha = 0.0f;
	//ゲーム制御変数
	g_CheckDeath = false;
	g_GroupLeft = 0;
	g_GroupUseQ	= 0;
	g_nDeleteTime = 0;
	pMoveTime->fMoveTimeLeft = 0.0f;
	g_TetrisStats = TETRIS_NOTH;
	g_bInput	= false;
	pNumberx = GetNumberX();
	pNumberx[PLAYER_SCORE].nNumber = 0;
	Set_NumberX(PLAYER_SCORE,				// 数列の名前
		true,								// 使用の状態設定
		false,								// Fade状態
		FULLCOLOR,							// 使用するテクスチャの名前
		4,									// 入力最大桁数
		pNumberx[PLAYER_SCORE].nNumber,		// 入力値
		30.0f,								// 文字組の大きさX
		50.0f,								// 文字組の大きさY
		1.0f,								// 透明度の設定
		D3DXVECTOR3(1020.0f,100.0f,0.0f)	// 描画開始位置
		);
	D3DXCreateTextureFromFile(pDevice,
		TEX_FIELD_BG,
		&g_pTextureFieldBG);

	D3DXCreateTextureFromFile(pDevice,
		TEX_BLOCK_TEXTURE,
		&g_pTextureField);

	D3DXCreateTextureFromFile(pDevice,
		TEX_BLOCK_TEXTURE,
		&g_pTextureBlock);
	D3DXCreateTextureFromFile(pDevice,
		TEXTURE_FIELD_WAIT,
		&g_pTextureFieldWait);
	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void UninitTetris_Main(void)
{
	if(g_pTextureField != NULL)
	{// テクスチャの開放
		g_pTextureField->Release();
		g_pTextureField = NULL;
	}

	if(g_pVtxBuffField != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffField->Release();
		g_pVtxBuffField = NULL;
	}
	if(g_pTextureFieldWait != NULL)
	{// テクスチャの開放
		g_pTextureFieldWait->Release();
		g_pTextureFieldWait = NULL;
	}

	if(g_pVtxBuffFieldWait != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffFieldWait->Release();
		g_pVtxBuffFieldWait = NULL;
	}
	if(g_pTextureFieldBG != NULL)
	{// テクスチャの開放
		g_pTextureFieldBG->Release();
		g_pTextureFieldBG = NULL;
	}

	if(g_pVtxBuffFieldBG != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffFieldBG->Release();
		g_pVtxBuffFieldBG = NULL;
	}
	
	if(g_pVtxBuffFieldFade != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffFieldFade->Release();
		g_pVtxBuffFieldFade = NULL;
	}
	if(g_pTextureBlock != NULL)
	{// テクスチャの開放
		g_pTextureBlock->Release();
		g_pTextureBlock = NULL;
	}

	if(g_pVtxBuffBlock != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffBlock->Release();
		g_pVtxBuffBlock = NULL;
	}


}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateTetris_Main(void)
{
	//g_bInput = true;

	MOVETIME *pMoveTime;
	pMoveTime = GetMoveTimeLeft();
	nCntTime++;//時間制御変数を動かす  
	//wait mark の処理
	g_fieldWaitAlpha = sinf((nCntTime * D3DX_PI)/120);
	SetVerTexFieldWait();

	//int BattleStats = GetBattleStats();

	//if(BattleStats == BATTLE_WAIT_SKILL_OR_TETRIS_PLAYER ||  BattleStats == BATTLE_WAIT_SKILL_RUNNING ||
	//	BattleStats == BATTLE_WAIT_SKILL_RESULT || BattleStats == BATTLE_TETRIS)
	//	g_FieldMove_Type = FIELD_MOVE_IN;
	//else
	//{
	//	g_FieldMove_Type = FIELD_MOVE_OUT;
	//}




	

	//if(BattleStats == BATTLE_WAIT_SKILL_OR_TETRIS_PLAYER ||  BattleStats == BATTLE_WAIT_SKILL_RUNNING ||
	//	BattleStats == BATTLE_WAIT_SKILL_RESULT || BattleStats == BATTLE_TETRIS)
	//{
	//	g_FieldMove.x += 20;
	//	if(g_FieldMove.x > 0)
	//		g_FieldMove.x = 0;
	//	g_FieldFadeAlpha -= 0.02f;
	//	if(g_FieldFadeAlpha < 0.0f)
	//		g_FieldFadeAlpha = 0.0f;

	//}
	//else
	//{
	//	g_FieldMove.x -= 20;
	//	if(g_FieldMove.x < -400.0f)
	//		g_FieldMove.x = -400.0f;
	//	g_FieldFadeAlpha += 0.05f;
	//	if(g_FieldFadeAlpha > 0.8f)
	//		g_FieldFadeAlpha = 0.8f;
	//}
	SetVerTexFieldFade();
	SetVerTexFieldBG();




	switch(g_TetrisStats)
	{
	case TETRIS_NOTH:
		break;
	case TETRIS_START://初期一回のみ実行
		if(pMoveTime->nType != TIME_MOVE_FULL)
			SetMoveTime(TIME_MOVE_RECOVER,VALUE_DEFAULT_MOVE_TIME);
		g_TetrisStats = TETRIS_WAIT_TIME;
		break;
	case TETRIS_ROW_CHARGE:
		if(GetRowChargeType() == ROW_CHARGE_MAX_TYPE)
			g_TetrisStats = TETRIS_WAIT_TIME;
		break;
	case TETRIS_WAIT_TIME:
		//if(pMoveTime->nType == TIME_MOVE_FULL)
		//	g_TetrisStats = TETRIS_STOP;
		break;
	case TETRIS_STOP://セット待機状態
		g_GroupUseQ = 0;//使用するグループのＩＤを1にれセットする
		g_RowComboCount = 0;
		InitBlock_Q_ALL();
		InitGroup_Q_ALL();
		//if(g_bInput == true)
		//{
			//if(GetKeyboardRelease(KEY_SETUP))
			//{
				g_TetrisStats = TETRIS_SET_Q;
			//}
		//}
		break;
	case TETRIS_SET_Q:
		for(int Q_ID = 0; Q_ID < MAX_GROUP_Q;Q_ID++)
			SetGroup_Q(Q_ID);
		g_TetrisStats = TETRIS_WAIT_SET;
		break;
	case TETRIS_WAIT_SET://開始待機状態
		if(g_bInput == true && GetBattleStats() == BATTLE_TETRIS)
		{
			if(GetKeyboardTrigger(KEY_START))
			{
				g_GroupLeft = MAX_GROUP_Q;
				SetGroupTop(GROUP_USING);//使いグループを天井に配置
				InitGroup(GROUP_USING);//使いグループを設置
				InitGroup(GROUP_SHADOW);//射影グループを設置
				SetGroup_Q_Type(g_GroupUseQ,GROUP_BACK);//Ｑグループを消す
				SetMoveTime(TIME_MOVE_LOSING,VALUE_DEFAULT_MOVE_TIME);//時間が減り始めいる
				g_TetrisStats = TETRIS_UPDATE;//テトリスの状態をアップデートへ移行

			
			}
		}
		break;
	case TETRIS_UPDATE:
		if(g_bInput == true)
		{
			//回転処理
			if(GetKeyboardTrigger(KEY_ROT_LEFT))
				Rotation(GROUP_USING,0);
			if(GetKeyboardTrigger(KEY_ROT_RIGHT))
				Rotation(GROUP_USING,1);

			//================================左への移動
			if(GetKeyboardTrigger(KEY_LEFT))
			{
				g_Counter = 0;//操作時間制御変数の初期化
				g_BlockMove.x = -1;
				
			}
			if(GetKeyboardPress(KEY_LEFT))
			{
				g_Counter ++;
				if(g_Counter > 10 && nCntTime%g_AutoTime==0)//10/60秒間隔が始め，そして自動移動の時間を持つ
				{
					
					g_BlockMove.x = -1;
				}
			}
			if(GetKeyboardRelease(KEY_LEFT))
			{
				
				g_Counter = 0;//時間を初期化
			}
			//================================右への移動
			if(GetKeyboardTrigger(KEY_RIGHT))
			{
				g_Counter = 0;//操作時間制御変数の初期化
				g_BlockMove.x =  1;
				
			}
			if(GetKeyboardPress(KEY_RIGHT))
			{
				g_Counter ++;
				if(g_Counter > 10 && nCntTime%g_AutoTime==0)//10/60秒間隔が始め，そして自動移動の時間を持つ
				{
					
					g_BlockMove.x =  1;
				}
			}
			if(GetKeyboardRelease(KEY_RIGHT))
			{
				
				g_Counter = 0;//時間を初期化
			}
			//=================================下への移動
			if(GetKeyboardTrigger(KEY_DOWN))
			{
				
				g_Counter = 0;
				g_BlockMove.y = -1;
			}
			if(GetKeyboardPress(KEY_DOWN))
			{
				g_Counter ++;
				if(g_Counter > 10 && nCntTime %(g_AutoTime/2)==0)//ちょっと早めにする
				{
					
					g_BlockMove.y = -1;
				}
				g_bAutoDrop = false;//下をタッチした時に，自動落下を不可にする
			}
			else
			{g_bAutoDrop = true;}

			if(GetKeyboardRelease(KEY_DOWN))
			{
				
				g_Counter = 0;
			}
			//=================================自動移動
			if(nCntTime%g_TimeAutoDrop==0&&g_bAutoDrop == true)
			{
				
				g_BlockMove.y = -1;
			}
			if(GetKeyboardTrigger(KEY_CHEAT_UP))//チートkey
				g_BlockMove.y =  1;


			//==============================瞬間落下
			if(GetKeyboardTrigger(KEY_DROP)&&g_aGroup[GROUP_USING].Place.y > g_aGroup[GROUP_SHADOW].Place.y + 2)
			{
				PlaySound(SOUND_LABEL_SE_BLOCK_STOP);
				{
					g_aGroup[GROUP_USING].Place.y = g_aGroup[GROUP_SHADOW].Place.y + 1;
					nCntTime = g_TimeAutoDrop;
				}


				D3DXVECTOR3 pos;//エフェクト処理用ワーク
				pos.x = VALUE_FIELD_START_X + (g_aGroup[GROUP_USING].Place.x+1) * VALUE_BLOCK_SIZE; 
				pos.y = VALUE_FIELD_START_Y - g_aGroup[GROUP_USING].Place.y * VALUE_BLOCK_SIZE;
				pos.z = 0.0f;
				SetLight(pos);
				SetEffect_Star(pos,g_aGroup[GROUP_USING].nColor,1);
			}			
		}//if(input== true)
		//================ここからは移動のチェック処理================//
		//左右移動の処理
		if(g_BlockMove.x != 0)
			PlaySound(SOUND_LABEL_SE_BLOCK_MOVE);
		for(int nCntBlock = g_aGroup[GROUP_USING].B_start;
			nCntBlock <= g_aGroup[GROUP_USING].B_end;
			nCntBlock ++)
		{
			int NowPlaceX	= (int)g_aBlock[nCntBlock].Place.x;
			int NowPlaceY	= (int)g_aBlock[nCntBlock].Place.y;//現在位置保存ワーク
			int DestPlaceX	= (int)g_aBlock[nCntBlock].Place.x + (int)g_BlockMove.x;
			int DestPlaceY	= (int)g_aBlock[nCntBlock].Place.y + (int)g_BlockMove.y;//目的位置保存ワーク	
			if(g_aField[NowPlaceY][DestPlaceX].nType != FIELD_NONE)
				g_BlockMove.x = 0;//移動量を0にする
		}//for
		//上下移動の処理
		if(g_BlockMove.y != 0)
		{
			int NowPlaceX	= 0;
			int NowPlaceY	= 0;
			int DestPlaceX	= 0;
			int DestPlaceY	= 0;

			PlaySound(SOUND_LABEL_SE_BLOCK_MOVE);

			for(int nCntBlock = g_aGroup[GROUP_USING].B_start;
				nCntBlock <= g_aGroup[GROUP_USING].B_end;
				nCntBlock ++)
			{
				NowPlaceX	= (int)g_aBlock[nCntBlock].Place.x;
				NowPlaceY	= (int)g_aBlock[nCntBlock].Place.y;//現在位置保存ワーク
				DestPlaceX	= NowPlaceX + (int)g_BlockMove.x;
				DestPlaceY	= NowPlaceY + (int)g_BlockMove.y;//目的位置保存ワーク

				if(g_aField[DestPlaceY][NowPlaceX].nType != FIELD_NONE)
				{
					g_BlockMove.y = 0;//移動量を0にする
					g_CheckDeath = true;
				}
			}//for
		}//(g_BlockMove.y != 0)
		//====================ここからは非かかるときの処理================//
		if(g_CheckDeath == true)
		{
			PlaySound(SOUND_LABEL_SE_BLOCK_STOP); // 停止音

			//ブロックをフィル度上に反映
			for(int nCntBlock  = g_aGroup[GROUP_USING].B_start;
				nCntBlock <= g_aGroup[GROUP_USING].B_end;
				nCntBlock ++)
			{

				int NowPlaceX = (int)g_aBlock[nCntBlock].Place.x;
				int NowPlaceY = (int)g_aBlock[nCntBlock].Place.y;

				g_aField[NowPlaceY][NowPlaceX].nType	= FIELD_USE;
				g_aField[NowPlaceY][NowPlaceX].nColor	= g_aBlock[nCntBlock].nColor;
				//g_aField[NowPlaceY][NowPlaceX].nColor	= g_aGroup[GROUP_USING].nColor;

				//エフェクトを付けようか
				D3DXVECTOR3 Epos = D3DXVECTOR3(0.0f,0.0f,0.0f);
				Epos.x = g_aField[NowPlaceY][NowPlaceX].pos.x;
				Epos.y = g_aField[NowPlaceY][NowPlaceX].pos.y + 15.0f;//15.0fは修正値
				int EColor = g_aGroup[GROUP_USING].nColor;
				if(EColor != BLOCK_MP)
				SetEffect_Broke(Epos,D3DXVECTOR3(0.0f,0.0f,0.0f),EColor); 
			}
			//あと制御処理
				g_GroupLeft--;//使用可能ブロック数を減らす
				Check_Row();//とりあえず一回チェックしとく
			if(g_Wk_Delete[0]==0)
			{
				if( g_GroupLeft != 0)//残りのＱグループが0じゃない場合
				{
					g_GroupUseQ ++;//次のＱグループへ移行
					SetGroupTop(GROUP_USING);//使用グループを天井に設置
					InitGroup(GROUP_USING);//操作グループを設置
					InitGroup(GROUP_SHADOW);//ゴーストグループを設置
					SetGroup_Q_Type(g_GroupUseQ,GROUP_BACK);//現在のグループを無効にする

					
				}
				else
				{//残りのＱグループが0の場合
					InitBlockAll();//すべてのブロックを初期化する
					InitBlock_Q_ALL();
					InitGroup_Q_ALL();
					SetMoveTime(TIME_MOVE_RECOVER,VALUE_DEFAULT_MOVE_TIME);	
					g_TetrisStats = TETRIS_WAIT_TIME;
				}

			}//if(g_Wk_Delete[0]==0)
			else
			{
				g_TetrisStats = TETRIS_WAIT_DELETE;
			}
			g_CheckDeath = false;
		}
		else//(if checkDeath == false)
		{
			//中心グループの絶対位置を反映
			g_aGroup[GROUP_USING].Place += g_BlockMove;
			for(int nCntBlock = g_aGroup[GROUP_USING].B_start;
				nCntBlock <= g_aGroup[GROUP_USING].B_end;
				nCntBlock ++)
			{
				g_aBlock[nCntBlock].Place = g_aGroup[GROUP_USING].Place + g_aBlock[nCntBlock].FormPos;

				g_aBlock[nCntBlock].Pos.x = VALUE_FIELD_START_X + g_aBlock[nCntBlock].Place.x * VALUE_BLOCK_SIZE; 
				g_aBlock[nCntBlock].Pos.y = VALUE_FIELD_START_Y - g_aBlock[nCntBlock].Place.y * VALUE_BLOCK_SIZE;
			}
			//==============ここからはブロックの射影処理================//
		
			GroupShadowMove();
		
			//==============ここまではブロックの射影処理================//
		}
		break;
		case TETRIS_WAIT_DELETE:
			g_nDeleteTime --;//エフェクトの時間が減り始める
			PlaySound(SOUND_LABEL_SE_ICE_SLASH);
			//エフェクト処理の実体
			for(int nCount = g_Wk_Delete[0],Idx = 1;nCount >0 ;nCount--,Idx++)
			{

				for(int nCloumn = 1; nCloumn < MAX_FIELD_COLUMN -1;nCloumn++)
				{
					if(g_nDeleteTime % 4 == 0)
						g_aField[g_Wk_Delete[Idx]][nCloumn].g_fAlpha = 1.0f;
					else
						g_aField[g_Wk_Delete[Idx]][nCloumn].g_fAlpha = 0.1f;

				}
			}
			//フィールド反映処理のエフェクト
			if(g_nDeleteTime ==0)
			{
				for(int nCount = g_Wk_Delete[0],Idx = 1;nCount >0 ;nCount--,Idx++)
				{
					
					//コンボチャージ
					//SetRowCharge(1,ROW_CHARGE_INCREASING);
					//行増加処理
					g_RowComboCount ++;
					for(int nCloumn = 1; nCloumn < MAX_FIELD_COLUMN -1;nCloumn++)
					{
						BATTLE_T_MEMBER *pMember = GetTMember();
						D3DXVECTOR3 Pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
						g_aField[g_Wk_Delete[Idx]][nCloumn].g_fAlpha = 1.0f;
						Pos.x = g_aField[g_Wk_Delete[Idx]][nCloumn].pos.x + VALUE_BLOCK_SIZE;
						Pos.y = g_aField[g_Wk_Delete[Idx]][nCloumn].pos.y - VALUE_BLOCK_SIZE;
						int Color = g_aField[g_Wk_Delete[Idx]][nCloumn].nColor;

						if(Color != BLOCK_MP)
						{
							bool team = false;
							for(int nMember = 0; nMember < TEAM_PLAYER_MAX;nMember++)
							{
								if(pMember[nMember].Color == Color)
								{
									SetEffect_Broke(Pos,pMember[nMember].pos,Color);
									team = true;
								}
							}
							if(team == false)
							{
								SetEffect_Broke(Pos,D3DXVECTOR3(0.0f,0.0f,0.0f),Color);
								SetEffect_Broke(Pos,D3DXVECTOR3(0.0f,0.0f,0.0f),Color);
							}
						}
					}

				}	
				//フィールド反映の実体
				DeleteRow();
				if( g_GroupLeft != 0)//残りのＱグループが0じゃない場合
				{
					g_GroupUseQ ++;//次のＱグループへ移行
					SetGroupTop(GROUP_USING);//使用グループを天井に設置
					InitBlockAll();//すべてのブロックを初期化する
					InitGroup(GROUP_USING);//操作グループを設置
					InitGroup(GROUP_SHADOW);//ゴーストグループを設置
					SetGroup_Q_Type(g_GroupUseQ,GROUP_BACK);//現在のグループを無効にする
					g_TetrisStats = TETRIS_UPDATE;
				}
				else
				{//残りのＱグループが0の場合
					InitBlockAll();//すべてのブロックを初期化する
					InitBlock_Q_ALL();
					InitGroup_Q_ALL();
					SetGroupTop(GROUP_USING);//使用グループを天井に設置
					SetMoveTime(TIME_MOVE_RECOVER,VALUE_DEFAULT_MOVE_TIME);	
					g_TetrisStats = TETRIS_ROW_CHARGE;//チャージバッファタイムへ変更
				}
			}
			break;
		}//case Master

		//==============以上はブロックのmain処理================//
		nCntTime = (int)SetMAX((float)nCntTime,(float)120,0.0f,1);
		g_BlockMove = D3DXVECTOR2(0,0);

	//==============ここからははターン中移動時間切れの処理================//
	if(g_GroupLeft != 0 && pMoveTime->fMoveTimeLeft == 0)
	{
		g_GroupLeft = 0;					
		InitBlockAll();
		InitGroup_Q_ALL();
		InitBlock_Q_ALL();
		//一行ずつ上に移す
		for(int nRow = MAX_FIELD_ROW -1; nRow > 1; nRow--)
		{
			for(int nCol = 1; nCol < MAX_FIELD_COLUMN-1;nCol ++)
			{
				g_aField[nRow][nCol].nType  = g_aField[nRow-1][nCol].nType;
				g_aField[nRow][nCol].nColor = g_aField[nRow-1][nCol].nColor;
			}

		}
		//最下行をランダムに色を入れる
		int UseCount = 0;
		for(int nCol = 1; nCol < MAX_FIELD_COLUMN-1;nCol ++)
		{
			if(rand()%100<10)
				g_aField[1][nCol].nType = FIELD_NONE;
			else
			{
				g_aField[1][nCol].nType = FIELD_USE;
				g_aField[1][nCol].nColor = rand()%6;
				UseCount ++;
			}

		}
		//もしすべて有効と判断した場合，ランダムに一個を無効にする
		if(UseCount = MAX_FIELD_COLUMN - 2)
		{
			int Dest = rand()%(MAX_FIELD_COLUMN-2)+1;
			g_aField[1][Dest].nType = FIELD_NONE;
		}
		g_TetrisStats = TETRIS_WAIT_TIME;
		SetMoveTime(TIME_MOVE_RECOVER,VALUE_DEFAULT_MOVE_TIME);//時間ケージを回復させる
	}
	SetVerTexBlock();
	SetVerTexField();
	//==================shining Effect========================
	for(int nCntField_Y = 0; nCntField_Y < MAX_FIELD_ROW; nCntField_Y++)
	{
		for(int nCntField_X = 0; nCntField_X < MAX_FIELD_COLUMN; nCntField_X++)
		{
			if(g_aField[nCntField_Y][nCntField_X].nType == FIELD_USE)
				if(rand()%10000 < 1)
					SetBlockShining(D3DXVECTOR3(g_aField[nCntField_Y][nCntField_X].pos.x + g_FieldMove.x,g_aField[nCntField_Y][nCntField_X].pos.y,0.0f));
		}
	}
	switch(g_FieldMove_Type)// = FIELD_MOVE_KEEP;//g_FieldMoveとFadeの描画
	{
	case FIELD_MOVE_IN:
		if(g_FieldMove_Type != FIELD_MOVE_KEEP)
		{
			for(int nCntField_Y = 0; nCntField_Y < MAX_FIELD_ROW; nCntField_Y++)
			{
				for(int nCntField_X = 0; nCntField_X < MAX_FIELD_COLUMN; nCntField_X++)
				{
					if(nCntField_Y != 0 && nCntField_X != 0 && nCntField_X != MAX_FIELD_COLUMN - 1)
					{
						g_aField[nCntField_Y][nCntField_X].g_fAlpha += 0.03f; 
						if(g_aField[nCntField_Y][nCntField_X].g_fAlpha > 1.0f)
						{
							g_aField[nCntField_Y][nCntField_X].g_fAlpha = 1.0f;
							//g_FieldMove_Type = FIELD_MOVE_KEEP;
						}
					}
				}
			}
			SetInterPhaseChange(INTERFACE_BG_PHASE_IN);
			g_FieldMove.x += 5;
			//g_FieldFadeAlpha -= 0.02f;
			//if(g_FieldFadeAlpha < 0.0f)
			//	g_FieldFadeAlpha = 0.0f;

			if(g_FieldMove.x > 0)
			{
				g_FieldMove.x = 0;
				g_FieldMove_Type = FIELD_MOVE_KEEP;
			}
		}
		break;
	case FIELD_MOVE_OUT:
		if(g_FieldMove_Type != FIELD_MOVE_KEEP)
		{
			for(int nCntField_Y = 0; nCntField_Y < MAX_FIELD_ROW; nCntField_Y++)
			{
				for(int nCntField_X = 0; nCntField_X < MAX_FIELD_COLUMN; nCntField_X++)
				{
					if(nCntField_Y != 0 && nCntField_X != 0 && nCntField_X != MAX_FIELD_COLUMN - 1)
					{
						g_aField[nCntField_Y][nCntField_X].g_fAlpha -= 0.03f; 
						if(g_aField[nCntField_Y][nCntField_X].g_fAlpha < 0.1f)
						{
							g_aField[nCntField_Y][nCntField_X].g_fAlpha = 0.1f;
							//g_FieldMove_Type = FIELD_MOVE_KEEP;
						}
					}
				}
			}
			SetInterPhaseChange(INTERFACE_BG_PHASE_OUT);
			g_FieldMove.x -= 5;
			//g_FieldFadeAlpha += 0.05f;
			//if(g_FieldFadeAlpha > 0.8f)
			//	g_FieldFadeAlpha = 0.8f;
			if(g_FieldMove.x < -400.0f)
			{
				g_FieldMove.x = -400.0f;
				g_FieldMove_Type = FIELD_MOVE_KEEP;
			}
		}
		break;
	case FIELD_MOVE_KEEP:
		break;
	}
	SetSpecialBlockEffect();
}
void SetSpecialBlockEffect(void)
{
	int Start = g_aGroup[GROUP_USING].B_start;
	
	for(int nCntBlock = Start,Idx = 0;Idx < MAX_FORM_PLACE;nCntBlock++,Idx++)
	{
		if(g_aBlock[nCntBlock].Type == BLOCK_GRAVITY && rand()%5 == 0)
		{
			D3DXVECTOR3 pos = g_aBlock[nCntBlock].Pos;
			D3DXVECTOR3 posBlock  = g_aBlock[nCntBlock].Pos;
			pos.x	+= (rand()%21 - 10);
			pos.y	-= rand()%100;
			SetEffect_Broke(posBlock,pos,g_aBlock[nCntBlock].nColor);
		}
	}

}

//=============================================================================
//フィールドの移動
//=============================================================================
void SetTetrisFieldMove(FIELD_MOVE_TYPE type)
{
	g_FieldMove_Type = type;
}
FIELD_MOVE_TYPE GetFieldMoveType(void)
{
	return g_FieldMove_Type;
}

//=============================================================================
// 射影グループの処理
//=============================================================================
void GroupShadowMove(void)
{
	D3DXVECTOR2 Place = g_aGroup[GROUP_USING].Place;
	int FormPosX[MAX_FORM_PLACE] = {0,0,0,0};
	int FormPosY[MAX_FORM_PLACE] = {0,0,0,0};

		for(int nCntBlock = g_aGroup[GROUP_USING].B_start,FormIdx = 0;
			nCntBlock <= g_aGroup[GROUP_USING].B_end;
			nCntBlock ++,FormIdx++)
		{
			FormPosX[FormIdx] = g_aBlock[nCntBlock].FormPos.x;
			FormPosY[FormIdx] = g_aBlock[nCntBlock].FormPos.y;
		}
		int PlaceY = 0;
		for(PlaceY = Place.y;PlaceY>1;PlaceY--)
		{
			bool check = true;
			for(int FormIdx = 0; FormIdx < MAX_FORM_PLACE; FormIdx ++)
			{
				int DestPlaceX = Place.x + FormPosX[FormIdx];
				int DestPlaceY = PlaceY + FormPosY[FormIdx];

				if(g_aField[DestPlaceY-1][DestPlaceX].nType != FIELD_NONE)
				{
					check = false;
					break;
				}
			}
			if(check == false)
				break;
		}
		g_aGroup[GROUP_SHADOW].Place.x = g_aGroup[GROUP_USING].Place.x;
		g_aGroup[GROUP_SHADOW].Place.y = PlaceY;
		for(int AnCntBlock = g_aGroup[GROUP_SHADOW].B_start,BnCntBlock = g_aGroup[GROUP_USING].B_start;
			AnCntBlock <= g_aGroup[GROUP_SHADOW].B_end;
			AnCntBlock ++,BnCntBlock++)
		{
			g_aBlock[AnCntBlock].FormPos = g_aBlock[BnCntBlock].FormPos;
			g_aBlock[AnCntBlock].Place.x = g_aGroup[GROUP_SHADOW].Place.x + g_aBlock[AnCntBlock].FormPos.x;
			g_aBlock[AnCntBlock].Place.y = g_aGroup[GROUP_SHADOW].Place.y + g_aBlock[AnCntBlock].FormPos.y;
			g_aBlock[AnCntBlock].Pos.x = VALUE_FIELD_START_X + g_aBlock[AnCntBlock].Place.x * VALUE_BLOCK_SIZE; 
			g_aBlock[AnCntBlock].Pos.y = VALUE_FIELD_START_Y - g_aBlock[AnCntBlock].Place.y * VALUE_BLOCK_SIZE;
		}


}
//=============================================================================
// Blockのチェック
//=============================================================================
void Check_Row(void)
{
	int nDel_WK = 1;
	//=================ここからは行のチェック及び消滅=================//
	for(int nCheck_Row = 1; nCheck_Row < MAX_FIELD_ROW -1;nCheck_Row++)
	{
		int nCheck = 0;
		for(int nCheck_Column = 1;nCheck_Column < MAX_FIELD_COLUMN -1;nCheck_Column++)
		{
			if(g_aField[nCheck_Row][nCheck_Column].nType == FIELD_USE)
				nCheck ++;
		}
		if(nCheck == MAX_FIELD_COLUMN -2)
		{	
			g_Wk_Delete[0]++;//ワーク内の行総数更新
			g_Wk_Delete[nDel_WK] = nCheck_Row;//現在の行をワークに保存
			nDel_WK++;//インデックスの更新
			g_nDeleteTime = VALUE_TIME_DELAY;//更新ストップ時間セット
		}
	}
	//=================ここまでは行のチェック及び消滅=================//
}
void DeleteRow(void)
{
	MOVETIME *pMoveTime;
	pMoveTime = GetMoveTimeLeft();
	DAMAGE_STRUCT	*pDamage;
	pDamage = GetDamageStruct();
	InitBattleDamage();
	BATTLE_T_MEMBER *pMember = GetTMember();
	COMBO_DAMAGE_X	*pDamageComobo = GetDamageCombo();
	BATTLE_PLAYER *pBattlePlayer = GetBattlePlayer();
	int SkillBlockCombo = 0;
	int Idx = g_Wk_Delete[0];//消すべき最上行に設定
	int RowCount = g_Wk_Delete[0] -1;
	//コンボチャージ
	SetRowCharge(RowCount+1,ROW_CHARGE_INCREASING);
	for(int IndexRow = g_Wk_Delete[Idx];g_Wk_Delete[0] > 0;Idx--,g_Wk_Delete[0]--)
	{
		IndexRow = g_Wk_Delete[Idx];
		for(int nCopy_Row = IndexRow;nCopy_Row<MAX_FIELD_ROW -1 ;nCopy_Row++)
		{
			for(int nCopy_Column = 1;nCopy_Column<MAX_FIELD_COLUMN -1;nCopy_Column++)
			{
				if(nCopy_Row == IndexRow)//コンボの計算
				{
					int Color= g_aField[IndexRow][nCopy_Column].nColor ;
					if(Color ==  BLOCK_MP)
						SkillBlockCombo++;
					else
					pDamage[Color].nCombo ++;
				}

				g_aField[nCopy_Row][nCopy_Column].nColor = g_aField[nCopy_Row + 1][nCopy_Column].nColor;
				g_aField[nCopy_Row][nCopy_Column].nType = g_aField[nCopy_Row + 1][nCopy_Column].nType;
			}
		}
		pNumberx[PLAYER_SCORE].nNumber += 100;		//スコア更新
		pMoveTime->fMoveTimeLeft += 60;				//移動時間を少し復元，ボーナスとして	
	}

	//Cheers の　文字エフェクト
	D3DXVECTOR3 Cheerpos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	Cheerpos.y	= g_aField[g_Wk_Delete[1]][0].pos.y - (RowCount + 1)*VALUE_BLOCK_SIZE;
	Cheerpos.x	= g_aField[g_Wk_Delete[1]][0].pos.x + rand()%((int)(VALUE_FIELD_SIZE_Y * 0.8f));
	SetUpBattleBattle_Effect_ComboCheers(RowCount,Cheerpos);
	//member ごとのダメージを事前計算
	for(int nMember = 0; nMember < TEAM_PLAYER_MAX;nMember++)
	{
		int nColor = pMember[nMember].Color;
		int BaseDamagewk = pMember[nMember].DamageBase;
			pMember[nMember].DamageBase +=(int)(pDamage[nColor].nCombo * ATTACK_MAGNIFCATION_PER_PIECE * pMember[nMember].fATK);
		int Value = pMember[nMember].DamageBase - BaseDamagewk;
		if(Value > 0)
		{
			SetComboDamageType(nMember,Value,COMBO_DAMAGE_UP,COMBO_DAMAGE_TIME);
		}
	}
	//MP 増加の計算
	int MPvalue = pBattlePlayer->MP_Recover * SkillBlockCombo;
	if(MPvalue > 0 )
	{	
		SetBattle_PlayerMP_Recover(MP_TYPE_RECOVER,MPvalue * -1);
	}
	
	//回復の増加
	pBattlePlayer->HP_RecoverResult += pBattlePlayer->HP_Recover * pDamage[BLOCK_PINK].nCombo;
	
	//InitBattleDamage();
}
//=============================================================================
// 描画処理
//=============================================================================
void DrawTetris_Main(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//フィールドBGの描画
	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffFieldBG, 0, sizeof(VERTEX_2D));
	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	// テクスチャの設定
	pDevice->SetTexture(0,g_pTextureFieldBG);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,0, NUM_POLYGON);


	DrawBattle_Effect_TimeLeftCount();


	//フィールドWAITの描画
	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffFieldWait, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetTexture(0,g_pTextureFieldWait);

	if(g_TetrisStats != TETRIS_UPDATE && g_TetrisStats != TETRIS_WAIT_DELETE)
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);



	//フィールドの描画
	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffField, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetTexture(0,g_pTextureField);
	for(int nCntField_Y = 0; nCntField_Y < MAX_FIELD_ROW; nCntField_Y++)
	{
		for(int nCntField_X = 0; nCntField_X < MAX_FIELD_COLUMN; nCntField_X++)
		{
			//if(g_aField[nCntField_Y][nCntField_X].nType == FIELD_NONE)
			//	continue;
			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, ((nCntField_Y* MAX_FIELD_COLUMN + nCntField_X) * 4), NUM_POLYGON);
		}
	}
	//エフェクトの描画
	DrawBlock_Light();


	//ロックの描画
	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffBlock, 0, sizeof(VERTEX_2D));
	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	// テクスチャの設定
	pDevice->SetTexture(0,g_pTextureBlock);
	for(int nCnt_Block = 0; nCnt_Block < MAX_BLOCK;nCnt_Block ++)
	{
		if(g_aBlock[nCnt_Block].bUse == false)
			continue;
		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,nCnt_Block*4, NUM_POLYGON);
	}

	//フィールドFADEの描画
	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffFieldFade, 0, sizeof(VERTEX_2D));
	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	// テクスチャの設定
	pDevice->SetTexture(0,NULL);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,0, NUM_POLYGON);


	//DrawBattle_Effect_TimeLeftCount();
	
}
//=============================================================================
// Groupの初期化処理
//=============================================================================
void InitGroup(int Group_ID)
{
	int start = 0;
	int nCntBlock = 0;

	if(Group_ID == GROUP_SHADOW)
	{
		g_aGroup[Group_ID] = g_aGroup[GROUP_USING];
		g_aGroup[Group_ID].nType	= BLOCK_SHADOW;

	}
	else
	{
		g_aGroup[Group_ID].nType	= BLOCK_NORMAL;
		g_aGroup[Group_ID].NumForm	= GetGroup_Q_NumForm(g_GroupUseQ);
		g_aGroup[Group_ID].nColor	= GetGroup_Q_Color(g_GroupUseQ);
		g_aGroup[Group_ID].Form		= g_aForm[g_aGroup[Group_ID].NumForm];
		g_aGroup[Group_ID].Rotation = 0;
	}

	while(g_aBlock[nCntBlock].bUse == true)
		nCntBlock++;

	//======念のために　これを書いとく======//
	if(nCntBlock > MAX_BLOCK - 10)
	{
		for(int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock ++)
		{
			g_aBlock[nCntBlock].bUse	= false;
			g_aBlock[nCntBlock].nColor	= BLOCK_RED;
			g_aBlock[nCntBlock].Place	= D3DXVECTOR2(0,0);
			g_aBlock[nCntBlock].Pos		= D3DXVECTOR2(0.0f,0.0f);
			g_aBlock[nCntBlock].FormPos	= D3DXVECTOR2(0,0);
		}

	}
	//======念のために　これを書いとく======//

	g_aGroup[Group_ID].B_start	= nCntBlock;
	start	= nCntBlock;
	g_aGroup[Group_ID].B_end	= start + g_aGroup[Group_ID].Form.nCount-1;


	//GetGroup_Q_D_Color
	int GroupQ_Start =  GetGroup_Q_Start(g_GroupUseQ);
	for(int nBlock = 0; nBlock < g_aGroup[Group_ID].Form.nCount;nBlock++)
	{
		g_aBlock[nBlock + start].bUse	 = true;
		if(Group_ID == GROUP_SHADOW)

		{
			g_aBlock[nBlock + start].Type = BLOCK_SHADOW;
		}
		else
		{
			g_aBlock[nBlock + start].Type = BLOCK_NORMAL;
		}


		g_aBlock[nBlock + start].nColor	 = g_aGroup[Group_ID].nColor;
		int nBlockQD = nBlock + GroupQ_Start;
		int nColor = GetGroup_Q_D_Color(nBlockQD);
		g_aBlock[nBlock + start].nColor	 = nColor;

		g_aBlock[nBlock + start].FormPos = g_aGroup[Group_ID].Form.Place[nBlock];
		g_aBlock[nBlock + start].Type	 = g_aGroup[Group_ID].nType;
		g_aBlock[nBlock + start].Place	 = g_aGroup[Group_ID].Place + g_aBlock[nBlock + start].FormPos;
		g_aBlock[nBlock + start].Pos.x	 = g_aGroup[Group_ID].Pos.x + g_aBlock[nBlock + start].Place.x * VALUE_BLOCK_SIZE;
		g_aBlock[nBlock + start].Pos.y	 = g_aGroup[Group_ID].Pos.y + g_aBlock[nBlock + start].Place.y * VALUE_BLOCK_SIZE;
	}
	//SetVerTexBlock();
}
//=============================================================================
// Groupのセット処理
//=============================================================================
void SetGroupTop(int Group_ID)
{
	g_aGroup[Group_ID].nType	= GROUP_USE;
	g_aGroup[Group_ID].Place.x	= (int)MAX_FIELD_COLUMN/2;
	g_aGroup[Group_ID].Place.y	= (int)MAX_FIELD_ROW - 2;
}
//=============================================================================
// FieldBG頂点の作成
//=============================================================================
HRESULT MakeVertexFieldBG(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * 1,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
		FVF_VERTEX_2D,						// 使用する頂点フォーマット
		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
		&g_pVtxBuffFieldBG,					// 頂点バッファインターフェースへのポインタ
		NULL)))								// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffFieldBG->Lock(0, 0, (void**)&pVtx, 0);

				// 頂点座標の設定
				pVtx[0].pos.x = VALUE_FIELD_START_X;///* + g_FieldMove.x*/;	
				pVtx[0].pos.y = VALUE_FIELD_START_Y - VALUE_FIELD_SIZE_Y;
				pVtx[0].pos.z = 0.0f;

				pVtx[1].pos.x = VALUE_FIELD_START_X/* + g_FieldMove.x*/ + VALUE_FIELD_SIZE_X;
				pVtx[1].pos.y = VALUE_FIELD_START_Y - VALUE_FIELD_SIZE_Y;
				pVtx[1].pos.z = 0.0f;

				pVtx[2].pos.x = VALUE_FIELD_START_X;///* + g_FieldMove.x*/;
				pVtx[2].pos.y = VALUE_FIELD_START_Y;
				pVtx[2].pos.z = 0.0f;

				pVtx[3].pos.x = VALUE_FIELD_START_X/* + g_FieldMove.x*/ + VALUE_FIELD_SIZE_X;
				pVtx[3].pos.y = VALUE_FIELD_START_Y;
				pVtx[3].pos.z = 0.0f;
				// rhwの設定
				pVtx[0].rhw =
					pVtx[1].rhw =
					pVtx[2].rhw =
					pVtx[3].rhw = 1.0f;

				// 頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点データをアンロックする
		g_pVtxBuffFieldBG->Unlock();
	}
	return S_OK;
}
void SetVerTexFieldBG(void)
{//頂点バッファの中身を埋める
VERTEX_2D *pVtx;

// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
g_pVtxBuffFieldBG->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].pos.x = VALUE_FIELD_START_X;///* + g_FieldMove.x*/;	
		pVtx[0].pos.y = VALUE_FIELD_START_Y - VALUE_FIELD_SIZE_Y;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = VALUE_FIELD_START_X/* + g_FieldMove.x*/ + VALUE_FIELD_SIZE_X;
		pVtx[1].pos.y = VALUE_FIELD_START_Y - VALUE_FIELD_SIZE_Y;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = VALUE_FIELD_START_X;///* + g_FieldMove.x*/;
		pVtx[2].pos.y = VALUE_FIELD_START_Y;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = VALUE_FIELD_START_X/* + g_FieldMove.x*/ + VALUE_FIELD_SIZE_X;
		pVtx[3].pos.y = VALUE_FIELD_START_Y;
		pVtx[3].pos.z = 0.0f;

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

// 頂点データをアンロックする
g_pVtxBuffFieldBG->Unlock();
}
//=============================================================================
// FieldFade頂点の作成
//=============================================================================
HRESULT MakeVertexFieldFade(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * 1,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
		FVF_VERTEX_2D,						// 使用する頂点フォーマット
		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
		&g_pVtxBuffFieldFade,					// 頂点バッファインターフェースへのポインタ
		NULL)))								// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffFieldFade->Lock(0, 0, (void**)&pVtx, 0);

				// 頂点座標の設定
				pVtx[0].pos.x = VALUE_FIELD_START_X;///* + g_FieldMove.x*/;	
				pVtx[0].pos.y = VALUE_FIELD_START_Y - VALUE_FIELD_SIZE_Y;
				pVtx[0].pos.z = 0.0f;

				pVtx[1].pos.x = VALUE_FIELD_START_X/* + g_FieldMove.x*/ + VALUE_FIELD_SIZE_X;
				pVtx[1].pos.y = VALUE_FIELD_START_Y - VALUE_FIELD_SIZE_Y;
				pVtx[1].pos.z = 0.0f;

				pVtx[2].pos.x = VALUE_FIELD_START_X;///* + g_FieldMove.x*/;
				pVtx[2].pos.y = VALUE_FIELD_START_Y;
				pVtx[2].pos.z = 0.0f;

				pVtx[3].pos.x = VALUE_FIELD_START_X/* + g_FieldMove.x*/ + VALUE_FIELD_SIZE_X;
				pVtx[3].pos.y = VALUE_FIELD_START_Y;
				pVtx[3].pos.z = 0.0f;
				// rhwの設定
				pVtx[0].rhw =
					pVtx[1].rhw =
					pVtx[2].rhw =
					pVtx[3].rhw = 1.0f;

				// 頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点データをアンロックする
		g_pVtxBuffFieldFade->Unlock();
	}
	return S_OK;
}
void SetVerTexFieldFade(void)
{//頂点バッファの中身を埋める
VERTEX_2D *pVtx;

// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
g_pVtxBuffFieldFade->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].pos.x = VALUE_FIELD_START_X;///* + g_FieldMove.x*/;	
		pVtx[0].pos.y = VALUE_FIELD_START_Y - VALUE_FIELD_SIZE_Y;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = VALUE_FIELD_START_X/* + g_FieldMove.x*/ + VALUE_FIELD_SIZE_X;
		pVtx[1].pos.y = VALUE_FIELD_START_Y - VALUE_FIELD_SIZE_Y;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = VALUE_FIELD_START_X;///* + g_FieldMove.x*/;
		pVtx[2].pos.y = VALUE_FIELD_START_Y;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = VALUE_FIELD_START_X/* + g_FieldMove.x*/ + VALUE_FIELD_SIZE_X;
		pVtx[3].pos.y = VALUE_FIELD_START_Y;
		pVtx[3].pos.z = 0.0f;

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_FieldFadeAlpha);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_FieldFadeAlpha);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_FieldFadeAlpha);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_FieldFadeAlpha);

// 頂点データをアンロックする
g_pVtxBuffFieldFade->Unlock();
}
//=============================================================================
// FieldWait頂点の作成
//=============================================================================
HRESULT MakeVertexFieldWait(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * 1,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
		FVF_VERTEX_2D,						// 使用する頂点フォーマット
		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
		&g_pVtxBuffFieldWait,					// 頂点バッファインターフェースへのポインタ
		NULL)))								// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffFieldWait->Lock(0, 0, (void**)&pVtx, 0);

				// 頂点座標の設定
		pVtx[0].pos.x = VALUE_FIELD_START_X + VALUE_FIELD_SIZE_X*0.2f;
		pVtx[0].pos.y = VALUE_FIELD_START_Y	- VALUE_FIELD_SIZE_Y*0.2f;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = VALUE_FIELD_START_X + VALUE_FIELD_SIZE_X*0.8f;
		pVtx[1].pos.y = VALUE_FIELD_START_Y	- VALUE_FIELD_SIZE_Y*0.2f;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = VALUE_FIELD_START_X + VALUE_FIELD_SIZE_X*0.2f;
		pVtx[2].pos.y = VALUE_FIELD_START_Y - VALUE_FIELD_SIZE_Y*0.3f;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = VALUE_FIELD_START_X + VALUE_FIELD_SIZE_X*0.8f;
		pVtx[3].pos.y = VALUE_FIELD_START_Y - VALUE_FIELD_SIZE_Y*0.3f;
		pVtx[3].pos.z = 0.0f;
			// rhwの設定
		pVtx[0].rhw =
			pVtx[1].rhw =
			pVtx[2].rhw =
			pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fieldWaitAlpha);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fieldWaitAlpha);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fieldWaitAlpha);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fieldWaitAlpha);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点データをアンロックする
		g_pVtxBuffFieldWait->Unlock();
	}
	return S_OK;
}
void SetVerTexFieldWait(void)
{
	//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffFieldWait->Lock(0, 0, (void**)&pVtx, 0);
		pVtx[0].pos.x = VALUE_FIELD_START_X + VALUE_FIELD_SIZE_X*0.2f;
		pVtx[0].pos.y = VALUE_FIELD_START_Y	- VALUE_FIELD_SIZE_Y*0.8f;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = VALUE_FIELD_START_X + VALUE_FIELD_SIZE_X*0.8f;
		pVtx[1].pos.y = VALUE_FIELD_START_Y	- VALUE_FIELD_SIZE_Y*0.8f;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = VALUE_FIELD_START_X + VALUE_FIELD_SIZE_X*0.2f;
		pVtx[2].pos.y = VALUE_FIELD_START_Y - VALUE_FIELD_SIZE_Y*0.7f;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = VALUE_FIELD_START_X + VALUE_FIELD_SIZE_X*0.8f;
		pVtx[3].pos.y = VALUE_FIELD_START_Y - VALUE_FIELD_SIZE_Y*0.7f;
		pVtx[3].pos.z = 0.0f;
	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fieldWaitAlpha);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fieldWaitAlpha);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fieldWaitAlpha);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fieldWaitAlpha);

	if(GetBattleStats() == BATTLE_WAIT_SKILL_OR_TETRIS_PLAYER)
	{
		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 0.5f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.5f);
	}
	else
	{
		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.5f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.5f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}



	// 頂点データをアンロックする
	g_pVtxBuffFieldWait->Unlock();
}
//=============================================================================
// Field頂点の作成
//=============================================================================
HRESULT MakeVertexField(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * MAX_FIELD_BLOCK,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
		FVF_VERTEX_2D,						// 使用する頂点フォーマット
		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
		&g_pVtxBuffField,					// 頂点バッファインターフェースへのポインタ
		NULL)))								// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);

		for(int nCntField_Y = 0; nCntField_Y < MAX_FIELD_ROW; nCntField_Y++)
		{
			for(int nCntField_X = 0; nCntField_X < MAX_FIELD_COLUMN; nCntField_X++,pVtx += 4)
			{
				// 頂点座標の設定
				pVtx[0].pos.x = g_aField[nCntField_Y][nCntField_X].pos.x;
				pVtx[0].pos.y = g_aField[nCntField_Y][nCntField_X].pos.y - VALUE_BLOCK_SIZE;
				pVtx[0].pos.z = 0.0f;

				pVtx[1].pos.x = g_aField[nCntField_Y][nCntField_X].pos.x + VALUE_BLOCK_SIZE;
				pVtx[1].pos.y = g_aField[nCntField_Y][nCntField_X].pos.y - VALUE_BLOCK_SIZE;
				pVtx[1].pos.z = 0.0f;

				pVtx[2].pos.x = g_aField[nCntField_Y][nCntField_X].pos.x;
				pVtx[2].pos.y = g_aField[nCntField_Y][nCntField_X].pos.y;
				pVtx[2].pos.z = 0.0f;

				pVtx[3].pos.x = g_aField[nCntField_Y][nCntField_X].pos.x + VALUE_BLOCK_SIZE;
				pVtx[3].pos.y = g_aField[nCntField_Y][nCntField_X].pos.y;
				pVtx[3].pos.z = 0.0f;
				// rhwの設定
				pVtx[0].rhw =
					pVtx[1].rhw =
					pVtx[2].rhw =
					pVtx[3].rhw = 1.0f;

				// 頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2(g_aField[nCntField_Y][nCntField_X].nColor * (1.0f/BLOCK_MAX_COLOR), 0.0f);
				pVtx[1].tex = D3DXVECTOR2(g_aField[nCntField_Y][nCntField_X].nColor * (1.0f/BLOCK_MAX_COLOR)+(1.0f/BLOCK_MAX_COLOR), 0.0f);
				pVtx[2].tex = D3DXVECTOR2(g_aField[nCntField_Y][nCntField_X].nColor * (1.0f/BLOCK_MAX_COLOR), 0.5f);
				pVtx[3].tex = D3DXVECTOR2(g_aField[nCntField_Y][nCntField_X].nColor * (1.0f/BLOCK_MAX_COLOR)+(1.0f/BLOCK_MAX_COLOR), 0.5f);

			}
		}
		// 頂点データをアンロックする
		g_pVtxBuffField->Unlock();
	}
	return S_OK;
}
//=============================================================================
// Block頂点の作成
//=============================================================================
HRESULT MakeVertexBlock(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * MAX_BLOCK,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
		FVF_VERTEX_2D,						// 使用する頂点フォーマット
		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
		&g_pVtxBuffBlock,					// 頂点バッファインターフェースへのポインタ
		NULL)))								// NULLに設定
	{
		return E_FAIL;
	}

	{
		//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

		for(int nCnt_Block = 0; nCnt_Block < MAX_BLOCK;nCnt_Block ++,pVtx += 4)
		{

			// 頂点座標の設定
			pVtx[0].pos.x = g_aBlock[nCnt_Block].Pos.x;
			pVtx[0].pos.y = g_aBlock[nCnt_Block].Pos.y - VALUE_BLOCK_SIZE;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aBlock[nCnt_Block].Pos.x + VALUE_BLOCK_SIZE;
			pVtx[1].pos.y = g_aBlock[nCnt_Block].Pos.y - VALUE_BLOCK_SIZE;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aBlock[nCnt_Block].Pos.y;
			pVtx[2].pos.y = g_aBlock[nCnt_Block].Pos.y;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aBlock[nCnt_Block].Pos.y + VALUE_BLOCK_SIZE;
			pVtx[3].pos.y = g_aBlock[nCnt_Block].Pos.y;
			pVtx[3].pos.z = 0.0f;
			// rhwの設定
			pVtx[0].rhw =
				pVtx[1].rhw =
				pVtx[2].rhw =
				pVtx[3].rhw = 1.0f;

			// 頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			float TexY = 0.0f;
			if(g_aBlock[nCnt_Block].Type == BLOCK_SHADOW)
			 TexY = 0.5f;
			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(g_aBlock[nCnt_Block].nColor * (1.0f/BLOCK_MAX_COLOR), TexY+0.0f);
			pVtx[1].tex = D3DXVECTOR2(g_aBlock[nCnt_Block].nColor * (1.0f/BLOCK_MAX_COLOR)+(1.0f/BLOCK_MAX_COLOR), TexY+0.0f);
			pVtx[2].tex = D3DXVECTOR2(g_aBlock[nCnt_Block].nColor * (1.0f/BLOCK_MAX_COLOR), TexY+0.5f);
			pVtx[3].tex = D3DXVECTOR2(g_aBlock[nCnt_Block].nColor * (1.0f/BLOCK_MAX_COLOR)+(1.0f/BLOCK_MAX_COLOR), TexY+0.5f);
		}

		// 頂点データをアンロックする
		g_pVtxBuffBlock->Unlock();
		return S_OK;
	}
}
//=============================================================================
// Fieldの設定
//=============================================================================
void SetVerTexField(void)
{
	//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;
	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);

	for(int nCntField_Y = 0; nCntField_Y < MAX_FIELD_ROW; nCntField_Y++)
	{
		for(int nCntField_X = 0; nCntField_X < MAX_FIELD_COLUMN; nCntField_X++,pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].pos.x = g_aField[nCntField_Y][nCntField_X].pos.x;///* + g_FieldMove.x*/;
			pVtx[0].pos.y = g_aField[nCntField_Y][nCntField_X].pos.y - VALUE_BLOCK_SIZE;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aField[nCntField_Y][nCntField_X].pos.x/* + g_FieldMove.x*/ + VALUE_BLOCK_SIZE;
			pVtx[1].pos.y = g_aField[nCntField_Y][nCntField_X].pos.y - VALUE_BLOCK_SIZE;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aField[nCntField_Y][nCntField_X].pos.x;///* + g_FieldMove.x*/;
			pVtx[2].pos.y = g_aField[nCntField_Y][nCntField_X].pos.y;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aField[nCntField_Y][nCntField_X].pos.x/* + g_FieldMove.x*/ + VALUE_BLOCK_SIZE;
			pVtx[3].pos.y = g_aField[nCntField_Y][nCntField_X].pos.y;
			pVtx[3].pos.z = 0.0f;


			if(g_aField[nCntField_Y][nCntField_X].nType != FIELD_NONE)
				//continue;
			// テクスチャ座標の設定
			//pVtx += (EnemyID * 4);
			{
				pVtx[0].tex = D3DXVECTOR2(g_aField[nCntField_Y][nCntField_X].nColor * (1.0f/BLOCK_MAX_COLOR),							0.0f);
				pVtx[1].tex = D3DXVECTOR2(g_aField[nCntField_Y][nCntField_X].nColor * (1.0f/BLOCK_MAX_COLOR)+(1.0f/BLOCK_MAX_COLOR),	0.0f);
				pVtx[2].tex = D3DXVECTOR2(g_aField[nCntField_Y][nCntField_X].nColor * (1.0f/BLOCK_MAX_COLOR),							0.5f);
				pVtx[3].tex = D3DXVECTOR2(g_aField[nCntField_Y][nCntField_X].nColor * (1.0f/BLOCK_MAX_COLOR)+(1.0f/BLOCK_MAX_COLOR),	0.5f);
			}
			else
			{
				pVtx[0].tex = D3DXVECTOR2(BLOCK_WALL * (1.0f/BLOCK_MAX_COLOR),							0.5f);
				pVtx[1].tex = D3DXVECTOR2(BLOCK_WALL * (1.0f/BLOCK_MAX_COLOR)+(1.0f/BLOCK_MAX_COLOR),	0.5f);
				pVtx[2].tex = D3DXVECTOR2(BLOCK_WALL * (1.0f/BLOCK_MAX_COLOR),							1.0f);
				pVtx[3].tex = D3DXVECTOR2(BLOCK_WALL * (1.0f/BLOCK_MAX_COLOR)+(1.0f/BLOCK_MAX_COLOR),	1.0f);
			}

			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aField[nCntField_Y][nCntField_X].g_fAlpha);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aField[nCntField_Y][nCntField_X].g_fAlpha);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aField[nCntField_Y][nCntField_X].g_fAlpha);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aField[nCntField_Y][nCntField_X].g_fAlpha);

		}
	}
	// 頂点データをアンロックする
	g_pVtxBuffField->Unlock();        

}
//=============================================================================
// Blockの設定
//=============================================================================
void SetVerTexBlock(void)
{
	//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for(int nCnt_Block = 0; nCnt_Block < MAX_BLOCK;nCnt_Block ++,pVtx += 4)
	{
		if(g_aBlock[nCnt_Block].bUse == false)
			continue;
		//頂点座標設定
		pVtx[0].pos.x = g_aBlock[nCnt_Block].Pos.x;
		pVtx[0].pos.y = g_aBlock[nCnt_Block].Pos.y - VALUE_BLOCK_SIZE;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_aBlock[nCnt_Block].Pos.x + VALUE_BLOCK_SIZE;
		pVtx[1].pos.y = g_aBlock[nCnt_Block].Pos.y - VALUE_BLOCK_SIZE;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_aBlock[nCnt_Block].Pos.x;
		pVtx[2].pos.y = g_aBlock[nCnt_Block].Pos.y;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_aBlock[nCnt_Block].Pos.x + VALUE_BLOCK_SIZE;
		pVtx[3].pos.y = g_aBlock[nCnt_Block].Pos.y;
		pVtx[3].pos.z = 0.0f;

			float TexY = 0.0f;
			if(g_aBlock[nCnt_Block].Type == BLOCK_SHADOW)
			 TexY = 0.5f;
			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(g_aBlock[nCnt_Block].nColor * (1.0f/BLOCK_MAX_COLOR), TexY+0.0f);
			pVtx[1].tex = D3DXVECTOR2(g_aBlock[nCnt_Block].nColor * (1.0f/BLOCK_MAX_COLOR)+(1.0f/BLOCK_MAX_COLOR), TexY+0.0f);
			pVtx[2].tex = D3DXVECTOR2(g_aBlock[nCnt_Block].nColor * (1.0f/BLOCK_MAX_COLOR), TexY+0.5f);
			pVtx[3].tex = D3DXVECTOR2(g_aBlock[nCnt_Block].nColor * (1.0f/BLOCK_MAX_COLOR)+(1.0f/BLOCK_MAX_COLOR), TexY+0.5f);
	}
	// 頂点データをアンロックする
	g_pVtxBuffBlock->Unlock();
}

//=============================================================================
// 回転処理
//=============================================================================
void Rotation(int Group_ID,int Rot)
{
	bool bRot = true;
	GROUP_FORM NowForm = g_aGroup[Group_ID].Form;
	GROUP_FORM DestForm = NowForm;
	int DestPlaceX	= 0;
	int DestPlaceY	= 0;
	g_aGroup[Group_ID].Rotation++;
	g_aGroup[Group_ID].Rotation = (int)SetMAX((float)g_aGroup[Group_ID].Rotation,(float)g_aGroup[Group_ID].Form.Rot,0.0f,1);

	//*********************回転
	if(g_aGroup[Group_ID].Rotation == 0)
	{
		DestForm = g_aForm[g_aGroup[Group_ID].NumForm];
	}
	else
	{
		if(Rot == 0)
		for(int nCntPlace = 0; nCntPlace < NowForm.nCount; nCntPlace++)
		{
			DestForm.Place[nCntPlace].x = 0 * NowForm.Place[nCntPlace].x - 1 * NowForm.Place[nCntPlace].y;	
			DestForm.Place[nCntPlace].y	= 0 * NowForm.Place[nCntPlace].y + 1 * NowForm.Place[nCntPlace].x;
		}
		else
		for(int nCntPlace = 0; nCntPlace < NowForm.nCount; nCntPlace++)
		{
			DestForm.Place[nCntPlace].x = 0 * NowForm.Place[nCntPlace].x + 1 * NowForm.Place[nCntPlace].y;	
			DestForm.Place[nCntPlace].y	= 0 * NowForm.Place[nCntPlace].y - 1 * NowForm.Place[nCntPlace].x;
		}
	}
	//********************仮回転後のチェッ
	for(int nCntPlace = 0; nCntPlace < NowForm.nCount; nCntPlace++)
	{
		DestPlaceX = (int)DestForm.Place[nCntPlace].x + (int)g_aGroup[Group_ID].Place.x;
		DestPlaceY = (int)DestForm.Place[nCntPlace].y + (int)g_aGroup[Group_ID].Place.y;

		if(g_aField[DestPlaceY][DestPlaceX].nType != FIELD_NONE)
		{
			g_aGroup[Group_ID].Rotation--;
			bRot = false;
			break;
		}
	}
	//********************回転の反映
	if(bRot == true)
	{
		PlaySound(SOUND_LABEL_SE_BLOCK_ROT);
		g_aGroup[Group_ID].Form = DestForm;
		for(int nCntBlock = g_aGroup[Group_ID].B_start,nCntPlace = 0;
			nCntBlock <= g_aGroup[Group_ID].B_end;
			nCntBlock ++,nCntPlace ++)
		{
			g_aBlock[nCntBlock].FormPos = DestForm.Place[nCntPlace];
			g_aBlock[nCntBlock].Place = g_aGroup[GROUP_USING].Place + g_aBlock[nCntBlock].FormPos;
		}			
	}
}
//================================================================
//GrouFormの取得
//================================================================
GROUP_FORM GetGroupForm(int Form_ID)
{
	return g_aForm[Form_ID];
}
//================================================================
//残組の取得と設定
//================================================================
int	GetGroupLeft(void)
{
	return g_GroupLeft;
}

//================================================================
//すべてのブロックを初期化
//================================================================
void InitBlockAll(void)
{
	for(int nCntBlock = 0; nCntBlock < MAX_BLOCK;nCntBlock++)
	{
		g_aBlock[nCntBlock].Type	= BLOCK_NORMAL;
		g_aBlock[nCntBlock].bUse	= false;
		g_aBlock[nCntBlock].nColor	= BLOCK_MP;
		g_aBlock[nCntBlock].Place	= D3DXVECTOR2(0,0);
		g_aBlock[nCntBlock].Pos		= D3DXVECTOR2(0.0f,0.0f);
		g_aBlock[nCntBlock].FormPos	= D3DXVECTOR2(0,0);
	}
}
//================================================================
//入力可能の設定
//================================================================
void SetTetrisInput(bool input)
{
	g_bInput = input;
}
//================================================================
//状態設定
//================================================================
void SetTerisMainStats(int STATS)
{
	g_TetrisStats = STATS;
}
//================================================================
//状態の取得
//================================================================
int GetGameStats(void)
{
	return g_TetrisStats;
}
//================================================================
//Fieldの取得
//================================================================
FIELD *GetField(void)
{
	return &g_aField[0][0];
}
//================================================================
//Field色の変換
//================================================================
void ChangeFieldColor(int RecColor,int TargetColor)
{
	
	for(int nCntField_Y = 0; nCntField_Y < MAX_FIELD_ROW; nCntField_Y++)
	{
		for(int nCntField_X = 0; nCntField_X < MAX_FIELD_COLUMN; nCntField_X++)
		{

			if(nCntField_Y == 0 || nCntField_X == 0 || nCntField_X == MAX_FIELD_COLUMN - 1)
				continue;
			else
				if(g_aField[nCntField_Y][nCntField_X].nType == FIELD_USE &&
					g_aField[nCntField_Y][nCntField_X].nColor == RecColor)
				{
					g_aField[nCntField_Y][nCntField_X].nColor = TargetColor;
					
					D3DXVECTOR3 Epos = D3DXVECTOR3(0.0f,0.0f,0.0f);
					Epos.x = g_aField[nCntField_Y][nCntField_X].pos.x;
					Epos.y = g_aField[nCntField_Y][nCntField_X].pos.y;// + 15.0f;//15.0fは修正値
					int EColor = RecColor;
					if(EColor != BLOCK_MP)
					SetEffect_Broke(Epos,D3DXVECTOR3(0.0f,0.0f,0.0f),EColor); 
						
				}
		}
	}
}
