#include "PlayScene.h"
#include "GameManager.h"
#include "Engine/SceneManager.h"
#include "Player.h"
#include "CollisionMap.h"
#include "CreateStage.h"
#include "AudioManager.h"
#include "Engine/Input.h"
#include "Warp.h"

//デバッグ用
#include "WeaponObjectManager.h"
#include "PlayerWeapon.h"

namespace {
	static const SCENE_ID WARP_STAGE[2] = { SCENE_ID_PLAY1, SCENE_ID_PLAY2 };

}

PlayScene::PlayScene(GameObject* parent)
	: StageBase(parent)
{
	objectName_ = "PlayScene";

}

void PlayScene::Initialize()
{
	AudioManager::Initialize();

	GameManager::SetStage(this);
	GameManager::GetCreateStage()->CreateStageData("Csv/Map1.csv");
	GameManager::SetPlayer(Instantiate<Player>(this));
	GameManager::SetCollisionMap(Instantiate<CollisionMap>(this));
	GameManager::GetCollisionMap()->CreatIntersectDataTriangle();

	for (int i = 0; i < (int)warpList_.size(); i++) {
		warpList_[i]->SetWarpScene(WARP_STAGE[i]);
	}

	//デバッグ用
	GameManager::GetWeaponObjectManager()->AddWeaponObject(WeaponObjectManager::WEAPON_TYPE::WT_SUB1, GameManager::GetCreateStage()->GetPlayerStartPos());

	OnStageCleared();

}

void PlayScene::Update()
{
	//デバッグ用
	if (Input::IsKeyDown(DIK_C)) {
		SceneManager* pSceneManager = static_cast<SceneManager*>(FindObject("SceneManager"));
		pSceneManager->ChangeScene(SCENE_ID_TITLE);
	}

}

void PlayScene::Draw()
{
	GameManager::Draw();
	GameManager::GetCreateStage()->Draw();

}

void PlayScene::Release()
{
	AudioManager::Release();

}

void PlayScene::OnStageCleared()
{
	for (int i = 0; i < (int)warpList_.size(); i++) {
		warpList_[i]->SetValid(true);
	}

}
