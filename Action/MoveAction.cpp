#include "MoveAction.h"
#include "../GameManager.h"
#include "../Character.h"
#include "../Player/Player.h"
#include "../Stage/CollisionMap.h"
#include "../Enemy/EnemyManager.h"
#include "../Stage/NavigationAI.h"
#include "../Stage/CreateStage.h"
#include "../Enemy/EnemyBase.h"

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

	//Target�ʒu����
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

	//�ړ��I������
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

	//����Enemy�Ƃ̓����蔻��
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

	//Target�ʒu�����F�J�N�J�N���Ȃ��悤�ɍċN��������
	float length = XMVectorGetX(XMVector3Length(vTar - vPos));
	if (length <= moveRange_ + (XMVectorGetX(XMVector3Length(vSafeMove * 0.9f)))) {
		targetList_.pop_back();
		Update();
		return;
	}

	pCharacter_->SetMovement(-vMove);
	XMStoreFloat3(&pos, vPos + vMove);

	//�ǂƂ̂����蔻�肵�Ă���|�W�V�����Z�b�g
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

//------------------------------Oriented----------------------

OrientedMoveAction::OrientedMoveAction(Character* obj, float speed)
	: MoveAction(obj, speed, 0.0f), direction_ { 0, 0, -1, 0 }, move_{XMVectorZero()}
{
}

void OrientedMoveAction::Update() {
	XMFLOAT3 position = pCharacter_->GetPosition();
	XMVECTOR vPosition = XMLoadFloat3(&position);

	//�^�[�Q�b�g�ւ̕�����rotateY���v�Z
	XMVECTOR vec = XMLoadFloat3(&targetPos_) - vPosition;
	vec = XMVector3Normalize(vec);
	float rotationY = atan2f(XMVectorGetX(vec), XMVectorGetZ(vec));

	//���̕������Ɉړ�
	XMMATRIX mRotY = XMMatrixRotationY(rotationY);
	XMVECTOR vMove = XMVector3TransformCoord(direction_, mRotY);
	vMove = XMVector3Normalize(vMove);
	move_ = vMove * moveSpeed_;
	XMStoreFloat3(&position, vPosition + move_);
	pCharacter_->SetPosition(position);
		
}

bool OrientedMoveAction::CheckWallCollision(int count)
{
	//�i�s�����𒲂ׂ�
	XMFLOAT3 position = pCharacter_->GetPosition();
	position = { position.x + (XMVectorGetX(move_) * (float)count), 0.0f, position.z + (XMVectorGetZ(move_) * (float)count) };
	
	//bodyRange�𑫂�
	XMVECTOR vR = XMVector3Normalize(move_) * pCharacter_->GetBodyRange();
	position = { position.x + XMVectorGetX(vR), 0.0f, position.z + XMVectorGetZ(vR) };

	if (GameManager::GetCollisionMap()->IsWall((int)position.x, (int)position.z)) return true;

	return false;
}
