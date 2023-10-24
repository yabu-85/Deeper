#include "TitleScene.h"
#include "Engine/SceneManager.h"
#include "Engine/Input.h"

TitleScene::TitleScene(GameObject* parent)
	: GameObject(parent, "TitleScene")
{
}

void TitleScene::Initialize()
{
}

void TitleScene::Update()
{
	if (Input::IsMouseButton(0)) {
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_PLAY);

	}

}

void TitleScene::Draw()
{
}

void TitleScene::Release()
{
}