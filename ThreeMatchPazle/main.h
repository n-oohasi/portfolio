/*==============================================================================

   ���ʃw�b�_�[ [main.h]
                                                         Author : 
                                                         Date   : 
--------------------------------------------------------------------------------

==============================================================================*/
#pragma once


#pragma warning(push)
#pragma warning(disable:4005)

#define _CRT_SECURE_NO_WARNINGS			// scanf ��warning�h�~
#include <stdio.h>

#include <d3d11.h>
#include <d3dcompiler.h>

#define DIRECTINPUT_VERSION 0x0800		// �x���Ώ�
#include "dinput.h"
#include "mmsystem.h"

#pragma warning(pop)



#include <DirectXMath.h>
using namespace DirectX;

#include"DirectXTex.h"

#if _DEBUG
#pragma comment(lib,"DirectXTex_Debug.lib")
#else
#pragma comment(lib, "DirectXTex_Release.lib")
#endif

//*****************************************************************************
// �}�N����`
//*****************************************************************************
enum GAME_SCENE {
	SCENE_NONE,
	SCENE_TITLE,
	SCENE_GAME,
	SCENE_OVER,
	SCENE_RESULT
};

#define SCREEN_WIDTH	(1280)				// �E�C���h�E�̕�
#define SCREEN_HEIGHT	(720)				// �E�C���h�E�̍���

#define FIELD_LEFT		(490)				// �t�B�[���h�̍��[
#define FIELD_RIGHT		(790)				// �t�B�[���h�̉E�[
#define FIELD_TOP		(60)				// �t�B�[���h�̏�[
#define FIELD_BOTTOM	(660)				// �t�B�[���h�̉��[

#define PIECE_WIDTH		(50)				// �s�[�X�̕�
#define PIECE_HEIGHT	(50)				// �s�[�X�̍���

#define BLOCK_COLS		(6)					// �u���b�N��
#define BLOCK_ROWS		(14)					// �u���b�N�s��

#define MOVE_SPEED		(0.3f)				//�u���b�N�̏㏸���x
#define DROP_SPEED		(1.0f)				//�u���b�N�̗������x

#define CURSOR_WIDTH	(100)				// �J�[�\���̕�
#define CURSOR_HEIGHT	(50)				// �J�[�\���̍���

#define TEX_COUNT	(5)				// �e�N�X�`���[�̎��


void SetSCENE(GAME_SCENE scene);
GAME_SCENE GetGameScene();
