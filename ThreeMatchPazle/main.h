/*==============================================================================

   共通ヘッダー [main.h]
                                                         Author : 
                                                         Date   : 
--------------------------------------------------------------------------------

==============================================================================*/
#pragma once


#pragma warning(push)
#pragma warning(disable:4005)

#define _CRT_SECURE_NO_WARNINGS			// scanf のwarning防止
#include <stdio.h>

#include <d3d11.h>
#include <d3dcompiler.h>

#define DIRECTINPUT_VERSION 0x0800		// 警告対処
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
// マクロ定義
//*****************************************************************************
enum GAME_SCENE {
	SCENE_NONE,
	SCENE_TITLE,
	SCENE_GAME,
	SCENE_OVER,
	SCENE_RESULT
};

#define SCREEN_WIDTH	(1280)				// ウインドウの幅
#define SCREEN_HEIGHT	(720)				// ウインドウの高さ

#define FIELD_LEFT		(490)				// フィールドの左端
#define FIELD_RIGHT		(790)				// フィールドの右端
#define FIELD_TOP		(60)				// フィールドの上端
#define FIELD_BOTTOM	(660)				// フィールドの下端

#define PIECE_WIDTH		(50)				// ピースの幅
#define PIECE_HEIGHT	(50)				// ピースの高さ

#define BLOCK_COLS		(6)					// ブロック列数
#define BLOCK_ROWS		(14)					// ブロック行数

#define MOVE_SPEED		(0.3f)				//ブロックの上昇速度
#define DROP_SPEED		(1.0f)				//ブロックの落下速度

#define CURSOR_WIDTH	(100)				// カーソルの幅
#define CURSOR_HEIGHT	(50)				// カーソルの高さ

#define TEX_COUNT	(5)				// テクスチャーの種類


void SetSCENE(GAME_SCENE scene);
GAME_SCENE GetGameScene();
