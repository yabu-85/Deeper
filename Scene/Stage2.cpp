#include "Stage2.h"
#include "../GameManager.h"
#include "../WaveManager.h"
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

namespace {
	static const SCENE_ID WARP_STAGE[] = { SCENE_ID_STAGE3, SCENE_ID_STAGE1 };

}

Stage2::Stage2(GameObject* parent)
	: StageBase(parent, "Stage2")
{
}

void Stage2::Initialize()
{
	Model::Load("DebugCollision/SphereCollider.fbx");
	Model::Load("Model/stoneGolem.fbx");
	AudioManager::Initialize();

	GameManager::SetStage(this);
	GameManager::GetCreateStage()->CreateStageData("Csv/Map2.csv");
	Instantiate<Player>(this);
	GameManager::SetCollisionMap(Instantiate<CollisionMap>(this));
	GameManager::GetCollisionMap()->CreatIntersectDataTriangle();
	SkyBox* sky = InstantiateFront<SkyBox>(GetParent());
	sky->LoadModel("Model/Stage/SkyBox.fbx");

	TransitionEffect::SetFade(TRANSITION_TYPE::TYPE_ALPHA);
	TransitionEffect::SetAlphaDecrease(0.01f);

	for (int i = 0; i < (int)warpList_.size(); i++) {
		warpList_[i]->SetWarpScene(WARP_STAGE[i]);
	}

	//デバッグ用
	GameManager::GetWeaponObjectManager()->AddWeaponObject(WeaponObjectManager::WEAPON_TYPE::WT_STONE, GameManager::GetCreateStage()->GetPlayerStartPos());

}

void Stage2::Update()
{
	if (IsClearStage()) {
		OnStageCleared();
	}

	//デバッグ用
	if (Input::IsKeyDown(DIK_C)) {
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
	for (int i = 0; i < (int)warpList_.size(); i++) {
		warpList_[i]->SetValid(true);
	}

}
