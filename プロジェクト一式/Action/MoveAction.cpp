#include "MoveAction.h"
#include "../GameManager/GameManager.h"
#include "../Character/Character.h"
#include "../Player/Player.h"
#include "../Stage/CollisionMap.h"
#include "../Enemy/EnemyManager.h"
#include "../Enemy/EnemyBase.h"
#include "../Stage/NavigationAI.h"
#include "../Stage/CreateStage.h"
#include "../Engine/Global.h"

//デバッグ用
#include "../Engine/Model.h"

namespace {
	static const XMVECTOR FLOAR_HALF = XMVectorSet(0.5f, 0.0f, 0.5f, 0.0f);
	
	//Orientに使用
	static const float DIR_X[4] = { 0, 1, 0, -1 };
	static const float DIR_Z[4] = { 1, 0, -1, 0 };
	enum DIRECTION {
		FRONT = 0,
		RIGHT,
		BACK,
		LEFT,
	};
}

MoveAction::MoveAction(Character* obj, float speed, float range)
	: BaseAction(obj), isInRange_(false), moveSpeed_(speed), moveRange_(range), targetPos_(0, 0, 0)
{
}

void MoveAction::Update()
{
	//移動量計算
	XMFLOAT3 pos = pCharacter_->GetPosition();
	XMVECTOR vPos = XMLoadFloat3(&pos);
	XMVECTOR vTar = XMLoadFloat3(&targetPos_);
	XMVECTOR vMove = vTar - vPos;
	
	//移動スピード抑制
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

void MoveAction::CalcDodge(XMVECTOR& move)
{
	const float safeSize = 2.5f;
	XMFLOAT3 pos = pCharacter_->GetPosition();
	XMVECTOR vPos = XMLoadFloat3(&pos);
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

		move += vSafeMove;
	}
}

//------------------------------Astar----------------------

AstarMoveAction::AstarMoveAction(Character* obj, float speed, float range) : MoveAction(obj, speed, range), lastTarget_(0,0,0), handle_(-1)
{
#ifdef _DEBUG
	handle_ = Model::Load("Model/Mas.fbx");
	assert(handle_ >= 0);
#endif

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
	
	//移動方向計算
	XMFLOAT3 pos = pCharacter_->GetPosition();
	XMVECTOR vPos = XMLoadFloat3(&pos);
	XMVECTOR vTar = XMLoadFloat3(&targetList_.back()) + FLOAR_HALF;
	XMVECTOR vMove = vTar - vPos;

	//移動スピード抑制
	float currentSpeed = XMVectorGetX(XMVector3Length(vMove));
	if (currentSpeed > moveSpeed_) vMove = XMVector3Normalize(vMove) * moveSpeed_;

	//Target位置ついた：カクカクしないように再起処理する
	float length = XMVectorGetX(XMVector3Length(vTar - vPos));

	//CalcDodge使う場合はmoveRange＋vSafeMoveでやったほうがいい
	if (length <= moveRange_) {
		targetList_.pop_back();
		Update();
		return;
	}

	pCharacter_->SetMovement(-vMove);
	XMStoreFloat3(&pos, vPos + vMove);
	pCharacter_->SetPosition(pos);
}

bool AstarMoveAction::IsOutTarget(float range)
{
	//lastTargetを更新・outTarget関数以外でも使うならUpdateに置いたほうがいい
	if (!targetList_.empty()) {
		lastTarget_ = targetList_.front();
	}

	//range外だからtrue
	if (range < CalculationDistance(targetPos_, lastTarget_)) return true;
	return false;
}

void AstarMoveAction::UpdatePath(XMFLOAT3 target)
{
	targetList_ = GameManager::GetNavigationAI()->Navi(target, pCharacter_->GetPosition(), pCharacter_->GetBodyRange());
	if(!targetList_.empty()) targetPos_ = targetList_.front();
}

void AstarMoveAction::Draw()
{
#ifdef _DEBUG
	Transform t;
	for (int i = 0; i < targetList_.size(); i++) {
		t.position_ = targetList_.at(i);
		t.position_.y += 0.01f;
		Model::SetTransform(handle_, t);
		Model::Draw(handle_);
	}
#endif
}

//------------------------------Oriented----------------------

OrientedMoveAction::OrientedMoveAction(Character* obj, float speed)
	: MoveAction(obj, speed, 0.0f), direction_ { 0, 0, -1, 0 }, move_{XMVectorZero()}, targetDirection_(XMVectorZero())
{
}

void OrientedMoveAction::Update() {
	//ターゲットへの方向のrotateYを計算
	float rotationY = atan2f(XMVectorGetX(targetDirection_), XMVectorGetZ(targetDirection_));

	//その方句を基準に移動
	XMMATRIX mRotY = XMMatrixRotationY(rotationY);
	XMVECTOR vMove = XMVector3TransformCoord(direction_, mRotY);
	vMove = XMVector3Normalize(vMove);
	move_ = vMove * moveSpeed_;
	
	XMFLOAT3 position = pCharacter_->GetPosition();
	XMVECTOR vPosition = XMLoadFloat3(&position); 
	XMStoreFloat3(&position, vPosition + move_);
	pCharacter_->SetPosition(position);
}

void OrientedMoveAction::SetTarget(XMFLOAT3 target)
{
	targetPos_ = target;
	XMFLOAT3 position = pCharacter_->GetPosition();
	XMVECTOR vPosition = XMLoadFloat3(&position); 
	targetDirection_ = XMLoadFloat3(&targetPos_) - vPosition;
	targetDirection_ = XMVector3Normalize(targetDirection_);
}

bool OrientedMoveAction::CheckWallCollision(int count)
{
	//進行方向を調べる
	XMFLOAT3 position = pCharacter_->GetPosition();
	position = { position.x + (XMVectorGetX(move_) * (float)count), 0.0f, position.z + (XMVectorGetZ(move_) * (float)count) };
	
	//bodyRangeを足す
	XMVECTOR vR = XMVector3Normalize(move_) * pCharacter_->GetBodyRange();
	position = { position.x + XMVectorGetX(vR), 0.0f, position.z + XMVectorGetZ(vR) };

	if (GameManager::GetCollisionMap()->IsWall((int)position.x, (int)position.z)) return true;
	return false;
}

void OrientedMoveAction::SelectProbabilityDirection(int f, int b, int r, int l)
{
	int max = f + b + r + l;
	if (max <= 0) return;
	int random = rand() % max;

	if(random < f) direction_ = { DIR_X[FRONT], 0.0f, DIR_Z[FRONT] };
	else if(random < f + b) direction_ = { DIR_X[BACK], 0.0f, DIR_Z[BACK] };
	else if(random < f + b + r) direction_ = { DIR_X[RIGHT], 0.0f, DIR_Z[RIGHT] };
	else if(random <= f + b + r + l) direction_ = { DIR_X[LEFT], 0.0f, DIR_Z[LEFT] };
}

void OrientedMoveAction::InverseDirection()
{
	direction_ *= -1.0f;
}
