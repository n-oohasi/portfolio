/*=====================================================================
|
|   �Q�[���Ǘ��N���X[game.cpp]
|
|												�쐬�ҁF�勴�͌�
|												�쐬���F2024/09/03
======================================================================*/
#include"game.h"
#include"sprite.h"
#include "field.h"
#include "piece.h"
#include "block.h"
#include "effect.h"
#include "cursor.h"
#include "score.h"
#include "sound.h"
#include "keyboard.h"

/*******************************************************************************
*  �O���[�o���ϐ�
*******************************************************************************/
static int g_BGMNo = 0;
static bool g_Pause;
static bool g_oldPause;

/*******************************************************************************
*  ����������
*******************************************************************************/
void InitGame(void)
{
	InitPiece();
	InitBlock();
	InitEffect();
	InitCursor();

	g_BGMNo = LoadSound((char*)"asset/sound/BGM.wav");

	SetVolume(g_BGMNo, 1.0f);
	PlaySound(g_BGMNo, -1);

	g_Pause = false;
	g_oldPause = false;
}

/*******************************************************************************
*  �I������
*******************************************************************************/
void UninitGame(void)
{
	UninitCursor();
	UninitEffect();
	UninitBlock();
	UninitPiece();
}

/*******************************************************************************
*  �X�V����
*******************************************************************************/
void UpdateGame(void)
{
	if (!g_oldPause) {
		if (Keyboard_IsKeyDown(KK_P)) {
			g_Pause = !g_Pause;
		}
	}

	if (!g_Pause) {
		UpdateField();
		UpdateBlock();
		UpdatePiece();
		UpdateEffect();
		UpdateCursor();
		UpdateScore();
	}

	g_oldPause = Keyboard_IsKeyDown(KK_P);
}

/*******************************************************************************
*  �`�揈��
*******************************************************************************/
void DrawGame(void)
{
	DrawField();
	if (!g_Pause) {
		DrawBlock();
	}
	DrawEffect();
	DrawCursor();
	DrawScore();
}

int GetBgmNo() {
	return g_BGMNo;
}

