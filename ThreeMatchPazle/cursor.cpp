/*=====================================================================
|
|   カーソルクラス[cursor.cpp]
|
|												作成者：大橋範顕
|												作成日：2024/09/03
======================================================================*/
#include"cursor.h"
#include "renderer.h"
#include "sprite.h"
#include "keyboard.h"
#include "block.h"

/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/
static ID3D11ShaderResourceView* g_Texture = { NULL }; // テクスチャ―

/*******************************************************************************
*  グローバル変数
*******************************************************************************/
static XMFLOAT2 g_position;

static bool g_OldKeystateLeft = false; //過去のキーの状態
static bool g_OldKeystateRight = false; //過去のキーの状態
static bool g_OldKeystateUp = false; //過去のキーの状態
static bool g_OldKeystateDown = false; //過去のキーの状態
static bool g_OldKeystateSpace = false; //過去のキーの状態


/*******************************************************************************
*  初期化処理
*******************************************************************************/
void InitCursor(void)
{
	//テクスチャ読み込み
	TexMetadata metadata;
	ScratchImage image;

	LoadFromWICFile(L"asset\\texture\\cursor.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &g_Texture);
	assert(g_Texture);

	g_position = XMFLOAT2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
}

/*******************************************************************************
*  終了処理
*******************************************************************************/
void UninitCursor(void)
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
void UpdateCursor(void)
{
	//左移動
	bool keystateLeft = Keyboard_IsKeyDown(KK_LEFT);
	if (!g_OldKeystateLeft && keystateLeft)
	{
		g_position.x -= PIECE_WIDTH;

		if (g_position.x <= FIELD_LEFT) {
			g_position.x += PIECE_WIDTH;
		}
	}
	g_OldKeystateLeft = keystateLeft;

	//右移動
	bool keystateRight = Keyboard_IsKeyDown(KK_RIGHT);
	if (!g_OldKeystateRight && keystateRight)
	{
		g_position.x += PIECE_WIDTH;

		if (g_position.x >= FIELD_RIGHT) {
			g_position.x -= PIECE_WIDTH;
		}
	}
	g_OldKeystateRight = keystateRight;

	//上移動
	bool keystateUp = Keyboard_IsKeyDown(KK_UP);
	if (!g_OldKeystateUp && keystateUp)
	{
		g_position.y -= PIECE_HEIGHT;

		if (g_position.y < FIELD_TOP) {
			g_position.y += PIECE_HEIGHT;
		}
	}
	g_OldKeystateUp = keystateUp;

	//下移動
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

	//ピース入れ替え
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
*  描画処理
*******************************************************************************/
void DrawCursor(void)
{
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture);

	DrawSprite(g_position, 0.0f, XMFLOAT2(CURSOR_WIDTH, CURSOR_HEIGHT));
}

