#include "Warp.h"
#include "../VFXManager.h"
#include "../Engine/SceneManager.h"
#include "../Engine/BoxCollider.h"
#include "CreateStage.h"
#include "../GameManager.h"
#include "../Player/Player.h"
#include "../Player/Aim.h"
#include "../Player/PlayerCommand.h"
#include "../Engine/Direct3D.h"
#include "../Engine/Model.h"

Warp::Warp(GameObject* parent)
	: GameObject(parent, "Warp"), warpScene_(SCENE_ID::SCENE_ID_TITLE), isPlayerHit_(false), isValid_(false), hModel_(-1)
{
}

Warp::~Warp()
{
}

void Warp::Initialize()
{
	BoxCollider* collider = new BoxCollider(XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f));
	AddCollider(collider);

	hModel_ = Model::Load("Model/Stage/Warp.fbx");
	assert(hModel_ >= 0);

	transform_.position_.x += 0.5f;
	transform_.position_.y += 0.5f;
	transform_.position_.z += 0.5f;

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
}

void Warp::OnCollision(GameObject* pTarget)
{
	if (!isValid_) return;

	if (pTarget->GetObjectName() != "Player") {
		isPlayerHit_ = false;
		return;
	}

	//Player‚ÉÕ“Ë‚µŽn‚ß‚½
	isPlayerHit_ = true;

	//Player‚ÌAim‹­§ˆÚ“®Žg‚Á‚Ä‚Ý‚é
	XMFLOAT3 cPos = XMFLOAT3(transform_.position_.x, transform_.position_.y + 3.0f, transform_.position_.z + 10.0f);
	GameManager::GetPlayer()->GetAim()->SetCompulsion(cPos, transform_.position_);

	if (GameManager::GetPlayer()->GetCommand()->CmdDownAction()) {
		SceneManager* pSceneManager = static_cast<SceneManager*>(FindObject("SceneManager"));
		pSceneManager->ChangeScene(warpScene_);
	}
	
}

void Warp::OutCollision()
{
	isPlayerHit_ = false;

}
