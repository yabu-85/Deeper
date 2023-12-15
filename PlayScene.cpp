#include "PlayScene.h"
#include "GameManager.h"
#include "Engine/SceneManager.h"

#include "AudioManager.h"
#include "Engine/Input.h"

PlayScene::PlayScene(GameObject* parent)
	: GameObject(parent, "PlayScene")
{
}

void PlayScene::Initialize()
{
	GameManager::Initialize(this);
	AudioManager::Initialize();

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
	AudioManager::Release();

}
