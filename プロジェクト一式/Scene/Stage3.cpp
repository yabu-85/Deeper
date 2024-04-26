#include "Stage3.h"
#include "../Other/AudioManager.h"
#include "../Engine/TransitionEffect.h"
#include "../GameManager/GameManager.h"
#include "../Stage/CreateStage.h"
#include "../Stage/Warp.h"
#include "../Enemy/EnemyManager.h"

//デバッグ用
#include "../Weapon/WeaponObjectManager.h"
#include "../Engine/SceneManager.h"
#include "../Engine/Input.h"

namespace {
	static const int FADE_TIME = 100;

}

Stage3::Stage3(GameObject* parent)
	: StageBase(parent, "Stage3"), fadeTime_(0)
{
}

void Stage3::Initialize()
{
	AudioManager::Initialize();
	TransitionEffect::SetFade(TRANSITION_TYPE::TYPE_ALPHA);
	TransitionEffect::SetAlphaDecrease(0.01f);

	InitializeStage("Csv/Map3.csv", "Model/Stage/SkyBox.fbx");
	GameManager::GetEnemyManager()->SpawnEnemy(ENEMY_SWORDBOSS);
}

void Stage3::Update()
{
	if (IsClearStage()) {
		TransitionEffect::SetFade(TRANSITION_TYPE::TYPE_ALPHA);
		TransitionEffect::SetAlpha(0.0f);
		TransitionEffect::SetAlphaDecrease(-0.01f);
		OnStageCleared();
	}
	
	if (IsAlwaysClearStage()) {
		fadeTime_++;
		if (fadeTime_ > FADE_TIME) GameManager::GetSceneManager()->ChangeScene(SCENE_ID_RESULT);
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

}
