#include "LifeManager.h"
#include "GameManager.h"
#include "Engine/Transform.h"
#include "Engine/Image.h"
#include "Engine/Text.h"

namespace
{
	static const int DEFAULT_LIFE = 100; //���ʂ�Player�̃��C�t�̐�
	static const int INVINCIBLE = 60; //���G����

}

namespace LifeManager
{
	int hPict_[2];				//�_���[�W�̉摜
	int playerLife_;

	int invincibleTime_;		//�_���[�W�\�����Ԃ̌v�Z�p
	int damageImageAlpha_;		//�_���[�W�摜�̓����x

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
		//�����Ń��C�t�̕\��
		Transform t = Transform();
		Image::SetTransform(hPict_[0], t);
		Image::Draw(hPict_[0]);

		if (damageImageAlpha_ > 0) DamageEffectDraw();
	}

	void DamageEffectDraw()
	{
		//�����x���X�ɉ�����
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