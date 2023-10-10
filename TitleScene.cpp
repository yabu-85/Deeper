#include "TitleScene.h"
#include "Player.h"
#include "Stage.h"

//コンストラクタ
TitleScene::TitleScene(GameObject* parent)
	: GameObject(parent, "TitleScene")
{
}

//初期化
void TitleScene::Initialize()
{
	Instantiate<Player>(this);
	Instantiate<Stage>(this);

}

//更新
void TitleScene::Update()
{
}

//描画
void TitleScene::Draw()
{
}

//開放
void TitleScene::Release()
{
}