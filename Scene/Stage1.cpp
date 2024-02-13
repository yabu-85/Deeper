#include "Stage1.h"
#include "../GameManager.h"
#include "../Engine/SceneManager.h"
#include "../Player/Player.h"
#include "../Stage/CollisionMap.h"
#include "../Stage/CreateStage.h"
#include "../AudioManager.h"
#include "../Engine/Input.h"
#include "../Stage/Warp.h"

//デバッグ用
#include "../Weapon/WeaponObjectManager.h"
#include "../Player/PlayerWeapon.h"
#include "../Engine/Model.h"

namespace {
	static const SCENE_ID WARP_STAGE[2] = { SCENE_ID_STAGE2 };

}

Stage1::Stage1(GameObject* parent)
	: StageBase(parent, "Stage1")
{
}

void Stage1::Initialize()
{
	Model::Load("DebugCollision/SphereCollider.fbx"); 
	Model::Load("Model/stoneGolem.fbx");
	AudioManager::Initialize();

	GameManager::SetStage(this);
	GameManager::GetCreateStage()->CreateStageData("Csv/Map1.csv");
	GameManager::SetPlayer(Instantiate<Player>(this));
	GameManager::SetCollisionMap(Instantiate<CollisionMap>(this));
	GameManager::GetCollisionMap()->CreatIntersectDataTriangle();

	for (int i = 0; i < (int)warpList_.size(); i++) {
		warpList_[i]->SetWarpScene(WARP_STAGE[i]);
	}

	OnStageCleared();

	//デバッグ用
	GameManager::GetWeaponObjectManager()->AddWeaponObject(WeaponObjectManager::WEAPON_TYPE::WT_SUB1, GameManager::GetCreateStage()->GetPlayerStartPos());

}

void Stage1::Update()
{
	//デバッグ用
	if (Input::IsKeyDown(DIK_C)) {
		SceneManager* pSceneManager = GameManager::GetSceneManager();
		pSceneManager->ChangeScene(SCENE_ID_TITLE);
	}

}

void Stage1::Draw()
{
	GameManager::GetCreateStage()->Draw();

}

void Stage1::Release()
{
	AudioManager::Release();

}

void Stage1::OnStageCleared()
{
	for (int i = 0; i < (int)warpList_.size(); i++) {
		warpList_[i]->SetValid(true);
	}

}
