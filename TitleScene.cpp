#include "TitleScene.h"
#include "Player.h"
#include "Stage.h"
#include "CollisionMap.h"
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
	Instantiate<Stage>(this);
	Instantiate<Player>(this);
	Instantiate<CollisionMap>(this);

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