/*=====================================================================
|
|   ランキングクラス[ranking.cpp]
|
|												作成者：大橋範顕
|												作成日：2024/09/03
======================================================================*/
#include"ranking.h"
#include "renderer.h"
#include "sprite.h"

/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/
static ID3D11ShaderResourceView* g_Texture = { NULL }; // テクスチャ―

/*******************************************************************************
*  定数
*******************************************************************************/
const int c_scoreCount = 4;

/*******************************************************************************
*  グローバル変数
*******************************************************************************/
static int g_score[c_scoreCount] = {0};

/*******************************************************************************
*  初期化処理
*******************************************************************************/
void InitRanking(void)
{
	//テクスチャ読み込み
	TexMetadata metadata;
	ScratchImage image;

	LoadFromWICFile(L"asset\\texture\\number.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &g_Texture);
	assert(g_Texture);
}

/*******************************************************************************
*  終了処理
*******************************************************************************/
void UninitRanking(void)
{
	//テクスチャの解放
	if (g_Texture)
	{
		g_Texture->Release();
		g_Texture = { NULL };
	}
}

/*******************************************************************************
*  更新処理
*******************************************************************************/
void UpdateRanking(void)
{

}

/*******************************************************************************
*  描画処理
*******************************************************************************/
void DrawRanking(void)
{
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture);

	for (int j = 0; j < c_scoreCount - 1; j++) {
		int score = g_score[j];
		for (int i = 0; i < 3; i++) {
			DrawSpriteAnim(XMFLOAT2(200.0f - 60.0f * i, 200.0f + j * 80.0f), 0.0f, XMFLOAT2(100.0f, 100.0f), 5, 5, (score % 10));

			score /= 10;
		}
	}
}


void AddRanking(int score) {
	g_score[c_scoreCount - 1] = score;

	for (int j = 0; j < c_scoreCount; j++) {
		for (int i = j + 1; i < c_scoreCount; i++) {
			if (g_score[j] <= g_score[i]) {
				int backUp = g_score[j];
				g_score[j] = g_score[i];
				g_score[i] = backUp;
			}
		}
	}
}

