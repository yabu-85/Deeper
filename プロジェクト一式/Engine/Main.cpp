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

//定数宣言
const char* WIN_CLASS_NAME = "SampleGame";	//ウィンドウクラス名

//マウス
bool isCursorLimited = TRUE;	// マウスポインターの制限

//フルスクリーンようにグローバル
bool isFullscreen = false;
LONG_PTR g_windowStyle;
RECT winRect;

//プロトタイプ宣言
HWND InitApp(HINSTANCE hInstance, int screenWidth, int screenHeight, int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void ToggleFullscreen(HWND hwnd);
void LimitMousePointer(HWND hwnd);
void ReleaseMousePointer();

// エントリーポイント
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	srand((unsigned)time(NULL));
	SetCurrentDirectory("Assets");

	//初期化ファイル（setup.ini）から必要な情報を取得
	int screenWidth = GetPrivateProfileInt("SCREEN", "Width", 800, ".\\setup.ini");		//スクリーンの幅
	int screenHeight = GetPrivateProfileInt("SCREEN", "Height", 600, ".\\setup.ini");	//スクリーンの高さ
	int fpsLimit = GetPrivateProfileInt("GAME", "Fps", 60, ".\\setup.ini");				//FPS（画面更新速度）
	int isDrawFps = GetPrivateProfileInt("DEBUG", "ViewFps", 0, ".\\setup.ini");		//キャプションに現在のFPSを表示するかどうか

#ifdef _DEBUG
	screenWidth = 600;
	screenHeight = 400;
#endif

	//ウィンドウを作成
	HWND hWnd = InitApp(hInstance, screenWidth, screenHeight, nCmdShow);

	//Direct3D準備
	Direct3D::Initialize(hWnd, screenWidth, screenHeight);

	//カメラを準備
	Camera::Initialize();

	//入力処理（キーボード、マウス、コントローラー）の準備
	Input::Initialize(hWnd);

	//オーディオ（効果音）の準備
	Audio::Initialize();

	GameManager::Initialize();

	InputManager::SetDefaultKeyConfig();

	//ルートオブジェクト準備
	//すべてのゲームオブジェクトの親となるオブジェクト
	RootObject* pRootObject = new RootObject;
	pRootObject->Initialize();

	//メッセージループ（何か起きるのを待つ）
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		//メッセージあり（こっちが優先）
		if (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//メッセージなし（ここでゲームの処理）
		else
		{
			//時間計測
			timeBeginPeriod(1);	//時間計測の制度を上げる
			static int FPS = 0;								//画面更新回数のカウンタ
			static DWORD lastFpsResetTime = timeGetTime();	//最後にキャプションにFPSを表示した
			static DWORD lastUpdateTime = timeGetTime();	//最後に画面を更新した時間
			DWORD nowTime = timeGetTime();					//現在の時間

			//キャプションに現在のFPSを表示する
			if (isDrawFps)
			{
				//1秒（1000ミリ秒）経ったら
				if (nowTime - lastFpsResetTime > 1000)
				{
					//FPSの値を表示
					char string[16];
					wsprintf(string, "FPS:%d", FPS);
					SetWindowText(GetActiveWindow(), string);
					FPS = 0;
					lastFpsResetTime = nowTime;
				}
			}

			//指定した時間（FPSを60に設定した場合は60分の1秒）経過していたら更新処理
			if ((nowTime - lastUpdateTime) * fpsLimit > 1000.0f)
			{
				//時間計測関連
				lastUpdateTime = nowTime;	//現在の時間（最後に画面を更新した時間）を覚えておく
				FPS++;						//画面更新回数をカウントする

				//入力（キーボード、マウス、コントローラー）情報を更新
				Input::Update();

				pRootObject->UpdateSub();

				GameManager::Update();

				//Pauseなどでのウィンドウスタイル変更のため
				if (GameManager::IsMouseLimitedScene() != isCursorLimited) {
					isCursorLimited = !isCursorLimited;
					if (isCursorLimited) LimitMousePointer(hWnd);
					else ReleaseMousePointer();
				}

				//カメラを更新
				Camera::Update();

				//エフェクトの更新
				VFX::Update();

				//このフレームの描画開始
				Direct3D::BeginDraw();

				//全オブジェクトを描画
				//ルートオブジェクトのDrawを呼んだあと、自動的に子、孫のUpdateが呼ばれる
				pRootObject->DrawSub();

				//エフェクトの描画
				VFX::Draw();

				GameManager::Draw();

				//描画終了
				Direct3D::EndDraw();

				//ちょっと休ませる
				Sleep(1);
			}
			timeEndPeriod(1);	//時間計測の制度を戻す
		}
	}

	//いろいろ解放
	VFX::Release();
	Audio::Release();
	Model::AllRelease();
	Image::AllRelease();
	pRootObject->ReleaseSub();
	SAFE_DELETE(pRootObject);
	Direct3D::Release();

	return 0;
}


//ウィンドウの作成
HWND InitApp(HINSTANCE hInstance, int screenWidth, int screenHeight, int nCmdShow)
{
	//ウィンドウクラス（設計図）を作成
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);					//この構造体のサイズ
	wc.hInstance = hInstance;						//インスタンスハンドル
	wc.lpszClassName = WIN_CLASS_NAME;				//ウィンドウクラス名
	wc.lpfnWndProc = WndProc;						//ウィンドウプロシージャ
	wc.style = CS_VREDRAW | CS_HREDRAW;				//スタイル（デフォルト）
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);	//アイコン
	wc.hIconSm = LoadIcon(nullptr, IDI_WINLOGO);	//小さいアイコン
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);	//マウスカーソル
	wc.lpszMenuName = nullptr;						//メニュー（なし）
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	//背景（白）
	RegisterClassEx(&wc);

	//ウィンドウサイズの計算
	winRect = { 0, 0, screenWidth, screenHeight };
	AdjustWindowRect(&winRect, WS_OVERLAPPEDWINDOW, FALSE);

	//タイトルバーに表示する内容
	char caption[64];
	GetPrivateProfileString("SCREEN", "Caption", "***", caption, 64, ".\\setup.ini");

	//ウィンドウを作成
	HWND hWnd = CreateWindow(
		WIN_CLASS_NAME,					            //ウィンドウクラス名
		caption,						            //タイトルバーに表示する内容
		
		//WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX),	//スタイル（通常のスタイル & 最大化不可 | サイズ変更不可）
		WS_OVERLAPPEDWINDOW,
		
		CW_USEDEFAULT,					            //表示位置左（おまかせ）
		CW_USEDEFAULT,					            //表示位置上（おまかせ）
		winRect.right - winRect.left,	            //ウィンドウ幅
		winRect.bottom - winRect.top,	            //ウィンドウ高さ
		nullptr,						            //親ウインドウ（なし）
		nullptr,						            //メニュー（なし）
		hInstance,						            //インスタンス
		nullptr							            //パラメータ（なし）
	);

	//ウィンドウを表示
	ShowWindow(hWnd, nCmdShow);

	//カーソルの描画設定
	while (ShowCursor(FALSE) >= 0);

	//マウス制限
	if (isCursorLimited) LimitMousePointer(hWnd);

	return hWnd;
}

void ToggleFullscreen(HWND hwnd)
{
	isFullscreen = !isFullscreen;

	if (isFullscreen)
	{
		g_windowStyle = GetWindowLongPtr(hwnd, GWL_STYLE);
		// ウィンドウスタイルをフルスクリーンに変更
		SetWindowLong(hwnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
		// ウィンドウサイズをディスプレイの解像度に合わせる
		SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_FRAMECHANGED | SWP_SHOWWINDOW);

		Direct3D::screenWidth_ = GetSystemMetrics(SM_CXSCREEN);
		Direct3D::screenHeight_ = GetSystemMetrics(SM_CYSCREEN);
	}

	else
	{
		// ウィンドウスタイルを通常のウィンドウに戻す
		SetWindowLong(hwnd, GWL_STYLE, (LONG)g_windowStyle);
		// ウィンドウサイズを元に戻す
		SetWindowPos(hwnd, HWND_NOTOPMOST, winRect.left, winRect.top, winRect.right - winRect.left, winRect.bottom - winRect.top, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
		Direct3D::screenWidth_ = winRect.right - winRect.left;
		Direct3D::screenHeight_ = winRect.bottom - winRect.top;
	}
}

//ウィンドウプロシージャ（何かあった時によばれる関数）
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	//ウィンドウを閉じた
	case WM_DESTROY:
		ReleaseMousePointer();
		PostQuitMessage(0);		//プログラム終了
		return 0;

	//マウスが動いた
	case WM_MOUSEMOVE:
		Input::SetMousePosition(LOWORD(lParam), HIWORD(lParam));
		// マウスカーソルをウィンドウの中心に移動
		if (isCursorLimited) LimitMousePointer(hWnd);

		return 0; 
	case WM_KEYDOWN:
		// フルスクリーンに切り替え
		if (wParam == VK_F11) ToggleFullscreen(hWnd);

		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

// マウスポインターを中央に固定する関数
void LimitMousePointer(HWND hwnd)
{
	RECT windowRect;
	GetClientRect(hwnd, &windowRect);

	// ウィンドウの矩形領域をスクリーン座標に変換（閉じるボタン押せないようにするため少し小さく
	MapWindowPoints(hwnd, nullptr, reinterpret_cast<POINT*>(&windowRect), 2);
	windowRect.left++;
	windowRect.top++;
	windowRect.right--;
	windowRect.bottom--;

	// マウスポインターを制限
	ClipCursor(&windowRect);

	// ウィンドウの中心座標を計算してセット
	POINT windowCenter = { Direct3D::screenWidth_ / 2, Direct3D::screenHeight_ / 2 };
	SetCursorPos(windowRect.left + windowCenter.x , windowRect.top + windowCenter.y);

}

// マウスポインターの制限を解除する関数
void ReleaseMousePointer()
{
	ClipCursor(nullptr);
}