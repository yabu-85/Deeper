#include "SubPlayScene.h"
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
	static const SCENE_ID WARP_STAGE[1] = { SCENE_ID_PLAY2 };

}

SubPlayScene::SubPlayScene(GameObject* parent)
	: StageBase(parent)
{
	objectName_ = "SubPlayScene";

}

void SubPlayScene::Initialize()
{
	AudioManager::Initialize();

	GameManager::SetStage(this);
	GameManager::GetCreateStage()->CreateStageData("Csv/Map2.csv");
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

void SubPlayScene::Update()
{
	//デバッグ用
	if (Input::IsKeyDown(DIK_C)) {
		SceneManager* pSceneManager = static_cast<SceneManager*>(FindObject("SceneManager"));
		pSceneManager->ChangeScene(SCENE_ID_TITLE);
	}

}

void SubPlayScene::Draw()
{
	GameManager::Draw();
	GameManager::GetCreateStage()->Draw();

}

void SubPlayScene::Release()
{
	AudioManager::Release();

}

void SubPlayScene::OnStageCleared()
{
	for (int i = 0; i < (int)warpList_.size(); i++) {
		warpList_[i]->SetValid(true);
	}

}
