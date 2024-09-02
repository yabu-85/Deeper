#pragma once
#include "SceneBase.h"

/// <summary>
/// ステージクリアとゲームオーバーの表示
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