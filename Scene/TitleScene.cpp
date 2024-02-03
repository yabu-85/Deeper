#include "TitleScene.h"
#include "../Engine/SceneManager.h"
#include "../Engine/Input.h"
#include "../AudioManager.h"
#include "../UI/UIManager.h"
#include "../UI/TitleUIManager.h"
#include "../UI/ExitUIManager.h"
#include "../GameManager.h"

TitleScene::TitleScene(GameObject* parent)
	: SceneBase(parent, "TitleScene")
{
}

void TitleScene::Initialize()
{
	AudioManager::Initialize();
	AddUIManager(new TitleUIManager(this));

}

void TitleScene::Update()
{
	if (Input::IsKeyDown(DIK_X)) {
		GameManager::GetSceneManager()->ChangeScene(SCENE_ID_STAGE1);
		return;
	}

	if(!pUIManagerList_.empty())
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

void TitleScene::Draw()
{
	if (!pUIManagerList_.empty())
	pUIManagerList_.back()->Draw();

}

void TitleScene::Release()
{
	for (auto u : pUIManagerList_)
	{
		delete u;
	}
	pUIManagerList_.clear();
	AudioManager::Release();

}