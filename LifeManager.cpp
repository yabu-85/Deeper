#include "LifeManager.h"
#include "GameManager.h"
#include "Engine/Transform.h"
#include "Engine/Image.h"
#include "Engine/Text.h"

namespace
{
	static const int DEFAULT_LIFE = 100; //普通のPlayerのライフの数
	static const int INVINCIBLE = 60; //無敵時間

}

namespace LifeManager
{
	int hPict_[2];				//ダメージの画像
	int playerLife_;

	int invincibleTime_;		//ダメージ表示時間の計算用
	int damageImageAlpha_;		//ダメージ画像の透明度

	void LifeManager::Initialize()
	{
		std::string fileName[] = { "damage", "damage" };
		for (int i = 0; i < 2; i++) {
			hPict_[0] = Image::Load("Image/" + fileName[i] + ".png");
			assert(hPict_[0] >= 0);
		}
		
		playerLife_ = DEFAULT_LIFE;
		damageImageAlpha_ = 0;
	}

	void Update()
	{
		invincibleTime_--;

	}

	void LifeManager::Draw()
	{
		//ここでライフの表示
		Transform t = Transform();
		Image::SetTransform(hPict_[0], t);
		Image::Draw(hPict_[0]);

		if (damageImageAlpha_ > 0) DamageEffectDraw();
	}

	void DamageEffectDraw()
	{
		//透明度徐々に下げる
		damageImageAlpha_--;

		Transform t;
		Image::SetAlpha(hPict_[0], damageImageAlpha_);
		Image::SetTransform(hPict_[0], t);
		Image::Draw(hPict_[0]);
	}

	void LifeManager::SceneTransitionInitialize()
	{
	}

	void LifeManager::Damage(int i)
	{
		if (invincibleTime_ > 0) return;

		invincibleTime_ = INVINCIBLE;
		playerLife_ -= i;

		if (!IsDie())
		{
			damageImageAlpha_ = 255;
		}
	}

	void LifeManager::ResetLife() { playerLife_ = DEFAULT_LIFE; }
	bool LifeManager::IsDie() { return playerLife_ <= 0; }

}