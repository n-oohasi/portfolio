/*=====================================================================
|
|   ブロッククラス[block.cpp]
|
|												作成者：大橋範顕
|												作成日：2024/09/02
======================================================================*/
#include "block.h"
#include "sprite.h"
#include "piece.h"
#include "effect.h"
#include "score.h"
#include "ranking.h"

/*******************************************************************************
* グローバル変数
*******************************************************************************/
static BLOCK g_Block[BLOCK_ROWS][BLOCK_COLS];

static ID3D11ShaderResourceView* g_Texture[TEX_COUNT] = { NULL }; // テクスチャ―

static float g_moveDistance;
static int g_dropDistance;

static bool g_alertFlg;

/*******************************************************************************
*  初期化処理
*******************************************************************************/
void InitBlock(void)
{
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
			g_Block[y][x].Position = XMFLOAT2(FIELD_LEFT + PIECE_WIDTH / 2 + PIECE_WIDTH * x, FIELD_TOP + PIECE_HEIGHT * y);
			g_Block[y][x].Enable = false;
			g_Block[y][x].Erase = false;
			g_Block[y][x].Ground = false;
			g_Block[y][x].Type = rand() % TEX_COUNT;
		}
	}

	g_moveDistance = 0;
	g_dropDistance = 0;
	g_alertFlg = false;
}

/*******************************************************************************
*  終了処理
*******************************************************************************/
void UninitBlock(void)
{
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
void UpdateBlock(void)
{
	for (int y = 0; y < BLOCK_ROWS; y++)
	{
		for (int x = 0; x < BLOCK_COLS; x++)
		{
			g_Block[y][x].Position = XMFLOAT2(g_Block[y][x].Position.x, g_Block[y][x].Position.y - MOVE_SPEED);
		}
	}
	g_moveDistance += MOVE_SPEED;
	if (g_moveDistance >= PIECE_HEIGHT) {
		g_moveDistance -= PIECE_HEIGHT;
	}
	g_dropDistance += DROP_SPEED;
	g_dropDistance %= PIECE_HEIGHT;
	if (g_moveDistance < MOVE_SPEED) {
		for (int x = 0; x < BLOCK_COLS; x++) {
			if (g_Block[0][x].Enable) {
				g_alertFlg = false;
				int score = GetScore();
				SetSCENE(SCENE_OVER);
				AddRanking(score);
				return;
			}
		}
		for (int y = 0; y < BLOCK_ROWS; y++)
		{
			for (int x = 0; x < BLOCK_COLS; x++)
			{
				if (g_Block[y + 1][x].Enable&& y < BLOCK_ROWS - 1) {
					g_Block[y][x].Enable = g_Block[y + 1][x].Enable;
					g_Block[y][x].Erase = g_Block[y + 1][x].Erase;
					g_Block[y][x].Type = g_Block[y + 1][x].Type;

					g_Block[y + 1][x].Enable = false;
					g_Block[y + 1][x].Erase = false;
					g_Block[y + 1][x].Type = -1;
				}
				g_Block[y][x].Position = XMFLOAT2(g_Block[y][x].Position.x, g_Block[y][x].Position.y + 50);
			}
		}
	}

	if (g_dropDistance == 0) {
		for (int y = BLOCK_ROWS - 2; y > 0; y--)
		{
			for (int x = 0; x < BLOCK_COLS; x++)
			{
				if (!g_Block[y][x].Enable && g_Block[y - 1][x].Enable) {
					g_Block[y][x].Enable = g_Block[y - 1][x].Enable;
					g_Block[y][x].Erase = g_Block[y - 1][x].Erase;
					g_Block[y][x].Type = g_Block[y - 1][x].Type;

					g_Block[y - 1][x].Enable = false;
					g_Block[y - 1][x].Erase = false;
					g_Block[y - 1][x].Type = -1;
				}
			}
		}

		for (int x = 0; x < BLOCK_COLS; x++) {
			for (int y = BLOCK_ROWS - 1; y >= 0; y--) {
				g_Block[y][x].Ground = false;
			}
		}

		for (int x = 0; x < BLOCK_COLS; x++) {
			for (int y = BLOCK_ROWS - 2; y >= 0; y--) {
				if (g_Block[y][x].Enable) {
					g_Block[y][x].Ground = true;
				}
				else {
					break;
				}
			}
		}
	}
	g_alertFlg = false;
	for (int i = 0; i < BLOCK_COLS; i++) {
		if (g_Block[2][i].Enable) {
			g_alertFlg = true;
			break;
		}
	}
	EraseBlock();
}

/*******************************************************************************
*  描画処理
*******************************************************************************/
void DrawBlock(void)
{
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
					DrawTopPiece(position, 0.0f, XMFLOAT2(PIECE_WIDTH, PIECE_HEIGHT), g_moveDistance);
				}
				else if (y == BLOCK_ROWS - 1) {
					DrawBottomPiece(XMFLOAT2(position.x, position.y), 0.0f, XMFLOAT2(PIECE_WIDTH, PIECE_HEIGHT), g_moveDistance);
				}
				else {
					DrawSprite(position, 0.0f, XMFLOAT2(PIECE_WIDTH, PIECE_HEIGHT));
				}
			}
		}
	}
}

void SetBlock(int x, int y, int Type)
{
	g_Block[y][x].Enable = true;
	g_Block[y][x].Erase = false;
	g_Block[y][x].Type = Type;
}

BLOCK GetBlock(int x, int y)
{
	return g_Block[y][x];
}

void EraseBlock()
{
	bool erase = false;

	//横方向チェック
	int type = -1;
	int count = 0;

	for (int y = 0; y < BLOCK_ROWS; y++)
	{
		for (int x = 0; x < BLOCK_COLS; x++)
		{
			if (g_Block[y][x].Enable && g_Block[y][x].Ground)
			{
				if (g_Block[y][x].Type == type)
				{
					count++;

					if (count >= 2)
					{
						for (int i = x; i > x - 3; i--)
						{
							g_Block[y][i].Erase = true;
						}

						erase = true;
					}
				}
				else
				{
					type = g_Block[y][x].Type;
					count = 0;
				}
			}
			else
			{
				type = -1;
				count = 0;
			}
		}
		type = -1;
		count = 0;
	}

	//縦方向チェック
	for (int x = 0; x < BLOCK_COLS; x++)
	{
		for (int y = 0; y < BLOCK_ROWS; y++)
		{
			if (g_Block[y][x].Enable && g_Block[y][x].Ground)
			{
				if (g_Block[y][x].Type == type)
				{
					count++;

					if (count >= 2)
					{
						for (int i = y; i > y - 3; i--)
						{
							g_Block[i][x].Erase = true;
						}

						erase = true;
					}
				}
				else
				{
					type = g_Block[y][x].Type;
					count = 0;
				}
			}
			else
			{
				type = -1;
				count = 0;
			}
		}
		type = -1;
		count = 0;
	}

	int eraseCount = 0;

	//ブロック削除
	for (int y = 0; y < BLOCK_ROWS; y++)
	{
		for (int x = 0; x < BLOCK_COLS; x++)
		{
			if (g_Block[y][x].Erase)
			{
				g_Block[y][x].Erase = false;
				g_Block[y][x].Enable = false;

				XMFLOAT2 position = g_Block[y][x].Position;

				CreateEffect(position);

				eraseCount++;
			}
		}
	}

	if (erase)
	{
		AddScore(eraseCount);
	}
}

void ChangeBlock(int x, int y) {
	BLOCK m_block = g_Block[y][x];

	g_Block[y][x].Enable = g_Block[y][x + 1].Enable;
	g_Block[y][x].Erase = g_Block[y][x + 1].Erase;
	g_Block[y][x].Ground = g_Block[y][x + 1].Ground;
	g_Block[y][x].Type = g_Block[y][x + 1].Type;

	g_Block[y][x + 1].Enable = m_block.Enable;
	g_Block[y][x + 1].Erase = m_block.Erase;
	g_Block[y][x + 1].Ground = m_block.Ground;
	g_Block[y][x + 1].Type = m_block.Type;
}

bool GetAlertFlg() {
	return g_alertFlg;
}

float GetMoveDistance() {
	return g_moveDistance;
}


