/*=====================================================================
|
|   �J�[�\���N���X[cursor.cpp]
|
|												�쐬�ҁF�勴�͌�
|												�쐬���F2024/09/03
======================================================================*/
#include"cursor.h"
#include "renderer.h"
#include "sprite.h"
#include "keyboard.h"
#include "block.h"

/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/
static ID3D11ShaderResourceView* g_Texture = { NULL }; // �e�N�X�`���\

/*******************************************************************************
*  �O���[�o���ϐ�
*******************************************************************************/
static XMFLOAT2 g_position;

static bool g_OldKeystateLeft = false; //�ߋ��̃L�[�̏��
static bool g_OldKeystateRight = false; //�ߋ��̃L�[�̏��
static bool g_OldKeystateUp = false; //�ߋ��̃L�[�̏��
static bool g_OldKeystateDown = false; //�ߋ��̃L�[�̏��
static bool g_OldKeystateSpace = false; //�ߋ��̃L�[�̏��


/*******************************************************************************
*  ����������
*******************************************************************************/
void InitCursor(void)
{
	//�e�N�X�`���ǂݍ���
	TexMetadata metadata;
	ScratchImage image;

	LoadFromWICFile(L"asset\\texture\\cursor.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &g_Texture);
	assert(g_Texture);

	g_position = XMFLOAT2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
}

/*******************************************************************************
*  �I������
*******************************************************************************/
void UninitCursor(void)
{
	//�e�N�X�`���̉��
	if (g_Texture)
	{
		g_Texture->Release();
		g_Texture = { NULL };
	}
}

/*******************************************************************************
*  �X�V����
*******************************************************************************/
void UpdateCursor(void)
{
	//���ړ�
	bool keystateLeft = Keyboard_IsKeyDown(KK_LEFT);
	if (!g_OldKeystateLeft && keystateLeft)
	{
		g_position.x -= PIECE_WIDTH;

		if (g_position.x <= FIELD_LEFT) {
			g_position.x += PIECE_WIDTH;
		}
	}
	g_OldKeystateLeft = keystateLeft;

	//�E�ړ�
	bool keystateRight = Keyboard_IsKeyDown(KK_RIGHT);
	if (!g_OldKeystateRight && keystateRight)
	{
		g_position.x += PIECE_WIDTH;

		if (g_position.x >= FIELD_RIGHT) {
			g_position.x -= PIECE_WIDTH;
		}
	}
	g_OldKeystateRight = keystateRight;

	//��ړ�
	bool keystateUp = Keyboard_IsKeyDown(KK_UP);
	if (!g_OldKeystateUp && keystateUp)
	{
		g_position.y -= PIECE_HEIGHT;

		if (g_position.y < FIELD_TOP) {
			g_position.y += PIECE_HEIGHT;
		}
	}
	g_OldKeystateUp = keystateUp;

	//���ړ�
	bool keystateDown = Keyboard_IsKeyDown(KK_DOWN);
	if (!g_OldKeystateDown && keystateDown)
	{
		g_position.y += PIECE_HEIGHT;

		if (g_position.y >= FIELD_BOTTOM + 50) {
			g_position.y -= PIECE_HEIGHT;
		}
	}
	g_OldKeystateDown = keystateDown;

	g_position.y -= MOVE_SPEED;

	if (g_position.y < FIELD_TOP) {
		g_position.y += PIECE_HEIGHT;
	}

	//�s�[�X����ւ�
	bool keystateSpace = Keyboard_IsKeyDown(KK_SPACE);
	if (!g_OldKeystateSpace && keystateSpace)
	{
		int x = (g_position.x - FIELD_LEFT) / PIECE_WIDTH - 1;
		int y = (g_position.y - FIELD_TOP) / PIECE_HEIGHT + 1;

		ChangeBlock(x, y);
	}
	g_OldKeystateSpace = keystateSpace;
}

/*******************************************************************************
*  �`�揈��
*******************************************************************************/
void DrawCursor(void)
{
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture);

	DrawSprite(g_position, 0.0f, XMFLOAT2(CURSOR_WIDTH, CURSOR_HEIGHT));
}

