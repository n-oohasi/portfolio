/*=====================================================================
|
|   メインクラス[main.cpp]
|
|												作成者：大橋範顕
|												作成日：2024/09/02
======================================================================*/


/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include <Windows.h>
#include"renderer.h"
#include"keyboard.h"

#include"sprite.h"

#include"title.h"
#include "sound.h"
#include "game.h"
#include "over.h"
#include "field.h"
#include "score.h"
#include "result.h"
#include "fade.h"
#include "ranking.h"

/*******************************************************************************
* ライブラリのリンク
*******************************************************************************/
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"Winmm.lib")

/*******************************************************************************
* 定数定義
*******************************************************************************/
#define CLASS_NAME		"GameWindow"
#define WINDOW_CAPTION	"ウィンドウ"

/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bwundow);
void Uninit(void);
void Update(void);
void Draw(void);


/*******************************************************************************
グローバル変数
*******************************************************************************/
#ifdef _DEBUG
int g_CountFPS;							// FPSカウンタ
char g_DebugStr[2048] = WINDOW_CAPTION;	// デバック文字表示用
#endif

GAME_SCENE g_gameScene;


/*******************************************************************************
メイン
*******************************************************************************/
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	CoInitializeEx(nullptr,COINITBASE_MULTITHREADED);

	//ウィンドウクラスの登録
	WNDCLASS wc = {};
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = CLASS_NAME;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1);
	RegisterClass(&wc);

	RECT rc = { 0,0,(LONG)SCREEN_WIDTH,(LONG)SCREEN_HEIGHT };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	//ウィンドウの作成
	HWND hWnd = CreateWindow(
		CLASS_NAME,
		WINDOW_CAPTION,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,		// x座標
		CW_USEDEFAULT,		// y座標
		rc.right - rc.left,
		rc.bottom - rc.top,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	// DirectXの初期化（ウィンドウを作成してから行う）
	if (FAILED(Init(hInstance, hWnd, true)))
	{
		return -1;
	}

	//時間観測用
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;

	//フレームカウント初期化
	timeBeginPeriod(1);//分解能を設定
	dwExecLastTime = dwFPSLastTime = timeGetTime();//システム時刻をミリ単位で取得
	dwCurrentTime = dwFrameCount = 0;


	//ウィンドウ表示(Init()の後に呼ばないと駄目）
	ShowWindow(hWnd, nCmdShow);
	//UpdateWindow(hWnd);

	MSG			msg;

	//メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{//PostQuitMessage()が呼ばれたらループ終了
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			dwCurrentTime = timeGetTime();					// システム時刻を会得

			if ((dwCurrentTime - dwFPSLastTime) >= 1000)	// 1秒ごとに実行
			{
#ifdef _DEBUG
				g_CountFPS = dwFrameCount;

				wsprintf(g_DebugStr, WINDOW_CAPTION);
				wsprintf(&g_DebugStr[strlen(g_DebugStr)], "FPS:%d", g_CountFPS);
				SetWindowText(hWnd, g_DebugStr);
#endif
				dwFPSLastTime = dwCurrentTime;				// FPSを測定した時刻を保存
				dwFrameCount = 0;							// カウントをクリア
			}

			if ((dwCurrentTime - dwExecLastTime) >= ((float)1000 / 60))// 1/60秒ごとに実行
			{
				dwExecLastTime = dwCurrentTime;// 処理した時刻を保存

				Update();		//更新処理
				Draw();			//描画処理

				dwFrameCount++;	//処理回数のカウントを加算
			}
		}
	}

	timeEndPeriod(1);			//分解能を戻す

	//終了処理
	Uninit();
	//終了
	return (int)msg.wParam;
}

/*******************************************************************************
プロシ―ジャ
*******************************************************************************/
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	switch ( uMsg )
	{
	case WM_KEYDOWN://キー押下
		if (wParam == VK_ESCAPE) {
			SendMessage(hWnd, WM_CLOSE, 0, 0);
		}

	case WM_ACTIVATEAPP:
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		Keyboard_ProcessMessage(uMsg, wParam, lParam);
		break;


		case WM_CLOSE://ウィンドウを閉じる
			if ( MessageBox(hWnd, "本当に終了してよろしいですか？", "確認", MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2) == IDOK) {
				DestroyWindow(hWnd);
			}
			return 0;

		case WM_DESTROY://アプリケーション終了
			PostQuitMessage(0);
			return 0;
	};

	//その他のメッセージはwindowsに任せる
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


/*******************************************************************************
*  初期化処理
*******************************************************************************/
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	// レンダリング処理の初期化
	InitRenderer(hInstance, hWnd, bWindow);

	// 入力処理の初期化
	InitRenderer(hInstance, hWnd, bWindow);

	// 画像の描画
	InitSprite();

	// サウンドの初期化
	InitSound(hWnd);
	InitFade();

	g_gameScene = SCENE_NONE;

	SetSCENE(SCENE_TITLE);

	return S_OK;
}

/*******************************************************************************
*  終了処理
*******************************************************************************/
void Uninit(void)
{
	SetSCENE(SCENE_NONE);

	UninitFade();

	//サウンドの終了
	UninitSound();

	UninitSprite();

	//レンダリングの終了処理
	UninitRenderer();
}

/*******************************************************************************
*  更新処理
*******************************************************************************/
void Update(void)
{
	UpdateFade();

	switch (g_gameScene) {
	case SCENE_NONE:
		break;
	case SCENE_TITLE:
		UpdateTitle();
		break;
	case SCENE_GAME:
		UpdateGame();
		break;
	case SCENE_OVER:
		UpdateField();
		UpdateOver();
		break;
	case SCENE_RESULT:
		UpdateResult();
		break;
	}
}

/*******************************************************************************
*  描画処理
*******************************************************************************/
void Draw(void)
{
	// バックバッファリア
	Clear();

	// 2D描画なので深度無効
	SetDepthEnable(false);

	//ここに描画処理
	switch (g_gameScene) {
	case SCENE_NONE:
		break;
	case SCENE_TITLE:
		DrawTitle();
		break;
	case SCENE_GAME:
		DrawGame();
		break;
	case SCENE_OVER:
		DrawOver();
		break;
	case SCENE_RESULT:
		DrawResult();
		DrawRanking();
		break;
	}

	DrawFade();

	// バックバッファ、フロントバッファ入れ替え
	Present();
}

void SetSCENE(GAME_SCENE scene) {
	static int BGMNo = 0;
	switch (g_gameScene) {
	case SCENE_NONE:
		break;
	case SCENE_TITLE:
		UninitTitle();
		break;
	case SCENE_GAME:
		UninitGame();
		break;
	case SCENE_OVER:
		BGMNo = GetBgmNo();
		StopSound(BGMNo);
		UninitOver();
		break;
	case SCENE_RESULT:
		UninitResult();
		UninitScore();
		UninitField();
		UninitRanking();
		break;
	}

	g_gameScene = scene;

	switch (g_gameScene) {
	case SCENE_NONE:
		break;
	case SCENE_TITLE:
		InitTitle();
		break;
	case SCENE_GAME:
		InitField();
		InitGame();
		InitScore();
		break;
	case SCENE_OVER:
		InitOver();
		break;
	case SCENE_RESULT:
		InitRanking();
		InitResult();
		break;
	}
}


GAME_SCENE GetGameScene() {
	return g_gameScene;
}

