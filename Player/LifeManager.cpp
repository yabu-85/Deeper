#include "LifeManager.h"
#include "../GameManager.h"
#include "../Engine/Transform.h"
#include "../Engine/Sprite.h"
#include "../Engine/Text.h"

LifeManager::LifeManager(GameObject* parent)
	: GameObject(parent, "LifeManager"), hPict_{-1, -1, -1}, playerLife_(0), defPlayerLife_(0), defInvincibleTime_(0), invincibleTime_(0)
{
}

LifeManager::~LifeManager()
{
}

void LifeManager::Initialize()
{
	const char* fileName[] = { "Image/hpGaugeA0.png","Image/hpGaugeA1.png", "Image/damage.png" };
	for (int i = 0; i < 3; i++) { //壁と床のロード
		hPict_[i] = Image::Load(fileName[i]);
		assert(hPict_[i] >= 0);
	}
	pic0Pos.scale_.y = 0.6f;
	pic0Pos.position_.x = -0.95f;
	pic0Pos.position_.y = -0.8f;
	pic1Pos = pic0Pos;
}

void LifeManager::Update()
{
	invincibleTime_--;

}

void LifeManager::Draw()
{
	//ライフの表示
	float hpGauge = (float)((playerLife_ * 100) / defPlayerLife_) * 0.01f;
	pic0Pos.scale_.x = hpGauge;
	Image::SetTransform(hPict_[0], pic0Pos);
	Image::Draw(hPict_[0], 0);
	Image::SetTransform(hPict_[1], pic1Pos);
	Image::Draw(hPict_[1], 0);

	//ダメージ画像の表示
	if (invincibleTime_ > 0) DamageEffectDraw();
}

void LifeManager::Release()
{
}

void LifeManager::SetLife(int i)
{
	defPlayerLife_ = i;
	playerLife_ = i;
}

void LifeManager::SetInvincible(int i)
{
	defInvincibleTime_ = i;
}

void LifeManager::DamageEffectDraw()
{
	Image::SetTransform(hPict_[2], pic1Pos);
	Image::Draw(hPict_[2]);

}

void LifeManager::Damage(int i)
{
	if (IsInvincible()) return;

	invincibleTime_ = invincibleTime_;
	playerLife_ -= i;
}

bool LifeManager::IsInvincible() { return (invincibleTime_ > 0.0f); }
bool LifeManager::IsDie() { return playerLife_ <= 0; }