#include "Stage3.h"
#include "GameManager.h"
#include "Engine/SceneManager.h"
#include "Player.h"
#include "CollisionMap.h"
#include "CreateStage.h"
#include "AudioManager.h"
#include "Engine/Input.h"
#include "Warp.h"
#include "EnemyManager.h"

namespace {
	static const SCENE_ID WARP_STAGE[] = { SCENE_ID_PLAY0 };

}

Stage3::Stage3(GameObject* parent)
	: StageBase(parent)
{
	objectName_ = "Stage3";

}

void Stage3::Initialize()
{
	AudioManager::Initialize();

	GameManager::SetStage(this);
	GameManager::GetCreateStage()->CreateStageData("Csv/Map3.csv");
	GameManager::SetPlayer(Instantiate<Player>(this));
	GameManager::SetCollisionMap(Instantiate<CollisionMap>(this));
	GameManager::GetCollisionMap()->CreatIntersectDataTriangle();
	
	for (int i = 0; i < (int)warpList_.size(); i++) {
		warpList_[i]->SetWarpScene(WARP_STAGE[i]);
	}

	GameManager::GetEnemyManager()->SpawnEnemy(ENEMY_FEET);
	GameManager::GetEnemyManager()->SpawnEnemy(ENEMY_FEET);

}

void Stage3::Update()
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

void Stage3::Draw()
{
	GameManager::Draw();
	GameManager::GetCreateStage()->Draw();

}

void Stage3::Release()
{
	AudioManager::Release();

}

void Stage3::OnStageCleared()
{
	for (int i = 0; i < (int)warpList_.size(); i++) {
		warpList_[i]->SetValid(true);
	}

}
