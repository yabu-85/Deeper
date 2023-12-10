#include "TestBullet.h"
#include "Engine/Model.h"
#include "Engine/SphereCollider.h"
#include "GameManager.h"
#include "DamageCtrl.h"
#include "Engine/VFX.h"

TestBullet::TestBullet(GameObject* parent)
	: BulletBase(parent), collision_(nullptr), damage_(0), pDamageCtrl_(nullptr)
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
	damage_ = 1;

	collision_ = new SphereCollider(XMFLOAT3(0, 0, 0), 0.5f);
	AddAttackCollider(collision_);

	pDamageCtrl_ = GameManager::GetDamageCtrl();

}

void TestBullet::Update()
{
	LifeTime();
	Move();

	//CollisionMap�Ƃ̔���i����y<=0�����j
	if (transform_.position_.y <= 0.0f) {
		CreatVfx();
		KillMe();
	}

	//����������
	if (pDamageCtrl_->CalcBullet(collision_, damage_)) {
		CreatVfx();
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

void TestBullet::CreatVfx()
{
	//�΂̕�
	EmitterData data;
	data.textureFileName = "Particle/cloudA.png";
	data.delay = 0;
	data.number = 30;
	data.lifeTime = 50;
	data.position = transform_.position_;
	data.positionRnd = XMFLOAT3(1.0f, 1.0f, 1.0f);
	data.direction = XMFLOAT3(1.0f, 1.0f, 1.0f);
	data.directionRnd = XMFLOAT3(90.0f, 90.0f, 90.0f);
	data.speed = 0.2f;
	data.speedRnd = 1.0f;
	data.accel = 0.9f;
	data.size = XMFLOAT2(0.3f, 0.3f);
	data.sizeRnd = XMFLOAT2(0.1f, 0.1f);
	data.scale = XMFLOAT2(0.97f, 0.97f);
	data.color = XMFLOAT4(1.0f, 1.0f, 0.1f, 1.0f);
	data.deltaColor = XMFLOAT4(0.0f, 0.0f, 0.0f, -1.0f / data.lifeTime);
	data.gravity = 0.001f;
	VFX::Start(data);

	//����
	data.textureFileName = "Particle/cloudA.png";
	data.delay = 0;
	data.number = 6;
	data.lifeTime = 20;
	data.position = transform_.position_;
	data.positionRnd = XMFLOAT3(0.5f, 0.0f, 0.5f);
	data.direction = XMFLOAT3(1.0f, 1.0f, 1.0f);
	data.directionRnd = XMFLOAT3(90.0f, 90.0f, 90.0f);
	data.speed = 0.1f;
	data.speedRnd = 0.8f;
	data.size = XMFLOAT2(1.2f, 1.2f);
	data.sizeRnd = XMFLOAT2(0.4f, 0.4f);
	data.scale = XMFLOAT2(1.05f, 1.05f);
	data.color = XMFLOAT4(1.0f, 1.0f, 0.1f, 1.0f);
	data.deltaColor = XMFLOAT4(0.0f, -1.0f / 20.0f, 0.0f, -1.0f / 20.0f);
	VFX::Start(data);

}