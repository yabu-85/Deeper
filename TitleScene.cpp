#include "TitleScene.h"
#include "GameManager.h"

namespace {
	GameManager *gm = nullptr;

}

TitleScene::TitleScene(GameObject* parent)
	: GameObject(parent, "TitleScene")
{
}

void TitleScene::Initialize()
{
	gm = Instantiate<GameManager>(this);

}

void TitleScene::Update()
{
}

void TitleScene::Draw()
{
}

void TitleScene::Release()
{
}