#include "BulletBase.h"
#include "../Engine/Global.h"

BulletBase::BulletBase(GameObject* parent, std::string name)
	: GameObject(nullptr, name), hModel_(-1), lifeTime_(0), moveVec_(0.0f, 0.0f, 0.0f), velocity_(1.0f), damage_(0), pParent_(nullptr)
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
	transform_.position_ = Float3Add(transform_.position_, moveVec_);
}

void BulletBase::LifeTime()
{
	lifeTime_--;
	if (lifeTime_ <= 0) {
		LifeDead();
	}
}

void BulletBase::LifeDead()
{
	KillMe();
}
