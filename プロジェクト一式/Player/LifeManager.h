#pragma once

/// <summary>
/// ダメージの画像エフェクト、プレイヤーのHPGaugeの表示
/// </summary>
namespace LifeManager {
	void Initialize();
	void Update();
	void Draw();
	void Release();

	void SetLife(int current, int max);
	void Damage(int i);
	void DamageEffectDraw();
	
	int GetReceiveDamage();
	float GetLifeParcent();
	bool IsDie();

};