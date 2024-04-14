#pragma once
#include "SceneBase.h"
#include "../Engine/SceneManager.h"

class Warp;

class StageBase : public SceneBase
{
	bool isCleared_;				//クリアしたかどうか
	Warp* pWarp_;					//ワープのリスト
	XMFLOAT3 startPosition_;		//プレイヤーのスタート座標
	XMFLOAT3 bossPosition_;			//ボスのスタートポジション

public:
	StageBase(GameObject* parent, std::string name);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	//引数：CSV・SkyBoxの名前
	void InitializeStage(std::string csv, std::string sky);
	
	//Warpをリストに追加
	void SetWarp(Warp* w) { pWarp_ = w; }
	Warp* GetWarp() { return pWarp_; }

	//WarpのValidをセット
	void SetWarpValid(bool b);

	//Warp先の設定
	void SetWarpStage(SCENE_ID id);

	//ステージをクリアした時に呼ぶ関数
	virtual void OnStageCleared() {}
	
	//ステージをクリアした時に一回だけtrueになる
	bool IsClearStage();
	
	//クリア一回したら常時trueを返す
	bool IsAlwaysClearStage() { return isCleared_; }

	void SetStartPosition(XMFLOAT3 pos) { startPosition_ = pos; }
	XMFLOAT3 GetStartPosition() { return startPosition_; }
	void SetBossPosition(XMFLOAT3 pos) { bossPosition_ = pos; }
	XMFLOAT3 GetBossPosition() { return bossPosition_; }

};

