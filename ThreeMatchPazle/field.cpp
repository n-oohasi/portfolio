/*=====================================================================
|
|   フィールドクラス[field.cpp]
|
|												作成者：大橋範顕
|												作成日：2024/09/02
======================================================================*/
#include"field.h"
#include"sprite.h"
#include "block.h"

/*******************************************************************************
*  グローバル変数
*******************************************************************************/
static ID3D11ShaderResourceView* g_Texture1 = NULL; // テクスチャ―
static ID3D11ShaderResourceView* g_Texture2 = NULL; // テクスチャ―
static bool g_alertFlg;
static float g_alertTime;
static float g_addTime = 0.02f;

/*******************************************************************************
*  初期化処理
*******************************************************************************/
void InitField(void)
{
	//テクスチャ読み込み
	TexMetadata metadata;
	ScratchImage image;
	LoadFromWICFile(L"asset\\texture\\BG.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &g_Texture1);
	assert(g_Texture1);
	LoadFromWICFile(L"asset\\texture\\manual2.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &g_Texture2);
	assert(g_Texture2);

	g_alertFlg = false;
	g_alertTime = 0.0f;
}

/*******************************************************************************
*  終了処理
*******************************************************************************/
void UninitField(void)
{
	//テクスチャの開放
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
}

/*******************************************************************************
*  更新処理
*******************************************************************************/
void UpdateField(void)
{
	g_alertFlg = GetAlertFlg();

	if (g_alertFlg) {
		g_alertTime += g_addTime;
		if (g_alertTime < 0.0f) {
			g_alertTime = 0.0f;
			g_addTime *= -1.0f;
		}
		else if (g_alertTime >= 0.7f) {
			g_alertTime = 0.7f;
			g_addTime *= -1.0f;
		}
	}
	else {
		g_alertTime = 0.0f;
	}
}

/*******************************************************************************
*  描画処理
*******************************************************************************/
void DrawField(void)
{
	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture1);

	// スプライト描画
	DrawSpriteAlert(XMFLOAT2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2),
		0.0f, XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT), g_alertFlg, g_alertTime);

	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture2);

	// スプライト描画
	DrawSprite(XMFLOAT2(1060.0f, 200.0f),
		0.0f, XMFLOAT2(300.0f, 200.0f));
}