#include "StageBase.h"
#include "../GameManager/WaveManager.h"
#include "../GameManager/GameManager.h"
#include "../Stage/Warp.h"
#include "../Stage/CollisionMap.h"
#include "../Stage/CreateStage.h"
#include "../Stage/SkyBox.h"
#include "../Player/Player.h"
#include "../Enemy/EnemyManager.h"
#include "../UI/PlayUIManager.h"

StageBase::StageBase(GameObject* parent, std::string name)
	: SceneBase(parent, name), isCleared_(false)
{
	AddUIManager(new PlayUIManager(this));

}

void StageBase::Initialize()
{
}

void StageBase::Update()
{
}

void StageBase::Draw()
{
}

void StageBase::Release()
{
}

void StageBase::InitializeStage(std::string csv, std::string sky)
{
	GameManager::SetStage(this);
	GameManager::GetEnemyManager()->SetParent(this);
	GameManager::GetCreateStage()->CreateStageData(csv);
	GameManager::SetPlayer(Instantiate<Player>(this));
	GameManager::SetCollisionMap(Instantiate<CollisionMap>(this));
	GameManager::GetCollisionMap()->CreatIntersectDataTriangle();
	SkyBox* skyBox = InstantiateFront<SkyBox>(GetParent());
	skyBox->LoadModel(sky);
}

void StageBase::SetAllWarpValid(bool b)
{
	for (int i = 0; i < (int)warpList_.size(); i++) {
		warpList_[i]->SetValid(b);
	}
}

void StageBase::SetWarpStage(SCENE_ID* list)
{
	for (int i = 0; i < (int)warpList_.size(); i++) {
		warpList_[i]->SetWarpScene(list[i]);
	}
}

bool StageBase::IsClearStage()
{
	if (!isCleared_ && WaveManager::IsClearStage()) {
		isCleared_ = true;
		return true;
	}
	return false;
}
