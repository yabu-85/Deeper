#pragma once

namespace LifeManager {
	void Initialize();
	void Update();
	void Draw();
	void Release();

	void SetLife(int current, int max);
	void SetInvincible(int i);
	void DirectDamage(int i);
	void Damage(int i);
	void DamageEffectDraw();
	
	int GetReceiveDamage();
	bool IsInvincible();
	bool IsDie();

};