#include "MoveAction.h"
#include "Player.h"
#include "CollisionMap.h"
#include "GameManager.h"
#include "EnemyManager.h"
#include "NavigationAI.h"
#include "Stage.h"
#include "EnemyBase.h"

void MoveAction::CalcMapWall(XMFLOAT3 _pos)
{
	CollisionMap* pMap = (CollisionMap*)pCharacter_->FindObject("CollisionMap");

	int checkX1, checkX2;
	int checkZ1, checkZ2;

	checkX1 = (int)(_pos.x + 0.15f); //前
	checkZ1 = (int)(_pos.z + 0.3f);
	checkX2 = (int)(_pos.x - 0.15f);
	checkZ2 = (int)(_pos.z + 0.3f);
	if (pMap->IsWall(checkX1, checkZ1) == 1 || pMap->IsWall(checkX2, checkZ2) == 1) { //床やけやったら
		_pos.z = (float)((int)_pos.z) + (1.0f - 0.3f);
	}

	checkX1 = (int)(_pos.x + 0.15f); //後ろ
	checkZ1 = (int)(_pos.z - 0.3f);
	checkX2 = (int)(_pos.x - 0.15f);
	checkZ2 = (int)(_pos.z - 0.3f);
	if (pMap->IsWall(checkX1, checkZ1) == 1 || pMap->IsWall(checkX2, checkZ2) == 1) {
		_pos.z = (float)((int)_pos.z) + 0.3f;
	}

	checkX1 = (int)(_pos.x + 0.3f); //右
	checkZ1 = (int)(_pos.z + 0.15f);
	checkX2 = (int)(_pos.x + 0.3f);
	checkZ2 = (int)(_pos.z - 0.15f);
	if (pMap->IsWall(checkX1, checkZ1) == 1 || pMap->IsWall(checkX2, checkZ2) == 1) {
		_pos.x = (float)((int)_pos.x + 1) - 0.3f;  // x　だけ戻すことで斜め移動ができるようになる     
	}

	checkX1 = (int)(_pos.x - 0.3f); //左
	checkZ1 = (int)(_pos.z + 0.15f);
	checkX2 = (int)(_pos.x - 0.3f);
	checkZ2 = (int)(_pos.z - 0.15f);
	if (pMap->IsWall(checkX1, checkZ1) == 1 || pMap->IsWall(checkX2, checkZ2) == 1) {
		_pos.x = (float)((int)_pos.x) + 0.3f;
	}

}

MoveAction::MoveAction(Character* obj, float speed, float range)
	: BaseAction(obj), isInRange_(false), moveSpeed_(speed), moveRange_(range), targetPos_(0, 0, 0)
{
}

void MoveAction::Update()
{
	XMFLOAT3 pos = pCharacter_->GetPosition();
	XMVECTOR vPos = XMLoadFloat3(&pos);
	XMVECTOR vTar = XMLoadFloat3(&targetPos_);
	XMVECTOR vMove = vTar - vPos;
	float currentSpeed = XMVectorGetX(XMVector3Length(vTar - vPos));
	if(currentSpeed > moveSpeed_) vMove = XMVector3Normalize(vMove) * moveSpeed_;

	//Target位置ついた
	float length = XMVectorGetX(XMVector3Length(vTar - vPos));
	if (length <= moveRange_) {
		isInRange_ = true;
	}

	XMStoreFloat3(&pos, vPos + vMove);
	pCharacter_->SetPosition(pos);
	isInRange_ = false;;
}

//------------------------------Astar----------------------

AstarMoveAction::AstarMoveAction(Character* obj, float speed, float range) : MoveAction(obj, speed, range)
{
}

void AstarMoveAction::Update()
{
	isInRange_ = false;

	//移動終了した
	if (targetList_.empty()) {
		isInRange_ = true;
		return;
	}

	XMFLOAT3 pos = pCharacter_->GetPosition();
	XMVECTOR vPos = XMLoadFloat3(&pos);
	XMVECTOR vTar = XMLoadFloat3(&targetList_.back()) * floarSize;
	XMVECTOR vMove = vTar - vPos;
	XMVECTOR vMoveN = XMVector3Normalize(vMove);
	const float safeSize = 6.0f;

	EnemyBase* enemy = dynamic_cast<EnemyBase*>(pCharacter_);
	if (enemy) {
		EnemyManager* enemyMa = GameManager::GetEnemyManager();
		std::vector<EnemyBase*> eList = enemyMa->GetAllEnemy();
		XMVECTOR vSafeMove = XMVectorZero();

		for (auto& e : eList) {
			if (e == enemy) continue;
			XMFLOAT3 f = e->GetPosition();
			XMVECTOR vTarget = XMLoadFloat3(&f);
			XMVECTOR vec = vTarget - vPos;
			float range = XMVectorGetX(XMVector3Length(vec));
			
			if (range < safeSize) {
				if (range > safeSize) range = safeSize;
				range -= safeSize;
				vSafeMove += XMVector3Normalize(vec) * range;
			}
		}
		float range = XMVectorGetX(XMVector3Length(vSafeMove));
		if (range > safeSize) vSafeMove = XMVector3Normalize(vSafeMove) * safeSize;

		vMove += vSafeMove;
	}
	
	float currentSpeed = XMVectorGetX(XMVector3Length(vMove));
	if (currentSpeed > moveSpeed_ * floarSize) vMove = XMVector3Normalize(vMove) * (moveSpeed_ * floarSize);

	//Target位置ついた：カクカクしないように再起処理する
	float length = XMVectorGetX(XMVector3Length(vTar - vPos));
	if (length <= moveRange_ + safeSize) {
		targetList_.pop_back();
		Update();
		return;
	}

	XMStoreFloat3(&pos, vPos + vMove);

	//壁とのあたり判定してからポジションセット
	CalcMapWall(pos);

	pCharacter_->SetPosition(pos);
}

void AstarMoveAction::SetTarget(XMFLOAT3 target)
{
	targetList_ = GameManager::GetNavigationAI()->NaviDiagonal(target, pCharacter_->GetPosition());
}