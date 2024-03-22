#include "Stage2.h"
#include "../AudioManager.h"
#include "../Engine/TransitionEffect.h"
#include "../GameManager/GameManager.h"
#include "../Stage/CreateStage.h"
#include "../Stage/Warp.h"

//デバッグ用
#include "../Weapon/WeaponObjectManager.h"
#include "../Engine/SceneManager.h"
#include "../Engine/Input.h"

Stage2::Stage2(GameObject* parent)
	: StageBase(parent, "Stage2")
{
}

void Stage2::Initialize()
{
	AudioManager::Initialize();
	TransitionEffect::SetFade(TRANSITION_TYPE::TYPE_ALPHA);
	TransitionEffect::SetAlphaDecrease(0.01f);

	InitializeStage("Csv/Map2.csv", "Model/Stage/SkyBox.fbx");
	SCENE_ID WARP_STAGE[] = { SCENE_ID_STAGE3, SCENE_ID_STAGE1 };
	SetWarpStage(WARP_STAGE);

	//デバッグ用
	GameManager::GetWeaponObjectManager()->AddWeaponObject(WeaponObjectManager::WEAPON_TYPE::WT_STONE, GameManager::GetStage()->GetStartPosition());

}

void Stage2::Update()
{
	if (IsClearStage()) {
		OnStageCleared();
	}

	//デバッグ用
	if (Input::IsKeyDown(DIK_X)) {
		GameManager::GetSceneManager()->ChangeScene(SCENE_ID_TITLE);
	}
}

void Stage2::Draw()
{
	GameManager::GetCreateStage()->Draw();

}

void Stage2::Release()
{
	AudioManager::Release();

}

void Stage2::OnStageCleared()
{
	SetAllWarpValid(true);

}
