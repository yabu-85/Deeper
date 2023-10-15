#include "TitleScene.h"
#include "Player.h"
#include "Stage.h"
#include "CollisionMap.h"
#include "EnemyBase.h"

TitleScene::TitleScene(GameObject* parent)
	: GameObject(parent, "TitleScene")
{
}

void TitleScene::Initialize()
{
	Instantiate<Stage>(this);
	Instantiate<Player>(this);
	Instantiate<CollisionMap>(this);
	Instantiate<EnemyBase>(this);

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