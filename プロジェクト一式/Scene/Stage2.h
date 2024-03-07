#pragma once
#include "../Engine/GameObject.h"
#include "StageBase.h"

class Stage2 : public StageBase
{
public:
	Stage2(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void OnStageCleared() override;

};