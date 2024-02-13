#include "TitleUIManager.h"
#include "ExitUIManager.h"
#include "../Engine/Image.h"
#include "../Engine/Direct3D.h"
#include "../Engine/SceneManager.h"
#include "../GameManager.h"
#include "../AudioManager.h"

TitleUIManager::TitleUIManager(SceneBase* parent)
	: UIManager(parent), hPict_{ -1, -1 }
{
	const char* fileName[] = { "Image/Title.png", "Image/TitleBG.png" };
	const int png = sizeof(fileName) / sizeof(fileName[0]);
	for (int i = 0; i < png; i++) {
		hPict_[i] = Image::Load(fileName[i]);
		assert(hPict_[i] >= 0);
	}

	AddUi("Play", XMFLOAT2(0.0f, 0.0f), [this]() { GameManager::GetSceneManager()->ChangeScene(SCENE_ID_STAGE1); });
	AddUi("Option", XMFLOAT2(0.0f, -0.35f), [this]() { AudioManager::Play(); });
	AddUi("Exit", XMFLOAT2(0.0f, -0.7f), [this]() { pParent_->AddUIManager(new ExitUIManager(pParent_)); });
}

TitleUIManager::~TitleUIManager()
{
}

void TitleUIManager::Draw()
{
	Transform title;
	title.position_.y = 0.5f;
	Image::SetTransform(hPict_[0], title);
	Image::Draw(hPict_[0]);

	float screenWidth = (float)Direct3D::screenWidth_;		//ÉXÉNÉäÅ[ÉìÇÃïù
	float screenHeight = (float)Direct3D::screenHeight_;
	XMFLOAT3 size = Image::GetTextureSize(hPict_[1]);
	Transform bg;
	bg.scale_.x = (screenWidth / size.x);
	bg.scale_.y = (screenHeight / size.y);
	Image::SetTransform(hPict_[1], bg);
	Image::Draw(hPict_[1]);

	UIManager::Draw();
}
