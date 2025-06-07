/*=====================================================================
|
|   �u���b�N�w�b�_�[[block.h]
|
|												�쐬�ҁF�勴�͌�
|												�쐬���F2024/09/02
======================================================================*/
#pragma once

#include "main.h"
#include"renderer.h"

//*****************************************************************************
// �\����
//*****************************************************************************
struct BLOCK
{
	XMFLOAT2 Position;	//�`��ʒu
	bool Enable;
	bool Erase;
	bool Ground;
	int Type;
};

//*****************************************************************************
// �v���g�^�C�v�錾
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




