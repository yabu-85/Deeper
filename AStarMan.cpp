#include "AStarMan.h"
#include "Engine/Model.h"
#include "NavigationAI.h"
#include "EnemyUi.h"
#include "Engine/SphereCollider.h"
#include "StateManager.h"
#include "FeetState.h"

#include "MoveAction.h"
#include "RotateAction.h"

#include <vector>
#include "Stage.h"
namespace {
	std::vector<std::vector<int>> mapData;
	
}

AStarMan::AStarMan(GameObject* parent)
	:EnemyBase(parent), hModel_(-1), targetPos_{0.0f, 0.0f, 0.0f}
{
}

AStarMan::~AStarMan()
{
}

void AStarMan::Initialize()
{
	//モデルデータのロード
	hModel_ = Model::Load("Model/SphereCollider.fbx");
	assert(hModel_ >= 0);
	
	aimTargetPos_ = 1.0f;
	
	transform_.position_ = XMFLOAT3(55.0f, 0.0f, 60.0f);
	//targetPos_ = XMFLOAT3((rand() % 24 + 1) * 5, 0.0f, (rand() % 24 + 1) * 5);
	targetPos_ = XMFLOAT3(5.0f, 0.0f, 5.0f);

	Stage* pStage = (Stage*)FindObject("Stage");
	mapData = pStage->GetMapData();

}

void AStarMan::Update()
{


}

void AStarMan::Draw()
{
	Transform draw = transform_;
	draw.position_.y += 0.5f;
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);

	Transform target;
	target.position_ = targetPos_;
	target.position_.y += 0.5f;
	Model::SetTransform(hModel_, target);
	Model::Draw(hModel_);

}

void AStarMan::Release()
{
}
