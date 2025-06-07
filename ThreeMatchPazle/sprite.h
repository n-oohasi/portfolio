/*=====================================================================
|
|   �`��w�b�_�[[sprite.h]
|
|												�쐬�ҁF�勴�͌�
|												�쐬���F2024/09/02
======================================================================*/
#pragma once

#include "main.h"
#include"renderer.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitSprite(void);
void UninitSprite(void);
void DrawSprite(XMFLOAT2 Position, float Rotation, XMFLOAT2 Scale, float Alpha = 1.0f);
void DrawSpriteAlert(XMFLOAT2 Position, float Rotation, XMFLOAT2 Scale, bool alertFlg, float alertTime);
void DrawTopPiece(XMFLOAT2 Position, float Rotation, XMFLOAT2 Scale, float Move);
void DrawBottomPiece(XMFLOAT2 Position, float Rotation, XMFLOAT2 Scale, float Move);
void DrawSpriteAnim(XMFLOAT2 Position, float Rotation, XMFLOAT2 Scale, int AnimCols, int AnimRows, int AnimPattern, float Alpha = 1.0f);
void SetVertexSprite(void);