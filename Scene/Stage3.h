#pragma once
#include "StageBase.h"

class Stage3 : public StageBase
{
public:
	Stage3(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void OnStageCleared() override;

};

