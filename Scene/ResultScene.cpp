#include "ResultScene.h"
#include "../Engine/SceneManager.h"
#include "../Engine/Input.h"
#include "../Engine/Camera.h"
#include "../AudioManager.h"
#include "../UI/ResultUIManager.h"
#include "../GameManager.h"
#include "../Stage/CreateStage.h"
#include "../Player/PlayerData.h"

ResultScene::ResultScene(GameObject* parent)
	: SceneBase(parent, "ResultScene")
{
}

ResultScene::~ResultScene()
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

	UIUpdate();

}

void ResultScene::Draw()
{
	UIDraw();

}

void ResultScene::Release()
{
}