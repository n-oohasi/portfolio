/*=====================================================================
|
|   ���C���N���X[main.cpp]
|
|												�쐬�ҁF�勴�͌�
|												�쐬���F2024/09/02
======================================================================*/


/*******************************************************************************
* �C���N���[�h�t�@�C��
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
* ���C�u�����̃����N
*******************************************************************************/
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"Winmm.lib")

/*******************************************************************************
* �萔��`
*******************************************************************************/
#define CLASS_NAME		"GameWindow"
#define WINDOW_CAPTION	"�E�B���h�E"

/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bwundow);
void Uninit(void);
void Update(void);
void Draw(void);


/*******************************************************************************
�O���[�o���ϐ�
*******************************************************************************/
#ifdef _DEBUG
int g_CountFPS;							// FPS�J�E���^
char g_DebugStr[2048] = WINDOW_CAPTION;	// �f�o�b�N�����\���p
#endif

GAME_SCENE g_gameScene;


/*******************************************************************************
���C��
*******************************************************************************/
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	CoInitializeEx(nullptr,COINITBASE_MULTITHREADED);

	//�E�B���h�E�N���X�̓o�^
	WNDCLASS wc = {};
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = CLASS_NAME;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1);
	RegisterClass(&wc);

	RECT rc = { 0,0,(LONG)SCREEN_WIDTH,(LONG)SCREEN_HEIGHT };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	//�E�B���h�E�̍쐬
	HWND hWnd = CreateWindow(
		CLASS_NAME,
		WINDOW_CAPTION,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,		// x���W
		CW_USEDEFAULT,		// y���W
		rc.right - rc.left,
		rc.bottom - rc.top,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	// DirectX�̏������i�E�B���h�E���쐬���Ă���s���j
	if (FAILED(Init(hInstance, hWnd, true)))
	{
		return -1;
	}

	//���Ԋϑ��p
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;

	//�t���[���J�E���g������
	timeBeginPeriod(1);//����\��ݒ�
	dwExecLastTime = dwFPSLastTime = timeGetTime();//�V�X�e���������~���P�ʂŎ擾
	dwCurrentTime = dwFrameCount = 0;


	//�E�B���h�E�\��(Init()�̌�ɌĂ΂Ȃ��Ƒʖځj
	ShowWindow(hWnd, nCmdShow);
	//UpdateWindow(hWnd);

	MSG			msg;

	//���b�Z�[�W���[�v
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{//PostQuitMessage()���Ă΂ꂽ�烋�[�v�I��
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
			dwCurrentTime = timeGetTime();					// �V�X�e���������

			if ((dwCurrentTime - dwFPSLastTime) >= 1000)	// 1�b���ƂɎ��s
			{
#ifdef _DEBUG
				g_CountFPS = dwFrameCount;

				wsprintf(g_DebugStr, WINDOW_CAPTION);
				wsprintf(&g_DebugStr[strlen(g_DebugStr)], "FPS:%d", g_CountFPS);
				SetWindowText(hWnd, g_DebugStr);
#endif
				dwFPSLastTime = dwCurrentTime;				// FPS�𑪒肵��������ۑ�
				dwFrameCount = 0;							// �J�E���g���N���A
			}

			if ((dwCurrentTime - dwExecLastTime) >= ((float)1000 / 60))// 1/60�b���ƂɎ��s
			{
				dwExecLastTime = dwCurrentTime;// ��������������ۑ�

				Update();		//�X�V����
				Draw();			//�`�揈��

				dwFrameCount++;	//�����񐔂̃J�E���g�����Z
			}
		}
	}

	timeEndPeriod(1);			//����\��߂�

	//�I������
	Uninit();
	//�I��
	return (int)msg.wParam;
}

/*******************************************************************************
�v���V�\�W��
*******************************************************************************/
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	switch ( uMsg )
	{
	case WM_KEYDOWN://�L�[����
		if (wParam == VK_ESCAPE) {
			SendMessage(hWnd, WM_CLOSE, 0, 0);
		}

	case WM_ACTIVATEAPP:
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		Keyboard_ProcessMessage(uMsg, wParam, lParam);
		break;


		case WM_CLOSE://�E�B���h�E�����
			if ( MessageBox(hWnd, "�{���ɏI�����Ă�낵���ł����H", "�m�F", MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2) == IDOK) {
				DestroyWindow(hWnd);
			}
			return 0;

		case WM_DESTROY://�A�v���P�[�V�����I��
			PostQuitMessage(0);
			return 0;
	};

	//���̑��̃��b�Z�[�W��windows�ɔC����
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


/*******************************************************************************
*  ����������
*******************************************************************************/
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	// �����_�����O�����̏�����
	InitRenderer(hInstance, hWnd, bWindow);

	// ���͏����̏�����
	InitRenderer(hInstance, hWnd, bWindow);

	// �摜�̕`��
	InitSprite();

	// �T�E���h�̏�����
	InitSound(hWnd);
	InitFade();

	g_gameScene = SCENE_NONE;

	SetSCENE(SCENE_TITLE);

	return S_OK;
}

/*******************************************************************************
*  �I������
*******************************************************************************/
void Uninit(void)
{
	SetSCENE(SCENE_NONE);

	UninitFade();

	//�T�E���h�̏I��
	UninitSound();

	UninitSprite();

	//�����_�����O�̏I������
	UninitRenderer();
}

/*******************************************************************************
*  �X�V����
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
*  �`�揈��
*******************************************************************************/
void Draw(void)
{
	// �o�b�N�o�b�t�@���A
	Clear();

	// 2D�`��Ȃ̂Ő[�x����
	SetDepthEnable(false);

	//�����ɕ`�揈��
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

	// �o�b�N�o�b�t�@�A�t�����g�o�b�t�@����ւ�
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

