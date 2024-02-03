#pragma once
#include "../Engine/GameObject.h"
#include <vector>

class Warp;

class StageBase : public GameObject
{
public:
	bool isCleared_;
	std::vector<Warp*> warpList_;

public:
	StageBase(GameObject* parent, std::string name);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void AddWarpList(Warp* w) { warpList_.push_back(w); }
	virtual void OnStageCleared() {}

};

