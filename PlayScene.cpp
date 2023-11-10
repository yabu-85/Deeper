#include "PlayScene.h"
#include "GameManager.h"

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

}

void PlayScene::Draw()
{
}

void PlayScene::Release()
{
}
