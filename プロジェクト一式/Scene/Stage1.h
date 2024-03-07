#pragma once
#include "../Engine/GameObject.h"
#include "StageBase.h"

class Stage1 : public StageBase
{
public:
	Stage1(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void OnStageCleared() override;

};