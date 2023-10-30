#include "TestBullet.h"
#include "Engine/Model.h"

TestBullet::TestBullet(GameObject* parent)
	: BulletBase(parent)
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
}

void TestBullet::Update()
{
	LifeTime();
	Move();

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
