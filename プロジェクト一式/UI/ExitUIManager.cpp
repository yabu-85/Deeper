#include "ExitUIManager.h"
#include "../Engine/Image.h"

namespace {
	XMFLOAT2 EXIT_POSITION = { 0.f, 0.3f };

}

ExitUIManager::ExitUIManager(SceneBase* parent)
	: UIManager(parent), hPict_{-1,-1}
{
	const char* fileName[] = { "Image/ExitGame.png", "Image/WhiteFade.png" };
	for (int i = 0; i < 2; i++) {
		hPict_[i] = Image::Load(fileName[i]);
		assert(hPict_[i] >= 0);
	}

	exitTrans_.position_ = { EXIT_POSITION.x, EXIT_POSITION.y, 1.0f };

	AddUi("Ok", XMFLOAT2(0.0f, -0.25f), [this]() { PostQuitMessage(0); });
	AddUi("Back", XMFLOAT2(0.0f, -0.6f), [this]() { state_ = UI_STATE::ENDDRAW; });
}

ExitUIManager::~ExitUIManager()
{
}

void ExitUIManager::Draw()
{
	Image::SetFullScreenTransform(hPict_[1]);
	Image::SetAlpha(hPict_[1], 150);
	Image::Draw(hPict_[1], 0);

	Image::SetTransform(hPict_[0], exitTrans_);
	Image::Draw(hPict_[0], 0);

	UIManager::Draw();
}