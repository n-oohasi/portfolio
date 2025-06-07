/*=====================================================================
|
|   ���U���g�N���X[result.cpp]
|
|												�쐬�ҁF�勴�͌�
|												�쐬���F2024/09/11
======================================================================*/
#include"cursor.h"
#include "renderer.h"
#include "sprite.h"
#include "keyboard.h"
#include "block.h"
#include "score.h"

/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/
static ID3D11ShaderResourceView* g_Texture1 = { NULL }; // �e�N�X�`���\
static ID3D11ShaderResourceView* g_Texture2 = { NULL }; // �e�N�X�`���\
static ID3D11ShaderResourceView* g_Texture3 = { NULL }; // �e�N�X�`���\
static ID3D11ShaderResourceView* g_Texture4 = { NULL }; // �e�N�X�`���\
static ID3D11ShaderResourceView* g_Texture5 = { NULL }; // �e�N�X�`���\

/*******************************************************************************
*  �O���[�o���ϐ�
*******************************************************************************/
static int g_score;

static bool g_OldKeystateSpace = false; //�ߋ��̃L�[�̏��


/*******************************************************************************
*  ����������
*******************************************************************************/
void InitResult(void)
{
	//�e�N�X�`���ǂݍ���
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
*  �I������
*******************************************************************************/
void UninitResult(void)
{
	//�e�N�X�`���̉��
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
*  �X�V����
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
*  �`�揈��
*******************************************************************************/
void DrawResult(void)
{
	// �e�N�X�`���ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture2);

	// �X�v���C�g�`��
	DrawSprite(XMFLOAT2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2),
		0.0f, XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT));

	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture1);

	int score = g_score;
	for (int i = 0; i < 3; i++) {
		DrawSpriteAnim(XMFLOAT2(SCREEN_WIDTH / 2 + 10.0f - 60.0f * i, SCREEN_HEIGHT / 2), 0.0f, XMFLOAT2(100.0f, 100.0f), 5, 5, (score % 10));

		score /= 10;
	}

	// �e�N�X�`���ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture3);

	// �X�v���C�g�`��
	DrawSprite(XMFLOAT2(320.0f, 540.0f),
		0.0f, XMFLOAT2(300.0f, 210.0f));

	// �e�N�X�`���ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture4);

	// �X�v���C�g�`��
	DrawSprite(XMFLOAT2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 100.0f),
		0.0f, XMFLOAT2(300.0f, 60.0f));

	// �e�N�X�`���ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture5);

	// �X�v���C�g�`��
	DrawSprite(XMFLOAT2(SCREEN_WIDTH / 2 + 100.0f, SCREEN_HEIGHT / 2 + 20.0f),
		0.0f, XMFLOAT2(80.0f, 40.0f));
}

