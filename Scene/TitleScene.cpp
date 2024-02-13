#include "TitleScene.h"
#include "../Engine/SceneManager.h"
#include "../Engine/Input.h"
#include "../AudioManager.h"
#include "../UI/TitleUIManager.h"
#include "../GameManager.h"

TitleScene::TitleScene(GameObject* parent)
	: SceneBase(parent, "TitleScene")
{
}

TitleScene::~TitleScene()
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
	if (Input::IsKeyDown(DIK_C)) {
		GameManager::GetSceneManager()->ChangeScene(SCENE_ID_RESULT);
		return;
	}

	UIUpdate();

}

void TitleScene::Draw()
{
	UIDraw();

}

void TitleScene::Release()
{
}