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
	: GameObject(parent, "Warp"), warpScene_(SCENE_ID::SCENE_ID_TITLE)
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
	//ここでフラグ制御はおかしいけどとりま
	GameManager::GetPlayer()->GetAim()->SetCompulsion(false);

	if (pTarget->GetObjectName() != "Player") return;
	
	//ここでKeyのUIを表示させる
	GameManager::GetPlayer()->GetCommand()->SetDrawActionUI();

	//PlayerのAim強制移動使ってみる
	GameManager::GetPlayer()->GetAim()->SetCompulsion(true);
	//GameManager::GetPlayer()->GetAim()->


	if (GameManager::GetPlayer()->GetCommand()->CmdDownAction()) {
		SceneManager* pSceneManager = static_cast<SceneManager*>(FindObject("SceneManager"));
		pSceneManager->ChangeScene(warpScene_);
	}
	
}
