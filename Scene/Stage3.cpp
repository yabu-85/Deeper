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

//�f�o�b�O�p
#include "../Weapon/WeaponObjectManager.h"

namespace {
	static const SCENE_ID WARP_STAGE[] = { SCENE_ID_STAGE1, SCENE_ID_TITLE };

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
	Instantiate<Player>(this);
	GameManager::SetCollisionMap(Instantiate<CollisionMap>(this));
	GameManager::GetCollisionMap()->CreatIntersectDataTriangle();
	SkyBox* sky = InstantiateFront<SkyBox>(GetParent());
	sky->LoadModel("Model/Stage/SkyBox.fbx");
	WaveManager::SetStageData();

	TransitionEffect::SetFade(TRANSITION_TYPE::TYPE_ALPHA);
	TransitionEffect::SetAlphaDecrease(0.01f);

	for (int i = 0; i < (int)warpList_.size(); i++) {
		warpList_[i]->SetWarpScene(WARP_STAGE[i]);
	}

	//�f�o�b�O�p
	GameManager::GetWeaponObjectManager()->AddWeaponObject(WeaponObjectManager::WEAPON_TYPE::WT_STONE, GameManager::GetCreateStage()->GetPlayerStartPos());

}

void Stage3::Update()
{
	if (IsClearStage()) {
		OnStageCleared();

		//�C���ӏ��AStage�̐��䂪�S�ďo�����璼���ׂ��ꏊ
		GameManager::GetSceneManager()->ChangeScene(SCENE_ID_RESULT);
	}
	
	//�f�o�b�O�p
	if (Input::IsKeyDown(DIK_C)) {
		GameManager::GetSceneManager()->ChangeScene(SCENE_ID_TITLE);
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
