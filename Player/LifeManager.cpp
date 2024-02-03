#include "LifeManager.h"
#include "../GameManager.h"
#include "../Engine/Transform.h"
#include "../Engine/Image.h"
#include "../Engine/Text.h"

namespace
{
	static const int DEFAULT_ALPHA = 255;	//���G���Ԃ����炷��
	static const int DEFAULT_LIFE = 100;	//���ʂ�Player�̃��C�t�̐�
	static const float INVINCIBLE = 0.07f;	//���G���Ԃ����炷��
	 
}

namespace LifeManager
{
	int hPict_[2];				//�_���[�W�̉摜
	int playerLife_;			//�v���C���[�̃��C�t��
	float invincibleTime_;		//�_���[�W�\�����Ԃ̌v�Z�p

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
		//�����Ń��C�t�̕\��
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