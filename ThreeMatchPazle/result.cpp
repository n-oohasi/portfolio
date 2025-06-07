/*=====================================================================
|
|   リザルトクラス[result.cpp]
|
|												作成者：大橋範顕
|												作成日：2024/09/11
======================================================================*/
#include"cursor.h"
#include "renderer.h"
#include "sprite.h"
#include "keyboard.h"
#include "block.h"
#include "score.h"

/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/
static ID3D11ShaderResourceView* g_Texture1 = { NULL }; // テクスチャ―
static ID3D11ShaderResourceView* g_Texture2 = { NULL }; // テクスチャ―
static ID3D11ShaderResourceView* g_Texture3 = { NULL }; // テクスチャ―
static ID3D11ShaderResourceView* g_Texture4 = { NULL }; // テクスチャ―
static ID3D11ShaderResourceView* g_Texture5 = { NULL }; // テクスチャ―

/*******************************************************************************
*  グローバル変数
*******************************************************************************/
static int g_score;

static bool g_OldKeystateSpace = false; //過去のキーの状態


/*******************************************************************************
*  初期化処理
*******************************************************************************/
void InitResult(void)
{
	//テクスチャ読み込み
	TexMetadata metadata;
	ScratchImage image;

	LoadFromWICFile(L"asset\\texture\\number.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &g_Texture1);
	assert(g_Texture1);

	LoadFromWICFile(L"asset\\texture\\result.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &g_Texture2);
	assert(g_Texture2);

	LoadFromWICFile(L"asset\\texture\\biku.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &g_Texture3);
	assert(g_Texture3);

	LoadFromWICFile(L"asset\\texture\\catch.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &g_Texture4);
	assert(g_Texture4);

	LoadFromWICFile(L"asset\\texture\\tani.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &g_Texture5);
	assert(g_Texture5);

	g_score = GetScore();
	g_OldKeystateSpace = Keyboard_IsKeyDown(KK_SPACE);
}

/*******************************************************************************
*  終了処理
*******************************************************************************/
void UninitResult(void)
{
	//テクスチャの解放
	if (g_Texture1)
	{
		g_Texture1->Release();
		g_Texture1 = { NULL };
	}
	if (g_Texture2)
	{
		g_Texture2->Release();
		g_Texture2 = { NULL };
	}
	if (g_Texture3)
	{
		g_Texture3->Release();
		g_Texture3 = { NULL };
	}
	if (g_Texture4)
	{
		g_Texture4->Release();
		g_Texture4 = { NULL };
	}
	if (g_Texture5)
	{
		g_Texture5->Release();
		g_Texture5 = { NULL };
	}
}

/*******************************************************************************
*  更新処理
*******************************************************************************/
void UpdateResult(void)
{
	bool keystateSpace = Keyboard_IsKeyDown(KK_SPACE);
	if (!g_OldKeystateSpace && keystateSpace) {
		SetSCENE(SCENE_TITLE);
	}
	g_OldKeystateSpace = keystateSpace;
}

/*******************************************************************************
*  描画処理
*******************************************************************************/
void DrawResult(void)
{
	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture2);

	// スプライト描画
	DrawSprite(XMFLOAT2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2),
		0.0f, XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT));

	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture1);

	int score = g_score;
	for (int i = 0; i < 3; i++) {
		DrawSpriteAnim(XMFLOAT2(SCREEN_WIDTH / 2 + 10.0f - 60.0f * i, SCREEN_HEIGHT / 2), 0.0f, XMFLOAT2(100.0f, 100.0f), 5, 5, (score % 10));

		score /= 10;
	}

	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture3);

	// スプライト描画
	DrawSprite(XMFLOAT2(320.0f, 540.0f),
		0.0f, XMFLOAT2(300.0f, 210.0f));

	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture4);

	// スプライト描画
	DrawSprite(XMFLOAT2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 100.0f),
		0.0f, XMFLOAT2(300.0f, 60.0f));

	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture5);

	// スプライト描画
	DrawSprite(XMFLOAT2(SCREEN_WIDTH / 2 + 100.0f, SCREEN_HEIGHT / 2 + 20.0f),
		0.0f, XMFLOAT2(80.0f, 40.0f));
}

