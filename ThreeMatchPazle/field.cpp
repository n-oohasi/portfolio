/*=====================================================================
|
|   �t�B�[���h�N���X[field.cpp]
|
|												�쐬�ҁF�勴�͌�
|												�쐬���F2024/09/02
======================================================================*/
#include"field.h"
#include"sprite.h"
#include "block.h"

/*******************************************************************************
*  �O���[�o���ϐ�
*******************************************************************************/
static ID3D11ShaderResourceView* g_Texture1 = NULL; // �e�N�X�`���\
static ID3D11ShaderResourceView* g_Texture2 = NULL; // �e�N�X�`���\
static bool g_alertFlg;
static float g_alertTime;
static float g_addTime = 0.02f;

/*******************************************************************************
*  ����������
*******************************************************************************/
void InitField(void)
{
	//�e�N�X�`���ǂݍ���
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
*  �I������
*******************************************************************************/
void UninitField(void)
{
	//�e�N�X�`���̊J��
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
*  �X�V����
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
*  �`�揈��
*******************************************************************************/
void DrawField(void)
{
	// �e�N�X�`���ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture1);

	// �X�v���C�g�`��
	DrawSpriteAlert(XMFLOAT2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2),
		0.0f, XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT), g_alertFlg, g_alertTime);

	// �e�N�X�`���ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture2);

	// �X�v���C�g�`��
	DrawSprite(XMFLOAT2(1060.0f, 200.0f),
		0.0f, XMFLOAT2(300.0f, 200.0f));
}