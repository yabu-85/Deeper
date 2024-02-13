#include "ResultScene.h"
#include "../Engine/SceneManager.h"
#include "../Engine/Input.h"
#include "../Engine/Camera.h"
#include "../AudioManager.h"
#include "../UI/ResultUIManager.h"
#include "../UI/ExitUIManager.h"
#include "../GameManager.h"
#include "../Stage/CreateStage.h"
#include "../Player/PlayerData.h"

namespace {
	CreateStage* pCreateStage = nullptr;

}

ResultScene::ResultScene(GameObject* parent)
	: SceneBase(parent, "ResultScene")
{
}

void ResultScene::Initialize()
{
	Camera::SetPosition(XMFLOAT3(10, 10, 15));
	Camera::SetTarget(XMFLOAT3(10, 0, 0));

	AudioManager::Initialize();
	AddUIManager(new ResultUIManager(this));

	GameManager::GetCreateStage()->CreateStageData("Csv/Map1.csv");
	PlayerData::ResetData();

}

void ResultScene::Update()
{
	if (Input::IsKeyDown(DIK_Z)) {
		GameManager::GetSceneManager()->ChangeScene(SCENE_ID_TITLE);
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

void ResultScene::Draw()
{
	GameManager::GetCreateStage()->Draw();
	
	if (!pUIManagerList_.empty())
	pUIManagerList_.back()->Draw();

}

void ResultScene::Release()
{
	for (auto u : pUIManagerList_)
	{
		delete u;
	}
	pUIManagerList_.clear();
	AudioManager::Release();

}