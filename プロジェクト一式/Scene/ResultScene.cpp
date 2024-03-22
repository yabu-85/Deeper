#include "ResultScene.h"
#include "../Engine/SceneManager.h"
#include "../Engine/Input.h"
#include "../Engine/Camera.h"
#include "../Engine/Model.h"
#include "../AudioManager.h"
#include "../UI/ResultUIManager.h"
#include "../GameManager/GameManager.h"
#include "../Stage/CreateStage.h"
#include "../Player/PlayerData.h"
#include "../Ornament/PlayerOrnament.h"
#include "../Ornament/WeaponMainOrnament.h"
#include "../Player/LifeManager.h"
#include "../Player/Player.h"

ResultScene::ResultScene(GameObject* parent)
	: SceneBase(parent, "ResultScene")
{
}

ResultScene::~ResultScene()
{
}

void ResultScene::Initialize()
{
	AudioManager::Initialize();
	AddUIManager(new ResultUIManager(this));

	PlayerData::ResetData();
	
	GameManager::GetCreateStage()->CreateStageData("Csv/Map1.csv");
	PlayerOrnament* po = InstantiateFront<PlayerOrnament>(GetParent());
	if (LifeManager::IsDie()) {
		Camera::SetPosition(XMFLOAT3(11, 3, 10));
		Camera::SetTarget(XMFLOAT3(15, 1, 13));

		po->SetPosition(14.5f, 0, 13.0f);
		po->SetRotateY(230.0f);
		InstantiateFront<WeaponMainOrnament>(po);
		Model::SetAnimFrame(po->GetModelHandle(), 
			PLAYER_ANIMATION_DATA[(int)PLAYER_ANIMATION::DEAD].endFrame, PLAYER_ANIMATION_DATA[(int)PLAYER_ANIMATION::DEAD].endFrame, 1.0f);
	}
	else {
		Camera::SetPosition(XMFLOAT3(15, 1, 15));
		Camera::SetTarget(XMFLOAT3(5, 2, 0));

		po->SetPosition(14.5f, 0, 13.0f);
		po->SetRotateY(230.0f);
		InstantiateFront<WeaponMainOrnament>(po);
		Model::SetAnimFrame(po->GetModelHandle(), 
			PLAYER_ANIMATION_DATA[(int)PLAYER_ANIMATION::IDLE].startFrame, PLAYER_ANIMATION_DATA[(int)PLAYER_ANIMATION::IDLE].endFrame, 1.0f);
	}
}

void ResultScene::Update()
{
	UIUpdate();

}

void ResultScene::Draw()
{
	GameManager::GetCreateStage()->Draw();
	UIDraw();

}

void ResultScene::Release()
{
}