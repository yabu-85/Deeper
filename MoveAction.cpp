#include "MoveAction.h"
#include "EnemyBase.h"

MoveTarget::MoveTarget(EnemyBase* owner, float speed, float range) : Action(), owner_(owner), speed_(speed), range_(range) {}

MoveTarget::Status MoveTarget::Update()
{
	XMFLOAT3 pos = owner_->GetPosition();
	XMVECTOR vPos = XMLoadFloat3(&pos);
	XMFLOAT3 targetPos = owner_->GetTargetPos();
	XMVECTOR vTar = XMLoadFloat3(&targetPos);
	XMVECTOR vMove = vTar - vPos;

	vMove = XMVector3Normalize(vMove) * speed_;

	//ëñÇËèIÇÌÇ¡ÇΩ
	float length = XMVectorGetX(XMVector3Length(vTar - vPos));
	if (length <= range_) {
		return Status::SUCCESS;
	}

	XMStoreFloat3(&pos, vPos + vMove);
	owner_->SetPosition(pos);

	return Status::RUNNING;
}