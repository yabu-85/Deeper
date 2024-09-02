#pragma once

/// <summary>
/// �_���[�W�̉摜�G�t�F�N�g�A�v���C���[��HPGauge�̕\��
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