#include "LifeManager.h"
#include "../GameManager/GameManager.h"
#include "../AnimationController.h"
#include "../Engine/Transform.h"
#include "../Engine/Sprite.h"
#include "../Player/Player.h"
#include "../Player/Aim.h"
#include "../State/StateManager.h"

namespace LifeManager {
	static const int MAX_ALPHA = 255;

	Sprite* pSprite_[3];
	Transform pic0Pos;
	Transform pic1Pos;

	int defInvincibleTime_ = 0;		//無敵時間
	int invincibleTime_ = 0;		//ダメージ表示時間の計算用
	int defPlayerLife_ = 1;			//今のHpMaxを入れる
	int playerLife_ = 1;			//プレイヤーのライフ量

}

void LifeManager::Initialize()
{
	pic0Pos.scale_.y = 0.6f;
	pic0Pos.position_.x = -0.95f;
	pic0Pos.position_.y = -0.8f;
	pic1Pos = pic0Pos;

	const char* fileName[] = { "Image/hpGaugeA0.png","Image/hpGaugeA1.png", "Image/damage.png" };
	for (int i = 0; i < 3; i++) {
		pSprite_[i] = new Sprite();
		pSprite_[i]->Load(fileName[i]);
	}

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

	XMFLOAT3 size = pSprite_[0]->GetTextureSize();
	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = (long)size.x;
	rect.bottom = (long)size.y;

	pic0Pos.Calclation();
	pSprite_[0]->Draw(pic0Pos, rect, 1.0f, 0);
	pic1Pos.Calclation();
	pSprite_[1]->Draw(pic1Pos, rect, 1.0f, 0);

	//ダメージ画像の表示
	if (invincibleTime_ > 0) DamageEffectDraw();
}

void LifeManager::Release()
{
}

void LifeManager::SetLife(int current, int max)
{
	defPlayerLife_ = max;
	playerLife_ = current;
}

void LifeManager::SetInvincible(int i)
{
	defInvincibleTime_ = i;
}

void LifeManager::DamageEffectDraw()
{
	XMFLOAT3 size = pSprite_[2]->GetTextureSize();
	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = (long)size.x;
	rect.bottom = (long)size.y;
	Transform trans;
	float a = (float)invincibleTime_ / (float)defInvincibleTime_;

	pic0Pos.Calclation();
	pSprite_[2]->Draw(trans, rect, a, 0);

}

void LifeManager::DirectDamage(int i)
{
	invincibleTime_ = defInvincibleTime_;
	playerLife_ -= i;
}

void LifeManager::Damage(int i)
{
	if (IsInvincible() || IsDie()) return;

	DirectDamage(i);

	Player* pPlayer = GameManager::GetPlayer();
	pPlayer->GetAim()->SetCameraShake(4, 0.1f, 0.7f, 0.3f, 0.8f);

	//ダメージ計算したら、HPが０以下になった
	if (LifeManager::IsDie()) {
		pPlayer->GetStateManager()->ChangeState("Dead");
		pPlayer->GetAnimationController()->SetNextAnime(6, 1.0f);
		pPlayer->GetAim()->SetAimMove(false);
		return;
	}

	pPlayer->GetStateManager()->ChangeState("Hear");
	pPlayer->ResetMovement();
	pPlayer->GetAnimationController()->SetNextAnime(5, 1.0f);

}

int LifeManager::GetReceiveDamage() { return (defPlayerLife_ - playerLife_); }
float LifeManager::GetLifeParcent() { return ((float)playerLife_ / (float)defPlayerLife_); }
bool LifeManager::IsInvincible() { return (invincibleTime_ > 0.0f); }
bool LifeManager::IsDie() { return playerLife_ <= 0; }