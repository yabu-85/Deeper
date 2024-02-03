#include "Stage2.h"
#include "../GameManager.h"
#include "../Engine/SceneManager.h"
#include "../Player/Player.h"
#include "../Stage/CollisionMap.h"
#include "../Stage/CreateStage.h"
#include "../AudioManager.h"
#include "../Engine/Input.h"
#include "../Stage/Warp.h"
#include "../Enemy/EnemyManager.h"

//デバッグ用
#include "../Weapon/WeaponObjectManager.h"
#include "../Player/PlayerWeapon.h"

namespace {
	static const SCENE_ID WARP_STAGE[] = { SCENE_ID_STAGE3, SCENE_ID_STAGE1 };

}

Stage2::Stage2(GameObject* parent)
	: StageBase(parent, "Stage2")
{
}

void Stage2::Initialize()
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

	GameManager::GetEnemyManager()->SpawnEnemy(ENEMY_FEET);
	GameManager::GetEnemyManager()->SpawnEnemy(ENEMY_FEET);

	//デバッグ用
	GameManager::GetWeaponObjectManager()->AddWeaponObject(WeaponObjectManager::WEAPON_TYPE::WT_SUB1, GameManager::GetCreateStage()->GetPlayerStartPos());

}

void Stage2::Update()
{
	//デバッグ用
	if (Input::IsKeyDown(DIK_C)) {
		GameManager::GetSceneManager()->ChangeScene(SCENE_ID_TITLE);
	}

	if (!isCleared_ && GameManager::GetEnemyManager()->IsEnemyListEmpty()) {
		isCleared_ = true;
		OnStageCleared();
	}

}

void Stage2::Draw()
{
	GameManager::Draw();
	GameManager::GetCreateStage()->Draw();

}

void Stage2::Release()
{
	AudioManager::Release();

}

void Stage2::OnStageCleared()
{
	for (int i = 0; i < (int)warpList_.size(); i++) {
		warpList_[i]->SetValid(true);
	}

}
