#include "TitleUIManager.h"
#include "ExitUIManager.h"
#include "../AudioManager.h"
#include "../Engine/Image.h"
#include "../Engine/Direct3D.h"
#include "../Engine/SceneManager.h"
#include "../GameManager/GameManager.h"

namespace {
	XMFLOAT2 TITLE_POSITION = { 0.f, 0.3f };

}

TitleUIManager::TitleUIManager(SceneBase* parent) : UIManager(parent), hPict_(-1)
{
	hPict_ = Image::Load("Image/DEEPER.png");
	assert(hPict_ >= 0);

	titleTrans_.position_ = { TITLE_POSITION.x, TITLE_POSITION.y, 1.0f };

	AddUi("Play", XMFLOAT2(0.0f, -0.25f), [this]() { GameManager::GetSceneManager()->ChangeScene(SCENE_ID_STAGE1); });
	AddUi("Exit", XMFLOAT2(0.0f, -0.6f), [this]() { pParent_->AddUIManager(new ExitUIManager(pParent_)); });
	
}

TitleUIManager::~TitleUIManager()
{
}

void TitleUIManager::Draw()
{
	Image::SetTransform(hPict_, titleTrans_);
	Image::Draw(hPict_, 0);

	UIManager::Draw();
}
