#include "TitleScene.h"
#include "Player.h"
#include "Stage.h"

TitleScene::TitleScene(GameObject* parent)
	: GameObject(parent, "TitleScene")
{
}

void TitleScene::Initialize()
{
	Instantiate<Player>(this);
	Instantiate<Stage>(this);

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