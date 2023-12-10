#include "BulletBase.h"

BulletBase::BulletBase(GameObject* parent)
	: GameObject(parent, "BulletBase"), hModel_(-1), lifeTime_(0), moveVec_(0.0f, 0.0f, 0.0f), velocity_(1.0f)
{
}

void BulletBase::Shot(XMFLOAT3 pos, XMFLOAT3 target)
{
	transform_.position_ = pos;

	XMFLOAT3 move = XMFLOAT3(target.x - pos.x, target.y - pos.y, target.z - pos.z);
	XMVECTOR vMove = XMLoadFloat3(&move);
	vMove = XMVector3Normalize(vMove) * velocity_;
	XMStoreFloat3(&moveVec_, vMove);

}

void BulletBase::Move()
{
	transform_.position_.x += moveVec_.x;
	transform_.position_.y += moveVec_.y;
	transform_.position_.z += moveVec_.z;

}

void BulletBase::LifeTime()
{
	lifeTime_--;
	if (lifeTime_ <= 0) {
		IsLifeDead();
		KillMe();
	}
}

void BulletBase::IsLifeDead()
{
	KillMe();
}
