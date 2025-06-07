/*=====================================================================
|
|   �^�C�g���N���X[title.cpp]
|
|												�쐬�ҁF�勴�͌�
|												�쐬���F2024/09/02
======================================================================*/
#include"title.h"
#include"sprite.h"
#include "keyboard.h"
#include "fade.h"

/*******************************************************************************
*  �O���[�o���ϐ�
*******************************************************************************/
static ID3D11ShaderResourceView* g_Texture1 = NULL; // �e�N�X�`���\
static ID3D11ShaderResourceView* g_Texture2 = NULL; // �e�N�X�`���\
static ID3D11ShaderResourceView* g_Texture3 = NULL; // �e�N�X�`���\
static bool g_OldKeystateSpace = false; //�ߋ��̃L�[�̏��

/*******************************************************************************
*  ����������
*******************************************************************************/
void InitTitle(void)
{
	//�e�N�X�`���ǂݍ���
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
*  �I������
*******************************************************************************/
void UninitTitle(void)
{
	//�e�N�X�`���̉��
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
*  �X�V����
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
*  �`�揈��
*******************************************************************************/
void DrawTitle(void)
{
	// �e�N�X�`���ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture1);

	// �X�v���C�g�`��
	DrawSprite(XMFLOAT2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), 
				0.0f, XMFLOAT2( SCREEN_WIDTH, SCREEN_HEIGHT));
	// �e�N�X�`���ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture2);

	// �X�v���C�g�`��
	DrawSprite(XMFLOAT2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2),
		0.0f, XMFLOAT2(600.0f, 200.0f));
	// �e�N�X�`���ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture3);

	// �X�v���C�g�`��
	DrawSprite(XMFLOAT2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 240.0f),
		0.0f, XMFLOAT2(200.0f, 80.0f));
}