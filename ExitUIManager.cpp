#include "ExitUIManager.h"
#include "Engine/Image.h"
#include "Engine/Direct3D.h"

ExitUIManager::ExitUIManager()
	:UIManager(), hPict_(-1)
{
	AddUi("Ok", XMFLOAT2(0.0f, 0.0f), [this]() {
		PostQuitMessage(0);
	});

	AddUi("Back", XMFLOAT2(0.0f, -0.35f), [this]() {
		state_ = UI_STATE::ENDDRAW;
	});

}

ExitUIManager::~ExitUIManager()
{
	UIManager::~UIManager();

}

void ExitUIManager::Draw()
{
	UIManager::Draw();
}
