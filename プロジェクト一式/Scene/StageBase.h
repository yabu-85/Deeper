#pragma once
#include "SceneBase.h"
#include "../Engine/SceneManager.h"
#include <vector>

class Warp;

class StageBase : public SceneBase
{
	bool isCleared_;				//クリアしたかどうか
	std::vector<Warp*> warpList_;	//ワープのリスト
	XMFLOAT3 startPosition_;		//プレイヤーのスタート座標

public:
	StageBase(GameObject* parent, std::string name);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	//引数：CSV・SkyBoxの名前
	void InitializeStage(std::string csv, std::string sky);

	//Warpをリストに追加
	void AddWarpList(Warp* w) { warpList_.push_back(w); }
	
	//全てのWarpのValidをセット
	void SetAllWarpValid(bool b);

	void SetWarpStage(SCENE_ID* list);

	//ステージをクリアした時に呼ぶ関数
	virtual void OnStageCleared() {}
	
	//ステージをクリアした時に一回だけtrueになる
	bool IsClearStage();

	void SetStartPosition(XMFLOAT3 pos) { startPosition_ = pos; }
	XMFLOAT3 GetStartPosition() { return startPosition_; }

};

