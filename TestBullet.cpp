#include "TestBullet.h"
#include "Engine/Model.h"

TestBullet::TestBullet(GameObject* parent)
	: BulletBase(parent)
{
	objectName_ = "TestBullet";
}

TestBullet::~TestBullet()
{
}

void TestBullet::Initialize()
{
	//モデルデータのロード
	hModel_ = Model::Load("Model/Feet.fbx");
	assert(hModel_ >= 0);

	transform_.scale_ = XMFLOAT3(0.2f, 0.2f, 0.2f);
}

void TestBullet::Update()
{
	Move();
}

void TestBullet::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void TestBullet::Release()
{

}
