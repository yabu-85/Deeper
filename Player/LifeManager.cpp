#include "LifeManager.h"
#include "../GameManager.h"
#include "../Engine/Transform.h"
#include "../Engine/Image.h"
#include "../Engine/Text.h"

namespace
{
	static const int DEFAULT_ALPHA = 255;	//無敵時間を減らす量
	static const int DEFAULT_LIFE = 100;	//普通のPlayerのライフの数
	static const float INVINCIBLE = 0.07f;	//無敵時間を減らす量
	 
}

namespace LifeManager
{
	int hPict_[2];				//ダメージの画像
	int playerLife_;			//プレイヤーのライフ量
	float invincibleTime_;		//ダメージ表示時間の計算用

	void LifeManager::Initialize()
	{
		std::string fileName[] = { "damage", "damage" };
		for (int i = 0; i < 2; i++) {
			hPict_[0] = Image::Load("Image/" + fileName[i] + ".png");
			assert(hPict_[0] >= 0);
		}
		
		playerLife_ = DEFAULT_LIFE;
		invincibleTime_ = 0.0f;

	}

	void Update()
	{
		invincibleTime_ -= INVINCIBLE;

	}

	void LifeManager::Draw()
	{
		//ここでライフの表示
		Transform t = Transform();
		//Image::SetTransform(hPict_[0], t);
		//Image::Draw(hPict_[0]);

		if (invincibleTime_ > 0.0f) DamageEffectDraw();
	}

	void DamageEffectDraw()
	{
		Transform t;
		Image::SetAlpha(hPict_[0], (int)((float)(DEFAULT_ALPHA) * invincibleTime_));
		Image::SetTransform(hPict_[0], t);
		Image::Draw(hPict_[0]);
	}

	void LifeManager::SceneTransitionInitialize()
	{
	}

	void LifeManager::Damage(int i)
	{
		if (invincibleTime_ > 0.0f) return;

		invincibleTime_ = 1.0f;
		playerLife_ -= i;
	}

	void LifeManager::ResetLife() { playerLife_ = DEFAULT_LIFE; }
	bool LifeManager::IsDie() { return playerLife_ <= 0; }

}