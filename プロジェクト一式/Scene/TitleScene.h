#pragma once
#include "SceneBase.h"

/// <summary>
/// �^�C�g�����
/// </summary>
class TitleScene : public SceneBase
{
public:
	TitleScene(GameObject* parent);
	~TitleScene() override;
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

};