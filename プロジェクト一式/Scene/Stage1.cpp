#include "Stage1.h"
#include "../Other/AudioManager.h"
#include "../GameManager/GameManager.h"
#include "../Engine/TransitionEffect.h"
#include "../Stage/CreateStage.h"

//デバッグ用
#include "../Weapon/WeaponObjectManager.h"
#include "../Enemy/EnemyManager.h"
#include "../Engine/Input.h"
#include "../Engine/SceneManager.h"

Stage1::Stage1(GameObject* parent)
	: StageBase(parent, "Stage1")
{
}

void Stage1::Initialize()
{	
	AudioManager::Initialize();
	TransitionEffect::SetFade(TRANSITION_TYPE::TYPE_ALPHA);
	TransitionEffect::SetAlphaDecrease(0.01f);
	
	InitializeStage("Csv/Map1.csv", "Model/Stage/SkyBox.fbx");
	SetWarpStage(SCENE_ID_STAGE2);	
	OnStageCleared();

#if _DEBUG
	//武器とかかし
	GameManager::GetWeaponObjectManager()->AddWeaponObject(ENEMY_TYPE::ENEMY_STONEGOLEM, GameManager::GetStage()->GetStartPosition());
	GameManager::GetEnemyManager()->SpawnEnemy(ENEMY_SCARECROW);
#endif

}

void Stage1::Update()
{
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
	SetWarpValid(true);

}
