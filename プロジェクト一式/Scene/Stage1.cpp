#include "Stage1.h"
#include "../AudioManager.h"
#include "../GameManager/GameManager.h"
#include "../Engine/TransitionEffect.h"
#include "../Stage/CreateStage.h"

//デバッグ用
#include "../Weapon/WeaponObjectManager.h"
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

	//デバッグ用
	GameManager::GetWeaponObjectManager()->AddWeaponObject(WeaponObjectManager::WEAPON_TYPE::WT_STONE, GameManager::GetStage()->GetStartPosition());

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
