#include "SceneBase.h"
#include "../UI/UIManager.h"
#include "../AudioManager.h"

SceneBase::SceneBase(GameObject* parent, std::string name)
	: GameObject(parent, name)
{
}

SceneBase::~SceneBase()
{
	for (auto u : pUIManagerList_)
	{
		delete u;
	}
	pUIManagerList_.clear();
	AudioManager::Release();

}

void SceneBase::AddUIManager(UIManager* ma)
{
	pUIManagerList_.push_back(ma);
}

void SceneBase::UIUpdate()
{
	if (!pUIManagerList_.empty())
		pUIManagerList_.back()->Update();

	// UI‚Ìdelete
	for (auto iter = pUIManagerList_.begin(); iter != pUIManagerList_.end();) {
		if ((*iter)->GetUIState() == UIManager::UI_STATE::ENDDRAW)
		{
			delete* iter;
			iter = pUIManagerList_.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void SceneBase::UIDraw()
{
	if (!pUIManagerList_.empty()) pUIManagerList_.back()->Draw();
}
