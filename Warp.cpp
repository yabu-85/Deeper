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

Warp::Warp(GameObject* parent)
	: GameObject(parent, "Warp"), warpScene_(SCENE_ID::SCENE_ID_TITLE), isPlayerHit_(false)
{
}

Warp::~Warp()
{
}

void Warp::Initialize()
{
	BoxCollider* collider = new BoxCollider(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(floarSize, floarSize, floarSize));
	AddCollider(collider);

}

void Warp::Update()
{
	if (rand() % 60 == 0) VFXManager::CreatVfxExplode1(transform_.position_);


}

void Warp::Draw()
{
	CollisionDraw();

	GameManager::GetPlayer()->GetCommand()->DrawActionUI();

}

void Warp::Release()
{
}

void Warp::OnCollision(GameObject* pTarget)
{
	if (pTarget->GetObjectName() != "Player") {
		isPlayerHit_ = false;
		return;
	}

	//Player‚ÉÕ“Ë‚µŽn‚ß‚½
	isPlayerHit_ = true;

	//‚±‚±‚ÅKey‚ÌUI‚ð•\Ž¦‚³‚¹‚é
	GameManager::GetPlayer()->GetCommand()->SetDrawActionUI();

	//Player‚ÌAim‹­§ˆÚ“®Žg‚Á‚Ä‚Ý‚é
	XMFLOAT3 cPos = XMFLOAT3(transform_.position_.x, transform_.position_.y - 5.0f, transform_.position_.z + 13.0f);
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
