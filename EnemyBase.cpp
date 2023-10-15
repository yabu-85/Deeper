#include "EnemyBase.h"
#include "Engine/Model.h"

namespace {
	int hModel_ = -1;

}

EnemyBase::EnemyBase(GameObject* parent)
	: GameObject(parent, "EnemyBase")
{
}

EnemyBase::~EnemyBase()
{
}

void EnemyBase::Initialize()
{
	//モデルデータのロード
	hModel_ = Model::Load("Model/Enemy.fbx");
	assert(hModel_ >= 0);

	transform_.position_.x = 20.0f;
	transform_.rotate_.y = -90;

}

void EnemyBase::Update()
{
}

void EnemyBase::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);

}

void EnemyBase::Release()
{
}
