#pragma once
#include "Engine/GameObject.h"
#include "Engine/Text.h"

//�����V�[�����Ǘ�����N���X
class TitleScene : public GameObject
{
public:
	TitleScene(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

};