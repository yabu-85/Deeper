#include "Stage1.h"
#include "../AudioManager.h"
#include "../GameManager/GameManager.h"
#include "../Engine/TransitionEffect.h"
#include "../Stage/CreateStage.h"

//�f�o�b�O�p
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
	SCENE_ID WARP_STAGE[] = { SCENE_ID_STAGE2 };
	SetWarpStage(WARP_STAGE);
	
	OnStageCleared();

	//�f�o�b�O�p
	GameManager::GetWeaponObjectManager()->AddWeaponObject(WeaponObjectManager::WEAPON_TYPE::WT_STONE, GameManager::GetStage()->GetStartPosition());

}

void Stage1::Update()
{
	//�f�o�b�O�p
	if (Input::IsKeyDown(DIK_X)) {
		SceneManager* pSceneManager = GameManager::GetSceneManager();
		pSceneManager->ChangeScene(SCENE_ID_TITLE);
	}

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
	SetAllWarpValid(true);

}
