#include "TestBullet.h"
#include "Engine/Model.h"
#include "Engine/SphereCollider.h"
#include "GameManager.h"
#include "AudioManager.h"
#include "VFXManager.h"

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
	//���f���f�[�^�̃��[�h
	hModel_ = Model::Load("Model/Feet.fbx");
	assert(hModel_ >= 0);

	transform_.scale_ = XMFLOAT3(0.1f, 0.1f, 0.1f);
	velocity_ = 0.7f;
	lifeTime_ = 30;
	damage_ = rand() % 11;

	collision_ = new SphereCollider(XMFLOAT3(0, 0, 0), 0.5f);
	AddCollider(collision_);

}

void TestBullet::Update()
{
	LifeTime();
	Move();

	//CollisionMap�Ƃ̔���i����y<=0�����j
	if (transform_.position_.y <= 0.0f) {
		VFXManager::CreatVfxExplode1(transform_.position_);
		AudioManager::Play(transform_.position_, 10.0f);
		KillMe();
	}

	//����������
	//if (pDamageManager_->CalcEnemy(collision_, damage_)) {
	//	VFXManager::CreatVfxExplode1(transform_.position_);
	//	AudioManager::Play(transform_.position_, 10.0f);
	//	KillMe();
	//}

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