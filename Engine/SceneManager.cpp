#include "sceneManager.h"
#include "Model.h"
#include "Image.h"
#include "Audio.h"
#include "../Scene/Stage1.h"
#include "../Scene/Stage2.h"
#include "../Scene/Stage3.h"
#include "../GameManager.h"
#include "../Player/PlayerData.h"
#include "../Scene/TitleScene.h"
#include "../Scene/ResultScene.h"

//コンストラクタ
SceneManager::SceneManager(GameObject * parent)
	: GameObject(parent, "SceneManager"), currentSceneID_(SCENE_ID_TITLE), nextSceneID_(currentSceneID_)
{
}

//初期化
void SceneManager::Initialize()
{
	//最初のシーンを準備
#if 1
	currentSceneID_ = SCENE_ID_STAGE1;
	nextSceneID_ = currentSceneID_;
	Instantiate<Stage1>(this);
#else
	currentSceneID_ = SCENE_ID_TITLE;
	nextSceneID_ = currentSceneID_;
	Instantiate<TitleScene>(this);
#endif

	GameManager::SetSceneManager(this);

}

//更新
void SceneManager::Update()
{
	//次のシーンが現在のシーンと違う　＝　シーンを切り替えなければならない
	if (currentSceneID_ != nextSceneID_)
	{
		//今のプレイヤーのデータを保存する
		PlayerData::SavePlayerData();

		//指定のStageならStageClear数を増やす
		PlayerData::AddClearStageCount(currentSceneID_);

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
			case SCENE_ID_RESULT: Instantiate<ResultScene>(this); break;
			case SCENE_ID_STAGE1: Instantiate<Stage1>(this); break;
			case SCENE_ID_STAGE2: Instantiate<Stage2>(this); break;
			case SCENE_ID_STAGE3: Instantiate<Stage3>(this); break;
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