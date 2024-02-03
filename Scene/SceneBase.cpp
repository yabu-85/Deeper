#include "SceneBase.h"
#include "../UI/UIManager.h"

SceneBase::SceneBase(GameObject* parent, std::string name)
	: GameObject(parent, name)
{
}

void SceneBase::AddUIManager(UIManager* ma)
{
	pUIManagerList_.push_back(ma);
}