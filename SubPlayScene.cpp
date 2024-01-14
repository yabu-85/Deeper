#include "SubPlayScene.h"
#include "GameManager.h"
#include "Engine/SceneManager.h"
#include "Player.h"
#include "CollisionMap.h"
#include "CreateStage.h"
#include "AudioManager.h"
#include "Engine/Input.h"
#include "Warp.h"

//デバッグ用
#include "WeaponObjectManager.h"
#include "PlayerWeapon.h"

SubPlayScene::SubPlayScene(GameObject* parent)
	: GameObject(parent, "SubPlayScene")
{
}

void SubPlayScene::Initialize()
{
	AudioManager::Initialize();

	GameManager::SetStage(this);
	GameManager::GetCreateStage()->CreateStageData("Csv/Map2.csv");
	GameManager::SetPlayer(Instantiate<Player>(this));
	GameManager::SetCollisionMap(Instantiate<CollisionMap>(this));
	GameManager::GetCollisionMap()->CreatIntersectDataTriangle();

	Warp* warp = static_cast<Warp*>(FindObject("Warp"));
	warp->SetWarpScene(SCENE_ID_PLAY);

	//デバッグ用
	GameManager::GetWeaponObjectManager()->AddWeaponObject(WeaponObjectManager::WEAPON_TYPE::WT_SUB1, GameManager::GetCreateStage()->GetPlayerStartPos());

}

void SubPlayScene::Update()
{
	//デバッグ用
	if (Input::IsKeyDown(DIK_C)) {
		SceneManager* pSceneManager = static_cast<SceneManager*>(FindObject("SceneManager"));
		pSceneManager->ChangeScene(SCENE_ID_TITLE);
	}
	if (Input::IsKeyDown(DIK_F)) {
		SceneManager* pSceneManager = static_cast<SceneManager*>(FindObject("SceneManager"));
		pSceneManager->ChangeScene(SCENE_ID_PLAY);
	}

}

void SubPlayScene::Draw()
{
	GameManager::Draw();
	GameManager::GetCreateStage()->Draw();

}

void SubPlayScene::Release()
{
	AudioManager::Release();

}
