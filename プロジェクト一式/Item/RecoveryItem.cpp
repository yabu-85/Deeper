#include "RecoveryItem.h"
#include "../Engine/Model.h"
#include "../Engine/SphereCollider.h"
#include "../Player/Player.h"
#include "../Player/LifeManager.h"

namespace {
	static const int ITEM_RECOVERY_AMOUNT = 5;
	static const float INITIAL_ITEM_GRAVITY = 0.3f;
	static const float ITEM_GRAVITY = 0.01f;
	static const float HALF_SIZE = 0.4f;

}

RecoveryItem::RecoveryItem(GameObject* parent) 
	: GameObject(parent, "RecoveryItem"), hModel_(-1), gravity_(0.0f), recoveryAmount_(0)
{
}

RecoveryItem::~RecoveryItem()
{
}

void RecoveryItem::Initialize()
{
	hModel_ = Model::Load("Model/Egg.fbx");
	assert(hModel_ >= 0);

	gravity_ = INITIAL_ITEM_GRAVITY;
	recoveryAmount_ = ITEM_RECOVERY_AMOUNT;

	SphereCollider* collider = new SphereCollider(XMFLOAT3(0.f, HALF_SIZE, 0.f), HALF_SIZE);
	AddCollider(collider);
}

void RecoveryItem::Update()
{
	transform_.position_.y += gravity_;
	gravity_ -= ITEM_GRAVITY;

	if (transform_.position_.y < 0.0f) {
		transform_.position_.y = 0.0f;
	}
}

void RecoveryItem::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);

	CollisionDraw();
}

void RecoveryItem::Release()
{
	Model::Release(hModel_);
}

void RecoveryItem::OnCollision(GameObject* pTarget)
{
	if (pTarget->GetObjectName() == "Player") {
		Player* p = static_cast<Player*>(pTarget);
		p->SetHP(p->GetHP() + recoveryAmount_);
		KillMe();

		LifeManager::Damage(0);
	}
}