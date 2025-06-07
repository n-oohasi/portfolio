/*=====================================================================
|
|   �t�B�[���h�N���X[fade.cpp]
|
|												�쐬�ҁF�勴�͌�
|												�쐬���F2024/09/02
======================================================================*/
#include"fade.h"
#include"sprite.h"

/*******************************************************************************
*  �O���[�o���ϐ�
*******************************************************************************/
static ID3D11ShaderResourceView* g_Texture = NULL; // �e�N�X�`���\
enum FADE {
	FADE_NONE,
	FADE_OUT,
	FADE_IN
};

static FADE g_fade;
static float g_fadeAlpha;
static bool g_fadeOutFinish;

/*******************************************************************************
*  ����������
*******************************************************************************/
void InitFade(void)
{
	//�e�N�X�`���ǂݍ���
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
*  �I������
*******************************************************************************/
void UninitFade(void)
{
	//�e�N�X�`���̊J��
	if (g_Texture)
	{
		g_Texture->Release();
		g_Texture = NULL;
	}
}

/*******************************************************************************
*  �X�V����
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
*  �`�揈��
*******************************************************************************/
void DrawFade(void)
{
	// �e�N�X�`���ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture);

	// �X�v���C�g�`��
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