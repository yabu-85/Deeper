#include "LifeManager.h"
#include "GameManager.h"
#include "Engine/Transform.h"
#include "Engine/Global.h"
#include "Engine/Sprite.h"
#include "Engine/Text.h"

namespace
{
	static const int DEFAULT_LIFE = 100; //普通のPlayerのライフの数
	static const int INVINCIBLE = 60; //普通のPlayerのライフの数

}

namespace LifeManager
{
	int playerLife_;
	int invincibleTime_;
	float damageImageAlpha_;
	bool  isDrawDamageImage_;

	Sprite* damageImage_;
	Text* pLifeText_;
	XMFLOAT2 textPositiom_;

	void LifeManager::Initialize()
	{
		damageImage_ = new Sprite;
		damageImage_->Load("Image/damage.png");

		pLifeText_ = new Text;
		pLifeText_->Initialize();
		textPositiom_ = XMFLOAT2(30.0f, 200.0f);

		playerLife_ = DEFAULT_LIFE;
		damageImageAlpha_ = 1;
		isDrawDamageImage_ = false;
	}

	void Update()
	{
		invincibleTime_--;

	}

	void LifeManager::Draw()
	{
		//切り抜き範囲をリセット（画像全体を表示する）
		XMFLOAT3 size = damageImage_->GetTextureSize();
		RECT rect;
		rect.left = 0;
		rect.top = 0;
		rect.right = (long)size.x;
		rect.bottom = (long)size.y;
		Transform t;
		damageImage_->Draw(t, rect, 0.1f, 1);

		pLifeText_->Draw((int)textPositiom_.x, (int)textPositiom_.y, (int)playerLife_);

		if (isDrawDamageImage_) DamageEffectDraw();
	}

	void DamageEffectDraw()
	{
		//透明度徐々に下げる
		damageImageAlpha_ -= 0.05f;

		//切り抜き範囲をリセット（画像全体を表示する）
		XMFLOAT3 size = damageImage_->GetTextureSize();
		RECT rect;
		rect.left = 0;
		rect.top = 0;
		rect.right = (long)size.x;
		rect.bottom = (long)size.y;

		Transform t;
		damageImage_->Draw(t, rect, damageImageAlpha_, 1);

		if (damageImageAlpha_ < 0) isDrawDamageImage_ = false;
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
			damageImageAlpha_ = 1;
			isDrawDamageImage_ = true;
		}
	}

	void LifeManager::ResetLife() { playerLife_ = DEFAULT_LIFE; }
	bool LifeManager::IsDie() { return playerLife_ <= 0; }

}