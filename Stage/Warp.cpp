#include "Warp.h"
#include "../GameManager.h"
#include "../Engine/BoxCollider.h"
#include "../Engine/Model.h"
#include "../Engine/Camera.h"
#include "../Engine/TransitionEffect.h"
#include "../Player/Player.h"
#include "../Player/PlayerCommand.h"
#include "../Player/LifeManager.h"
#include "../Player/Aim.h"
#include "../UI/Interaction.h"
#include "../UI/InteractionUI.h"
#include "../State/StateManager.h"

namespace {
	static const int WARP_TIME = 100;

}

Warp::Warp(GameObject* parent)
	: GameObject(parent, "Warp"), warpScene_(SCENE_ID::SCENE_ID_TITLE), isPlayerHit_(false), isValid_(false), hModel_(-1),
	pInteractionUI_(nullptr), time_(0), isWarp_(false)
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
	if (!isWarp_) return;

	time_++;
	if (time_ >= WARP_TIME) {
		OutputDebugString("Warp : Warp Time\n");
		
		GameManager::GetSceneManager()->ChangeScene(warpScene_);

	}

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
		OutputDebugString("Warp : Use\n");

		GameManager::GetPlayer()->GetStateManager()->ChangeState("DisAppear");
		GameManager::GetPlayer()->GetAim()->SetValid(false);

		GetColliderList().front()->SetValid(false);
		TransitionEffect::SetFade(TRANSITION_TYPE::TYPE_ALPHA);
		TransitionEffect::SetAlpha(0.0f);
		TransitionEffect::SetAlphaDecrease(-0.01f);

		isValid_ = false;
		isWarp_ = true;
	}

}

void Warp::OutCollision()
{
	isPlayerHit_ = false;
	pInteractionUI_->SetValid(false);

}
