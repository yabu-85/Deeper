#include "NormalBullet.h"
#include "../AudioManager.h"
#include "../VFXManager.h"
#include "../Engine/Model.h"
#include "../Engine/SphereCollider.h"
#include "../GameManager/GameManager.h"
#include "../Enemy/EnemyBase.h"
#include "../Player/Player.h"
#include "../Player/LifeManager.h"

NormalBullet::NormalBullet(GameObject* parent)
	: BulletBase(parent, "TesteBullet")
{
}

NormalBullet::~NormalBullet()
{
}

void NormalBullet::Initialize()
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

void NormalBullet::Update()
{
	Move();

	//CollisionMapとの判定（今はy<=0だけ）修正箇所
	if (transform_.position_.y <= 0.0f) {
		VFXManager::CreatVfxExplode1(transform_.position_);
		AudioManager::Play(transform_.position_, 10.0f);
		KillMe();
		return;
	}

	LifeTime();

}

void NormalBullet::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void NormalBullet::Release()
{
	Model::Release(hModel_);
}

void NormalBullet::OnAttackCollision(GameObject* pTarget)
{
	//撃った本人に当たった場合は終了
	if (GetShotParent() == pTarget) return;

	if (objectName_ == "NormalEBullet") {
		if (pTarget->GetObjectName() == "Player") {
			Hit();
			GameManager::GetPlayer()->TargetRotate(GetPosition());
			LifeManager::Damage(damage_);
		}
		else if (pTarget->GetObjectName().find("Enemy") != std::string::npos) {
			EnemyBase* e = static_cast<EnemyBase*>(pTarget);
			e->ApplyDamage(damage_);
			Hit();
		}
	}
	else {
		if (pTarget->GetObjectName().find("Enemy") != std::string::npos) {
			EnemyBase* e = static_cast<EnemyBase*>(pTarget);
			e->ApplyDamage(damage_);
			Hit();
		}
	}

}

void NormalBullet::Hit()
{
	VFXManager::CreatVfxExplode1(transform_.position_);
	AudioManager::Play(transform_.position_, 10.0f);
	KillMe();
}
