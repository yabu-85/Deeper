#pragma once
#include "../Engine/GameObject.h"
#include <vector>

class UIManager;

class TitleScene : public GameObject
{
	std::vector<UIManager*> pUIManagerList_;

public:
	TitleScene(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

};