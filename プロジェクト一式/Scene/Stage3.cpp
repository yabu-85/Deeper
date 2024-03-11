#include "Stage3.h"
#include "../GameManager/GameManager.h"
#include "../GameManager/WaveManager.h"
#include "../Player/Player.h"
#include "../Stage/CollisionMap.h"
#include "../Stage/CreateStage.h"
#include "../Stage/Warp.h"
#include "../Stage/SkyBox.h"
#include "../AudioManager.h"
#include "../Engine/TransitionEffect.h"
#include "../Engine/Input.h"
#include "../Engine/SceneManager.h"
#include "../Engine/Model.h"

//デバッグ用
#include "../Weapon/WeaponObjectManager.h"

Stage3::Stage3(GameObject* parent)
	: StageBase(parent, "Stage3")
{
}

void Stage3::Initialize()
{
	Model::Load("DebugCollision/SphereCollider.fbx");
	Model::Load("Model/stoneGolem.fbx");

	AudioManager::Initialize();
	TransitionEffect::SetFade(TRANSITION_TYPE::TYPE_ALPHA);
	TransitionEffect::SetAlphaDecrease(0.01f);

	InitializeStage("Csv/Map3.csv", "Model/Stage/SkyBox.fbx");
	SCENE_ID WARP_STAGE[] = { SCENE_ID_STAGE1, SCENE_ID_TITLE };
	SetWarpStage(WARP_STAGE);

	//デバッグ用
	GameManager::GetWeaponObjectManager()->AddWeaponObject(WeaponObjectManager::WEAPON_TYPE::WT_STONE, GameManager::GetStage()->GetStartPosition());

}

void Stage3::Update()
{
	UIUpdate();
	
	if (IsClearStage()) {
		OnStageCleared();

		//修正箇所、Stageの制御が全て出来たら直すべき場所
		GameManager::GetSceneManager()->ChangeScene(SCENE_ID_RESULT);
	}
	
	//デバッグ用
	if (Input::IsKeyDown(DIK_C)) {
		GameManager::GetSceneManager()->ChangeScene(SCENE_ID_TITLE);
	}
}

void Stage3::Draw()
{
	GameManager::GetCreateStage()->Draw();
	UIDraw();

}

void Stage3::Release()
{
	AudioManager::Release();

}

void Stage3::OnStageCleared()
{
	SetAllWarpValid(true);

}
