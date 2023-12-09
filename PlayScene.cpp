#include "PlayScene.h"
#include "GameManager.h"
#include "Engine/SceneManager.h"

#include "Engine/Input.h"

#define EFFEKSEER 1

#if EFFEKSEER
#include <EffekseerForDXLib.h>

int effectResourceHandle = -1;
int grBackgroundHandle = -1;
int grFrontHandle = -1;

// 時間を初期化する(定期的にエフェクトを再生するため)
int time_ = 0;
float fTime_ = 0.0f;

// フルスクリーン切り替え用フラグを設定する。(F1、F2でウインドウ、フルスクリーンを切り替えれるようにする。)
bool isFullScreen = false;

// エフェクトの表示する位置を設定する。
float position_x = 0.0f;
float position_y = 0.0f;

// 再生中のエフェクトのハンドルを初期化する。
int playingEffectHandle = -1;
#endif

PlayScene::PlayScene(GameObject* parent)
	: GameObject(parent, "PlayScene")
{
}

void PlayScene::Initialize()
{
	GameManager::Initialize(this);

#if EFFEKSEER
	// DXライブラリの表示方法をウィンドウモードに変更する。
	ChangeWindowMode(true);

	//SetUserWindow(Direct3D::hWnd_);
	SetUserWindowMessageProcessDXLibFlag(0);

	//描画先を裏画面に変更する。
	SetDrawScreen(DX_SCREEN_BACK);

	// DirectX11を使用するようにする。(DirectX9も可、一部機能不可)
	// Effekseerを使用するには必ず設定する。
	SetUseDirect3DVersion(DX_DIRECT3D_11);

	// DXライブラリを初期化する。
	if (DxLib_Init() == -1) {
		return;
	}

	// Effekseerを初期化する。
	// 引数には画面に表示する最大パーティクル数を設定する。
	if (Effekseer_Init(5000) == -1)
	{
		DxLib_End();
		return;
	}

	// フルスクリーンウインドウの切り替えでリソースが消えるのを防ぐ。
	// Effekseerを使用する場合は必ず設定する。
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	// DXライブラリのデバイスロストした時のコールバックを設定する。
	// ウインドウとフルスクリーンの切り替えが発生する場合は必ず実行する。
	// ただし、DirectX11を使用する場合は実行する必要はない。
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	// エフェクトリソースを読み込む。
	// 読み込む時に大きさを指定する。
	effectResourceHandle = LoadEffekseerEffect("Effect/ToonWater.efkefc", 1.0f);

	// 何でもいいので画像を読み込む。
	grBackgroundHandle = LoadGraph(_T("Texture/Background.png"));
	grFrontHandle = LoadGraph(_T("Texture/Front.png"));

	// Zバッファを有効にする。
	// Effekseerを使用する場合、2DゲームでもZバッファを使用する。
	SetUseZBuffer3D(TRUE);

	// Zバッファへの書き込みを有効にする。
	// Effekseerを使用する場合、2DゲームでもZバッファを使用する。
	SetWriteZBuffer3D(TRUE);
#endif
}

void PlayScene::Update()
{
	GameManager::Update();

	if (Input::IsKeyDown(DIK_C)) {
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_TITLE);
	}

#if EFFEKSEER
	if (!ProcessMessage() && !ClearDrawScreen() && !CheckHitKey(KEY_INPUT_ESCAPE))
	{
		// DXライブラリのカメラを設定する。
		SetCameraPositionAndTarget_UpVecY(VGet(0, 10, -80), VGet(0, 0, 0));
		SetupCamera_Perspective(60.0f * DX_PI_F / 180.0f);
		SetCameraNearFar(1.0f, 150.0f);

		// DXライブラリのカメラとEffekseerのカメラを同期する。
		Effekseer_Sync3DSetting();

		// 定期的にエフェクトを再生する
		if (time_ % 1 == 0)
		{
			// エフェクトを再生する。
			if (Input::IsKey(DIK_E))
			playingEffectHandle = PlayEffekseer3DEffect(effectResourceHandle);
		}

		// 何でもいいので画像を描画する。
		// こうして描画した後でないと、Effekseerは描画できない。
		DrawGraph(0, 0, grBackgroundHandle, TRUE);

		// 再生中のエフェクトを移動する。
		position_x = 30.0f * cos(fTime_);
		position_y = 30.0f * sin(fTime_);
		SetPosPlayingEffekseer3DEffect(playingEffectHandle, position_x, position_y, 0);
		fTime_ += 0.02f;

		// Effekseerにより再生中のエフェクトを更新する。
		UpdateEffekseer3D();

		// 3Dを表示する。
		DrawCapsule3D(VGet(0.0f, 100.0f, 0.0f), VGet(0.0f, -100.0f, 0.0f), 6.0f, 16, GetColor(100, 100, 100), GetColor(255, 255, 255), TRUE);

		// Effekseerにより再生中のエフェクトを描画する。
		DrawEffekseer3D();

		// エフェクトの上にも画像を描画できる。
		//DrawGraph(0, 0, grFrontHandle, TRUE);

		// スクリーンを入れ替える。
		ScreenFlip();

		// 時間を経過させる。
		time_++;

	}
#endif

}

void PlayScene::Draw()
{
}

void PlayScene::Release()
{
	GameManager::Release();

#if EFFEKSEER
	// エフェクトリソースを削除する。(Effekseer終了時に破棄されるので削除しなくてもいい)
	DeleteEffekseerEffect(effectResourceHandle);

	// Effekseerを終了する。
	Effkseer_End();

	// DXライブラリを終了する。
	DxLib_End();
#endif

}
