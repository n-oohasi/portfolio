/*=====================================================================
|
|   ゲームオーバークラス[over.cpp]
|
|												作成者：大橋範顕
|												作成日：2024/09/09
======================================================================*/

#include "over.h"
#include "block.h"
#include "sprite.h"
#include "field.h"
#include "score.h"
#include "keyboard.h"



//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define OVER_DROP_SPEED (0.3f)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void OverDown(int x, bool checkFlg = false);
void OverUp(int x, bool checkFlg = false);

/*******************************************************************************
* グローバル変数
*******************************************************************************/
static BLOCK g_Block[BLOCK_ROWS * 2][BLOCK_COLS];

static ID3D11ShaderResourceView* g_Texture[TEX_COUNT] = { NULL }; // テクスチャ―

static int g_dropCount[BLOCK_COLS];

static float g_moveDistance[BLOCK_COLS];

static bool g_dropStartFlg[BLOCK_COLS];
static bool g_dropFlgOld[BLOCK_COLS];
static float g_dropSpeed[BLOCK_COLS];

static int g_startTime;
static int g_startCount;

/*******************************************************************************
*  初期化処理
*******************************************************************************/
void InitOver(void) {
	//テクスチャ読み込み
	TexMetadata metadata;
	ScratchImage image;

	LoadFromWICFile(L"asset\\texture\\jellyfish.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &g_Texture[0]);


	LoadFromWICFile(L"asset\\texture\\shellfish.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &g_Texture[1]);


	LoadFromWICFile(L"asset\\texture\\starfish.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &g_Texture[2]);


	LoadFromWICFile(L"asset\\texture\\coral.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &g_Texture[3]);


	LoadFromWICFile(L"asset\\texture\\mendako.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &g_Texture[4]);
	assert(g_Texture);

	//ブロック配列初期化
	for (int y = 0; y < BLOCK_ROWS; y++)
	{
		for (int x = 0; x < BLOCK_COLS; x++)
		{
			g_Block[y][x] = GetBlock(x, y);
		}
	}

	for (int i = 0; i < BLOCK_COLS; i++) {
		g_dropCount[i] = 0;
		g_moveDistance[i] = 0;
		g_dropSpeed[i] = 0.0f;
		g_dropStartFlg[i] = false;
		g_dropFlgOld[i] = false;
	}

	g_startTime = 0;
	g_startCount = 0;
}

/*******************************************************************************
*  終了処理
*******************************************************************************/

void UninitOver(void) {
	//テクスチャの解放
	for (int i = 0; i < TEX_COUNT; i++)
	{
		if (g_Texture[i])
		{
			g_Texture[i]->Release();
			g_Texture[i] = { NULL };
		}
	}
}

/*******************************************************************************
*  更新処理
*******************************************************************************/

void UpdateOver(void) {
	g_startTime++;
	if (g_startTime >= 20) {
		g_startTime %= 20;
	}if (g_startTime == 0) {
		if (g_startCount < BLOCK_COLS) {
			while (1)
			{
				int random = rand() % 6;
				if (!g_dropStartFlg[random]) {
					g_dropStartFlg[random] = true;
					g_startCount++;
					break;
				}
			}
		}
	}

	for (int x = 0; x < BLOCK_COLS; x++)
	{
		if (!g_dropStartFlg[x]) {
			continue;
		}
		g_dropSpeed[x] += OVER_DROP_SPEED;
		for (int y = 0; y < BLOCK_ROWS * 2; y++)
		{
			g_Block[y][x].Position = XMFLOAT2(g_Block[y][x].Position.x, g_Block[y][x].Position.y + g_dropSpeed[x]);
		}
	}

	for (int x = 0; x < BLOCK_COLS; x++)
	{
		if (!g_dropStartFlg[x]) {
			continue;
		}
		g_moveDistance[x] += g_dropSpeed[x];
		if (g_moveDistance[x] >= PIECE_HEIGHT) {
			g_moveDistance[x] -= PIECE_HEIGHT;
		}
		if (g_moveDistance[x] < g_dropSpeed[x]) {
			OverDown(x);
		}
	}
	if (Keyboard_IsKeyDown(KK_SPACE)) {
		SetSCENE(SCENE_RESULT);
	}
}

/*******************************************************************************
*  描画処理
*******************************************************************************/
void DrawOver(void)
{
	DrawField();
	DrawScore();

	for (int y = 0; y < BLOCK_ROWS; y++)
	{
		for (int x = 0; x < BLOCK_COLS; x++)
		{
			if (g_Block[y][x].Enable)
			{
				//テクスチャ設定
				int type = g_Block[y][x].Type;
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[type]);

				//スプライト描画
				XMFLOAT2 position;
				position = g_Block[y][x].Position;

				if (y == 0) {
					DrawTopPiece(position, 0.0f, XMFLOAT2(PIECE_WIDTH, PIECE_HEIGHT), PIECE_HEIGHT - g_moveDistance[x]);
				}
				else if (y == BLOCK_ROWS - 1) {
					DrawBottomPiece(XMFLOAT2(position.x, position.y), 0.0f, XMFLOAT2(PIECE_WIDTH, PIECE_HEIGHT), PIECE_HEIGHT - g_moveDistance[x]);
				}
				else {
					DrawSprite(position, 0.0f, XMFLOAT2(PIECE_WIDTH, PIECE_HEIGHT));
				}
			}
		}
	}
}

void OverDown(int x, bool checkFlg) {
	for (int y = BLOCK_ROWS * 2 - 1; y > 0; y--)
	{
		g_Block[y][x].Enable = g_Block[y - 1][x].Enable;
		g_Block[y][x].Erase = g_Block[y - 1][x].Erase;
		g_Block[y][x].Type = g_Block[y - 1][x].Type;

		g_Block[y - 1][x].Enable = false;
		g_Block[y - 1][x].Erase = false;
		g_Block[y - 1][x].Type = -1;

		if (!checkFlg) {
			g_Block[y][x].Position = XMFLOAT2(g_Block[y][x].Position.x, g_Block[y][x].Position.y - 50);
		}
	}

}

void OverUp(int x, bool checkFlg) {
	for (int y = 0; y < BLOCK_ROWS * 2 - 1; y++)
	{
		if (g_Block[y + 1][x].Enable) {
			g_Block[y][x].Enable = g_Block[y + 1][x].Enable;
			g_Block[y][x].Erase = g_Block[y + 1][x].Erase;
			g_Block[y][x].Type = g_Block[y + 1][x].Type;

			g_Block[y + 1][x].Enable = false;
			g_Block[y + 1][x].Erase = false;
			g_Block[y + 1][x].Type = -1;
		}
		if (!checkFlg) {
			g_Block[y][x].Position = XMFLOAT2(g_Block[y][x].Position.x, g_Block[y][x].Position.y + 50);
		}
	}
}



