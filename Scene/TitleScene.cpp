#include "TitleScene.h"
#include "../Engine/SceneManager.h"
#include "../Engine/Input.h"
#include "../AudioManager.h"
#include "../UI/UIManager.h"
#include "../UI/TitleUIManager.h"
#include "../UI/ExitUIManager.h"

TitleScene::TitleScene(GameObject* parent)
	: GameObject(parent, "TitleScene")
{
}

void TitleScene::Initialize()
{
	AudioManager::Initialize();

	TitleUIManager* title = new TitleUIManager();
	//�V�[�����ڂ����炩�ɂ��������
	title->AddUi("Play", XMFLOAT2(0.0f, 0.0f), [this]() {
		SceneManager* pSceneManager = static_cast<SceneManager*>(FindObject("SceneManager"));
		pSceneManager->ChangeScene(SCENE_ID_STAGE1); }
	);
	title->AddUi("Option", XMFLOAT2(0.0f, -0.35f), [this]() { AudioManager::Play(); });
	title->AddUi("Exit", XMFLOAT2(0.0f, -0.7f), [this]() {
		ExitUIManager* exit = new ExitUIManager();
		pUIManagerList_.push_back(exit); }
	);
	pUIManagerList_.push_back(title);

}

void TitleScene::Update()
{
	if (Input::IsKeyDown(DIK_X)) {
		SceneManager* pSceneManager = static_cast<SceneManager*>(FindObject("SceneManager"));
		pSceneManager->ChangeScene(SCENE_ID_STAGE1);
		return;
	}

	if(!pUIManagerList_.empty())
	pUIManagerList_.back()->Update();

	// UI��delete
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