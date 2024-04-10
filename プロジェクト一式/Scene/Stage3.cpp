#include "Stage3.h"
#include "../AudioManager.h"
#include "../Engine/TransitionEffect.h"
#include "../GameManager/GameManager.h"
#include "../Stage/CreateStage.h"
#include "../Stage/Warp.h"
#include "../Enemy/EnemyManager.h"

//�f�o�b�O�p
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

	//�f�o�b�O�p
	GameManager::GetWeaponObjectManager()->AddWeaponObject(WeaponObjectManager::WEAPON_TYPE::WT_STONE, GameManager::GetStage()->GetStartPosition());

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
	
	//�C���ӏ��AStage�̐��䂪�S�ďo�����璼���ׂ��ꏊ
	GameManager::GetSceneManager()->ChangeScene(SCENE_ID_RESULT);
}
