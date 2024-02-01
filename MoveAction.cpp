#include "MoveAction.h"
#include "Player.h"
#include "CollisionMap.h"
#include "GameManager.h"
#include "EnemyManager.h"
#include "NavigationAI.h"
#include "CreateStage.h"
#include "EnemyBase.h"

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

AstarMoveAction::AstarMoveAction(Character* obj, float speed, float range) : MoveAction(obj, speed, range), lastTarget_(0,0,0)
{
}

void AstarMoveAction::Update()
{
	isInRange_ = false;

	//移動終了した
	if (targetList_.empty()) {
		pCharacter_->SetMovement(XMFLOAT3(0,0,0));
		isInRange_ = true;
		return;
	}
	
	XMVECTOR half = XMVectorSet(0.5f, 0.0f, 0.5f, 0.0f);
	XMFLOAT3 pos = pCharacter_->GetPosition();
	XMVECTOR vPos = XMLoadFloat3(&pos);
	XMVECTOR vTar = XMLoadFloat3(&targetList_.back()) + half;
	XMVECTOR vMove = vTar - vPos;
	const float safeSize = 2.5f;

	//他のEnemyとの当たり判定
	XMVECTOR vSafeMove = XMVectorZero();
	if (pCharacter_) {
		std::vector<EnemyBase*> eList = GameManager::GetEnemyManager()->GetAllEnemy();

		for (auto& e : eList) {
			if (e == pCharacter_) continue;
			XMFLOAT3 f = e->GetPosition();
			XMVECTOR vTarget = XMLoadFloat3(&f);
			XMVECTOR vec = vTarget - vPos;
			float range = XMVectorGetX(XMVector3Length(vec));
			
			if (range < safeSize) {
				range -= safeSize;
				vSafeMove += XMVector3Normalize(vec) * range;
			}
		}

		vMove += vSafeMove;
	}
	
	float currentSpeed = XMVectorGetX(XMVector3Length(vMove));
	if (currentSpeed > moveSpeed_) vMove = XMVector3Normalize(vMove) * moveSpeed_;

	//Target位置ついた：カクカクしないように再起処理する
	float length = XMVectorGetX(XMVector3Length(vTar - vPos));
	if (length <= moveRange_ + (XMVectorGetX(XMVector3Length(vSafeMove * 0.9f)))) {
		targetList_.pop_back();
		Update();
		return;
	}

	pCharacter_->SetMovement(-vMove);
	XMStoreFloat3(&pos, vPos + vMove);

	//壁とのあたり判定してからポジションセット
	CollisionMap* pMap = static_cast<CollisionMap*>(pCharacter_->FindObject("CollisionMap"));
	pMap->CalcMapWall(pos, moveSpeed_);
	pCharacter_->SetPosition(pos);

}

bool AstarMoveAction::IsOutTarget(float range)
{
	if (!targetList_.empty()) {
		lastTarget_ = targetList_.front();
	}

	XMVECTOR vLatestTarget = XMLoadFloat3(&targetPos_);
	XMVECTOR vLastTarget = XMLoadFloat3(&lastTarget_);
	float r = XMVectorGetX(XMVector3Length(vLatestTarget - vLastTarget));
	if (range < r) {
		return true;
	}

	return false;
}

void AstarMoveAction::UpdatePath(XMFLOAT3 target)
{
	targetList_ = GameManager::GetNavigationAI()->Navi(target, pCharacter_->GetPosition());
	if(!targetList_.empty()) targetPos_ = targetList_.front();

}