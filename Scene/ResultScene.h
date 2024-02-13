#pragma once
#include "SceneBase.h"

class ResultScene : public SceneBase
{
public:
	ResultScene(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

};