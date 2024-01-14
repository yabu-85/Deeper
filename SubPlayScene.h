#pragma once
#include "Engine/GameObject.h"

class SubPlayScene : public GameObject
{
public:
	SubPlayScene(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

};