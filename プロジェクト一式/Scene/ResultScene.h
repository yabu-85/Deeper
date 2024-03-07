#pragma once
#include "SceneBase.h"

class ResultScene : public SceneBase
{
public:
	ResultScene(GameObject* parent);
	~ResultScene() override;
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

};