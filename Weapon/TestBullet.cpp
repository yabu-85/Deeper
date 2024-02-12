#include "TestBullet.h"
#include "../Engine/Model.h"
#include "../Engine/SphereCollider.h"
#include "../GameManager.h"
#include "../AudioManager.h"
#include "../VFXManager.h"
#include "../Enemy/EnemyBase.h"

TestBullet::TestBullet(GameObject* parent)
	: BulletBase(parent, "TesteBullet")
{
}

TestBullet::~TestBullet()
{
}

void TestBullet::Initialize()
{
	hModel_ = Model::Load("DebugCollision/SphereCollider.fbx");
	assert(hModel_ >= 0);

	transform_.scale_ = XMFLOAT3(0.1f, 0.1f, 0.1f);
	velocity_ = 0.7f;
	lifeTime_ = 30;
	damage_ = 10;

	SphereCollider* collision = new SphereCollider(XMFLOAT3(0, 0, 0), 0.5f);
	AddAttackCollider(collision);

}

void TestBullet::Update()
{
	LifeTime();
	Move();

	//CollisionMapÇ∆ÇÃîªíËÅiç°ÇÕy<=0ÇæÇØÅj
	if (transform_.position_.y <= 0.0f) {
		VFXManager::CreatVfxExplode1(transform_.position_);
		AudioManager::Play(transform_.position_, 10.0f);
		KillMe();
	}

}

void TestBullet::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void TestBullet::Release()
{
	Model::Release(hModel_);
}

void TestBullet::OnAttackCollision(GameObject* pTarget)
{
	if (pTarget->GetObjectName().find("Enemy") != std::string::npos) {
		EnemyBase* e = static_cast<EnemyBase*>(pTarget);
		e->ApplyDamage(damage_);
		VFXManager::CreatVfxExplode1(transform_.position_);
		AudioManager::Play(transform_.position_, 10.0f);
		KillMe();

	}

}