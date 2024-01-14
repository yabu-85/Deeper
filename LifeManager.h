#pragma once

namespace LifeManager
{
	void Initialize();
	void Update();
	void Draw();
	void DamageEffectDraw();
	void SceneTransitionInitialize();
	void Damage(int i);

	void ResetLife();
	bool IsDie();

}