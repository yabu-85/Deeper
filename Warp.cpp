#include "Warp.h"
#include "VFXManager.h"
#include "Engine/SceneManager.h"
#include "Engine/BoxCollider.h"
#include "CreateStage.h"
#include "GameManager.h"
#include "Player.h"
#include "Aim.h"
#include "PlayerCommand.h"
#include "Engine/Direct3D.h"
#include "Engine/Model.h"

Warp::Warp(GameObject* parent)
	: GameObject(parent, "Warp"), warpScene_(SCENE_ID::SCENE_ID_TITLE), isPlayerHit_(false), isValid_(false), hModel_(-1)
{
}

Warp::~Warp()
{
}

void Warp::Initialize()
{
	BoxCollider* collider = new BoxCollider(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(floarSize, floarSize, floarSize));
	AddCollider(collider);

	hModel_ = Model::Load("Model/Stage/Warp.fbx");
	assert(hModel_ >= 0);

}

void Warp::Update()
{
	if (rand() % 60 == 0) VFXManager::CreatVfxExplode1(transform_.position_);

}

void Warp::Draw()
{
	if (isValid_) {
		Transform t = transform_;
		t.position_ = { t.position_.x - floarSize / 2.0f, t.position_.y - floarSize / 2.0f, t.position_.z - floarSize / 2.0f };
		Model::SetTransform(hModel_, t);
		Model::Draw(hModel_, 0);

		CollisionDraw();

		GameManager::GetPlayer()->GetCommand()->DrawActionUI();
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

	//Player�ɏՓ˂��n�߂�
	isPlayerHit_ = true;

	//������Key��UI��\��������
	GameManager::GetPlayer()->GetCommand()->SetDrawActionUI();

	//Player��Aim�����ړ��g���Ă݂�
	XMFLOAT3 cPos = XMFLOAT3(transform_.position_.x, transform_.position_.y + 5.0f, transform_.position_.z + 33.0f);
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
