#pragma once
#include "Engine/GameObject.h"

class UIManager;

class TitleScene : public GameObject
{
	int hPict_[2];
	UIManager* pUIManager_;

public:
	TitleScene(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

};