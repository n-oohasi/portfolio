/*=====================================================================
|
|   ピースクラス[piece.cpp]
|
|												作成者：大橋範顕
|												作成日：2024/09/02
======================================================================*/
#include"piece.h"
#include"sprite.h"
#include"keyboard.h"
#include"block.h"
#include<time.h>

/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/
void CreatePiece(void);
void MovePiece(void);

struct PIECE
{
	XMFLOAT2 Position[BLOCK_COLS];	//描画位置
	int Type[BLOCK_COLS];		//マークの番号
};
static PIECE g_Piece;
static float moveDistance;

//Pieceの状態
enum PIECE_STATE
{
	PIECE_STATE_IDLE,
	PIECE_STATE_MOVE,
	PIECE_STATE_GROUND_IDLE,
	PIECE_STATE_STATE_MISS_IDLE,
};
static PIECE_STATE g_PieceState;
static int g_PieceStateCount;

static ID3D11ShaderResourceView* g_Texture[TEX_COUNT] = { NULL }; // テクスチャ―



/*******************************************************************************
*  初期化処理
*******************************************************************************/
void InitPiece(void)
{
	moveDistance = 0.0f;

	//ピースの初期化
	srand((unsigned)time(NULL));
	CreatePiece();
}

/*******************************************************************************
*  終了処理
*******************************************************************************/
void UninitPiece(void)
{

}

/*******************************************************************************
*  更新処理
*******************************************************************************/
void UpdatePiece(void)
{
	switch (g_PieceState)
	{
	case PIECE_STATE_IDLE:
		break;
	case PIECE_STATE_MOVE:
		MovePiece();
		break;
	case PIECE_STATE_GROUND_IDLE:
		g_PieceStateCount++;
		if (g_PieceStateCount > 60)
		{
			g_PieceState = PIECE_STATE_IDLE;
			g_PieceStateCount = 0;

			EraseBlock();
		}
		break;
	case PIECE_STATE_STATE_MISS_IDLE:
		break;
	default:
		break;
	}
}

void CreatePiece()
{
	//Pieceの初期化
	for (int i = 0; i < BLOCK_COLS; i++) {
		g_Piece.Position[i] = XMFLOAT2(SCREEN_WIDTH / 2 - PIECE_WIDTH * 2.5f + PIECE_WIDTH * i, FIELD_BOTTOM);
		g_Piece.Type[i] = rand() % TEX_COUNT; //乱数でマークの変更
	}

	g_PieceState = PIECE_STATE_MOVE;
	g_PieceStateCount = 0;
}

void MovePiece(void)
{
	//上昇
	for (int i = 0; i < BLOCK_COLS; i++) {
		g_Piece.Position[i].y -= 1.0f;
	}

	moveDistance += MOVE_SPEED;
	if (moveDistance >= PIECE_HEIGHT) {
		moveDistance -= PIECE_HEIGHT;
	}

	if (moveDistance < MOVE_SPEED) {
		for (int i = 0; i < BLOCK_COLS; i++) {
			SetBlock(i, BLOCK_ROWS - 1, g_Piece.Type[i]);
		}
		CreatePiece();
	}
}



/*******************************************************************************
*  描画処理
*******************************************************************************/
void DrawPiece(void)
{
	if (g_PieceState != PIECE_STATE_MOVE)
		return;

	for (int i = 0; i < BLOCK_COLS; i++)
	{
		// テクスチャ設定
		int type = g_Piece.Type[i];
		//int type = g_Piece.Type[i];
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[type]);

		// スプライト描画
		DrawBottomPiece(XMFLOAT2(
			g_Piece.Position[i].x,
			g_Piece.Position[i].y + PIECE_HEIGHT),
			0.0f, XMFLOAT2(PIECE_WIDTH, PIECE_HEIGHT), moveDistance);
	}
}