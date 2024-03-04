#include "ThrowBullet.h"
#include "../Engine/Model.h"
#include "../Engine/SphereCollider.h"
#include "../GameManager/GameManager.h"
#include "../AudioManager.h"
#include "../VFXManager.h"
#include "../Enemy/EnemyBase.h"
#include "../Engine/PolyLine.h"
#include "../Engine/Global.h"
#include "../Player/Player.h"
#include "../Player/LifeManager.h"

namespace {
	static const int DAMAGE = 1;
	static const int DEATH_TIME = 30;
	static const float DEF_TIME = 60.0f;
	static const float MAX_HEIGHT = 8.0f;
	static const float MAX_DISTANCE = 10.0f;
	static const float AUDIO_DISTANCE = 10.0f;

}

ThrowBullet::ThrowBullet(GameObject* parent)
	: BulletBase(parent, "ThrowBullet"), maxDistance_(0), maxHeight_(0), time_(0), pPolyLine_(nullptr), pSphereCollider_(nullptr),
	deathPosition_(XMFLOAT3()), isDeath_(false)
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
	damage_ = DAMAGE;
	time_ = (int)DEF_TIME;
	maxHeight_ = MAX_HEIGHT;
	maxDistance_ = MAX_DISTANCE;

	pSphereCollider_ = new SphereCollider(XMFLOAT3(0, 0, 0), 0.2f);
	AddAttackCollider(pSphereCollider_);
	pSphereCollider_->SetValid(false);

	pPolyLine_ = new PolyLine;
	pPolyLine_->Load("tex3.png");
	pPolyLine_->SetLength(30);

}

void ThrowBullet::Update()
{
	if (isDeath_) {
		pPolyLine_->AddPosition(deathPosition_);

		if (time_ <= 0) KillMe();
		time_--;
		return;
	}

	if(time_ > ((int)DEF_TIME / 2)) pSphereCollider_->SetValid(true);

	Move();
	pPolyLine_->AddPosition(transform_.position_);

	//CollisionMapÇ∆ÇÃîªíËÅiç°ÇÕy<=0ÇæÇØÅj
	if (transform_.position_.y <= 0.0f) Hit();

	time_--;
}

void ThrowBullet::Draw()
{
	pPolyLine_->Draw();

	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
	CollisionDraw();
}

void ThrowBullet::Release()
{
	Model::Release(hModel_);
	SAFE_RELEASE(pPolyLine_);
	SAFE_DELETE(pPolyLine_);

}

void ThrowBullet::OnAttackCollision(GameObject* pTarget)
{
	if (objectName_ == "ThrowEBullet") {
		if (pTarget->GetObjectName() == "Player") {
			Hit();
			GameManager::GetPlayer()->TargetRotate(GetPosition());
			LifeManager::Damage(damage_);
		}
	}
	else {
		if (pTarget->GetObjectName().find("Enemy") != std::string::npos) {
			Hit();
			EnemyBase* e = static_cast<EnemyBase*>(pTarget);
			e->ApplyDamage(damage_);
		}
	}

}

void ThrowBullet::SetThrowData(float maxHeight, float maxDist)
{
	maxHeight_ = maxHeight;
	maxDistance_ = maxDist;

}

void ThrowBullet::Shot(XMFLOAT3 pos, XMFLOAT3 target)
{
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

	//ç≈ëÂçÇìxó}êß
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

void ThrowBullet::Hit()
{
	//ìñÇΩÇ¡ÇΩÇÊÇ§Ç…å©ÇπÇÈÇΩÇﬂÇ…àÍâÒêiÇﬂÇÈ
	Move();

	VFXManager::CreatVfxExplode1(transform_.position_);
	AudioManager::Play(transform_.position_, AUDIO_DISTANCE);
	deathPosition_ = transform_.position_;
	time_ = DEATH_TIME;
	isDeath_ = true;
	transform_.position_.y = 10000.0f;

}
