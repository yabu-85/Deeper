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
#include "GameManager.h"
#include "Engine/Input.h"
#include "Player.h"
namespace {
	XMFLOAT3 currentTar{};
	float floarSize = 1.0f;
}

AStarMan::AStarMan(GameObject* parent)
	:EnemyBase(parent), hModel_(-1), pAstarMoveAction_(nullptr)
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
	
	pAstarMoveAction_ = new AstarMoveAction(this, 0.05f, 0.1f);
	pAstarMoveAction_->Initialize();

	struct Cell
	{
		float x, z;
	};
	std::vector<Cell> posList;
	Stage* pStage = (Stage*)FindObject("Stage");
	std::vector<std::vector<int>> mapData = pStage->GetMapData();
	for (int x = 0; x < 25; x++) {
		for (int z = 0; z < 25; z++) {
			if (mapData[x][z] == Stage::MAP::FLOAR) {
				Cell cell;
				cell.x = (float)x;
				cell.z = (float)z;
				posList.push_back(cell);
			}
		}
	}
	int index = rand() % posList.size();
	transform_.position_ = XMFLOAT3(posList.at(index).x * floarSize, 0.0f, posList.at(index).z * floarSize);
	index = rand() % posList.size();
	pAstarMoveAction_->SetTarget(XMFLOAT3(posList.at(index).x * floarSize, 0.0f, posList.at(index).z * floarSize));
	transform_.scale_ = XMFLOAT3(0.5f, 0.5f, 0.5f);

}

void AStarMan::Update()
{
	if (pAstarMoveAction_->IsInRange() && rand() % 60 == 0) {
		Player* pPlayer = (Player*)FindObject("Player");
		pAstarMoveAction_->SetTarget(pPlayer->GetPosition());
	}
	
	pAstarMoveAction_->Update();

}

void AStarMan::Draw()
{
	Transform draw = transform_;
	draw.position_.y += 1.0f;
	Model::SetTransform(hModel_, draw);
	Model::Draw(hModel_);

	if (!Input::IsKey(DIK_F)) {
		Transform target;
		target.scale_ = XMFLOAT3(0.2f, 0.2f, 0.2f);
		std::vector<XMFLOAT3> targetList = pAstarMoveAction_->GetTarget();
		if (targetList.empty()) return;
		for (auto pos : targetList) {
			target.position_ = XMFLOAT3(pos.x * floarSize, pos.y, pos.z * floarSize);
			target.position_.y += 1.0f;
			Model::SetTransform(hModel_, target);
			Model::Draw(hModel_);
		}
	}
	

}

void AStarMan::Release()
{
}
