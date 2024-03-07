#include "TitleScene.h"
#include "../Engine/SceneManager.h"
#include "../Engine/Input.h"
#include "../AudioManager.h"
#include "../UI/TitleUIManager.h"
#include "../GameManager/GameManager.h"
#include "../Stage/CreateStage.h"
#include "../Engine/Camera.h"
#include "../Engine/Model.h"
#include "../Ornament/PlayerOrnament.h"
#include "../Ornament/WeaponMainOrnament.h"

TitleScene::TitleScene(GameObject* parent)
	: SceneBase(parent, "TitleScene")
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{
	Camera::SetPosition(XMFLOAT3(10, 1, 15));
	Camera::SetTarget(XMFLOAT3(10, 5, 0)); 
	
	AudioManager::Initialize();
	AddUIManager(new TitleUIManager(this));

	GameManager::GetCreateStage()->CreateStageData("Csv/Map1.csv");
	PlayerOrnament* po = InstantiateFront<PlayerOrnament>(GetParent());
	po->SetPosition(12.0f, 0, 10.0f);
	po->SetRotateY(200.0f);
	InstantiateFront<WeaponMainOrnament>(po);
	Model::SetAnimFrame(po->GetModelHandle(), 0, 120, 1.0f);

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
	GameManager::GetCreateStage()->Draw();
	UIDraw();

}

void TitleScene::Release()
{
}