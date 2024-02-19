#include "ResultUIManager.h"
#include "ExitUIManager.h"
#include "../Engine/Image.h"
#include "../Engine/Direct3D.h"
#include "../Engine/SceneManager.h"
#include "../GameManager.h"
#include "../AudioManager.h"
#include "../Player/LifeManager.h"

ResultUIManager::ResultUIManager(SceneBase* parent)
	: UIManager(parent), hPict_{ -1, -1 }
{
	hPict_[0] = Image::Load("Image/Title.png");
	assert(hPict_[0] >= 0);
	
	const char* fileName[] = { "Image/GameOver.png", "Image/Clear.png" };
	if (LifeManager::IsDie()) hPict_[1] = Image::Load(fileName[0]);
	else hPict_[1] = Image::Load(fileName[1]);
	assert(hPict_[1] >= 0);

	AddUi("Play", XMFLOAT2(0.0f, 0.0f), [this]() { GameManager::GetSceneManager()->ChangeScene(SCENE_ID_TITLE); });
	AddUi("Option", XMFLOAT2(0.0f, -0.35f), [this]() { AudioManager::Play(); });
	AddUi("Exit", XMFLOAT2(0.0f, -0.7f), [this]() { pParent_->AddUIManager(new ExitUIManager(pParent_)); });
}

ResultUIManager::~ResultUIManager()
{
}

void ResultUIManager::Draw()
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
