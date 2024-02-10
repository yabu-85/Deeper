#include "ThrowBullet.h"
#include "../Engine/Model.h"
#include "../Engine/SphereCollider.h"
#include "../GameManager.h"
#include "../AudioManager.h"
#include "../VFXManager.h"
#include "../Enemy/EnemyBase.h"

namespace {
	static const float DEF_TIME = 60.0f;

}

ThrowBullet::ThrowBullet(GameObject* parent)
	: BulletBase(parent, "ThrowBullet"), damage_(0), maxDistance_(0), maxHeight_(0), time_(0)
{
}

ThrowBullet::~ThrowBullet()
{
}

void ThrowBullet::Initialize()
{
	hModel_ = Model::Load("DebugCollision/SphereCollider.fbx");
	assert(hModel_ >= 0);

	transform_.scale_ = XMFLOAT3(0.1f, 0.1f, 0.1f);
	velocity_ = 0.7f;
	damage_ = 0;
	lifeTime_ = 120;

	time_ = (int)DEF_TIME;

	SphereCollider* collision = new SphereCollider(XMFLOAT3(0, 0, 0), 0.2f);
	AddAttackCollider(collision);

}

void ThrowBullet::Update()
{
	Move();

	//CollisionMapÇ∆ÇÃîªíËÅiç°ÇÕy<=0ÇæÇØÅj
	if (transform_.position_.y <= 0.0f) {
		VFXManager::CreatVfxExplode1(transform_.position_);
		AudioManager::Play(transform_.position_, 10.0f);
		KillMe();
	}

	LifeTime();
	time_--;

}

void ThrowBullet::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
	CollisionDraw();
}

void ThrowBullet::Release()
{
	Model::Release(hModel_);
}

void ThrowBullet::OnAttackCollision(GameObject* pTarget)
{
	if (pTarget->GetObjectName() == "Player") {
		VFXManager::CreatVfxExplode1(transform_.position_);
		KillMe();
	}

}

void ThrowBullet::SetThrowData(float maxHeight, float maxDist)
{
	maxHeight_ = maxHeight;
	maxDistance_ = maxDist;

}

void ThrowBullet::Shot(XMFLOAT3 pos, XMFLOAT3 target)
{
	maxHeight_ = 10.0f;
	maxDistance_ = 10.0f; 
	transform_.position_ = pos;
	
	float distX = target.x - pos.x;
	float distZ = target.z - pos.z;
	float dist = sqrtf(distX * distX + distZ * distZ);

	//ç≈ëÂãóó£ó}êß
	if (dist > maxDistance_) {
		float supp = maxDistance_ / dist;
		distX *= supp;
		distZ *= supp;
		dist = maxDistance_;
	}
	moveVec_.x = distX / DEF_TIME;
	moveVec_.z = distZ / DEF_TIME;

	//ç≈ëÂçÇìx
	float timeToReachPeak = dist / (DEF_TIME * 0.5f);
	float maxHeightAdjusted = maxHeight_;
	if (timeToReachPeak < 1.0f) maxHeightAdjusted *= timeToReachPeak;
	moveVec_.y = maxHeightAdjusted / timeToReachPeak / DEF_TIME;

}

void ThrowBullet::Move()
{
	//-1Å`1ÇÃä‘Ç…ïœä∑ÇµÇƒheightÇåvéZ
	float time = (time_ / DEF_TIME) * 2.0f - 1.0f;
	float height = moveVec_.y * time;

	transform_.position_.x += moveVec_.x;
	transform_.position_.y += height;
	transform_.position_.z += moveVec_.z;

}