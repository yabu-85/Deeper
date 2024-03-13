#include "ExitUIManager.h"
#include "../Engine/Image.h"
#include "../Engine/Direct3D.h"

namespace {
	XMFLOAT2 EXIT_POSITION = { 0.f, 0.3f };

}

ExitUIManager::ExitUIManager(SceneBase* parent)
	: UIManager(parent), hPict_(-1)
{
	hPict_ = Image::Load("Image/ExitGame.png");
	assert(hPict_ >= 0);

	exitTrans_.position_ = { EXIT_POSITION.x, EXIT_POSITION.y, 1.0f };

	AddUi("Ok", XMFLOAT2(0.0f, -0.25f), [this]() { PostQuitMessage(0); });
	AddUi("Back", XMFLOAT2(0.0f, -0.6f), [this]() { state_ = UI_STATE::ENDDRAW; });
}

ExitUIManager::~ExitUIManager()
{
}

void ExitUIManager::Draw()
{
	Image::SetTransform(hPict_, exitTrans_);
	Image::Draw(hPict_, 0);

	UIManager::Draw();
}