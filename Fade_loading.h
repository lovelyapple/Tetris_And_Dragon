//====================================================
//loading.h
//====================================================
#ifndef _FADE_LOADING_
#define _FADE_LOADING_
#include "main.h"
typedef enum
{
	LOADING_BLOCK_START = 0,
	LOADING_BLOCK_RUNING,
	LOADING_BLOCK_STOP,
	LOADING_BLOCK_WAIT,
	LOADING_BLOCK_OPEN,	
	LOADING_BLOCK_RESET,
	LOADING_BLOCK_NOTH,
	LOADING_BLOCK_PHASE_MAX,
}LOADING_PLHASE;

HRESULT InitFade_loading(void);
void UninitFade_loading(void);
void UpdateFade_loading(void);
void DrawFade_loading(void);

void SetLoadStart(MODE NextMode);
void SetLoadEnd(void);
#endif
//typedef enum
//{
//	LOADING_BLOCK_START = 0,
//	LOADING_BLOCK_CHARGE,
//	LOADING_BLOCK_EXCHANGE,
//	LOADING_BLOCK_CHANGE,
//	LOADING_BLOCK_OUT,
//	LOADING_BLOCK_RESET,
//	LOADING_BLOCK_STOP,
//	LOADING_BLOCK_PHASE_MAX,
//}LOADING_PLHASE;

	//switch(g_FadeLoadPhase)
	//{
	//case LOADING_BLOCK_START:
	//	g_TimeCount++;
	//	if(g_TimeCount%3 == 0)
	//	{
	//		PlaySound(SOUND_LABEL_SE_BLOCK_MOVE);
	//		g_LoadBlock[g_BlockUp][g_BlockLeft].bUse = true;
	//		g_LoadBlock[g_BlockDown][g_BlockRight].bUse = true;
	//		g_BlockLeft++;
	//		g_BlockRight--;
	//		if(g_BlockUp == 3 && g_BlockLeft >2)
	//		{
	//			g_FadeLoadPhase = LOADING_BLOCK_CHARGE;
	//		}
	//		if(g_BlockLeft > LOADING_BLOCK_COUNT_X)
	//		{
	//			g_BlockLeft = 0;
	//			g_BlockUp ++;

	//			g_BlockRight = LOADING_BLOCK_COUNT_X -1;
	//			g_BlockDown --;
	//		}
	//	}
	//	break;
	//case LOADING_BLOCK_CHARGE:
	//	g_TimeCount++;
	//	if(g_TimeCount%30 == 0)
	//	{
	//		g_LoadBlock[g_BlockUp][g_BlockLeft].bUse = true;
	//		PlaySound(SOUND_LABEL_SE_BLOCK_MOVE);
	//		g_BlockLeft++;
	//	}
	//	if(g_BlockLeft > 9)
	//		g_FadeLoadPhase = LOADING_BLOCK_STOP;
	//	break;
	//case LOADING_BLOCK_OUT:
	//	if(g_BlockOutTime == LOADING_BLOCK_OUTTIME)
	//		PlaySound(SOUND_LABEL_SE_ICE_SLASH);
	//	g_BlockOutTime --;
	//	for(int nCntBlockY = 0; nCntBlockY < LOADING_BLOCK_COUNT_Y;nCntBlockY ++)
	//	{
	//		for(int nCntBlockX = 0; nCntBlockX < LOADING_BLOCK_COUNT_X;nCntBlockX ++)
	//		{
	//			if(nCntBlockX<=5)
	//				g_LoadBlock[nCntBlockY][nCntBlockX].DrawPos.x -=20.0f;
	//			else
	//				g_LoadBlock[nCntBlockY][nCntBlockX].DrawPos.x +=20.0f;
	//		}
	//	}
	//	if(g_BlockOutTime < 0)
	//	{
	//		g_BlockOutTime = LOADING_BLOCK_OUTTIME;
	//		g_FadeLoadPhase = LOADING_BLOCK_RESET;
	//	}

	//	break;
	//case LOADING_BLOCK_RESET:
	//	InitFade_Loading_Block();
	//	g_FadeLoadPhase = LOADING_BLOCK_PHASE_MAX;
	//	break;
	//case LOADING_BLOCK_STOP:
	//	break;
	//case LOADING_BLOCK_PHASE_MAX:
	//	break;
	//}
