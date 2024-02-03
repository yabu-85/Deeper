#include "UIManager.h"
#include "UIBase.h"
#include "../Engine/Input.h"
#include "../Scene/SceneBase.h"

UIManager::UIManager(SceneBase* parent)
	: state_(UI_STATE::DRAW), pParent_(parent)
{
}

UIManager::~UIManager()
{
	for (auto u : uiList_)
	{
		delete u;
	}
	uiList_.clear();
}

void UIManager::Update()
{
	for (auto u : uiList_)
	{
		if (u->IsWithinBound()) {
			if (Input::IsMouseButtonUp(0)) {
				u->OnClick();
			}
		}
	}

}

void UIManager::Draw()
{
	for (auto u : uiList_)
	{
		u->Draw();
	}

}

void UIManager::AddUi(std::string name, XMFLOAT2 pos, std::function<void()> onClick)
{
	UIBase* ui = new UIBase();
	ui->Initialize(name, pos, onClick);
	uiList_.push_back(ui);
}
