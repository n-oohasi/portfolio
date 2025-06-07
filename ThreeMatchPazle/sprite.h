/*=====================================================================
|
|   描画ヘッダー[sprite.h]
|
|												作成者：大橋範顕
|												作成日：2024/09/02
======================================================================*/
#pragma once

#include "main.h"
#include"renderer.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitSprite(void);
void UninitSprite(void);
void DrawSprite(XMFLOAT2 Position, float Rotation, XMFLOAT2 Scale, float Alpha = 1.0f);
void DrawSpriteAlert(XMFLOAT2 Position, float Rotation, XMFLOAT2 Scale, bool alertFlg, float alertTime);
void DrawTopPiece(XMFLOAT2 Position, float Rotation, XMFLOAT2 Scale, float Move);
void DrawBottomPiece(XMFLOAT2 Position, float Rotation, XMFLOAT2 Scale, float Move);
void DrawSpriteAnim(XMFLOAT2 Position, float Rotation, XMFLOAT2 Scale, int AnimCols, int AnimRows, int AnimPattern, float Alpha = 1.0f);
void SetVertexSprite(void);