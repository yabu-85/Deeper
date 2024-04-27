#include "ThrowBullet.h"
#include "../Engine/Model.h"
#include "../Engine/SphereCollider.h"
#include "../GameManager/GameManager.h"
#include "../Other/AudioManager.h"
#include "../Other/VFXManager.h"
#include "../Enemy/EnemyBase.h"
#include "../Engine/PolyLine.h"
#include "../Engine/Global.h"
#include "../Player/Player.h"
#include "../Player/LifeManager.h"

namespace {
	static const int DAMAGE = 5;
	static const int DEATH_TIME = 30;
	static const float DEF_TIME = 60.0f;
	static const float MAX_HEIGHT = 8.0f;
	static const float MAX_DISTANCE = 10.0f;
	static const float AUDIO_DISTANCE = 10.0f;
	static const XMFLOAT3 BULLET_SCALE = XMFLOAT3(0.2f, 0.2f, 0.2f);

}

ThrowBullet::ThrowBullet(GameObject* parent)
	: BulletBase(parent, "ThrowBullet"), maxDistance_(0), maxHeight_(0), time_(0), pPolyLine_(nullptr),
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

	transform_.scale_ = BULLET_SCALE;
	damage_ = DAMAGE;
	time_ = (int)DEF_TIME;
	maxHeight_ = MAX_HEIGHT;
	maxDistance_ = MAX_DISTANCE;

	SphereCollider* collider = new SphereCollider(XMFLOAT3(0, 0, 0), BULLET_SCALE.x);
	AddAttackCollider(collider);

	pPolyLine_ = new PolyLine;
	pPolyLine_->Load("PolyImage/Line.png");
	pPolyLine_->SetLength(30);

}

void ThrowBullet::Update()
{
	//éÄñSîªíË
	if (isDeath_) {
		pPolyLine_->AddPosition(deathPosition_);

		if (time_ <= 0) KillMe();
		time_--;
		return;
	}

	//à⁄ìÆ
	Move();
	pPolyLine_->AddPosition(transform_.position_);

	//CollisionMapÇ∆ÇÃîªíËÅiç°ÇÕy<=0ÇæÇØÅj
	if (transform_.position_.y <= 0.0f) {
		Hit();
		return;
	}

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
	//åÇÇ¡ÇΩñ{êlÇ…ìñÇΩÇ¡ÇΩèÍçáÇÕèIóπ
	if (GetShotParent() == pTarget) return;

	if (pTarget->GetObjectName() == "Player" || pTarget->GetObjectName().find("Enemy") != std::string::npos) {
		Character* tar = static_cast<Character*>(pTarget);
		DamageInfo damage(damage_);
		tar->ApplyDamageDirectly(damage);
		Hit();
	}
}

void ThrowBullet::Shot(XMFLOAT3 pos, XMFLOAT3 target)
{
	transform_.position_ = pos;
	float dist = CalculationDistance(target, pos);
	float distX = target.x - pos.x;
	float distZ = target.z - pos.z;

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

	VFXManager::CreateVfxExplode1(transform_.position_);
	AudioManager::Play(AUDIO_ID::BULLET_HIT1, transform_.position_, 10.0f);
	deathPosition_ = transform_.position_;
	time_ = DEATH_TIME;
	isDeath_ = true;
	transform_.position_.y = 10000.0f;

}

void ThrowBullet::SetThrowData(float maxHeight, float maxDist)
{
	maxHeight_ = maxHeight;
	maxDistance_ = maxDist;

}