#pragma once

namespace LifeManager {
	static const int INVINCIBLE = 50;		//–³“GŽžŠÔ‚ðŒ¸‚ç‚·—Ê
	
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