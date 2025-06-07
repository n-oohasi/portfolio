/*=====================================================================
|
|   ブロックヘッダー[block.h]
|
|												作成者：大橋範顕
|												作成日：2024/09/02
======================================================================*/
#pragma once

#include "main.h"
#include"renderer.h"

//*****************************************************************************
// 構造体
//*****************************************************************************
struct BLOCK
{
	XMFLOAT2 Position;	//描画位置
	bool Enable;
	bool Erase;
	bool Ground;
	int Type;
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitBlock(void);
void UninitBlock(void);
void UpdateBlock(void);
void DrawBlock(void);
void SetBlock(int x, int y, int Type);
BLOCK GetBlock(int x, int y);
void EraseBlock(void);
void ChangeBlock(int x, int y);
bool GetAlertFlg();
float GetMoveDistance();




