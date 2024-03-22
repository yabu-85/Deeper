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
#include "../Player/Player.h"

TitleScene::TitleScene(GameObject* parent)
	: SceneBase(parent, "TitleScene")
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{
	Camera::SetPosition(XMFLOAT3(15, 1, 15));
	Camera::SetTarget(XMFLOAT3(5, 2, 0)); 
	
	AudioManager::Initialize();
	AddUIManager(new TitleUIManager(this));

	GameManager::GetCreateStage()->CreateStageData("Csv/Map1.csv");
	
	PlayerOrnament* po = InstantiateFront<PlayerOrnament>(GetParent());
	po->SetPosition(14.5f, 0, 13.0f);
	po->SetRotateY(230.0f);
	InstantiateFront<WeaponMainOrnament>(po);
	Model::SetAnimFrame(po->GetModelHandle(), 
		PLAYER_ANIMATION_DATA[(int)PLAYER_ANIMATION::IDLE].startFrame, PLAYER_ANIMATION_DATA[(int)PLAYER_ANIMATION::IDLE].endFrame, 1.0f);

}

void TitleScene::Update()
{
	if (Input::IsKeyDown(DIK_Z)) {
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