#include "PauseUIManager.h"
#include "ExitUIManager.h"
#include "../Engine/Image.h"
#include "../Engine/Direct3D.h"
#include "../Engine/SceneManager.h"
#include "../GameManager/GameManager.h"
#include "../AudioManager.h"

namespace {
	XMFLOAT2 PAUSE_POSITION = { 0.f, 0.f };

}

PauseUIManager::PauseUIManager(SceneBase* parent)
	: UIManager(parent), hPict_(-1)
{
	hPict_ = Image::Load("Image/Pause.png");
	assert(hPict_ >= 0);

	pauseTrans_.position_ = { PAUSE_POSITION.x, PAUSE_POSITION.y, 1.0f };

	AddUi("ReturnGame", XMFLOAT2(0.0f, -0.25f), XMFLOAT2(1.5f, 1.0f), [this]() { GameManager::PauseClose(); });
	AddUi("Exit", XMFLOAT2(0.0f, -0.6f), [this]() { pParent_->AddUIManager(new ExitUIManager(pParent_)); });

}

PauseUIManager::~PauseUIManager()
{
}

void PauseUIManager::Draw()
{
	Image::SetTransform(hPict_, pauseTrans_);
	Image::Draw(hPict_, 0);

	UIManager::Draw();
}