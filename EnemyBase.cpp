#include "EnemyBase.h"
#include "Engine/Model.h"
#include "Engine/Input.h"

namespace {
	int hModel_ = -1;
	bool isUp = true;
	bool isActive = true;
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
	if(Input::IsKeyDown(DIK_3)) isActive = !isActive;
	if (isActive) return;

	if (isUp) {
		transform_.position_.y += 0.1f;
		if (transform_.position_.y >= 5.0f) isUp = false;
	}
	else {
		transform_.position_.y -= 0.1f;
		if (transform_.position_.y <= 0.0f) isUp = true;
	}

}

void EnemyBase::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_, 5);

}

void EnemyBase::Release()
{
}
