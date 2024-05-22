#include <Windows.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#include "global.h"
#include "RootObject.h"
#include "Model.h"
#include "Image.h"
#include "Camera.h"
#include "Input.h"
#include "Audio.h"
#include "VFX.h"
#include "../GameManager/GameManager.h"
#include "../Other/InputManager.h"

#pragma comment(lib,"Winmm.lib")

//�萔�錾
const char* WIN_CLASS_NAME = "SampleGame";	//�E�B���h�E�N���X��

//�}�E�X
bool isCursorLimited = TRUE;	// �}�E�X�|�C���^�[�̐���

//�t���X�N���[���悤�ɃO���[�o��
bool isFullscreen = false;
LONG_PTR g_windowStyle;
RECT winRect;

//�v���g�^�C�v�錾
HWND InitApp(HINSTANCE hInstance, int screenWidth, int screenHeight, int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void ToggleFullscreen(HWND hwnd);
void LimitMousePointer(HWND hwnd);
void ReleaseMousePointer();

// �G���g���[�|�C���g
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	srand((unsigned)time(NULL));
	SetCurrentDirectory("Assets");

	//�������t�@�C���isetup.ini�j����K�v�ȏ����擾
	int screenWidth = GetPrivateProfileInt("SCREEN", "Width", 800, ".\\setup.ini");		//�X�N���[���̕�
	int screenHeight = GetPrivateProfileInt("SCREEN", "Height", 600, ".\\setup.ini");	//�X�N���[���̍���
	int fpsLimit = GetPrivateProfileInt("GAME", "Fps", 60, ".\\setup.ini");				//FPS�i��ʍX�V���x�j
	int isDrawFps = GetPrivateProfileInt("DEBUG", "ViewFps", 0, ".\\setup.ini");		//�L���v�V�����Ɍ��݂�FPS��\�����邩�ǂ���

#ifdef _DEBUG
	screenWidth = 600;
	screenHeight = 400;
#endif

	//�E�B���h�E���쐬
	HWND hWnd = InitApp(hInstance, screenWidth, screenHeight, nCmdShow);

	//Direct3D����
	Direct3D::Initialize(hWnd, screenWidth, screenHeight);

	//�J����������
	Camera::Initialize();

	//���͏����i�L�[�{�[�h�A�}�E�X�A�R���g���[���[�j�̏���
	Input::Initialize(hWnd);

	//�I�[�f�B�I�i���ʉ��j�̏���
	Audio::Initialize();

	GameManager::Initialize();

	InputManager::SetDefaultKeyConfig();

	//���[�g�I�u�W�F�N�g����
	//���ׂẴQ�[���I�u�W�F�N�g�̐e�ƂȂ�I�u�W�F�N�g
	RootObject* pRootObject = new RootObject;
	pRootObject->Initialize();

	//���b�Z�[�W���[�v�i�����N����̂�҂j
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		//���b�Z�[�W����i���������D��j
		if (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//���b�Z�[�W�Ȃ��i�����ŃQ�[���̏����j
		else
		{
			//���Ԍv��
			timeBeginPeriod(1);	//���Ԍv���̐��x���グ��
			static int FPS = 0;								//��ʍX�V�񐔂̃J�E���^
			static DWORD lastFpsResetTime = timeGetTime();	//�Ō�ɃL���v�V������FPS��\������
			static DWORD lastUpdateTime = timeGetTime();	//�Ō�ɉ�ʂ��X�V��������
			DWORD nowTime = timeGetTime();					//���݂̎���

			//�L���v�V�����Ɍ��݂�FPS��\������
			if (isDrawFps)
			{
				//1�b�i1000�~���b�j�o������
				if (nowTime - lastFpsResetTime > 1000)
				{
					//FPS�̒l��\��
					char string[16];
					wsprintf(string, "FPS:%d", FPS);
					SetWindowText(GetActiveWindow(), string);
					FPS = 0;
					lastFpsResetTime = nowTime;
				}
			}

			//�w�肵�����ԁiFPS��60�ɐݒ肵���ꍇ��60����1�b�j�o�߂��Ă�����X�V����
			if ((nowTime - lastUpdateTime) * fpsLimit > 1000.0f)
			{
				//���Ԍv���֘A
				lastUpdateTime = nowTime;	//���݂̎��ԁi�Ō�ɉ�ʂ��X�V�������ԁj���o���Ă���
				FPS++;						//��ʍX�V�񐔂��J�E���g����

				//���́i�L�[�{�[�h�A�}�E�X�A�R���g���[���[�j�����X�V
				Input::Update();

				pRootObject->UpdateSub();

				GameManager::Update();

				//Pause�Ȃǂł̃E�B���h�E�X�^�C���ύX�̂���
				if (GameManager::IsMouseLimitedScene() != isCursorLimited) {
					isCursorLimited = !isCursorLimited;
					if (isCursorLimited) LimitMousePointer(hWnd);
					else ReleaseMousePointer();
				}

				//�J�������X�V
				Camera::Update();

				//�G�t�F�N�g�̍X�V
				VFX::Update();

				//���̃t���[���̕`��J�n
				Direct3D::BeginDraw();

				//�S�I�u�W�F�N�g��`��
				//���[�g�I�u�W�F�N�g��Draw���Ă񂾂��ƁA�����I�Ɏq�A����Update���Ă΂��
				pRootObject->DrawSub();

				//�G�t�F�N�g�̕`��
				VFX::Draw();

				GameManager::Draw();

				//�`��I��
				Direct3D::EndDraw();

				//������Ƌx�܂���
				Sleep(1);
			}
			timeEndPeriod(1);	//���Ԍv���̐��x��߂�
		}
	}

	//���낢����
	VFX::Release();
	Audio::Release();
	Model::AllRelease();
	Image::AllRelease();
	pRootObject->ReleaseSub();
	SAFE_DELETE(pRootObject);
	Direct3D::Release();

	return 0;
}


//�E�B���h�E�̍쐬
HWND InitApp(HINSTANCE hInstance, int screenWidth, int screenHeight, int nCmdShow)
{
	//�E�B���h�E�N���X�i�݌v�}�j���쐬
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);					//���̍\���̂̃T�C�Y
	wc.hInstance = hInstance;						//�C���X�^���X�n���h��
	wc.lpszClassName = WIN_CLASS_NAME;				//�E�B���h�E�N���X��
	wc.lpfnWndProc = WndProc;						//�E�B���h�E�v���V�[�W��
	wc.style = CS_VREDRAW | CS_HREDRAW;				//�X�^�C���i�f�t�H���g�j
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);	//�A�C�R��
	wc.hIconSm = LoadIcon(nullptr, IDI_WINLOGO);	//�������A�C�R��
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);	//�}�E�X�J�[�\��
	wc.lpszMenuName = nullptr;						//���j���[�i�Ȃ��j
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	//�w�i�i���j
	RegisterClassEx(&wc);

	//�E�B���h�E�T�C�Y�̌v�Z
	winRect = { 0, 0, screenWidth, screenHeight };
	AdjustWindowRect(&winRect, WS_OVERLAPPEDWINDOW, FALSE);

	//�^�C�g���o�[�ɕ\��������e
	char caption[64];
	GetPrivateProfileString("SCREEN", "Caption", "***", caption, 64, ".\\setup.ini");

	//�E�B���h�E���쐬
	HWND hWnd = CreateWindow(
		WIN_CLASS_NAME,					            //�E�B���h�E�N���X��
		caption,						            //�^�C�g���o�[�ɕ\��������e
		
		//WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX),	//�X�^�C���i�ʏ�̃X�^�C�� & �ő剻�s�� | �T�C�Y�ύX�s�j
		WS_OVERLAPPEDWINDOW,
		
		CW_USEDEFAULT,					            //�\���ʒu���i���܂����j
		CW_USEDEFAULT,					            //�\���ʒu��i���܂����j
		winRect.right - winRect.left,	            //�E�B���h�E��
		winRect.bottom - winRect.top,	            //�E�B���h�E����
		nullptr,						            //�e�E�C���h�E�i�Ȃ��j
		nullptr,						            //���j���[�i�Ȃ��j
		hInstance,						            //�C���X�^���X
		nullptr							            //�p�����[�^�i�Ȃ��j
	);

	//�E�B���h�E��\��
	ShowWindow(hWnd, nCmdShow);

	//�J�[�\���̕`��ݒ�
	while (ShowCursor(FALSE) >= 0);

	//�}�E�X����
	if (isCursorLimited) LimitMousePointer(hWnd);

	return hWnd;
}

void ToggleFullscreen(HWND hwnd)
{
	isFullscreen = !isFullscreen;

	if (isFullscreen)
	{
		g_windowStyle = GetWindowLongPtr(hwnd, GWL_STYLE);
		// �E�B���h�E�X�^�C�����t���X�N���[���ɕύX
		SetWindowLong(hwnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
		// �E�B���h�E�T�C�Y���f�B�X�v���C�̉𑜓x�ɍ��킹��
		SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_FRAMECHANGED | SWP_SHOWWINDOW);

		Direct3D::screenWidth_ = GetSystemMetrics(SM_CXSCREEN);
		Direct3D::screenHeight_ = GetSystemMetrics(SM_CYSCREEN);
	}

	else
	{
		// �E�B���h�E�X�^�C����ʏ�̃E�B���h�E�ɖ߂�
		SetWindowLong(hwnd, GWL_STYLE, (LONG)g_windowStyle);
		// �E�B���h�E�T�C�Y�����ɖ߂�
		SetWindowPos(hwnd, HWND_NOTOPMOST, winRect.left, winRect.top, winRect.right - winRect.left, winRect.bottom - winRect.top, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
		Direct3D::screenWidth_ = winRect.right - winRect.left;
		Direct3D::screenHeight_ = winRect.bottom - winRect.top;
	}
}

//�E�B���h�E�v���V�[�W���i�������������ɂ�΂��֐��j
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	//�E�B���h�E�����
	case WM_DESTROY:
		ReleaseMousePointer();
		PostQuitMessage(0);		//�v���O�����I��
		return 0;

	//�}�E�X��������
	case WM_MOUSEMOVE:
		Input::SetMousePosition(LOWORD(lParam), HIWORD(lParam));
		// �}�E�X�J�[�\�����E�B���h�E�̒��S�Ɉړ�
		if (isCursorLimited) LimitMousePointer(hWnd);

		return 0; 
	case WM_KEYDOWN:
		// �t���X�N���[���ɐ؂�ւ�
		if (wParam == VK_F11) ToggleFullscreen(hWnd);

		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

// �}�E�X�|�C���^�[�𒆉��ɌŒ肷��֐�
void LimitMousePointer(HWND hwnd)
{
	RECT windowRect;
	GetClientRect(hwnd, &windowRect);

	// �E�B���h�E�̋�`�̈���X�N���[�����W�ɕϊ��i����{�^�������Ȃ��悤�ɂ��邽�ߏ���������
	MapWindowPoints(hwnd, nullptr, reinterpret_cast<POINT*>(&windowRect), 2);
	windowRect.left++;
	windowRect.top++;
	windowRect.right--;
	windowRect.bottom--;

	// �}�E�X�|�C���^�[�𐧌�
	ClipCursor(&windowRect);

	// �E�B���h�E�̒��S���W���v�Z���ăZ�b�g
	POINT windowCenter = { Direct3D::screenWidth_ / 2, Direct3D::screenHeight_ / 2 };
	SetCursorPos(windowRect.left + windowCenter.x , windowRect.top + windowCenter.y);

}

// �}�E�X�|�C���^�[�̐�������������֐�
void ReleaseMousePointer()
{
	ClipCursor(nullptr);
}