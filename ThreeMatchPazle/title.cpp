/*=====================================================================
|
|   タイトルクラス[title.cpp]
|
|												作成者：大橋範顕
|												作成日：2024/09/02
======================================================================*/
#include"title.h"
#include"sprite.h"
#include "keyboard.h"
#include "fade.h"

/*******************************************************************************
*  グローバル変数
*******************************************************************************/
static ID3D11ShaderResourceView* g_Texture1 = NULL; // テクスチャ―
static ID3D11ShaderResourceView* g_Texture2 = NULL; // テクスチャ―
static ID3D11ShaderResourceView* g_Texture3 = NULL; // テクスチャ―
static bool g_OldKeystateSpace = false; //過去のキーの状態

/*******************************************************************************
*  初期化処理
*******************************************************************************/
void InitTitle(void)
{
	//テクスチャ読み込み
	TexMetadata metadata;
	ScratchImage image;
	LoadFromWICFile(L"asset\\texture\\title.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &g_Texture1);
	assert(g_Texture1);
	LoadFromWICFile(L"asset\\texture\\logo.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &g_Texture2);
	assert(g_Texture2);
	LoadFromWICFile(L"asset\\texture\\manual1.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &g_Texture3);
	assert(g_Texture3);

	g_OldKeystateSpace = Keyboard_IsKeyDown(KK_SPACE);
}

/*******************************************************************************
*  終了処理
*******************************************************************************/
void UninitTitle(void)
{
	//テクスチャの解放
	if (g_Texture1)
	{
		g_Texture1->Release();
		g_Texture1 = NULL;
	}
	if (g_Texture2)
	{
		g_Texture2->Release();
		g_Texture2 = NULL;
	}
	if (g_Texture3)
	{
		g_Texture3->Release();
		g_Texture3 = NULL;
	}
}

/*******************************************************************************
*  更新処理
*******************************************************************************/
void UpdateTitle(void)
{
	bool keystateSpace = Keyboard_IsKeyDown(KK_SPACE);
	if (!g_OldKeystateSpace && keystateSpace) {
		FadeOut();
	}
	if (IsFadeOutFinish()) {
		SetSCENE(SCENE_GAME);
	}
	g_OldKeystateSpace = keystateSpace;
}

/*******************************************************************************
*  描画処理
*******************************************************************************/
void DrawTitle(void)
{
	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture1);

	// スプライト描画
	DrawSprite(XMFLOAT2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), 
				0.0f, XMFLOAT2( SCREEN_WIDTH, SCREEN_HEIGHT));
	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture2);

	// スプライト描画
	DrawSprite(XMFLOAT2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2),
		0.0f, XMFLOAT2(600.0f, 200.0f));
	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture3);

	// スプライト描画
	DrawSprite(XMFLOAT2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 240.0f),
		0.0f, XMFLOAT2(200.0f, 80.0f));
}