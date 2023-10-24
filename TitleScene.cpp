#include "TitleScene.h"
#include "Engine/SceneManager.h"
#include "Engine/Input.h"
#include "Engine/Image.h"
#include "UIBase.h"
#include "AudioManager.h"

namespace {
	UIBase* button;
}

TitleScene::TitleScene(GameObject* parent)
	: GameObject(parent, "TitleScene"), hPict_{-1, -1}
{
}

void TitleScene::Initialize()
{
	AudioManager::Initialize();

	const char* fileName[] = { "Png/Title.png", "Png/TitleBG.png" };
	const int png = sizeof(fileName) / sizeof(fileName[0]);
	for (int i = 0; i < png; i++) {
		hPict_[i] = Image::Load(fileName[i]);
		assert(hPict_[i] >= 0);
	}

	button = new UIBase();
	button->Initialize("Play");
}

void TitleScene::Update()
{
	if (Input::IsMouseButton(0)) {
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_PLAY);
	}

	button->Update();

	XMFLOAT3 mouse = Input::GetMousePosition();
	std::string strNumber = std::to_string(mouse.x);
	OutputDebugStringA(strNumber.c_str());
	OutputDebugString(", ");

	strNumber = std::to_string(mouse.y);
	OutputDebugStringA(strNumber.c_str());
	OutputDebugString("\n");

}

void TitleScene::Draw()
{
	Transform title = transform_;
	title.position_.y = 0.5f;
	Image::SetTransform(hPict_[0], title);
	Image::Draw(hPict_[0]);

	float screenWidth = Direct3D::screenWidth_;		//ƒXƒNƒŠ[ƒ“‚Ì•
	float screenHeight = Direct3D::screenHeight_;
	XMFLOAT3 size = Image::GetTextureSize(hPict_[1]);
	Transform bg = transform_;
	bg.scale_.x = (screenWidth / size.x);
	bg.scale_.y = (screenHeight / size.y);
	Image::SetTransform(hPict_[1], bg);
	Image::Draw(hPict_[1]);

	button->Draw();
}

void TitleScene::Release()
{
}