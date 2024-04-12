#include "Stage3.h"
#include "../AudioManager.h"
#include "../Engine/TransitionEffect.h"
#include "../GameManager/GameManager.h"
#include "../Stage/CreateStage.h"
#include "../Stage/Warp.h"
#include "../Enemy/EnemyManager.h"

//デバッグ用
#include "../Weapon/WeaponObjectManager.h"
#include "../Engine/SceneManager.h"
#include "../Engine/Input.h"

Stage3::Stage3(GameObject* parent)
	: StageBase(parent, "Stage3")
{
}

void Stage3::Initialize()
{
	AudioManager::Initialize();
	TransitionEffect::SetFade(TRANSITION_TYPE::TYPE_ALPHA);
	TransitionEffect::SetAlphaDecrease(0.01f);

	InitializeStage("Csv/Map3.csv", "Model/Stage/SkyBox.fbx");
	SetWarpStage(SCENE_ID_STAGE1);
	GameManager::GetEnemyManager()->SpawnEnemyTable(ETABLE_NORMAL);

	//デバッグ用
	GameManager::GetWeaponObjectManager()->AddWeaponObject(ENEMY_TYPE::ENEMY_STONEGOLEM, GameManager::GetStage()->GetStartPosition());

}

void Stage3::Update()
{
	if (IsClearStage()) {
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
	//SetWarpValid(true);
	
	//修正箇所、Stageの制御が全て出来たら直すべき場所
	GameManager::GetSceneManager()->ChangeScene(SCENE_ID_RESULT);
}
