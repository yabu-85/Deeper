#pragma once
#include "../Engine/GameObject.h"

class LifeManager : public GameObject {
	int hPict_[3];
	Transform pic0Pos;
	Transform pic1Pos;

	int defInvincibleTime_;
	int invincibleTime_;		//ダメージ表示時間の計算用
	int defPlayerLife_;
	int playerLife_;			//プレイヤーのライフ量

public:
	LifeManager(GameObject* parent);
	~LifeManager();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void SetLife(int current, int max);
	void SetInvincible(int i);
	void Damage(int i);
	void DamageEffectDraw();
	
	int GetReceiveDamage();
	bool IsInvincible();
	bool IsDie();

};