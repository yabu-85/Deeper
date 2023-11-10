#include "PlayScene.h"
#include "GameManager.h"
#include "Engine/SceneManager.h"

#include "Engine/Input.h"

PlayScene::PlayScene(GameObject* parent)
	: GameObject(parent, "PlayScene")
{
}

void PlayScene::Initialize()
{
	GameManager::Initialize(this);

}

void PlayScene::Update()
{
	GameManager::Update();

	if (Input::IsKeyDown(DIK_C)) {
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_TITLE);
	}
}

void PlayScene::Draw()
{
}

void PlayScene::Release()
{
	GameManager::Release();

}
