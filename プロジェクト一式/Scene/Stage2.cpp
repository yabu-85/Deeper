#include "Stage2.h"
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

//�f�o�b�O�p
#include "../Weapon/WeaponObjectManager.h"

Stage2::Stage2(GameObject* parent)
	: StageBase(parent, "Stage2")
{
}

void Stage2::Initialize()
{
	Model::Load("DebugCollision/SphereCollider.fbx");
	Model::Load("Model/stoneGolem.fbx");

	AudioManager::Initialize();
	TransitionEffect::SetFade(TRANSITION_TYPE::TYPE_ALPHA);
	TransitionEffect::SetAlphaDecrease(0.01f);

	InitializeStage("Csv/Map2.csv", "Model/Stage/SkyBox.fbx");
	SCENE_ID WARP_STAGE[] = { SCENE_ID_STAGE3, SCENE_ID_STAGE1 };
	SetWarpStage(WARP_STAGE);

	//�f�o�b�O�p
	GameManager::GetWeaponObjectManager()->AddWeaponObject(WeaponObjectManager::WEAPON_TYPE::WT_STONE, GameManager::GetStage()->GetStartPosition());

}

void Stage2::Update()
{
	UIUpdate();
	
	if (IsClearStage()) {
		OnStageCleared();
	}

	//�f�o�b�O�p
	if (Input::IsKeyDown(DIK_C)) {
		GameManager::GetSceneManager()->ChangeScene(SCENE_ID_TITLE);
	}
}

void Stage2::Draw()
{
	GameManager::GetCreateStage()->Draw();
	UIDraw();

}

void Stage2::Release()
{
	AudioManager::Release();

}

void Stage2::OnStageCleared()
{
	SetAllWarpValid(true);

}
