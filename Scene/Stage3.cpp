#include "Stage3.h"
#include "../GameManager.h"
#include "../Engine/SceneManager.h"
#include "../Player/Player.h"
#include "../Stage/CollisionMap.h"
#include "../Stage/CreateStage.h"
#include "../AudioManager.h"
#include "../Engine/Input.h"
#include "../Stage/Warp.h"
#include "../Stage/SkyBox.h"
#include "../Enemy/EnemyManager.h"
#include "../Engine/Model.h"

namespace {
	static const SCENE_ID WARP_STAGE[] = { SCENE_ID_STAGE1 };

}

Stage3::Stage3(GameObject* parent)
	: StageBase(parent, "Stage3")
{
}

void Stage3::Initialize()
{
	Model::Load("DebugCollision/SphereCollider.fbx");
	Model::Load("Model/stoneGolem.fbx");
	AudioManager::Initialize();

	GameManager::SetStage(this);
	GameManager::GetCreateStage()->CreateStageData("Csv/Map3.csv");
	GameManager::SetPlayer(Instantiate<Player>(this));
	GameManager::SetCollisionMap(Instantiate<CollisionMap>(this));
	GameManager::GetCollisionMap()->CreatIntersectDataTriangle();
	SkyBox* sky = InstantiateFront<SkyBox>(GetParent());
	sky->LoadModel("Model/Stage/SkyBox.fbx");

	for (int i = 0; i < (int)warpList_.size(); i++) {
		warpList_[i]->SetWarpScene(WARP_STAGE[i]);
	}

	GameManager::GetEnemyManager()->SpawnEnemy(ENEMY_STONEGOLEM);
	GameManager::GetEnemyManager()->SpawnEnemy(ENEMY_STONEGOLEM);

}

void Stage3::Update()
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

void Stage3::Draw()
{
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
