#include "UIManager.h"
#include "UIBase.h"
#include "Cursor.h"
#include "../Engine/Input.h"

UIManager::UIManager(SceneBase* parent)
	: state_(UI_STATE::DRAW), pParent_(parent), pCursor_(nullptr)
{
	pCursor_ = new Cursor();

}

UIManager::~UIManager()
{
	for (auto u : uiList_)
	{
		delete u;
	}
	uiList_.clear();

	delete pCursor_;
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

	pCursor_->Draw();
}

void UIManager::AddUi(std::string name, XMFLOAT2 pos, std::function<void()> onClick)
{
	XMFLOAT2 size = { 1.0f, 1.0f };
	AddUi(name, pos, size, onClick);
}

void UIManager::AddUi(std::string name, XMFLOAT2 pos, XMFLOAT2 size, std::function<void()> onClick)
{
	UIBase* ui = new UIBase();
	ui->Initialize(name, pos, size, onClick);
	uiList_.push_back(ui);
}
