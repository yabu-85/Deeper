#pragma once
#include "SceneBase.h"

/// <summary>
/// �X�e�[�W�N���A�ƃQ�[���I�[�o�[�̕\��
/// </summary>
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