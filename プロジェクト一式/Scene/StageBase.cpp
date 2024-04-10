#include "StageBase.h"
#include "../GameManager/GameManager.h"
#include "../Stage/Warp.h"
#include "../Stage/CollisionMap.h"
#include "../Stage/CreateStage.h"
#include "../Stage/SkyBox.h"
#include "../Player/Player.h"
#include "../Enemy/EnemyManager.h"
#include "../UI/PauseUIManager.h"
#include "../Engine/Model.h"

StageBase::StageBase(GameObject* parent, std::string name)
	: SceneBase(parent, name), isCleared_(false), startPosition_{0.f,0.f,0.f}, pWarp_(nullptr)
{
	AddUIManager(new PauseUIManager(this));

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
	Model::Load("DebugCollision/SphereCollider.fbx");
	Model::Load("Model/stoneGolem.fbx");
	Model::Load("Model/walf.fbx");
	Model::Load("Model/RedBox.fbx");

	GameManager::SetStage(this);
	GameManager::GetEnemyManager()->SetParent(this);
	GameManager::GetCreateStage()->CreateStageData(csv);
	GameManager::SetPlayer(Instantiate<Player>(this));
	GameManager::SetCollisionMap(Instantiate<CollisionMap>(this));
	GameManager::GetCollisionMap()->CreatIntersectDataTriangle();
	SkyBox* skyBox = InstantiateFront<SkyBox>(GetParent());
	skyBox->LoadModel(sky);
}

void StageBase::SetWarpValid(bool b) { pWarp_->SetValid(b); }
void StageBase::SetWarpStage(SCENE_ID id) { pWarp_->SetWarpScene(id); }

bool StageBase::IsClearStage()
{
	if (!isCleared_ && GameManager::GetEnemyManager()->IsEnemyListEmpty()) {
		isCleared_ = true;
		return true;
	}
	return false;
}