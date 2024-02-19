#include "Stage2.h"
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
#include "../WaveManager.h"

//�f�o�b�O�p
#include "../Weapon/WeaponObjectManager.h"
#include "../Player/PlayerWeapon.h"
#include "../Engine/Model.h"

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
	WaveManager::SetStageData();

	for (int i = 0; i < (int)warpList_.size(); i++) {
		warpList_[i]->SetWarpScene(WARP_STAGE[i]);
	}

	//�f�o�b�O�p
	GameManager::GetWeaponObjectManager()->AddWeaponObject(WeaponObjectManager::WEAPON_TYPE::WT_STONE, GameManager::GetCreateStage()->GetPlayerStartPos());

}

void Stage2::Update()
{
	WaveManager::Update();

	if (!isCleared_ && WaveManager::IsClearStage()) {
		isCleared_ = true;
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
