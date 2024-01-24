#include "SubPlayScene.h"
#include "GameManager.h"
#include "Engine/SceneManager.h"
#include "Player.h"
#include "CollisionMap.h"
#include "CreateStage.h"
#include "AudioManager.h"
#include "Engine/Input.h"
#include "Warp.h"
#include "EnemyManager.h"
#include "ActionImage.h"

//デバッグ用
#include "WeaponObjectManager.h"
#include "PlayerWeapon.h"

namespace {
	static const SCENE_ID WARP_STAGE[] = { SCENE_ID_PLAY2, SCENE_ID_PLAY0 };

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

	Instantiate<ActionImage>(this);
	
	for (int i = 0; i < (int)warpList_.size(); i++) {
		warpList_[i]->SetWarpScene(WARP_STAGE[i]);
	}

	GameManager::GetEnemyManager()->SpawnEnemy(ENEMY_ASTAR);
	GameManager::GetEnemyManager()->SpawnEnemy(ENEMY_ASTAR);
	GameManager::GetEnemyManager()->SpawnEnemy(ENEMY_FEET);
	GameManager::GetEnemyManager()->SpawnEnemy(ENEMY_FEET);

	//デバッグ用
	GameManager::GetWeaponObjectManager()->AddWeaponObject(WeaponObjectManager::WEAPON_TYPE::WT_SUB1, GameManager::GetCreateStage()->GetPlayerStartPos());

}

void SubPlayScene::Update()
{
	//デバッグ用
	if (Input::IsKeyDown(DIK_C)) {
		SceneManager* pSceneManager = static_cast<SceneManager*>(FindObject("SceneManager"));
		pSceneManager->ChangeScene(SCENE_ID_TITLE);
	}

	if (!isCleared_ && GameManager::GetEnemyManager()->IsEnemyListEmpty()) {
		isCleared_ = true;
		OnStageCleared();
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
