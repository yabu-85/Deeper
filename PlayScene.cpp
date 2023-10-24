#include "PlayScene.h"
#include "GameManager.h"

namespace {
	GameManager* gm = nullptr;

}

PlayScene::PlayScene(GameObject* parent)
	: GameObject(parent, "PlayScene")
{
}

void PlayScene::Initialize()
{
	gm = Instantiate<GameManager>(this);

}

void PlayScene::Update()
{
}

void PlayScene::Draw()
{
}

void PlayScene::Release()
{
}
