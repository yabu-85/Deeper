#include "Warp.h"
#include "CreateStage.h"
#include "../VFXManager.h"
#include "../Engine/SceneManager.h"
#include "../Engine/BoxCollider.h"
#include "../GameManager.h"
#include "../Player/Player.h"
#include "../Player/PlayerCommand.h"
#include "../Engine/Direct3D.h"
#include "../Engine/Model.h"
#include "../Player/LifeManager.h"
#include "../UI/Interaction.h"
#include "../UI/InteractionUI.h"

Warp::Warp(GameObject* parent)
	: GameObject(parent, "Warp"), warpScene_(SCENE_ID::SCENE_ID_TITLE), isPlayerHit_(false), isValid_(false), hModel_(-1),
	pInteractionUI_(nullptr)
{
}

Warp::~Warp()
{
}

void Warp::Initialize()
{
	hModel_ = Model::Load("Model/Stage/Warp.fbx");
	assert(hModel_ >= 0);

	transform_.position_.x += 0.5f;
	transform_.position_.y += 0.5f;
	transform_.position_.z += 0.5f;

	BoxCollider* collider = new BoxCollider(XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f));
	AddCollider(collider);

	pInteractionUI_ = new InteractionUI(this);
	pInteractionUI_->Initialize();
	pInteractionUI_->SetOffset(XMFLOAT3{ 0.5f, 1.0f, 0.5f });
	pInteractionUI_->SetValid(false);

}

void Warp::Update()
{
	if (rand() % 60 == 0) VFXManager::CreatVfxExplode1(transform_.position_);

}

void Warp::Draw()
{
	if (isValid_) {
		Transform t = transform_;
		t.position_ = { t.position_.x, t.position_.y, t.position_.z};
		Model::SetTransform(hModel_, t);
		Model::Draw(hModel_, 0);

		CollisionDraw();

	}

}

void Warp::Release()
{
	pInteractionUI_->Release();
	SAFE_DELETE(pInteractionUI_);

}

void Warp::OnCollision(GameObject* pTarget)
{
	if (!isValid_) return;

	if (pTarget->GetObjectName() != "Player") {
		isPlayerHit_ = false;
		return;
	}

	//一番近いオブジェクトか
	pInteractionUI_->SetValid(true);
	if (!Interaction::IsMinDistance(this)) return;

	//Playerに衝突し始めた
	isPlayerHit_ = true;

	if (GameManager::GetPlayer()->GetCommand()->CmdDownAction() && !LifeManager::IsDie()) {
		GameManager::GetSceneManager()->ChangeScene(warpScene_);
	}
	
}

void Warp::OutCollision()
{
	isPlayerHit_ = false;
	pInteractionUI_->SetValid(false);

}
