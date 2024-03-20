#include "ResultUIManager.h"
#include "ExitUIManager.h"
#include "../AudioManager.h"
#include "../Engine/Image.h"
#include "../Engine/SceneManager.h"
#include "../GameManager/GameManager.h"
#include "../Player/LifeManager.h"

namespace {
	XMFLOAT2 RESULT_POSITION = { 0.f, 0.5f };

}

ResultUIManager::ResultUIManager(SceneBase* parent) : UIManager(parent), hPict_(-1)
{
	resultTrans_.position_ = { RESULT_POSITION.x, RESULT_POSITION.y, 1.0f };

	const char* fileName[] = { "Image/GameOver.png", "Image/Clear.png" };
	if (LifeManager::IsDie()) hPict_ = Image::Load(fileName[0]);
	else hPict_ = Image::Load(fileName[1]);
	assert(hPict_ >= 0);

	AddUi("ReturnToTitle", XMFLOAT2(0.0f, 0.0f), XMFLOAT2(1.6f, 1.0f), [this]() { GameManager::GetSceneManager()->ChangeScene(SCENE_ID_TITLE); });
}

ResultUIManager::~ResultUIManager()
{
}

void ResultUIManager::Draw()
{
	Image::SetTransform(hPict_, resultTrans_);
	Image::Draw(hPict_, 0);

	UIManager::Draw();
}
