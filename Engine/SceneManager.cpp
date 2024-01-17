#include "sceneManager.h"
#include "Model.h"
#include "Image.h"
#include "Audio.h"
#include "../TitleScene.h"
#include "../PlayScene.h"
#include "../SubPlayScene.h"
#include "../GameManager.h"
#include "../PlayerData.h"
#include "../Stage3.h"

//コンストラクタ
SceneManager::SceneManager(GameObject * parent)
	: GameObject(parent, "SceneManager")
{
}

//初期化
void SceneManager::Initialize()
{
	//最初のシーンを準備
	currentSceneID_ = SCENE_ID_PLAY;
	nextSceneID_ = currentSceneID_;
	Instantiate<PlayScene>(this);
}

//更新
void SceneManager::Update()
{
	//次のシーンが現在のシーンと違う　＝　シーンを切り替えなければならない
	if (currentSceneID_ != nextSceneID_)
	{
		//今のプレイヤーのデータを保存する
		PlayerData::SavePlayerData();

		//そのシーンのオブジェクトを全削除
		KillAllChildren();

		//ロードしたデータを全削除
		Audio::Release();
		Model::AllRelease();
		Image::AllRelease();

		//シーン遷移時の初期化
		GameManager::SceneTransitionInitialize();

		//次のシーンを作成
		switch (nextSceneID_)
		{
			case SCENE_ID_TITLE: Instantiate<TitleScene>(this); break;
			case SCENE_ID_PLAY: Instantiate<PlayScene>(this); break;
			case SCENE_ID_PLAY1: Instantiate<SubPlayScene>(this); break;
			case SCENE_ID_PLAY2: Instantiate<Stage3>(this); break;
		}

		Audio::Initialize();
		currentSceneID_ = nextSceneID_;
	}
}

//描画
void SceneManager::Draw()
{
}

//開放
void SceneManager::Release()
{
}

//シーン切り替え（実際に切り替わるのはこの次のフレーム）
void SceneManager::ChangeScene(SCENE_ID next)
{
	nextSceneID_ = next;
}