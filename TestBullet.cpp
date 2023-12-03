#include "TestBullet.h"
#include "Engine/Model.h"
#include "Engine/SphereCollider.h"
#include "GameManager.h"
#include "DamageCtrl.h"

TestBullet::TestBullet(GameObject* parent)
	: BulletBase(parent), collision_(nullptr), damage_(0)
{
	objectName_ = "TestBullet";
}

TestBullet::~TestBullet()
{
	Release();
}

void TestBullet::Initialize()
{
	//モデルデータのロード
	hModel_ = Model::Load("Model/Feet.fbx");
	assert(hModel_ >= 0);

	transform_.scale_ = XMFLOAT3(0.1f, 0.1f, 0.1f);
	velocity_ = 0.7f;
	lifeTime_ = 100;

	collision_ = new SphereCollider(XMFLOAT3(0, 0, 0), 0.5f);
	AddCollider(collision_);

	pDamageCtrl_ = GameManager::GetDamageCtrl();

	damage_ = 1;
}

void TestBullet::Update()
{
	if (transform_.position_.y <= 0.0f) KillMe();

	LifeTime();
	Move();

	if (pDamageCtrl_->CalcBullet(collision_, damage_))
		KillMe();

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