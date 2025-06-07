/*=====================================================================
|
|   フィールドクラス[fade.cpp]
|
|												作成者：大橋範顕
|												作成日：2024/09/02
======================================================================*/
#include"fade.h"
#include"sprite.h"

/*******************************************************************************
*  グローバル変数
*******************************************************************************/
static ID3D11ShaderResourceView* g_Texture = NULL; // テクスチャ―
enum FADE {
	FADE_NONE,
	FADE_OUT,
	FADE_IN
};

static FADE g_fade;
static float g_fadeAlpha;
static bool g_fadeOutFinish;

/*******************************************************************************
*  初期化処理
*******************************************************************************/
void InitFade(void)
{
	//テクスチャ読み込み
	TexMetadata metadata;
	ScratchImage image;
	LoadFromWICFile(L"asset\\texture\\black.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &g_Texture);
	assert(g_Texture);

	g_fade = FADE_NONE;
	g_fadeAlpha = 0.0f;
	g_fadeOutFinish = false;
}

/*******************************************************************************
*  終了処理
*******************************************************************************/
void UninitFade(void)
{
	//テクスチャの開放
	if (g_Texture)
	{
		g_Texture->Release();
		g_Texture = NULL;
	}
}

/*******************************************************************************
*  更新処理
*******************************************************************************/
void UpdateFade(void)
{
	if (g_fade == FADE_OUT) {
		g_fadeAlpha += 0.02f;
		if (g_fadeAlpha > 1.0f) {
			g_fadeAlpha = 1.0f;
			g_fade = FADE_IN;
			g_fadeOutFinish = true;
		}
	}
	else if (g_fade == FADE_IN) {
		g_fadeOutFinish = false;

		g_fadeAlpha -= 0.02f;
		if (g_fadeAlpha < 0.0f) {
			g_fadeAlpha = 0.0f;
			g_fade = FADE_NONE;
		}
	}
}

/*******************************************************************************
*  描画処理
*******************************************************************************/
void DrawFade(void)
{
	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture);

	// スプライト描画
	DrawSprite(XMFLOAT2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2),
		0.0f, XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT), g_fadeAlpha);
}

void FadeOut(void) {
	if (g_fade == FADE_NONE) {
		g_fade = FADE_OUT;
		g_fadeOutFinish = false;
	}
}

bool IsFadeOutFinish(void) {
	return g_fadeOutFinish;
}