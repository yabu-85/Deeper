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
namespace {
	XMFLOAT3 currentTar{};
	float floarSize = 1.0f;
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
	//���f���f�[�^�̃��[�h
	hModel_ = Model::Load("Model/SphereCollider.fbx");
	assert(hModel_ >= 0);
	
	aimTargetPos_ = 1.0f;
	
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
	targetPos_ = XMFLOAT3(posList.at(index).x * floarSize, 0.0f, posList.at(index).z * floarSize);

}

void AStarMan::Update()
{
	XMFLOAT3 target = GameManager::GetNavigationAI()->Navi(targetPos_, transform_.position_);

	//������������targetPos�ύX
	if (targetPos_.x == target.x && targetPos_.z == target.z) {

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
		targetPos_ = XMFLOAT3(posList.at(index).x * floarSize, 0.0f, posList.at(index).z * floarSize);
		return;
	}

	if (currentTar.x != target.x || currentTar.z != target.z) {
		currentTar = target;
	}

	XMVECTOR vPos = XMLoadFloat3(&transform_.position_);
	XMVECTOR vTar = XMLoadFloat3(&target);
	XMVECTOR vMove = vTar - vPos;
	float length = XMVectorGetX(XMVector3Length(vMove));
	if (length > 0.2f) {
		vMove = XMVector3Normalize(vMove) * 0.2f;
	}

	XMStoreFloat3(&transform_.position_, vPos + vMove);
	
}

void AStarMan::Draw()
{
	Transform draw = transform_;
	draw.position_.y += 1.0f;
	Model::SetTransform(hModel_, draw);
	Model::Draw(hModel_);

	if (!Input::IsKey(DIK_F)) {
		Transform target;
		target.position_ = targetPos_;
		target.position_.y += 1.0f;
		Model::SetTransform(hModel_, target);
		Model::Draw(hModel_);
	}
	

}

void AStarMan::Release()
{
}
