#pragma once
#include "Engine/GameObject.h"
#include "StageBase.h"

class SubPlayScene : public StageBase
{
public:
	SubPlayScene(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void OnStageCleared() override;

};