#include "PauseUIManager.h"
#include "ExitUIManager.h"
#include "../Other/AudioManager.h"
#include "../Engine/Image.h"
#include "../Engine/SceneManager.h"
#include "../GameManager/GameManager.h"

namespace {
	XMFLOAT2 PAUSE_POSITION = { 0.f, 0.5f };

}

PauseUIManager::PauseUIManager(SceneBase* parent)
	: UIManager(parent), hPict_{-1, -1}
{
	const char* fileName[] = { "Image/Pause.png", "Image/WhiteFade.png" };
	for (int i = 0; i < 2; i++) {
		hPict_[i] = Image::Load(fileName[i]);
		assert(hPict_[i] >= 0);
	}

	pauseTrans_.position_ = { PAUSE_POSITION.x, PAUSE_POSITION.y, 1.0f };

	AddUi("ReturnGame", XMFLOAT2(0.0f, 0.1f), XMFLOAT2(1.5f, 1.0f), [this]() { GameManager::PauseClose(); });
	AddUi("ReturnToTitle", XMFLOAT2(0.0f, -0.2f), XMFLOAT2(1.6f, 1.0f), [this]() { 
		GameManager::GetSceneManager()->ChangeScene(SCENE_ID_TITLE); }
	);
	AddUi("Exit", XMFLOAT2(0.0f, -0.5f), [this]() { pParent_->AddUIManager(new ExitUIManager(pParent_)); });

}

PauseUIManager::~PauseUIManager()
{
}

void PauseUIManager::Draw()
{
	Image::SetFullScreenTransform(hPict_[1]);
	Image::SetAlpha(hPict_[1], 150);
	Image::Draw(hPict_[1]);

	Image::SetTransform(hPict_[0], pauseTrans_);
	Image::Draw(hPict_[0]);

	UIManager::Draw();
}