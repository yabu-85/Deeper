#pragma once

namespace LifeManager {
	static const int INVINCIBLE = 50;		//���G���Ԃ����炷��
	
	void Initialize();
	void Update();
	void Draw();
	void DamageEffectDraw();
	void SceneTransitionInitialize();
	void Damage(int i);
	
	void ResetLife();
	bool IsInvincible();
	bool IsDie();

}