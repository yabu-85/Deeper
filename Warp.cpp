#include "Warp.h"
#include "VFXManager.h"
#include "Engine/SceneManager.h"
#include "Engine/BoxCollider.h"
#include "CreateStage.h"

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

}

void Warp::Release()
{
}

void Warp::OnCollision(GameObject* pTarget)
{
	//Player‚¾‚¯Õ“Ë”»’è
	if (pTarget->GetObjectName() != "Player")
		return;

	SceneManager* pSceneManager = static_cast<SceneManager*>(FindObject("SceneManager"));
	pSceneManager->ChangeScene(warpScene_);

}
