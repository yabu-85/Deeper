#include "TitleScene.h"
#include "Engine/SceneManager.h"
#include "Engine/Input.h"
#include "Engine/Image.h"
#include "AudioManager.h"
#include "UIManager.h"

TitleScene::TitleScene(GameObject* parent)
	: GameObject(parent, "TitleScene"), hPict_{-1, -1}, pUIManager_(nullptr)
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

	pUIManager_ = new UIManager();

	//シーン推移を滑らかにしたいよね
	pUIManager_->AddUi("Play", XMFLOAT2(0.0f, 0.0f), [this]() {
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_PLAY); });

	pUIManager_->AddUi("Option", XMFLOAT2(0.0f, -0.35f), [this]() { AudioManager::PlaySoundA(); });
	pUIManager_->AddUi("Exit", XMFLOAT2(0.0f, -0.7f), nullptr);

}

void TitleScene::Update()
{
	pUIManager_->Update();

}

void TitleScene::Draw()
{
	Transform title = transform_;
	title.position_.y = 0.5f;
	Image::SetTransform(hPict_[0], title);
	Image::Draw(hPict_[0]);

	float screenWidth = Direct3D::screenWidth_;		//スクリーンの幅
	float screenHeight = Direct3D::screenHeight_;
	XMFLOAT3 size = Image::GetTextureSize(hPict_[1]);
	Transform bg = transform_;
	bg.scale_.x = (screenWidth / size.x);
	bg.scale_.y = (screenHeight / size.y);
	Image::SetTransform(hPict_[1], bg);
	Image::Draw(hPict_[1]);

	pUIManager_->Draw();

}

void TitleScene::Release()
{
}