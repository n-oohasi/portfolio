#include"sprite.h"
#include"effect.h"
#include "sound.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
struct EFFECT
{
	bool Enable;
	XMFLOAT2 Position;
	int FrameCount;
};
#define EFFECT_MAX 100

/*******************************************************************************
*  グローバル変数
*******************************************************************************/
static EFFECT g_Effect[EFFECT_MAX];
static ID3D11ShaderResourceView* g_Texture = {}; // テクスチャ―
static int g_SENo = 0;


void InitEffect(void)
{
	//テクスチャ読み込み
	TexMetadata metadata;
	ScratchImage image;
	LoadFromWICFile(L"asset\\texture\\bubble.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &g_Texture);
	assert(g_Texture);

	//エフェクト配列初期化
	for (int i = 0; i < EFFECT_MAX; i++)
	{
		g_Effect[i].Enable = false;
	}

	g_SENo = LoadSound((char*)"asset/sound/bubble.wav");
}

void UninitEffect(void)
{
	g_Texture->Release();
}

void UpdateEffect(void)
{
	for (int i = 0; i < EFFECT_MAX; i++)
	{
		if (g_Effect[i].Enable)
		{
			g_Effect[i].FrameCount++;

			if(g_Effect[i].FrameCount > 60)
			{
				g_Effect[i].Enable = false;
			}
		}
	}
}

void DrawEffect(void)
{
	//テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture);

	for (int i = 0; i < EFFECT_MAX; i++)
	{
		if (g_Effect[i].Enable)
		{
			DrawSpriteAnim(g_Effect[i].Position, 0.0f, XMFLOAT2(PIECE_WIDTH, PIECE_HEIGHT * 2.0f),
							4,2,g_Effect[i].FrameCount / 8);
		}
	}
}

void CreateEffect(XMFLOAT2 Position)
{
	for (int i = 0; i < EFFECT_MAX; i++)
	{
		if(g_Effect[i].Enable == false)
		{
			SetVolume(g_SENo, 4.0f);
			PlaySound(g_SENo, 0);
			g_Effect[i].Enable = true;
			g_Effect[i].Position = Position;
			g_Effect[i].FrameCount = 0;

			break;
		}
	}
}